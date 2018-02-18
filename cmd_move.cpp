#include "stdafx.h"
#include "CSubscriber.h"
#include "CSubscriberManager.h"
#include "CmySQLQueue.h"
#include "CHighResTimer.h"
#include "CHelp.h"
#include "CTrigger.h"
#include "CClient.h"
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
#include "mud.h"

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
	if (ch->position == Character::Position::POSITION_SITTING)
	{
		ch->Send("You are already sitting.\n\r");
		return;
	}
	ch->Sit();
}

void cmd_stand(Character * ch, string argument)
{
	if (ch->position == Character::Position::POSITION_STANDING)
	{
		ch->Send("You are already standing.\n\r");
		return;
	}
	ch->Stand();
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
	Room * current = Game::GetGame()->GetRoom(ch->player->recall);
	if (current != nullptr)
	{
		ch->Send("Your current recall room is: " + current->name + "\n\r");
	}
	if(ch->room == NULL || !Utilities::FlagIsSet(ch->room->flags, Room::FLAG_RECALL))
	{
		ch->Send("You cannot set your recall location here.\n\r");
		return;
	}
	ch->SetQuery("Set recall location to: |W" + ch->room->name + "|X? (y/n) ", ch->room, cmd_recall_Query);
}

bool cmd_recall_Query(Character * ch, string argument)
{
	if (!Utilities::str_cmp(argument, "yes") || !Utilities::str_cmp(argument, "y"))
	{
		Room * recall = (Room*)ch->GetQueryData();
		ch->player->recall = recall->id;
		ch->Send("Recall location set.\n\r");
	}
	ch->QueryClear();
	return true;
}