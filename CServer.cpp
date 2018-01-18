#include "stdafx.h"
#include "CListener.h"
#include "CListenerManager.h"
#include "CmySQLQueue.h"
#include "CLogFile.h"
#include "CClient.h"
#include "CHighResTimer.h"
#include "CHelp.h"
#include "CTrigger.h"
#include "CItem.h"
#include "CSkill.h"
#include "CClass.h"
#include "CExit.h"
#include "CReset.h"
#include "CArea.h"
#include "CRoom.h"
#include "CQuest.h"
#include "CPlayer.h"
#include "CCharacter.h"
#include "CSpellAffect.h"
#include "CUser.h"
#include "CGame.h"
#include "CServer.h"
#include "utils.h"

//Op codes for IOCP
#define OP_READ     0
#define OP_WRITE    1

#define WAIT_TIMEOUT_INTERVAL 100

mySQLQueue * Server::sqlQueue;
sol::state Server::lua;
std::mt19937_64 Server::rand;

Server::Server(Game * g, int port) : nPort(port), mygame(g)
{
	hShutdownEvent = NULL;
	nThreads = 1;
	hAcceptThread = NULL;
	hIOCompletionPort = NULL;
}

Server::~Server()
{
   
}

bool Server::Initialize()
{
	//Find out number of processors and threads
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	nThreads = si.dwNumberOfProcessors;

	//Allocate memory to store thread handles
	phWorkerThreads = new HANDLE[nThreads];

	InitializeCriticalSection(&clientListCS);

	//Create shutdown event
	hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// Initialize Winsock
	WSADATA wsaData;
	int nResult;
	nResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (NO_ERROR != nResult)
	{
		LogFile::Log("error", "Error occurred while executing WSAStartup()");
		return false;
	}

	//Create I/O completion port
	hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if(NULL == hIOCompletionPort)
	{
		LogFile::Log("error", "Error occurred while creating IOCP : " + Utilities::itos(WSAGetLastError()));
		return false;
	}

	//Create worker threads
	DWORD nThreadID;
	for (int ii = 0; ii < nThreads; ii++)
	{
		phWorkerThreads[ii] = CreateThread(0, 0, WorkerThread, this, 0, &nThreadID);
	}

	ListenSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == ListenSocket)
	{
		LogFile::Log("error", "Error occurred while opening socket: " + Utilities::itos(WSAGetLastError()));
		closesocket(ListenSocket);
		return false;
	}

	ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = INADDR_ANY;
	ServerAddress.sin_port = htons(nPort);   

	if(SOCKET_ERROR == bind(ListenSocket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress)))
	{
		LogFile::Log("error", "Error occurred in bind()");
		closesocket(ListenSocket);
		return false;
	}

	//Make the socket a listening socket
	if(SOCKET_ERROR == listen(ListenSocket, SOMAXCONN))
	{
		LogFile::Log("error", "Error occurred in listen()");
		closesocket(ListenSocket);
		return false;
	}

	hAcceptEvent = WSACreateEvent();
	if(WSA_INVALID_EVENT == hAcceptEvent)
	{
		LogFile::Log("error", "Error occurred in WSACreateEvent()");
		return false;
	}

	if (SOCKET_ERROR == WSAEventSelect(ListenSocket, hAcceptEvent, FD_ACCEPT))
	{
		LogFile::Log("error", "Error occurred in WSAEventSelect()");
		WSACloseEvent(hAcceptEvent);
		return false;
	}

	sqlQueue = new mySQLQueue();

	return true;
}

void Server::Start()
{
	DWORD nThreadID;
	hAcceptThread = CreateThread(0, 0, AcceptThread, (void *)this, 0, &nThreadID);
}

