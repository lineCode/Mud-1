#include "stdafx.h"

using namespace std;

extern class Command cmd_table[];

void cmd_attack(Character * ch, string argument)
{
    if(!ch)
        return;

    string arg1;
    argument = Utilities::one_argument(argument, arg1);

    Character * target;
    if(arg1.empty())
    {
        if(ch->GetTarget())
        {
            target = ch->GetTarget();
			if (target->room != ch->room)
			{
				ch->Send("They aren't here.\n\r");
				return;
			}
        }
        else
        {
            ch->Send("Attack who?\n\r");
            return;
        }
    }
    else
    {
        target = ch->GetCharacterRoom(arg1);
        if(target == NULL)
        {
            ch->Send("They aren't here.\n\r");
            return;
        }
    }
	if(!target->IsAlive())
	{
		ch->Send("That target is already dead!\n\r");
		return;
	}
    if(target == ch)
    {
        ch->Send("You can't attack yourself!\n\r");
        return;
    }
    if((target->IsNPC() && Utilities::FlagIsSet(target->flags, Character::FLAG_FRIENDLY))
		|| (!target->IsNPC() && ch->room->pvp == 0))
    {
        ch->Send("You can't attack that target.\n\r");
        return;
    }

    ch->SetTarget(target);
    ch->Send("You begin attacking " + target->name + "!\n\r");
    target->Send(ch->name + " begins attacking you!\n\r");

    ch->EnterCombat(target);
    target->EnterCombat(ch);

    //Do first attack here so attacker has a minor advantage
    ch->AutoAttack(target);
}

/*void cmd_autoattack(Character * ch, string argument)
{
    //TODO 'set' autoattack to turn on/off automatic retaliation?
}*/

void cmd_cancel(Character * ch, string argument)
{
    if(ch && ch->CancelActiveDelay())
    {
        ch->Send("Action Cancelled!\n\r");
    }
    else if(ch && ch->meleeActive)
    {
        ch->Send("Melee attack cancelled.\n\r");
        ch->meleeActive = false;
    }
    else
    {
        ch->Send("No action is in progress.\n\r");
    }
}

void cmd_look(Character * ch, string argument)
{
    string arg1;
    argument = Utilities::one_argument(argument, arg1);

    if(arg1.empty()) //"look" with no argument
    {
        if(ch->room == NULL)
	    {
            LogFile::Log("error", "cmd_look: ch->room == NULL");
		    return;
	    }

	    Room * inroom = ch->room;
        bool exitstatus[Exit::DIR_LAST];
	    for(int i = 0; i < Exit::DIR_LAST; i++)
	    {
		    if(inroom->exits[i] && inroom->exits[i]->to)
			    exitstatus[i] = true;
		    else
			    exitstatus[i] = false;
	    }
    /*
     Northern Courtyard                                     --    N    --
    (-------------------------------------------------)     - <---X---> -
                                                            --    S    --
    */
        stringstream roomstream;
        roomstream << " |W" << left << setw(55) << inroom->name <<  
            setw(8) << (exitstatus[7] ? "|WNW" : "|B--") <<
            setw(7) << (exitstatus[0] ? "|WN" : "|B-") <<
            (exitstatus[1] ? "|WNE" : "|B--") << "|X\n\r";
        roomstream << left << setw(58) << "|B(-------------------------------------------------)" <<
            (exitstatus[6] ? "|WW" : "|B-") << "|Y <-" << (exitstatus[8] ? "|WU|Y" : "-") << "-X-" <<
            (exitstatus[9] ? "|WD|Y" : "-") << "-> " << (exitstatus[2] ? "|WE" : "|B-") << "|X\n\r";

        string areaname = "|X";
        Area * this_area = Game::GetGame()->GetArea(inroom->area);
        if(this_area != NULL)
        {
            //TODO add pvp room rules to Game somewhere...
            if(this_area->pvp == 0) //no pvp whatsoever
                areaname = "|C";
            else if(this_area->pvp == 1) //dueling and guild pvp
                areaname = "|G";
            else if(this_area->pvp == 2) //everyone attacks everyone
                areaname = "|Y";
            else if(this_area->pvp == 3) //everyone attacks everyone, lose all your equipment
                areaname = "|R";

            areaname += this_area->name;
        }

		roomstream << " " << left << setw(26) << areaname;
		std::string roomflags = "";
		if (Utilities::FlagIsSet(inroom->flags, Room::Flags::FLAG_RECALL))
			roomflags += "[Recall]";
		if (ch->player && ch->player->IMMORTAL())
			roomflags += "[Room ID: " + Utilities::itos(inroom->id) + "]";

		roomstream << "|Y" << right << setw(26) << roomflags;
		roomstream << right << setw(9) << (exitstatus[5] ? "|WSW" : "|B--") <<
					  setw(7) << (exitstatus[4] ? "|WS" : "|B-") <<
					  setw(8) << (exitstatus[3] ? "|WSE" : "|B--") << "|X\n\r\n\r";

        ch->Send(roomstream.str());

	    /*if(ch->in_room->area->terrainmap && ch->playerData)
	    {
		    string terrainbuf = "";
		    for(int i = ch->playerData->aty - 7; i <= ch->playerData->aty + 7; i++)
		    {
			    for(int j = ch->playerData->atx - 15; j <= ch->playerData->atx + 15; j++)
			    {
				    if(i == ch->playerData->aty && j == ch->playerData->atx)
				    {
					    terrainbuf += "|M@";
				    }
				    /*else if((int)ch->playerData->map[i][j].rgbtGreen == 1)
				    {
					    terrainbuf += "|YO";
				    }*/
				    /*else
				    {
					    terrainbuf += ch->in_room->area->terrainmap->terrainCodes[((int)ch->playerData->map[i][j].rgbtRed)];
				    }
			    }
			    terrainbuf += '\n';
			    ch->SendToChar(terrainbuf);
			    terrainbuf = "";
		    }
	    }*/
        if(!inroom->description.empty())
	        ch->Send("  " + inroom->description);

		cmd_scan(ch, "");
		ch->Send("\n\r");

		if (!ch->IsGhost() || (ch->IsGhost() && (inroom->id == ch->player->corpse_room || inroom->id == ch->player->graveyard_room)))
		{
			for (auto itemiter = inroom->items.begin(); itemiter != inroom->items.end(); itemiter++)
			{
				if(!(*itemiter)->inroom_name.empty())
					ch->Send((*itemiter)->inroom_name + "\n\r");
				else
					ch->Send((*itemiter)->name + "\n\r");
			}

			std::list<Character *>::iterator i;
			for (i = inroom->characters.begin(); i != inroom->characters.end(); i++)
			{
				if (ch == (*i))
					continue;

				if (ch->player && !ch->player->IMMORTAL() && (*i)->IsGhost())
				{
					continue;
				}

				//Determine appropriate quest icon. Hierarchy |Y?, |Y!, |W?, |D!
				//Search quests this npc ends
				string questicon = "";
				if ((*i)->IsNPC())
				{
					bool foundyellow = false; bool foundwhite = false;
					if (ch->player && !(*i)->questEnd.empty())
					{
						std::vector<Quest *>::iterator questiter;
						for (questiter = (*i)->questEnd.begin(); questiter != (*i)->questEnd.end(); ++questiter)
						{
							if (ch->player->QuestObjectivesComplete(*questiter))
							{
								questicon = "|Y[?] ";
								foundyellow = true;
								break;
							}
							else if (ch->player->QuestActive(*questiter))
							{
								questicon = "|W[?] ";
								foundwhite = true;
							}
						}
					}
					//Search quests this npc starts
					if (!foundyellow && ch->player && !(*i)->questStart.empty())
					{
						std::vector<Quest *>::iterator questiter;
						for (questiter = (*i)->questStart.begin(); questiter != (*i)->questStart.end(); ++questiter)
						{
							if (ch->player->QuestEligible(*questiter))
							{
								//TODO: white ! for soon to be available quests
								if (!foundwhite && Quest::GetDifficulty(ch->level, (*questiter)->level) == 0)
								{
									questicon = "|Y[|D!|Y] ";
								}
								else
								{
									questicon = "|Y[!] ";
									break;
								}
							}
						}
					}
				}
				string disconnected = "";
				string title = "";
				string corpse = "";
				string fighting = ".";
				string level = "";
				string aggressionColor = "|G";
				string tapped = "";

				if ((*i)->player && (*i)->player->user && !(*i)->player->user->IsConnected())
					disconnected = "|Y[DISCONNECTED] |X";

				if ((*i)->player)
					title = (*i)->title;
				else if (!(*i)->title.empty())
					title = " <" + (*i)->title + ">";

				level += "<" +
					Game::LevelDifficultyColor(Game::LevelDifficulty(ch->level, (*i)->level))
					+ Utilities::itos((*i)->level) + "|X> ";

				if ((*i)->IsCorpse())
				{
					corpse = "|DThe corpse of ";
				}
				if (ch->player && ch->player->IMMORTAL() && (*i)->IsGhost())
				{
					corpse = "The ghost of ";
				}
				if ((*i)->InCombat())
				{
					if ((*i)->IsFighting(ch))
						fighting = ", fighting YOU!";
					else if ((*i)->GetTarget() != NULL)
						fighting = ", fighting " + (*i)->GetTarget()->name + ".";
				}
				aggressionColor = ch->AggressionColor((*i));
				Character * tappedBy = (*i)->GetTap();
				if (tappedBy)
				{
					tapped = " |D(tapped by " + tappedBy->GetName() + ")";
				}
				ch->Send(disconnected + level + questicon + aggressionColor + corpse + (*i)->name + title + " is here" + fighting + tapped + "|X\n\r");
			}
			ch->Send("\n\r");	
		}
    }
    else // "look argument" //TODO, look at things in the room with higher priority
    {
        //TODO definitely want to make a function out of this
        Item * inspect = ch->player->GetItemInventory(arg1);
        if(inspect == NULL)
        {
            inspect = ch->player->GetItemEquipped(arg1);
        }
        if(!inspect)
        {
            ch->Send("You don't see that here.\n\r");
            return;
        }
		ch->Send(inspect->FormatItemInfo());
    }
}

