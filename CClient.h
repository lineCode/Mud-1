#ifndef CCLIENT_H
#define CCLIENT_H

#define MAX_INPUT_LENGTH 256

struct OVERLAPPEDEX : OVERLAPPED
{
	WSABUF			wsabuf;
	char			buffer[MAX_INPUT_LENGTH];
	int				totalBytes;
	int				sentBytes;
	int				opCode;

	OVERLAPPEDEX()
	{
		wsabuf.buf = buffer;
		wsabuf.len = MAX_INPUT_LENGTH;
		totalBytes = sentBytes = opCode = 0;
		ZeroMemory(buffer, MAX_INPUT_LENGTH);
	};
};

typedef boost::shared_ptr<OVERLAPPEDEX> OVERLAPPEDEXPtr;

class Client
{
	public:
		Client(SOCKET s);
		~Client();

		char * receiveBuffer;       
		std::string inputBuffer;    //Dump the receive buffer here, then parse into commandQueue
		std::deque<std::string> commandQueue;
		bool disconnect;

		//Get/Set calls
		/*void SetOpCode(int n);
		int GetOpCode(int op_id);
		void SetTotalBytes(int n, int op_id);
		int GetTotalBytes(int op_id);
		void SetSentBytes(int n, int op_id);
		void IncrSentBytes(int n, int op_id);
		int GetSentBytes(int op_id);*/
		void SetSocket(SOCKET s);
		SOCKET Socket();
		/*void SetWSABUFLength(int nLength);
		int GetWSABUFLength();
		WSABUF* GetWSABUFPtr();
		OVERLAPPED* GetOVERLAPPEDPtr();
		void ResetWSABUF();*/
		OVERLAPPEDEXPtr NewOperationData(int op_type);
		void FreeOperationData(OVERLAPPEDEX * ol);
		/*void SetBuffer(char *szBuffer);
		void SetBufferLength(int len);
		void GetBuffer(char *szBuffer);
		WSABUF * GetWSABUFPtr(OVERLAPPEDEXPtr ol);*/
		//OVERLAPPED * GetOVERLAPPEDPtr();

	private:

		SOCKET socket_; //accepted socket

		CRITICAL_SECTION critical_section; //not sure if this is necessary

		std::list<OVERLAPPEDEXPtr> overlappedData;

		//OVERLAPPED		*m_pol;
		//WSABUF            *m_pwbuf;

		//int               m_nTotalBytes;
		//int               m_nSentBytes;
		int id_count;
};

#endif