void Server::DeInitialize()
{
	closesocket(ListenSocket);

	//Ask all threads to start shutting down
	SetEvent(hShutdownEvent);

	//Let Accept thread go down
	WaitForSingleObject(hAcceptThread, INFINITE);

    std::list<User *>::iterator iter;
    iter = mygame->users.begin();
	while(iter != mygame->users.end())
	{
		User * user = (*iter);
        if(user->GetClient())
            closesocket(user->GetClient()->Socket());
		user->ImmediateDisconnect(); 

        //Save user/player
        if(user->character)
        {
            if(user->connectedState == User::CONN_PLAYING && user->character->level > 1) //don't save fresh characters
                user->character->Save();
            mygame->characters.remove(user->character);
        }
        delete user;
        iter = mygame->users.erase(iter);
    }

	for(int i = 0; i < nThreads; i++)
	{
		//Help threads get out of blocking - GetQueuedCompletionStatus()
		PostQueuedCompletionStatus(hIOCompletionPort, 0, (DWORD)NULL, NULL);
	}

	//Let Worker Threads shutdown
	WaitForMultipleObjects(nThreads, phWorkerThreads, TRUE, INFINITE);

	//We are done with this event
	WSACloseEvent(hAcceptEvent);

	DeleteCriticalSection(&clientListCS);

	//Cleanup IOCP.
	CloseHandle(hIOCompletionPort);

	//Clean up the event.
	CloseHandle(hShutdownEvent);

	//Clean up memory allocated for the storage of thread handles
	delete[] phWorkerThreads;

	//Cleanup Winsock
	WSACleanup();

    if(!clients.empty())
        LogFile::Log("error", "server client list is NOT empty at shutdown");
}

//This thread will look for accept event
DWORD WINAPI Server::AcceptThread(void * lParam)
{
	Server * theserver = (Server *)lParam;

	WSANETWORKEVENTS WSAEvents;

	//Accept thread will be around to look for accept event, until a Shutdown event is not Signaled.
	while (WAIT_OBJECT_0 != WaitForSingleObject(theserver->hShutdownEvent, 0))
	{
		if (WSA_WAIT_TIMEOUT != WSAWaitForMultipleEvents(1, &theserver->hAcceptEvent, FALSE, WAIT_TIMEOUT_INTERVAL, FALSE))
		{
			WSAEnumNetworkEvents(theserver->ListenSocket, theserver->hAcceptEvent, &WSAEvents);
			if ((WSAEvents.lNetworkEvents & FD_ACCEPT) && (0 == WSAEvents.iErrorCode[FD_ACCEPT_BIT]))
			{
				//Process it
				theserver->AcceptConnection(theserver->ListenSocket);
			}
		}
	}

	return 0;
}

void Server::AcceptConnection(SOCKET ListenSocket)
{
	sockaddr_in ClientAddress;
	int nClientLength = sizeof(ClientAddress);

	//Accept remote connection attempt from the client
	SOCKET Socket = accept(ListenSocket, (sockaddr*)&ClientAddress, &nClientLength);

	if (INVALID_SOCKET == Socket)
	{
		LogFile::Log("network", "accept(): " + Utilities::GetLastErrorAsString());
	}

	//Get client's IP
	char ipstr[INET6_ADDRSTRLEN];
	std::string addr = inet_ntop(AF_INET, &ClientAddress.sin_addr, ipstr, sizeof(ipstr));

	if (addr != "127.0.0.1") //Check for connection spam IP bans (Let's not ban localhost)
	{
		UpdateIPList(addr);
		if (!CheckTempBanList(addr))
		{
			//Banned due to connection spam!
			closesocket(Socket); //Haven't yet associated this socket with IOCP, this may be all we need to do?
			//LogFile::Log("network", "Banned client attempted connect from: " + addr);
			return;
		}
	}

	//Set TCP_NODELAY (Disable Nagle)
	int value = 1;
	setsockopt(Socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&value, sizeof(value));

	LogFile::Log("network", "Client connected from: " + addr);

	//Create a new ClientContext for this newly accepted client
	auto pClientContext = std::make_shared<Client>(Socket, addr);
	
	if (true == AssociateWithIOCP(pClientContext.get()))
	{
		AddClient(pClientContext);			//both have internal critical sections 
		mygame->NewUser(pClientContext);	// for client and user list access

		OVERLAPPEDEX * operationData = pClientContext->NewOperationData(OP_READ);
		OVERLAPPED * base_overlapped = static_cast<OVERLAPPED*>(operationData);

		DWORD dwFlags = 0;
		DWORD dwBytes = 0;

		//Post initial Recv
		pClientContext->RefCountAdjust(1);
		int err = WSARecv(pClientContext->Socket(), &operationData->wsabuf, 1, NULL, &dwFlags, base_overlapped, NULL);

		if ((SOCKET_ERROR == err) && (WSA_IO_PENDING != WSAGetLastError()))
		{
			LogFile::Log("network", "WSARecv(): " + Utilities::GetLastErrorAsString());
			if(pClientContext)
            {
                LogFile::Log("network", "Disconnect from " + pClientContext->GetIPAddress() + " in AcceptConnection()");
				pClientContext->RefCountAdjust(-1);
                pClientContext->FreeOperationData(operationData);
				pClientContext->DisconnectServer();
                closesocket(pClientContext->Socket());
				RemoveClient(pClientContext); 
            }
		}
	}
}