// adds the target's health/mana/energy/rage to the players prompt
// spells/skills done to a character automatically target that character
// allow targeting in same room and adjacent rooms
void cmd_target(Character * ch, string argument)
{
    if(argument.empty())
	{
		ch->Send("Target cleared.\n\r");
		ch->ClearTarget();
        ch->meleeActive = false; //Any target change stops auto attack
		return;
	}

	string arg1;
	string direction;
    argument = Utilities::one_argument(argument, arg1);
	argument = Utilities::one_argument(argument, direction);

	Character *vch;
	if((vch = ch->GetCharacterAdjacentRoom(arg1, direction)) != NULL ||
        (ch->player && ch->player->IMMORTAL() 
         && (vch = Game::GetGame()->GetPlayerWorld(ch, arg1)) != NULL))
	{
        if(ch->GetTarget() != vch)
            ch->meleeActive = false; //Any target change stops auto attack
		ch->SetTarget(vch);
		ch->Send("Targeting " + vch->name + "\n\r");
	}
    else
    {
        ch->Send("Target not found.\n\r");
    }
}

void cmd_commands(Character * ch, string argument)
{
	Command * whichTable = Command::GetCurrentCmdTable(ch);

    int newline = 1;
    for(int cmd = 0; whichTable[cmd].name.length() > 0; cmd++)
	{
		if (whichTable[cmd].level >= 0 || ch->player->IMMORTAL())
		{
			ch->Send(whichTable[cmd].name + "  ");
			newline++;
		}
		if(newline % 5 == 0)
			ch->Send("\n\r");
		
	}
	ch->Send("\n\r");
}

void cmd_score(Character * ch, string argument)
{
    if(!ch || !ch->player)
        return;

    ch->Send("Player information for " + ch->name + ":\n\r");
    ch->Send("Level: " + Utilities::itos(ch->level) + "\n\r");
    ch->Send("Class: " + ch->player->currentClass->name + "\n\r");
    ch->Send("Class Level: " + Utilities::itos(ch->player->GetClassLevel(ch->player->currentClass->id)) + "\n\r");
    ch->Send("Class History: ");

    bool found = false;
    std::list<Player::ClassData>::iterator iter;
    for(iter = ch->player->classList.begin(); iter != ch->player->classList.end(); ++iter)
    {
        Class * c = Game::GetGame()->GetClass((*iter).id);
        ch->Send("[" + c->name + " " + Utilities::itos((*iter).level) + "] ");
        found = true;
    }
    if(found)
    {
        ch->Send("\n\r");
    }
    else
    {
        ch->Send("None\n\r");
    }
    if(ch->player->IMMORTAL())
        ch->Send("Immortal Level: " + Utilities::itos(ch->player->immlevel) + "\n\r");
    ch->Send("Health: " + Utilities::itos(ch->GetHealth()) + "/" + Utilities::itos(ch->GetMaxHealth()));
    ch->Send("  Mana: " + Utilities::itos(ch->GetMana()) + "/" + Utilities::itos(ch->GetMaxMana()));
    ch->Send("  Energy: " + Utilities::itos(ch->GetEnergy()) + "/" + Utilities::itos(ch->GetMaxEnergy()));
	ch->Send("  Rage: " + Utilities::itos(ch->GetRage()) + "/" + Utilities::itos(ch->GetMaxRage()) + "\n\r");
    ch->Send("Agility: " + Utilities::itos(ch->agility) + " Intellect: " + Utilities::itos(ch->intellect)
        + " Strength: " + Utilities::itos(ch->strength) + " Stamina: " + Utilities::itos(ch->stamina) + " Wisdom: "
        + Utilities::itos(ch->wisdom) + " Spirit: " + Utilities::itos(ch->spirit) + "\n\r");
	ch->Send("Attribute Points available: " + Utilities::itos(ch->player->statPoints) + "\n\r");
    ch->Send("Experience: " + Utilities::itos(ch->player->experience) + "\n\r");
    ch->Send("You have " + Utilities::itos(ch->player->experience) + " experience and need " 
                    + Utilities::itos(Game::ExperienceForLevel(ch->level+1)) + " experience to reach level " 
                    + Utilities::itos(ch->level+1) + " (" + Utilities::itos((Game::ExperienceForLevel(ch->level+1) - ch->player->experience))
                    + " tnl)\n\r");
    double movespeed = ch->GetMoveSpeed();
    ch->Send("Current movement speed: " + Utilities::dtos(ch->movementSpeed * movespeed, 2) + " rooms per second ("
        + Utilities::dtos(movespeed*100, 0) + "% of normal)\n\r");
	if (!ch->IsAlive())
	{
		int res_at_graveyard = ch->player->death_timer - ch->TimeSinceDeath();
		int res_at_corpse = ch->player->death_timer_runback - ch->TimeSinceDeath();
		if(res_at_graveyard > 0)
			ch->Send("|W" + Utilities::itos(res_at_graveyard) + " seconds before you can resurrect at the graveyard.\n\r");
		if (res_at_corpse > 0)
			ch->Send("|W" + Utilities::itos(res_at_corpse) + " seconds before you can resurrect near your corpse.\n\r");
	}
}

