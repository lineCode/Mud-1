#include "stdafx.h"
#include "CListener.h"
#include "CListenerManager.h"
#include "CmySQLQueue.h"
#include "CLogFile.h"
#include "CHighResTimer.h"
#include "CHelp.h"
#include "CTrigger.h"
#include "CClient.h"
typedef boost::shared_ptr<Client> Client_ptr;
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
typedef boost::shared_ptr<Server> Server_ptr;
#include "CCommand.h"
#include "mud.h"
#include "utils.h"

Reset::Reset()
{
    id = 0;
    type = 1;
    npc = NULL;
    interval = 30;
    wanderDistance = 0;
    leashDistance = 0;
    lastReset = 0;
    removeme = false;
}

Reset::~Reset()
{

}

//Character/Object this reset refers to has been deleted
void Reset::Notify(ListenerManager * lm)
{
    //LogFile::Log("status", "Reset " + Utilities::itos(id) + " setting npc = NULL");
    npc = NULL;
    lastReset = Game::currentTime;
}