bool Server::AssociateWithIOCP(Client * pClientContext)
{
	//Associate the socket with IOCP
	HANDLE hTemp = CreateIoCompletionPort((HANDLE)pClientContext->Socket(), hIOCompletionPort, (ULONG_PTR)pClientContext, 0);

	if (NULL == hTemp)
	{
		LogFile::Log("network", "CreateIoCompletionPort(): " + Utilities::GetLastErrorAsString());
		if(pClientContext)
        {
            LogFile::Log("network", "Disconnect from " + pClientContext->GetIPAddress() + " in AssociateWithIOCP()");
            closesocket(pClientContext->Socket());
			pClientContext->DisconnectServer();
        }
		return false;
	}

	return true;
}

//Worker thread will service IOCP requests
DWORD WINAPI Server::WorkerThread(void * lpParam)
{
	Server * thisserver = (Server*)lpParam;

	void *lpContext = NULL;
	OVERLAPPED       *pOverlapped = NULL;
	Client   *pClientContext = NULL;
	DWORD            dwBytesTransfered = 0;
	int nBytesRecv = 0;
	int nBytesSent = 0;
	DWORD             dwBytes = 0, dwFlags = 0;

	//Worker thread will be around to process requests, until a Shutdown event is not Signaled.
	while (WAIT_OBJECT_0 != WaitForSingleObject(thisserver->hShutdownEvent, 0))
	{
		BOOL bReturn = GetQueuedCompletionStatus(
			thisserver->hIOCompletionPort,
			&dwBytesTransfered,
			(PULONG_PTR)&lpContext,
			&pOverlapped,
			INFINITE);

        //LogFile::Log("status", "GQCP worker awake # " );

		if (NULL == lpContext)
		{
			//We are shutting down
			break;
		}

		//Get the client context
		pClientContext = (Client *)lpContext;
		pClientContext->RefCountAdjust(-1); //GQCS completed
		//Get the extended overlapped structure
		OVERLAPPEDEX * pOverlappedEx = static_cast<OVERLAPPEDEX*>(pOverlapped);

        if(bReturn && WSAGetLastError() == ERROR_SUCCESS && pClientContext != NULL && dwBytesTransfered == 0)
        {
			//WE HIT HERE ON MUSHCLIENT DISCONNECT
            //LogFile::Log("error", "(1)GetQueuedCompletionStatus(): ThreadID:" + Utilities::itos(GetCurrentThreadId()) + " " + Utilities::itos(WSAGetLastError()));// + Utilities::GetLastErrorAsString());
		
			pClientContext->DisconnectServer();
			pClientContext->FreeOperationData(pOverlappedEx);
			closesocket(pClientContext->Socket());
			if(pClientContext->GetRefCount() == 0)
				thisserver->RemoveClient(pClientContext);
            continue;
        }

		if(!bReturn)
		{
			//WE HIT HERE ON "QUIT" (Err #1236) and stress tester disconnect (err 64)
			//LogFile::Log("error", "(2)GetQueuedCompletionStatus(): ThreadID:" + Utilities::itos(GetCurrentThreadId()) + " " + Utilities::itos(WSAGetLastError()));// + Utilities::GetLastErrorAsString());
			if(pClientContext)
            {
                //if(pOverlapped == NULL)
                //    LogFile::Log("status", "(!bReturn) && pOverlapped == NULL");
                //LogFile::Log("status", "(!bReturn) Disconnect from " + pClientContext->GetIPAddress());

				pClientContext->DisconnectServer();
				pClientContext->FreeOperationData(pOverlappedEx);
				//closesocket(pClientContext->Socket());
				if (pClientContext->GetRefCount() == 0)
					thisserver->RemoveClient(pClientContext);
            }
			continue;
		}

		switch (pOverlappedEx->opCode)
		{
			case OP_WRITE:
				pOverlappedEx->sentBytes += dwBytesTransfered;

				//Write operation was finished, see if all the data was sent.
				//Else post another write.
				if(pOverlappedEx->sentBytes < pOverlappedEx->totalBytes)
				{
					pOverlappedEx->opCode = (OP_WRITE);

					pOverlappedEx->wsabuf.buf += pOverlappedEx->sentBytes;
					pOverlappedEx->wsabuf.len = pOverlappedEx->totalBytes - pOverlappedEx->sentBytes;

					dwFlags = 0;

					//Overlapped send
					pClientContext->RefCountAdjust(1);
					//nBytesSent = WSASend(pClientContext->Socket(), &pOverlappedEx->wsabuf, 1, &dwBytes, dwFlags, pOverlapped, NULL);
					nBytesSent = WSASend(pClientContext->Socket(), &pOverlappedEx->wsabuf, 1, NULL, 0, pOverlapped, NULL);

					if ((SOCKET_ERROR == nBytesSent) && (WSA_IO_PENDING != WSAGetLastError()))
					{
						//LogFile::Log("error", "WSASend(): " + Utilities::GetLastErrorAsString());
						if(pClientContext)
                        {
                            //LogFile::Log("status", "(WSASend: OP_WRITE) Disconnect from " + pClientContext->GetIPAddress());
							pClientContext->RefCountAdjust(-1);
							pClientContext->DisconnectServer();
							pClientContext->FreeOperationData(pOverlappedEx);
							closesocket(pClientContext->Socket());
							if (pClientContext->GetRefCount() == 0)
								thisserver->RemoveClient(pClientContext);
                        }
					}
				}
				else
				{
					pClientContext->FreeOperationData(pOverlappedEx);
				}
				break;

			case OP_READ:

				//LogFile::Log("network", "Thread ?: The following message was received: " + std::string(localBuffer));

				//append the new input
				pClientContext->inputBuffer += pOverlappedEx->buffer; // += append;

				if (pClientContext->inputBuffer.length() > MAX_INPUT_BUFFER) //disconnect em
				{
					pClientContext->DisconnectServer();
					pClientContext->FreeOperationData(pOverlappedEx);
					closesocket(pClientContext->Socket());
					if (pClientContext->GetRefCount() == 0)
						thisserver->RemoveClient(pClientContext);
				}
				else if (pClientContext->inputBuffer.length() > MAX_COMMAND_LENGTH) //truncate the input
				{
					pClientContext->inputBuffer = pClientContext->inputBuffer.substr(0, MAX_COMMAND_LENGTH);
					pClientContext->inputBuffer += "\n\r";
				}

				//search for \n \r
				size_t cr_pos = pClientContext->inputBuffer.find('\r');
				size_t nl_pos = pClientContext->inputBuffer.find('\n');
				while(cr_pos != std::string::npos && nl_pos != std::string::npos)
				{
					//Copy a single command
					if(cr_pos < nl_pos) //telnet sends carriage return first... 
					{
						EnterCriticalSection(&pClientContext->command_cs);
						pClientContext->commandQueue.push_back(pClientContext->inputBuffer.substr(0, cr_pos));
						LeaveCriticalSection(&pClientContext->command_cs);
						if(nl_pos != std::string::npos)
							pClientContext->inputBuffer.erase(0, nl_pos+1);
						else
							pClientContext->inputBuffer.erase(0, cr_pos+1);
					}
					else if(nl_pos < cr_pos) //...is it ever done any other way?
					{
						EnterCriticalSection(&pClientContext->command_cs);
						pClientContext->commandQueue.push_back(pClientContext->inputBuffer.substr(0, nl_pos));
						LeaveCriticalSection(&pClientContext->command_cs);
						if(cr_pos != std::string::npos)
							pClientContext->inputBuffer.erase(0, cr_pos+1);
						else
							pClientContext->inputBuffer.erase(0, nl_pos+1);
					}
					cr_pos = pClientContext->inputBuffer.find('\r');
					nl_pos = pClientContext->inputBuffer.find('\n');
				}

				pOverlappedEx->opCode = (OP_READ);
				ZeroMemory(pOverlappedEx->buffer, NETWORK_BUFFER_SIZE);
				ZeroMemory(pOverlapped, sizeof(OVERLAPPED));
				pOverlappedEx->totalBytes = pOverlappedEx->sentBytes = 0;

				pClientContext->RefCountAdjust(1);
				int err = WSARecv(pClientContext->Socket(), &pOverlappedEx->wsabuf, 1, NULL, &dwFlags, pOverlapped, NULL);

				if ((SOCKET_ERROR == err) && (WSA_IO_PENDING != WSAGetLastError()))
				{
					//LogFile::Log("error", "WSARecv(): " + Utilities::GetLastErrorAsString());
					if(pClientContext)
                    {
                        //LogFile::Log("status", "(WSARecv: OP_READ) Disconnect from " + pClientContext->GetIPAddress());

						pClientContext->RefCountAdjust(-1);
						pClientContext->DisconnectServer();
						pClientContext->FreeOperationData(pOverlappedEx);
						closesocket(pClientContext->Socket());
						if (pClientContext->GetRefCount() == 0)
							thisserver->RemoveClient(pClientContext);
                    }
				}

				break;
		} 
	}

	return 0;
}