void cmd_scan(Character * ch, string argument)
{
	if (!ch || !ch->room)
		return;

	if (!ch->IsAlive())
		return;

	std::list<Character *>::iterator iter;
	Room * scan_room;
	stringstream out;
	int depth = 3;
	//string depthcolors[3] = { "|R", "|Y", "|G" };
	string depthstring[3] = { "||  ", "|||| ", "||||||" };
	bool found = false;
	string level = "";
	string corpse = "";

	for (int i = 0; i < Exit::DIR_LAST; i++)
	{
		out << setw(11) << left << Exit::exitNames[i] + ": ";
		scan_room = ch->room;
		for(int j = 0; j < depth; j++)
		{
			if (scan_room->exits[i] && scan_room->exits[i]->to)
			{
				scan_room = scan_room->exits[i]->to;
			}
			else
			{
				break;
			}
			if(scan_room->HasNonGhostCharacters())
			{
				if (found == true)
				{
					out << setw(13) << left << "\n\r";
				}
				found = true;
				out << "|W[" << /*depthcolors[j]*/"|202" << depthstring[j] << "|W]:|X ";
				std::list<Character *>::iterator iter = scan_room->characters.begin();
				while (iter != scan_room->characters.end())
				{
					if (!(*iter)->IsAlive())
						corpse = "corpse of ";
					else
						corpse = "";

					//if (j == 0) //depth
					{
						level = "<" +
							Game::LevelDifficultyColor(Game::LevelDifficulty(ch->level, (*iter)->level))
							+ Utilities::itos((*iter)->level) + "|X>";
						out << level << ch->AggressionColor(*iter) << corpse << (*iter)->name;
					}
					/*else
					{
						level = "<" +
							Game::LevelDifficultyLightColor(Game::LevelDifficulty(ch->level, (*iter)->level))
							+ Utilities::itos((*iter)->level) + "|X>";
						out << level << ch->AggressionLightColor(*iter) << corpse << (*iter)->name;
					}*/
					iter++;
					if (iter != scan_room->characters.end())
					{
						out << "|X, ";
					}
				}
				out << "|X ";
			}
		}
		if (found)
		{
			out << "|X\n\r";
			ch->Send(out.str());
			found = false;
		}
		out.str("");
		out.clear();
	}
}

void cmd_who(Character * ch, string argument)
{
	string arg1;
	argument = Utilities::one_argument(argument, arg1);

	if (arg1.empty()) //"who" with no arguments
	{
		ch->Send("|YPlayers online: |X\n\r");
		ch->Send("|B`-------------------------------------------------------------------------------'|X\n\r");
		int found = 0;
		Player * wplayer;
		std::list<User *>::iterator iter;
		std::map<int, Class *>::iterator classiter;
		Class * myclass = nullptr;
		std::stringstream sstr;
		for (iter = Game::GetGame()->users.begin(); iter != Game::GetGame()->users.end(); ++iter)
		{
			if ((*iter)->IsConnected() && (*iter)->connectedState == User::CONN_PLAYING && (*iter)->character && (*iter)->character->player)
			{
				wplayer = (*iter)->character->player;
				sstr.str("");
				sstr.clear();
				//name
				sstr << " " << left << setw(12) << (*iter)->character->name;
				//total level
				sstr << " |B[" << right;
				if (wplayer->IMMORTAL())
				{
					sstr << "|Y" << setw(3) << wplayer->immlevel;

				}
				else
				{
					sstr << "|W" << setw(3) << (*iter)->character->level;
				}
				sstr << "|B]";
				//gender
				sstr << " |B" << right << ((*iter)->character->gender == 1 ? "M" : "F");
				//race
				sstr << " |B" << left << setw(8) << Character::race_table[(*iter)->character->race].name << " ";
				//in combat & ghost
				sstr << left << ((*iter)->character->InCombat() ? "|R<X>" : "   ") << ((*iter)->character->IsGhost() ? "|D<G>" : "   ");
				//area
				if ((*iter)->character->room)
				{
					Area * a = Game::GetGame()->GetArea((*iter)->character->room->area);
					if (a)
						sstr << " |B[|C" << a->name << "|B]";
				}
				sstr << "|X\n\r";

				ch->Send(sstr.str());
				found++;
			}
		}
		ch->Send("|B`-------------------------------------------------------------------------------'|X\n\r");
		ch->Send(Utilities::itos(found) + " players found.\n\r");
	}
	else //"who" with argument. For now just look up by character name
	{
		Player * wplayer;
		std::list<User *>::iterator iter;
		std::map<int, Class *>::iterator classiter;
		Class * myclass = nullptr;
		std::stringstream sstr;
		for (iter = Game::GetGame()->users.begin(); iter != Game::GetGame()->users.end(); ++iter)
		{
			if ((*iter)->IsConnected() && (*iter)->connectedState == User::CONN_PLAYING && (*iter)->character 
				&& (*iter)->character->player && !Utilities::str_cmp((*iter)->character->name, arg1))
			{ 
				wplayer = (*iter)->character->player;
				ch->Send("|B`-------------------------------------------------------------------------------'|X\n\r");
				sstr.str("");
				sstr.clear();
				//name
				sstr << " " << left << setw(12) << (*iter)->character->name;
				//total level
				sstr << " |B[" << right;
				if (wplayer->IMMORTAL())
				{
					sstr << "|Y" << setw(3) << wplayer->immlevel;

				}
				else
				{
					sstr << "|W" << setw(3) << (*iter)->character->level;
				}
				sstr << "|B]";
				//gender
				sstr << " |B" << right << ((*iter)->character->gender == 1 ? "M" : "F");
				//race
				sstr << " |B" << left << setw(8) << Character::race_table[(*iter)->character->race].name << " ";
				//in combat & ghost
				sstr << left << ((*iter)->character->InCombat() ? "|R<X>" : "   ") << ((*iter)->character->IsGhost() ? "|D<G>" : "   ");
				//area
				if ((*iter)->character->room)
				{
					Area * a = Game::GetGame()->GetArea((*iter)->character->room->area);
					if (a)
						sstr << " |B[|C" << a->name << "|B]";
				}
				//classes
				sstr << "\n\r              ";
				if (wplayer->IMMORTAL())
				{
					sstr << "|B[|WImmortal|B]";
				}
				else
				{
					for (classiter = Game::GetGame()->classes.begin(); classiter != Game::GetGame()->classes.end(); classiter++)
					{
						myclass = (*classiter).second;
						sstr << "|B[" << myclass->color << myclass->name << " " << right << setw(3) << Utilities::itos(wplayer->GetClassLevel(myclass->id)) << "|B]";
					}
				}
				sstr << "|X\n\r";
				ch->Send(sstr.str());
				ch->Send("|B`-------------------------------------------------------------------------------'|X\n\r");
			}
		}
	}
}

void cmd_title(Character * ch, string argument)
{   
    if(!ch || !ch->player)
        return;

    if(argument.empty())
	{
		ch->Send("Title cleared.\n\r");
        ch->title.clear();
		return;
	}
	if(argument.length() > 45)
    {
        ch->Send("Title too long. Shortened to 45 characters.\n\r");
		argument = argument.substr(0, 45);
    }

	if(ch->player)
	{
		if(argument[0] != '.' && argument[0] != ',' && argument[0] != '!' && argument[0] != '?' )
		{
			argument = ' ' + argument;
		}
		ch->title = argument;
		ch->Send("Title set.\n\r");
	}
}

