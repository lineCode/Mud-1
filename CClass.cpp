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
#include "CCommand.h"
#include "utils.h"
#include "mud.h"

Class::Class()
{
    id = 0;
    name = "";
    color = "";
    items = "";
	agilityPerLevel = 0;
	intellectPerLevel = 0;
	strengthPerLevel = 0;
	vitalityPerLevel = 0;
	wisdomPerLevel = 0;

    changed = false;
}

Class::~Class()
{
    classSkills.clear();
}

void Class::Save()
{
    if(!changed)
        return;

    string sql = "INSERT INTO classes (id, name, color, agility_per_level, intellect_per_level, strength_per_level, vitality_per_level,";
    sql += " wisdom_per_level) values (";
    sql += Utilities::itos(id) + ",'" + Utilities::SQLFixQuotes(name) + "','" + Utilities::SQLFixQuotes(color);
    sql += "'," + Utilities::itos(agilityPerLevel) + "," + Utilities::itos(intellectPerLevel);
    sql += "," + Utilities::itos(strengthPerLevel) + "," + Utilities::itos(vitalityPerLevel) + ",";
    sql += Utilities::itos(wisdomPerLevel);

    sql += ") ON DUPLICATE KEY UPDATE id=VALUES(id), name=VALUES(name), color=VALUES(color), agility_per_level=VALUES(agility_per_level), ";
    sql += "intellect_per_level=VALUES(intellect_per_level), strength_per_level=VALUES(strength_per_level), vitality_per_level=VALUES(vitality_per_level), wisdom_per_level=VALUES(wisdom_per_level)";

    Server::sqlQueue->Write(sql);

	Server::sqlQueue->Write("DELETE FROM class_skills where class=" + Utilities::itos(id));
	std::list<SkillData>::iterator iter;
	for (iter = classSkills.begin(); iter != classSkills.end(); ++iter)
	{
		string skillssql = "INSERT INTO class_skills (class, skill, level) values (";
		skillssql += id + ", " + Utilities::itos((*iter).skill->id) + ", " + Utilities::itos((*iter).level) + ")";
		skillssql += " ON DUPLICATE KEY UPDATE class=VALUES(class), skill=VALUES(skill), level=VALUES(level)";
		Server::sqlQueue->Write(skillssql);
	}

    changed = false;
}