void Server::deliver(Client * c, const std::string msg)
{
	//deliver is called FROM THE GAME THREAD
	OVERLAPPEDEX * olptr = c->NewOperationData(OP_WRITE);
	memcpy(olptr->buffer, msg.c_str(), msg.length());
	olptr->wsabuf.len = (DWORD)msg.length();
	olptr->totalBytes = (DWORD)msg.length();
	OVERLAPPED * base_overlapped = static_cast<OVERLAPPED*>(olptr);

	c->RefCountAdjust(1);
	int wsaerr = WSASend(c->Socket(), &olptr->wsabuf, 1, NULL, 0, base_overlapped, NULL);

	if(wsaerr == 0)
	{
		//immediate success (A completion packet is still sent to unblock GetQueuedCompletionStatus!!, don't delete the OVERLAPPED)
		//LogFile::Log("status", "Immediate sent bytes: " + Utilities::itos(base_overlapped->InternalHigh));
	}
	else if(wsaerr == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING)
	{
		//queued for GetQueuedCompletionStatus
	}
	else if(wsaerr == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		//error
        //LogFile::Log("status", "(WSASend: deliver() immediate) Disconnect from " + c->GetIPAddress());
		//LogFile::Log("error", "WSASend(): " + Utilities::GetLastErrorAsString());

		c->RefCountAdjust(-1);
		c->DisconnectServer();
		c->FreeOperationData(olptr);
		closesocket(c->Socket());
		if(c->GetRefCount() == 0)
			RemoveClient(c);
	}
}