void cmd_group(Character * ch, string argument)
{
	if (!ch || ch->IsNPC())
		return;

	string arg1;
	string arg2;
	Character * vch;

	argument = Utilities::one_argument(argument, arg1);
	
	if (!Utilities::str_cmp(arg1, "invite"))
	{
		argument = Utilities::one_argument(argument, arg2);

		if (ch->HasGroup() && !ch->group->IsGroupLeader(ch))
		{
			ch->Send("Only the group leader can invite players.\n\r");
			return;
		}
		if (arg2.empty())
		{
			ch->Send("Invite who?\n\r");
			return;
		}
		int slot = -1;
		if (ch->HasGroup())
			slot = ch->group->GetNextFreeSlot();
		if (ch->HasGroup() && slot == -1)
		{
			ch->Send("Your group is full.\n\r");
			return;
		}
		if ((vch = Game::GetGame()->GetCharacterByPCName(arg2)) != NULL)
		{
			if (vch == ch)
			{
				ch->Send("You can't invite yourself.\n\r");
				return;
			}
			if (vch->HasGroup())
			{
				ch->Send("That player already has a group.\n\r");
				return;
			}
			if (vch->HasQuery())
			{
				ch->Send("That player is busy.\n\r");
				return;
			}
			ch->Send("You invite " + vch->name + " to join your group.\n\r");
			vch->SetQuery(ch->name + " has invited you to join " + ch->HisHer() + " group ('accept'/'decline') ", (void*)ch, cmd_groupQuery);
			ch->AddSubscriber(vch); //If the person asking us to join disappears before we answer it's a problem
			vch->Send("\n\r");
		}
		else
		{
			ch->Send("Player not found.\n\r");
			return;
		}
		return;
	}
	if (!Utilities::str_cmp(arg1, "remove"))
	{
		argument = Utilities::one_argument(argument, arg2);

		if (!ch->HasGroup())
		{
			ch->Send("You aren't in a group.\n\r");
			return;
		}
		if (!ch->group->IsGroupLeader(ch))
		{
			ch->Send("Only the group leader can remove players.\n\r");
			return;
		}
		if (arg2.empty())
		{
			ch->Send("Who would you like to remove from your group?\n\r");
			return;
		}
		Character * remove_me = ch->group->FindByName(arg2);
		if (remove_me == nullptr)
		{
			ch->Send("A player with that name is not in your group.\n\r");
			return;
		}
		if (remove_me == ch)
		{
			ch->Send("You cannot remove yourself from the group. Use \"group leave\".\n\r");
			return;
		}
		remove_me->Send("You have been removed from the group.\n\r");
		ch->group->Remove(remove_me);
		ch->Message(remove_me->name + " has been removed from the group.", Character::MSG_GROUP);

		if (ch->group->GetMemberCount() <= 1)
		{
			ch->Send("Your group has been disbanded.\n\r");
			delete ch->group;
			ch->group = nullptr;
		}
		return;
	}
	if (!Utilities::str_cmp(arg1, "leave"))
	{
		if (!ch->HasGroup())
		{
			ch->Send("You aren't in a group.\n\r");
			return;
		}
		ch->Message(ch->name + " has left the group.", Character::MSG_GROUP_NOTCHAR);
		ch->Send("You have left the group.\n\r");
		if (ch->group->GetMemberCount() <= 2)
		{
			ch->Message("Your group has been disbanded.", Character::MSG_GROUP_NOTCHAR);
			for (int i = 0; i < Group::MAX_RAID_SIZE; i++)
			{
				if (ch->group->members[i] != nullptr && ch->group->members[i] != ch)
				{
					ch->group->members[i]->group = nullptr;
				}
			}
			delete ch->group;
			ch->group = nullptr;
		}
		else
		{
			if (ch->group->IsGroupLeader(ch))
			{
				for (int i = 0; i < Group::MAX_RAID_SIZE; i++)
				{
					if (ch->group->members[i] != ch && ch->group->members[i] != nullptr)
					{
						ch->group->leader = ch->group->members[i];
						ch->group->leader->Message(ch->group->leader->name + " is now the group leader.", Character::MSG_GROUP_NOTCHAR);
						ch->group->leader->Send("You are now the group leader.\n\r");
						break;
					}
				}
			}
			ch->group->Remove(ch);
		}
		return;
	}
	if (!Utilities::str_cmp(arg1, "raid"))
	{
		if (!ch->HasGroup())
		{
			ch->Send("You aren't in a group.\n\r");
			return;
		}
		if (!ch->group->IsGroupLeader(ch))
		{
			ch->Send("You aren't the group leader.\n\r");
			return;
		}
		ch->group->israid = true;
		ch->Message("You have joined a raid group.", Character::MSG_GROUP);
		return;
	}
	if (!Utilities::str_cmp(arg1, "leader"))
	{
		if (!ch->HasGroup())
		{
			ch->Send("You aren't in a group.\n\r");
			return;
		}
		if (!ch->group->IsGroupLeader(ch))
		{
			ch->Send("You aren't the group leader.\n\r");
			return;
		}
		argument = Utilities::one_argument(argument, arg2);

		if (arg2.empty())
		{
			ch->Send("Who would you like to make the new group leader?\n\r");
			return;
		}
		Character * new_leader = ch->group->FindByName(arg2);
		if (new_leader == nullptr)
		{
			ch->Send("A player with that name is not in your group.\n\r");
			return;
		}
		if (new_leader == ch)
		{
			ch->Send("You are already the group leader.\n\r");
			return;
		}
		ch->group->leader = new_leader;
		new_leader->Message(new_leader->name + " is now the group leader.", Character::MSG_GROUP_NOTCHAR);
		new_leader->Send("You are now the group leader.\n\r");
		return;
	}
	if (!Utilities::str_cmp(arg1, "talk"))
	{
		if (!ch->HasGroup())
		{
			ch->Send("You aren't in a group.\n\r");
			return;
		}
		if (argument.empty())
		{
			ch->Send("What would you like to say?\n\r");
			return;
		}
		ch->Send("|CYou say to the group: '" + argument + "|C'|X\n\r");
		ch->Message("|C" + ch->name + " says to the group, '" + argument + "|C'|X", Character::MSG_GROUP_NOTCHAR);
		return;
	}
	if (!Utilities::str_cmp(arg1, "show"))
	{
		if (!ch->HasGroup())
		{
			ch->Send("You aren't in a group.\n\r");
			return;
		}
		stringstream group_format;

		//Display non-raid group
		if (!ch->group->israid)
		{
			group_format << "Your group (" << ch->group->GetMemberCount() << "/" << Group::MAX_GROUP_SIZE << "):\n\r";
			for (int i = 0; i < Group::MAX_GROUP_SIZE; i++)
			{
				if (ch->group->members[i] != nullptr)
				{
					group_format << ch->group->members[i]->name;
					if (ch->group->members[i] == ch->group->leader)
					{
						group_format << "|Y*|X";
					}
					group_format << "\n\r";
				}
			}
		}
		//Display raid group
		else
		{
			group_format << "Your raid group (" << ch->group->GetMemberCount() << "/" << Group::MAX_RAID_SIZE << "):\n\r";
			for (int format_loop = 0; format_loop < (Group::MAX_RAID_SIZE / Group::MAX_GROUP_SIZE) / 2; format_loop++)
			{
				group_format << "  Group " << (format_loop * 2) + 1 << "                   ";
				group_format << "Group " << (format_loop * 2) + 2 << "\n\r";
				for (int i = 0; i < Group::MAX_GROUP_SIZE; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						Character * current_member = ch->group->members[i + (format_loop * 8) + (j * 4)];
						if (current_member != nullptr)
						{
							current_member->InCombat() ? group_format << "|R(X)|X" : group_format << "   ";
							current_member == ch->group->leader ? group_format << "|Y*|X" : group_format << " ";

							group_format << "|C" << left << setw(12) << current_member->name;
						}
						else
						{
							group_format << "Empty         ";
						}
					}
					group_format << "\n\r";
				}
				group_format << "\n\r";
			}
		}
		ch->Send(group_format.str());
		return;
	}
	ch->Send("group invite <player>\n\r");
	ch->Send("group show\n\r");
	ch->Send("group remove <player>\n\r");
	ch->Send("group leave\n\r");
	ch->Send("group talk <message>\n\r");
	//ch->Send("group raid\n\r");
	//ch->Send("group leader <player>\n\r");
	//ch->Send("group move <player> <group #>\n\r");
}

