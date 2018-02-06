#include "stdafx.h"
#include "CListener.h"
#include "CListenerManager.h"
#include "CmySQLQueue.h"
#include "CLogFile.h"
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

extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

//#include "luabind/luabind.hpp"

using namespace std;

Exit::Direction FindDirection(Character * ch, Character * target, int depth);

void cmd_track(Character * ch, string argument)
{
    string arg1;

    if(argument.empty())
    {
        ch->Send("Track who?\n\r");
        return;
    }

    argument = Utilities::one_argument(argument, arg1);

    Character * target = Game::GetGame()->GetPlayerWorld(ch, arg1);   

    if(target == NULL)
    {
        ch->Send("Player not found.\n\r");
        return;
    }
    if(ch == target)
    {
        ch->Send("You cannot track yourself.\n\r");
        return;
    }
    if(ch->room == target->room)
    {
        ch->Send("Try looking around.\n\r");
        return;
    }

    HighResTimer timer;
    timer.Reset();
    Exit::Direction dir = FindDirection(ch, target, 30);
    ch->Send("GetDirection took " + Utilities::dtos(timer.ElapsedMillis(), 5) + "ms\n\r");
    
    if(dir == Exit::DIR_LAST)
    {
        ch->Send("Could not find a path.\n\r");
        return;
    }

    ch->Send("You sense a trail " + Exit::exitNames[dir] + " from here.\n\r");
}

Exit::Direction FindDirection(Character * ch, Character * target, int depth)
{
    //Depth limited breadth first search

    if(!ch || !target || !ch->room || !target->room)
        return Exit::DIR_LAST;

    static std::map<int, Room *> visited; //Mark a node as visited with its parent Room so we can backtrack
    static std::deque<Room *> searchQ;
    Exit::Direction retval = Exit::DIR_LAST;
    int currentDepth = 0;

    if(ch->room->id == target->room->id)
    {
        return Exit::DIR_LAST;
    }

    searchQ.push_back(ch->room);
    visited[ch->room->id] = NULL;
    searchQ.push_back(NULL); //NULL as depth marker
    currentDepth++;

    while(/*!searchQ.empty()*/1) //searchQ will always have a room or NULL
    {
        Room * searchme = searchQ.front();
        searchQ.pop_front();
        
        if(searchme == NULL) //depth marker
        {
            if(searchQ.empty()) //if all the queue has is NULL, we couldn't find it
                break;

            searchQ.push_back(NULL);
            currentDepth++;
            if(currentDepth > depth)
            {
                //ch->Send("depth limit exceeded " + Utilities::itos(currentDepth) + "\n\r");
                break;
            }
            continue;
        }

        if(target->room == searchme)
        {
            //find "searchme" in "visited" and trace the path back to our room
            std::map<int, Room *>::iterator iter = visited.find(searchme->id);
            while(iter->second->id != ch->room->id)
            {
                iter = visited.find(iter->second->id);
            }
            for(int i = 0; i < Exit::DIR_LAST; i++)
            {
                if(ch->room->exits[i] && ch->room->exits[i]->to->id == iter->first)
                {
                    retval = (Exit::Direction)i;
                }
            }
            visited.clear();
            searchQ.clear();
            return retval;
        }
        for(int i = 0; i < Exit::DIR_LAST; i++)
        {
            if(searchme->exits[i] && searchme->exits[i]->to)
            {
                if(visited.find(searchme->exits[i]->to->id) == visited.end())
                {
                    //ch->Send("searching room " + Utilities::itos(searchme->exits[i]->to->id) + "\n\r");
                    visited[searchme->exits[i]->to->id] = searchme;
                    searchQ.push_back(searchme->exits[i]->to);
                }
            }
        }
    }

    visited.clear();
    searchQ.clear();
    return Exit::DIR_LAST;
}