void Server::deliver(Client * c, const unsigned char * msg, int length)
{
	OVERLAPPEDEX * olptr = c->NewOperationData(OP_WRITE);
	memcpy(olptr->buffer, msg, length);
	olptr->wsabuf.len = length;
	olptr->totalBytes = length;
	OVERLAPPED * base_overlapped = static_cast<OVERLAPPED*>(olptr);

	c->RefCountAdjust(1);
	int wsaerr = WSASend(c->Socket(), &olptr->wsabuf, 1, NULL, 0, base_overlapped, NULL);

	if(wsaerr == 0)
	{
		//immediate success (A completion packet is still sent to unblock GetQueuedCompletionStatus!!, don't delete the OVERLAPPED)
		//LogFile::Log("status", "Immediate sent bytes: " + Utilities::itos(base_overlapped->InternalHigh));
	}
	else if(wsaerr == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING)
	{
		//queued for GetQueuedCompletionStatus
	}
	else if(wsaerr == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		//error
        //LogFile::Log("status", "(WSASend: deliver() immediate) Disconnect from " + c->GetIPAddress());
		//LogFile::Log("error", "WSASend(): " + Utilities::GetLastErrorAsString());

		c->RefCountAdjust(-1);
		c->DisconnectServer();
		c->FreeOperationData(olptr);
		closesocket(c->Socket());
		if (c->GetRefCount() == 0)
			RemoveClient(c);
	}
}