bool cmd_groupQuery(Character *ch, string argument)
{
	Character * vch = (Character *)ch->GetQueryData();

	if (!Utilities::str_cmp(argument, "accept"))
	{
		vch->RemoveSubscriber(ch);
		ch->QueryClear();

		if (ch->HasGroup())
		{
			ch->Send("You already have your own group.\n\r");
			return true;
		}

		if (!vch->HasGroup()) //start a new group
		{
			vch->group = new Group(vch);
			vch->group->Add(ch);
			vch->Send(ch->name + " has joined your group.\n\r");
			ch->Send("You have joined " + vch->name + "'s group.\n\r");
		}
		else //joining an existing group
		{
			int slot = vch->group->GetNextFreeSlot();
			if ((!vch->group->israid && slot >= Group::MAX_GROUP_SIZE) || slot == -1)
			{
				ch->Send("That group is full.\n\r");
			}
			else
			{
				if (vch->group->Add(ch))
				{
					vch->Send(ch->name + " has joined your group.\n\r");
					ch->Send("You have joined " + vch->name + "'s group.\n\r");
				}
			}
		}
		return true;
	}
	else if (!Utilities::str_cmp(argument, "decline"))
	{
		vch->Send(ch->name + " declines your group invitation.\n\r");
		ch->QueryClear();
		vch->RemoveSubscriber(ch);
		return true;
	}
	return false;
}

void cmd_class(Character * ch, string argument)
{
    if(!ch || !ch->player)
        return;

    string arg1;
    string arg2;
    argument = Utilities::one_argument(argument, arg1);
    argument = Utilities::one_argument(argument, arg2);

    Class * currentClass = ch->player->currentClass;

    if(!Utilities::str_cmp(arg1, "info"))
    {
        ch->Send("Current class: [" + currentClass->name + " " 
                   + Utilities::itos(ch->player->GetClassLevel(currentClass->id)) + "]\n\r");
        ch->Send("Class history: ");
        bool found = false;
        std::list<Player::ClassData>::iterator iter;
        for(iter = ch->player->classList.begin(); iter != ch->player->classList.end(); ++iter)
        {
            Class * c = Game::GetGame()->GetClass((*iter).id);
            ch->Send("[" + c->name + " " + Utilities::itos((*iter).level) + "] ");
            found = true;
        }
        if(found)
        {
            ch->Send("\n\r");
        }
        else
        {
            ch->Send("None\n\r");
        }
        return;
    }
    else if(!Utilities::str_cmp(arg1, "newclass"))
    {
        if(arg2.empty())
        {
			ch->Send("Valid classes are: ");
			std::map<int, Class *>::iterator iter;
			for (iter = Game::GetGame()->classes.begin(); iter != Game::GetGame()->classes.end(); ++iter)
			{
				ch->Send(" " + (*iter).second->name);
			}
			ch->Send("\n\r");
            return;
        }
        Class * newclass = Game::GetGame()->GetClassByName(arg2);
        if(newclass == NULL)
        {
			ch->Send("Valid classes are: ");
			std::map<int, Class *>::iterator iter;
			for (iter = Game::GetGame()->classes.begin(); iter != Game::GetGame()->classes.end(); ++iter)
			{
				ch->Send(" " + (*iter).second->name);
			}
			ch->Send("\n\r");
            return;
        }
        ch->player->currentClass = newclass;
        ch->Send("Class changed to " + newclass->name + ".\n\r");
        return;
    }
    else
    {
        ch->Send("Usage: class info\n\r");
        ch->Send("       class newclass <classname>\n\r");
        return;
    }
}

void cmd_levels(Character * ch, string argument)
{
    int prev = 0, next = 0;
    for(int i = 2; i <= Game::MAX_LEVEL; i++)
    {
        next = Game::ExperienceForLevel(i);
        std::stringstream sstr;
        sstr << "|B[|XLevel: " << setw(3) << i;
        sstr << "|B]|X Experience Required: " << setw(9) << next << "  Difference: " << next - prev << "\n\r";
        ch->Send(sstr.str());
        prev = next;
    }
}

void cmd_help(Character * ch, string argument)
{
	string arg1;
    argument = Utilities::one_argument(argument, arg1);

	if(arg1.empty())
	{
		ch->Send("HELP is available on the following topics.\n\rType 'help <topic>' for more information.\n\rType 'commands' for a full list of commands.\n\r");
		ch->Send("MOVEMENT    QUESTS    COMBAT    COMMUNICATION    SPELLS\n\r");
		return;
	}

	if(Utilities::IsNumber(arg1))
	{
		int helpnum = Utilities::atoi(arg1);
		std::map<int,Help*>::iterator found = Game::GetGame()->helpIndex.find(helpnum);
		if(found != Game::GetGame()->helpIndex.end())
		{
			ch->Send((*found).second->text);
		}
	}
	else
	{
		std::map<int, Help*>::iterator iter;
		std::vector<Help*> found;

		for(iter = Game::GetGame()->helpIndex.begin(); iter != Game::GetGame()->helpIndex.end(); ++iter)
		{
			if(!Utilities::str_prefix(arg1, (*iter).second->title))
			{
				found.push_back((*iter).second);
			}
		}
		if(found.size() == 1)
		{
			ch->Send(found[0]->text);
		}
		else if(found.size() > 1)
		{
			for(int i = 0; i < (int)found.size(); i++)
			{
				ch->Send("|M[|X" + Utilities::itos(found[i]->id) + "|M]|X " + Utilities::ToUpper(found[i]->title) + "\n\r");
			}
		}
		else
		{
			ch->Send("No matching help items found.\n\r");
		}
	}
}
	 /*
	 GENERAL        COMMUNICATION
	 OBJECTS        MOVEMENT           FORMATION
	 COMBAT         TRAVEL
	 SPELLS
	 INFORMATION
	 */

/*
Movement is allowed in the following directions:
north       south       east        west        up
northwest   northeast   southwest   southeast   down

    Directions can be abbreviated, for example 'n' is the same as 'north', 
'ne' is the same as 'northeast'. Movement consists of moving your character from location to 
location. Locations in the game are usually referred to as \"rooms\", 
even if they are outside.

Movement is allowed at a rate of three rooms per second, which can be altered by spell affects or the use of a mount.
*/

