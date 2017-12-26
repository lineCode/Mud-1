#include "stdafx.h"
#include "CListener.h"
#include "CListenerManager.h"
#include "CmySQLQueue.h"
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
#include "utils.h"

using namespace std;

void cmd_north(Character * ch, string argument)
{
    ch->Move(Exit::DIR_NORTH);
}

void cmd_east(Character * ch, string argument)
{
    ch->Move(Exit::DIR_EAST);
}

void cmd_south(Character * ch, string argument)
{
    ch->Move(Exit::DIR_SOUTH);
}

void cmd_west(Character * ch, string argument)
{
    ch->Move(Exit::DIR_WEST);
}

void cmd_northeast(Character * ch, string argument)
{
    ch->Move(Exit::DIR_NORTHEAST);
}

void cmd_southeast(Character * ch, string argument)
{
    ch->Move(Exit::DIR_SOUTHEAST);
}

void cmd_southwest(Character * ch, string argument)
{
    ch->Move(Exit::DIR_SOUTHWEST);
}

void cmd_northwest(Character * ch, string argument)
{
    ch->Move(Exit::DIR_NORTHWEST);
}

void cmd_up(Character * ch, string argument)
{
    ch->Move(Exit::DIR_UP);
}

void cmd_down(Character * ch, string argument)
{
    ch->Move(Exit::DIR_DOWN);
}

void cmd_sit(Character * ch, string argument)
{
    ch->Send("cmd_sit\n\r");
}

void cmd_stand(Character * ch, string argument)
{
    ch->Send("cmd_stand\n\r");
}

void cmd_sleep(Character * ch, string argument)
{
    ch->Send("cmd_sleep\n\r");
}

void cmd_wake(Character * ch, string argument)
{
    ch->Send("cmd_wake\n\r");
}

void cmd_recall(Character * ch, string argument)
{
	if(ch == NULL || ch->player == NULL)
		return;
	if(ch->room == NULL || !Utilities::FlagIsSet(ch->room->flags, Room::FLAG_RECALL))
	{
		ch->Send("You cannot set your recall location here.\n\r");
		return;
	}
	ch->player->recall = ch->room->id;
    ch->Send("Recall location set.\n\r");
}