void Server::AddClient(std::shared_ptr<Client> client)
{
	EnterCriticalSection(&clientListCS);
	clients.push_back(client);
	LeaveCriticalSection(&clientListCS);
}

void Server::RemoveClient(Client * client)
{
	EnterCriticalSection(&clientListCS);
	std::list<std::shared_ptr<Client>>::iterator iter;
	for (iter = clients.begin(); iter != clients.end(); iter++)
	{
		if ((*iter).get() == client)
		{
			clients.erase(iter);
			break;
		}
	}
	LeaveCriticalSection(&clientListCS);
}

void Server::RemoveClient(std::shared_ptr<Client> client)
{
	EnterCriticalSection(&clientListCS);
	clients.remove(client);
	LeaveCriticalSection(&clientListCS);
}

//Connection spam ban list handling
void Server::UpdateIPList(std::string address)
{
	struct IPAddressInfo * ipinfo;
	double thetime = Utilities::GetTime();
	std::vector<struct IPAddressInfo>::iterator iter;

	iter = IPList.begin();
	while(iter != IPList.end())	//Use this opportunity to clear the list of IPs that haven't registered a new connection over the defined interval
	{
		ipinfo = &(*iter);
		std::vector<double>::iterator timeiter = ipinfo->connectTimes.begin();
		while (timeiter != ipinfo->connectTimes.end()) //go through the connection times for this address and remove any older than the interval
		{
			if ((*timeiter) + SPAM_INTERVAL < thetime)
				timeiter = ipinfo->connectTimes.erase(timeiter);
			else
				timeiter++;
		}
		if (ipinfo->connectTimes.empty())
			iter = IPList.erase(iter);
		else
			iter++;
	}

	iter = IPList.begin();
	while (iter != IPList.end())
	{
		ipinfo = &(*iter);
		if (ipinfo->address == address)	//Found an entry for this address
		{
			if (ipinfo->connectTimes.size() >= SPAM_MAX_CONNECTIONS_PER_INTERVAL)
			{
				//Ban!
				LogFile::Log("network", "Banning client due to connection spam: " + ipinfo->address);
				tempBanList.push_back({ ipinfo->address, thetime });
				IPList.erase(iter);
				return;
			}
			else
			{
				ipinfo->connectTimes.push_back(thetime);
				return;
			}
		}
		iter++;
	}

	//else add a new entry
	struct IPAddressInfo newip;
	newip.address = address;
	newip.connectTimes.push_back(thetime);
	IPList.push_back(newip);

	return;
}

//Return false if we should not accept a connection from this address
bool Server::CheckTempBanList(std::string address)
{
	std::vector<struct IPBanInfo>::iterator iter = tempBanList.begin();
	while (iter != tempBanList.end())
	{
		if ((*iter).address == address)
		{
			if ((*iter).banTime + SPAM_BANTIME < Utilities::GetTime())
			{
				tempBanList.erase(iter);
				return true;
			}
			return false;
		}
		iter++;
	}
	return true;
}