void cmd_quest(Character * ch, string argument)
{
    if(!ch || !ch->player)
    {
        return;
    }
    if(argument.empty())
    {
        ch->Send("Quest: log, list, info #, accept #, drop #, complete #, progress #, share #\n\r");
        return;
    }
    //log, list, info #, accept #, drop #, complete #, progress #, share #
    string arg1;
    string arg2;
    argument = Utilities::one_argument(argument, arg1);
    argument = Utilities::one_argument(argument, arg2);

    if(!Utilities::str_cmp(arg1, "log"))
    {
        ch->Send("Active quests:\n\r");
        if(ch->player->questLog.empty())
        {
            ch->Send("None.\n\r");
            return;
        }
        std::vector<Quest *>::iterator iter;
        int ctr = 1;
        for(iter = ch->player->questLog.begin(); iter != ch->player->questLog.end(); ++iter)
        {
            Quest * q = (*iter);
            string complete = "";
            if(ch->player->QuestObjectivesComplete(q))
            {
                complete = " |G(Complete)";
            }
            
			//print current objective progress
			string objectives = "";
			std::vector<Quest::QuestObjective>::iterator objiter;
			int i = 0;
			for (objiter = q->objectives.begin(); objiter != q->objectives.end(); ++objiter)
			{
				int currentCount = ch->player->questObjectives[ctr - 1][i++];
				//type count objective description
				if (currentCount < (*objiter).count)
				{
					objectives += "|y" + (*objiter).description + " (" + Utilities::itos(currentCount) + "/" + Utilities::itos((*objiter).count) + ")";
				}
				else
				{
					objectives += "|g" + (*objiter).description + " (" + Utilities::itos(currentCount) + "/" + Utilities::itos((*objiter).count) + ")";
				}
			}
			ch->Send(Utilities::itos(ctr) + ". " + Quest::GetDifficultyColor(ch->level, q->level)
				+ q->name + " " + complete + " |X[" + objectives + "|X]\n\r");
			++ctr;
        }
        return;
    }
    else if(!Utilities::str_cmp(arg1, "list"))
    {
        if(!ch->GetTarget())
        {
            ch->Send("You need a target to list quests.\n\r");
            return;
        }
        if(ch->GetTarget()->questStart.empty() && ch->GetTarget()->questEnd.empty())
        {
            ch->Send("Your target has no quests to offer.\n\r");
            return;
        }
        bool found = false;
        std::vector<Quest *>::iterator questiter;
        for(questiter = ch->GetTarget()->questEnd.begin(); questiter != ch->GetTarget()->questEnd.end(); ++questiter)
        {
            if(ch->player->QuestActive(*questiter))
            {
				if (!found && !ch->GetTarget()->speechText.empty())
					ch->Send(ch->GetTarget()->speechText + "\n\r");
                //TODO color ? by complete status
                found = true;
                ch->Send("|Y? " + Quest::GetDifficultyColor(ch->level, (*questiter)->level) + (*questiter)->name + " (" + Utilities::itos((*questiter)->level) + ")|X");
				if (!(*questiter)->progressMessage.empty())
				{
					ch->Send(" : " + (*questiter)->progressMessage + "\n\r");
				}
				else
				{
					ch->Send("\n\r");
				}
            }
        }
        int ctr = 1;
        for(questiter = ch->GetTarget()->questStart.begin(); questiter != ch->GetTarget()->questStart.end(); ++questiter)
        {
            if(ch->player->QuestEligible(*questiter))
            {
				if (!found && !ch->GetTarget()->speechText.empty())
					ch->Send(ch->GetTarget()->speechText + "\n\r");
                found = true;
                ch->Send(Utilities::itos(ctr) + ". |Y! " + Quest::GetDifficultyColor(ch->level, (*questiter)->level) 
                           + (*questiter)->name + " (" + Utilities::itos((*questiter)->level) + ")|X\n\r");
                ctr++;
            }
        }
        if(!found)
        {
            ch->Send("Your target has no quests to offer.\n\r");
            return;
        }
        return;
    }
    else if(!Utilities::str_cmp(arg1, "info"))
    {
        if(!ch->GetTarget())
        {
            ch->Send("You need a target to get quest information.\n\r");
            return;
        }
        if(ch->GetTarget()->questStart.empty())
        {
            ch->Send("Your target has no quests to offer.\n\r");
            return;
        }
        int qnum = Utilities::atoi(arg2);
        if(qnum == 0)
        {
            ch->Send("Get information for which quest?\n\r");
            return;
        }

        std::vector<Quest *>::iterator questiter;
        int ctr = 1;
        for(questiter = ch->GetTarget()->questStart.begin(); questiter != ch->GetTarget()->questStart.end(); ++questiter)
        {
            if(ch->player->QuestEligible(*questiter))
            {
                if(qnum == ctr)
                {
                    Quest * q = (*questiter);
                    ch->Send(Quest::GetDifficultyColor(ch->level, q->level) + q->name + " (" 
                               + Utilities::itos(q->level) + ")|X\n\r\n\r");
                    ch->Send(q->longDescription + "\n\r\n\r");
                    ch->Send(q->shortDescription + "\n\r\n\r");
					ch->Send("You will receive |Y" + Utilities::itos(q->experienceReward) + " experience|X");
					if (q->moneyReward > 0)
						ch->Send(" and |Y" + Utilities::itos(q->moneyReward) + " gold|X");
					ch->Send("\n\r");

					if (!q->itemRewards.empty())
					{
						ch->Send("|YYou will be able to choose one of these rewards:|X\n\r");
						string combinedRewards;
						for (auto itemiter = std::begin(q->itemRewards); itemiter != std::end(q->itemRewards); ++itemiter)
						{
							string itemreward = Game::GetGame()->GetItemIndex(*itemiter)->FormatItemInfo();
							combinedRewards = Utilities::SideBySideString(combinedRewards, itemreward);
						}
						ch->Send(combinedRewards);
					}
                    return;
                }
                ctr++;
            }
        }
        ch->Send("Quest " + arg2 + " not found.\n\r");
        return;
    }
    else if(!Utilities::str_cmp(arg1, "accept"))
    {
        if(!ch->GetTarget())
        {
            ch->Send("You need a target to accept a quest.\n\r");
            return;
        }
        if(ch->GetTarget()->questStart.empty())
        {
            ch->Send("Your target has no quests to offer.\n\r");
            return;
        }
        if(ch->player->questLog.size() >= Player::QUESTLOG_MAX_SIZE)
        {
            ch->Send("Your quest log is full.\n\r");
            return;
        }
        int qnum = Utilities::atoi(arg2);
        if(qnum == 0)
        {
            ch->Send("Accept which quest?\n\r");
            return;
        }
        std::vector<Quest *>::iterator questiter;
        int ctr = 1;
        //Need to loop on QuestEligible quests only
        for(questiter = ch->GetTarget()->questStart.begin(); questiter != ch->GetTarget()->questStart.end(); ++questiter)
        {
            if(ch->player->QuestEligible(*questiter))
            {
                if(qnum == ctr)
                {
                    //accept this quest
                    Quest * q = (*questiter);
                    std::vector<int> playerObjectives;
                    playerObjectives.resize(q->objectives.size(), 0);
                    ch->player->questLog.push_back(q);
                    ch->player->questObjectives.push_back(playerObjectives);
                    
                    ch->Send("Accepted quest: " + q->name + "\n\r");

                    //TODO: check for already completed OBJECTIVE_ITEM
                    int objindex = 0;
                    std::vector<Quest::QuestObjective>::iterator iter;
                    for(iter = q->objectives.begin(); iter != q->objectives.end(); ++iter)
                    {
                        if((*iter).type == Quest::OBJECTIVE_ITEM)
                        {
                            std::list<Item*>::iterator inviter;
                            for(inviter = ch->player->inventory.begin(); inviter != ch->player->inventory.end(); ++inviter)
                            {
                                if((*inviter)->id == ((Item*)((*iter).objective))->id)
                                {
                                    ch->player->questObjectives[ch->player->questLog.size()-1][objindex]++;
                                }
                            }
                            if(ch->player->questObjectives[ch->player->questLog.size()-1][objindex] > 0)
                            {
                                ch->Send("|W" + q->name + ": ");
                                ch->Send("|Y" + (*iter).description + " (" 
                                    + Utilities::itos(ch->player->questObjectives[ch->player->questLog.size()-1][objindex]) 
                                    + "/" + Utilities::itos((*iter).count) + ")|X\n\r\n\r");
                            }
                        }
                        objindex++;
                    }
                    return;
                }
                ctr++;
            }
        }
        ch->Send("Quest " + arg2 + " not found.\n\r");

        return;
    }
    else if(!Utilities::str_cmp(arg1, "drop"))
    {
        if(ch->player->questLog.empty())
        {
            ch->Send("Your quest log is empty.\n\r");
            return;
        }
        int qnum = Utilities::atoi(arg2);
        if(qnum == 0)
        {
            ch->Send("Drop which quest?\n\r");
            return;
        }
        if(qnum < 1 || qnum > (int)ch->player->questLog.size())
        {
            ch->Send("Choose a quest from your quest log, 1-" + Utilities::itos((int)ch->player->questLog.size()) + "\n\r");
            return;
        }
        ch->Send("You have dropped " + ch->player->questLog[qnum-1]->name + ".\n\r");
        ch->player->questLog.erase(ch->player->questLog.begin() + qnum - 1);
        ch->player->questObjectives.erase(ch->player->questObjectives.begin() + qnum - 1);
        return;
    }
    else if(!Utilities::str_cmp(arg1, "complete"))
    {
        if(ch->player->questLog.empty())
        {
            ch->Send("Your quest log is empty.\n\r");
            return;
        }
        if(!ch->GetTarget())
        {
            ch->Send("You need a target to complete a quest.\n\r");
            return;
        }
        if(ch->GetTarget()->questEnd.empty())
        {
            ch->Send("Your target cannot complete your quest.\n\r");
            return;
        }
        int qnum = Utilities::atoi(arg2);
        if(qnum == 0)
        {
            ch->Send("Complete which quest?\n\r");
            return;
        }
        if(qnum < 1 || qnum > (int)ch->player->questLog.size())
        {
            ch->Send("Choose a quest from your quest log, 1-" + Utilities::itos((int)ch->player->questLog.size()) + "\n\r");
            return;
        }
        Quest * complete = ch->player->questLog[qnum-1];
        std::vector<Quest*>::iterator qiter;
        bool found = false;
        for(qiter = ch->GetTarget()->questEnd.begin(); qiter != ch->GetTarget()->questEnd.end(); ++qiter)
        {
            if((*qiter)->id == complete->id)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            ch->Send("Your target cannot complete your quest.\n\r");
            return;
        }
        if(!ch->player->QuestObjectivesComplete(complete))
        {
            ch->Send("Not all objectives of that quest are complete.\n\r");
            return;
        }


		if (!complete->itemRewards.empty())
		{
			string rewardQueryString = "Choose your reward: (";
			string combinedRewards;
			int ctr = 1;
			for (auto itemiter = std::begin(complete->itemRewards); itemiter != std::end(complete->itemRewards); ++itemiter)
			{
				rewardQueryString += Utilities::itos(ctr++);
				if (ctr <= complete->itemRewards.size())
					rewardQueryString += ", ";
				string itemreward = Game::GetGame()->GetItemIndex(*itemiter)->FormatItemInfo();
				combinedRewards = Utilities::SideBySideString(combinedRewards, itemreward);
			}
			rewardQueryString += "): ";
			ch->Send(combinedRewards);
			ch->SetQuery(rewardQueryString, (void*)qnum, questCompleteQuery); //Yes I'm casting an int to void*. Investigated templates, but...
		}
		else
		{
			//Remove items from inventory for OBJECTIVE_ITEM
			std::vector<Quest::QuestObjective>::iterator iter;
			for (iter = complete->objectives.begin(); iter != complete->objectives.end(); ++iter)
			{
				if ((*iter).type == Quest::OBJECTIVE_ITEM)
				{
					int found = 0;
					std::list<Item*>::iterator inviter;
					for (inviter = ch->player->inventory.begin(); inviter != ch->player->inventory.end();)
					{
						if ((*inviter)->id == ((Item*)((*iter).objective))->id)
						{
							if (found < (*iter).count)
							{
								//remove from inventory
								inviter = ch->player->inventory.erase(inviter);
								ch->player->inventorySize--;
							}
							else
							{
								break;
							}
							found++;
						}
						else
						{
							++inviter;
						}
					}
				}
			}

			qiter = ch->player->questLog.begin() + qnum - 1;
			std::vector< std::vector<int> >::iterator objiter = ch->player->questObjectives.begin() + qnum - 1;
			ch->player->questLog.erase(qiter);
			ch->player->questObjectives.erase(objiter);
			ch->player->completedQuests.insert(complete->id);

			ch->Send(complete->name + " completed!\n\r");
			ch->Send(complete->completionMessage + "\n\r");
			ch->Send("|BYou have gained |Y" + Utilities::itos(complete->experienceReward) + "|B experience.|X\n\r");
			ch->ApplyExperience(complete->experienceReward);
		}
        return;
    }
    else if(!Utilities::str_cmp(arg1, "progress"))
    {
        if(ch->player->questLog.empty())
        {
            ch->Send("Your quest log is empty.\n\r");
            return;
        }
        int qnum = Utilities::atoi(arg2);
        if(qnum == 0)
        {
            ch->Send("Get progress information for which quest?\n\r");
            return;
        }
        if(qnum < 1 || qnum > (int)ch->player->questLog.size())
        {
            ch->Send("Choose a quest from your quest log, 1-" + Utilities::itos((int)ch->player->questLog.size()) + "\n\r");
            return;
        }
        Quest * progress = ch->player->questLog[qnum-1];
        ch->Send("Quest information for: " + Quest::GetDifficultyColor(ch->level, progress->level) 
                   + progress->name + " (" + Utilities::itos(progress->level) + ")|X\n\r");
        ch->Send(progress->longDescription + "\n\r\n\r");
        ch->Send(progress->shortDescription + "\n\r\n\r");

        std::vector<Quest::QuestObjective>::iterator objiter;
        int i = 0;
        for(objiter = progress->objectives.begin(); objiter != progress->objectives.end(); ++objiter)
        {
            int currentCount = ch->player->questObjectives[qnum-1][i++];
            //type count objective description
            if(currentCount < (*objiter).count)
            {
                ch->Send("|Y" + (*objiter).description + " (" + Utilities::itos(currentCount) + "/" + Utilities::itos((*objiter).count) + ")|X\n\r");
            }
            else
            {
                ch->Send("|G" + (*objiter).description + " (" + Utilities::itos(currentCount) + "/" + Utilities::itos((*objiter).count) + ")|X\n\r");
            }
        }

		ch->Send("You will receive |Y" + Utilities::itos(progress->experienceReward) + " experience|X");
		if (progress->moneyReward > 0)
			ch->Send(" and |Y" + Utilities::itos(progress->moneyReward) + " gold|X");
		ch->Send("\n\r");

		if (!progress->itemRewards.empty())
		{
			ch->Send("|YYou will be able to choose one of these rewards:|X\n\r");
			string combinedRewards;
			for (auto itemiter = std::begin(progress->itemRewards); itemiter != std::end(progress->itemRewards); ++itemiter)
			{
				string itemreward = Game::GetGame()->GetItemIndex(*itemiter)->FormatItemInfo();
				combinedRewards = Utilities::SideBySideString(combinedRewards, itemreward);
			}
			ch->Send(combinedRewards);
		}
        return;
    }
    else if(!Utilities::str_cmp(arg1, "share"))
    {
        return;
    }
    ch->Send("Quest: log, list, info #, accept #, drop #, complete #, progress #, share #\n\r");
}


bool questCompleteQuery(Character * ch, string argument)
{
	int qnum = (int)ch->GetQueryData();
	Quest * quest = ch->player->questLog[qnum - 1];
	ch->QueryClear();

	string arg1;
	Utilities::one_argument(argument, arg1);
	if (!Utilities::IsNumber(arg1))
	{
		ch->Send("Invalid choice, quest not completed.\n\r");
		return true;
	}
	int choice = Utilities::atoi(arg1);
	if (choice <= 0 || choice > quest->itemRewards.size())
	{
		ch->Send("Invalid choice, quest not completed.\n\r");
		return true;
	}

	//Remove items from inventory for OBJECTIVE_ITEM
	std::vector<Quest::QuestObjective>::iterator iter;
	for (iter = quest->objectives.begin(); iter != quest->objectives.end(); ++iter)
	{
		if ((*iter).type == Quest::OBJECTIVE_ITEM)
		{
			int found = 0;
			std::list<Item*>::iterator inviter;
			for (inviter = ch->player->inventory.begin(); inviter != ch->player->inventory.end();)
			{
				if ((*inviter)->id == ((Item*)((*iter).objective))->id)
				{
					if (found < (*iter).count)
					{
						//remove from inventory
						inviter = ch->player->inventory.erase(inviter);
						ch->player->inventorySize--;
					}
					else
					{
						break;
					}
					found++;
				}
				else
				{
					++inviter;
				}
			}
		}
	}

	std::vector<Quest*>::iterator qiter = ch->player->questLog.begin() + qnum - 1;
	std::vector< std::vector<int> >::iterator objiter = ch->player->questObjectives.begin() + qnum - 1;
	ch->player->questLog.erase(qiter);
	ch->player->questObjectives.erase(objiter);
	ch->player->completedQuests.insert(quest->id);

	ch->Send(quest->name + " completed!\n\r");
	ch->Send(quest->completionMessage + "\n\r");

	Item * myreward = Game::GetGame()->GetItemIndex(quest->itemRewards[choice - 1]);
	if(myreward)
	{
		ch->Send("|WYou receive loot: " + string(Item::quality_strings[myreward->quality]) + myreward->name + "|X\n\r");
		ch->player->NewItemInventory(myreward);
	}

	ch->Send("|BYou have gained |Y" + Utilities::itos(quest->experienceReward) + "|B experience.|X\n\r");
	ch->ApplyExperience(quest->experienceReward);
	return true;
}

void cmd_quit(Character * ch, string argument)
{
    /*if(ch->HasQuery())
    {
        ch->Send("Answer your current question first.\n\r");
        return;
    }*/
    //Do a few checks here, but just query the player with a quit-function callback.
    if(ch->InCombat())
    {
        ch->Send("You can't do that while in combat!\n\r");
        return;
    }
    if(ch->delay_active)
    {
        ch->Send("You can't do that while casting.\n\r");
        return;
    }
	
	string qp = "Quit? (y/n) ";
	if (ch->level == 1)
	{
		qp += "|R(Level one characters will not be saved)|X: ";
	}
	ch->SetQuery(qp, NULL, cmd_quit_Query);
}

bool cmd_quit_Query(Character * ch, string argument)
{
    ch->QueryClear();
    if(!Utilities::str_cmp(argument, "yes") || !Utilities::str_cmp(argument, "y"))
	{
        if(ch && ch->InCombat())
        {
            ch->Send("You can't do that while in combat!\n\r");
            return true;
        }
        if(ch->delay_active)
        {
            ch->Send("You can't do that while casting.\n\r");
            return true;
        }

		ch->Send("Bye\n\r");
		LogFile::Log("status", ch->name + " \"quit\"");

        if(ch->player && ch->player->user)
        {
			ch->player->user->SetDisconnect();
        }
	}
	return true;
}

bool releaseSpiritQuery(Character * ch, string argument)
{
    if(!ch || !ch->player)
    { 
        return true;
    }

    if(!Utilities::str_cmp(argument, "release"))
    {
		ch->player->corpse_room = ch->room->id;
		ch->SetGhost();
        ch->QueryClear();
		Area * this_area = Game::GetGame()->GetArea(ch->room->area);
		if (!this_area)
		{
			LogFile::Log("error", "releaseSpiritQuery, bad ch->room->area");
			return true;
		}
		Room * graveyard = Game::GetGame()->GetRoom(this_area->death_room);
		if (!graveyard)
		{
			LogFile::Log("error", "releaseSpiritQuery, no area death_room");
			return true;
		}
		ch->Message("|W" + ch->name + "'s spirit separates from " + ch->HisHer() + " corpse, and disappates into nothingness.|X", Character::MessageType::MSG_ROOM_NOTCHAR);
		ch->MakeCorpse();
		ch->ChangeRooms(graveyard);
		ch->player->graveyard_room = graveyard->id;
		cmd_look(ch, "");

		int res_at_graveyard = ch->player->death_timer - ch->TimeSinceDeath();
		int res_at_corpse = ch->player->death_timer_runback - ch->TimeSinceDeath();
		if (res_at_graveyard > 0)
			ch->Send("|W" + Utilities::itos(res_at_graveyard) + " seconds before you can resurrect at the graveyard.\n\r");
		if (res_at_corpse > 0)
			ch->Send("|W" + Utilities::itos(res_at_corpse) + " seconds before you can resurrect near your corpse.\n\r");
        return true;
    }
    return false;
}

bool acceptResQuery(Character * ch, string argument)
{
	if (!ch || !ch->player)
	{
		return true;
	}
	if (!Utilities::str_cmp(argument, "accept"))
	{
		ch->SetHealth(ch->GetMaxHealth() / 2);
		ch->SetMana(ch->GetMaxMana() / 2);
		ch->SetEnergy(ch->GetMaxEnergy());
		ch->SetRage(0);

		ch->RemoveCorpse();
		
		ch->Message("|W" + ch->name + " appears in a shimmering silver mist.|X", Character::MessageType::MSG_ROOM_NOTCHAR);
		ch->SetAlive();
		ch->QueryClear();
		return true;
	}
	return false;
}

bool returnToGYQuery(Character * ch, string argument)
{
	if (!ch || !ch->player)
	{
		return true;
	}
	if (!Utilities::str_cmp(argument, "return"))
	{
		ch->ChangeRooms(Game::GetGame()->GetRoom(ch->player->graveyard_room));
		ch->QueryClear();
		cmd_look(ch, "");
		return true;
	}
	return false;
}

void cmd_alias(Character * ch, string argument)
{
	if (ch->IsNPC())
		return;

	string new_alias;
	argument = Utilities::one_argument(argument, new_alias);

	if (new_alias.empty())
	{
		if (ch->player->alias.empty())
		{
			ch->Send("You have no aliases.\n\r");
			ch->Send("Use: alias <word>\n\r     alias <word> <substitution>\n\r");
		}
		else
		{
			ch->Send("Existing aliases:\n\r");
			for (auto iter = ch->player->alias.begin(); iter != ch->player->alias.end(); iter++)
			{
				ch->Send("'" + iter->first + "' : '" + iter->second + "'\n\r");
			}
		}
		return;
	}

	if (!Utilities::str_cmp(new_alias, "alias") || !Utilities::str_cmp(new_alias, "unalias"))
	{
		ch->Send("That word is reserved.\n\r");
		return;
	}

	auto alias_iter = ch->player->alias.find(new_alias);
	if (alias_iter != ch->player->alias.end())
	{
		if (!argument.empty())
		{
			ch->Send("That alias already exists.\n\r");
		}
		ch->Send(new_alias + ": " + alias_iter->second + "\n\r");
		return;
	}
	if (argument.empty())
	{
		ch->Send("Alias '" + new_alias + "' to what?\n\r");
		return;
	}
	if (argument.length() > MAX_COMMAND_LENGTH)
	{
		ch->Send("That alias substitution is too long. Maximum length is " + Utilities::itos(MAX_COMMAND_LENGTH) + " characters.\n\r");
		return;
	}
	if (new_alias.length() > 25)
	{
		ch->Send("That alias keyword is too long. Maximum length is 25 characters.\n\r");
		return;
	}
	ch->player->alias[new_alias] = argument;
	ch->Send("Alias created: '" + new_alias + "': " + argument + "\n\r");
}

void cmd_unalias(Character * ch, string argument)
{
	if (ch->IsNPC())
		return;

	if (ch->player->alias.empty())
	{
		ch->Send("You have no aliases.\n\r");
		ch->Send("Use: unalias <word>\n\r");
		return;
	}

	string remove_alias;
	argument = Utilities::one_argument(argument, remove_alias);

	if (remove_alias.empty())
	{
		ch->Send("Use: unalias <word>\n\r");
		ch->Send("Existing aliases:\n\r");
		for (auto iter = ch->player->alias.begin(); iter != ch->player->alias.end(); iter++)
		{
			ch->Send("'" + iter->first + "' : '" + iter->second + "'\n\r");
		}
		return;
	}

	auto iter = ch->player->alias.find(remove_alias);
	if (iter == ch->player->alias.end())
	{
		ch->Send("You don't have an alias with that name.\n\r");
		return;
	}

	ch->player->alias.erase(remove_alias);
	ch->Send("Alias '" + remove_alias + "' deleted.\n\r");
}