#include "CSpellAffect.h"
#include "CServer.h"
#include "CGame.h"
#include "CSkill.h"
#include "CCharacter.h"
#include "CSubscriber.h"
#include "CSubscriberManager.h"
#include "CLogFile.h"
#include "CmySQLQueue.h"
#include "utils.h"

const struct SpellAffect::AuraTable aura_table[] = 
{
    { "MOVE_SPEED", SpellAffect::AURA_MOVE_SPEED },
    { "RESOURCE_COST", SpellAffect::AURA_RESOURCE_COST },
	{ "EATING", SpellAffect::AURA_EATING },
	{ "DRINKING", SpellAffect::AURA_DRINKING },
	{ "TAUNT", SpellAffect::AURA_TAUNT },
    { "", 0 }
};

const struct SpellAffect::CategoryType category_table[] = 
{
    { "", SpellAffect::AFFECT_NONE },
    { "|WMagic|X", SpellAffect::AFFECT_MAGIC },
    { "|MCurse|X", SpellAffect::AFFECT_CURSE },
    { "|GPoison|X", SpellAffect::AFFECT_POISON },
    { "|YDisease|X", SpellAffect::AFFECT_DISEASE },
    { "|RBleed|X", SpellAffect::AFFECT_BLEED },
    { "|CFrenzy|X", SpellAffect::AFFECT_FRENZY },
    { "AFFECT_LAST", -1 }
};

SpellAffect::SpellAffect()
{
    debuff = hidden = stackable = false;
    id = ticks = ticksRemaining = 0;
    duration = appliedTime = 0;
    caster = nullptr;
    skill = nullptr;
    affectCategory = SpellAffect::AFFECT_NONE;
    remove_me = false;
}

SpellAffect::~SpellAffect()
{
    if(caster)
    {
        caster->RemoveSubscriber(this);
		//cout << "~SpellAffect REMOVE" << endl;
    }
    auraAffects.clear();
}

void SpellAffect::ApplyAura(std::string affectName, int modifier)
{
    bool found = false;
    int i;
    for(i = 0; aura_table[i].auraID != 0; i++)
    {
        if(affectName == aura_table[i].affectName)
        {
            found = true;
            break;
        }
    }
    if(!found)
    {
        LogFile::Log("error", "SpellAffect::ApplyAura, bad affectName");
        return;
    }
    AuraAffect af = { aura_table[i].auraID, modifier };
    auraAffects.push_front(af);
}

bool SpellAffect::HasAura(int aura_id)
{
	for (auto iter = auraAffects.begin(); iter != auraAffects.end(); iter++)
	{
		if (iter->auraID == aura_id)
			return true;
	}
	return false;
}

void SpellAffect::RemoveAura(int aura_id)
{
	for (auto iter = auraAffects.begin(); iter != auraAffects.end();)
	{
		if (iter->auraID == aura_id)
			iter = auraAffects.erase(iter);
		else
			++iter;
	}
}

void SpellAffect::ApplyAura(int auraID, int modifier)
{
    bool found = false;
    for(int i = 0; aura_table[i].auraID != 0; i++)
    {
        if(auraID == aura_table[i].auraID)
        {
            found = true;
            break;
        }
    }
    if(!found)
    {
        LogFile::Log("error", "SpellAffect::ApplyAura, bad affectID");
        return;
    }
    AuraAffect af = { auraID, modifier };
    auraAffects.push_front(af);
}

std::string SpellAffect::GetCasterName()
{
    if(caster != nullptr)
        return caster->GetName();
    return casterName;
}

std::string SpellAffect::GetAffectCategoryName()
{
    if(affectCategory < 0 || affectCategory > SpellAffect::AFFECT_LAST)
        return "";
    return category_table[affectCategory].categoryName;
}

void SpellAffect::SaveDataInt(std::string tag, int val)
{
    affectDataInt[tag] = val;
}

void SpellAffect::SaveDataDouble(std::string tag, double val)
{
    affectDataDouble[tag] = val;
}

void SpellAffect::SaveDataString(std::string tag, std::string val)
{
    affectDataString[tag] = val;
}

int SpellAffect::GetDataInt(std::string tag)
{
    return affectDataInt[tag];
}

double SpellAffect::GetDataDouble(std::string tag)
{
    return affectDataDouble[tag];
}

std::string SpellAffect::GetDataString(std::string tag)
{
    return affectDataString[tag];
}

void SpellAffect::Notify(SubscriberManager * lm)
{
    //caster about to be deleted... player quit, npc killed etc. At least save the name
    casterName = caster->GetName();
	caster->RemoveSubscriber(this);
    caster = nullptr;
}

void SpellAffect::Save(std::string charname)
{
    double timeleft = (appliedTime + duration) - Game::currentTime;

	std::string affectsql = "INSERT INTO player_spell_affects (player, caster, skill, ticks, duration, timeleft, stackable, hidden, debuff, ";
	affectsql += "category, auras, data) ";
	affectsql += "values ('" + charname + "','" + casterName + "',";
	if (skill)
		affectsql += Utilities::itos(skill->id);
	else
		affectsql += "0";
	affectsql += "," + Utilities::itos(ticks) + "," + Utilities::dtos(duration, 1) + "," + Utilities::dtos(timeleft, 1);
	affectsql += "," + Utilities::itos(stackable) + "," + Utilities::itos(hidden) + "," + Utilities::itos(debuff);
	affectsql += "," + Utilities::itos(affectCategory) + ",'";

    std::list<struct AuraAffect>::iterator iter;

    if(!auraAffects.empty())
    {
        for(iter = auraAffects.begin(); iter != auraAffects.end(); ++iter)
        {
            affectsql += Utilities::itos((*iter).auraID) + "," + Utilities::itos((*iter).modifier) + ";";
        }
    }
    affectsql += "', '";
    
    for(std::map<std::string, int>::iterator iter = affectDataInt.begin(); iter != affectDataInt.end(); iter++)
    {
        affectsql += "i," + (*iter).first + "," + Utilities::itos((*iter).second) + ";";
    }
    for(std::map<std::string, double>::iterator iter = affectDataDouble.begin(); iter != affectDataDouble.end(); iter++)
    {
        affectsql += "d," + (*iter).first + "," + Utilities::dtos((*iter).second, 2) + ";";
    }
    for(std::map<std::string, std::string>::iterator iter = affectDataString.begin(); iter != affectDataString.end(); iter++)
    {
        affectsql += "s," + (*iter).first + "," + (*iter).second + ";";
    }

    affectsql += "');";
    Server::sqlQueue->Write(affectsql);
}

void SpellAffect::Load(Character * ch)
{
    StoreQueryResult affectres = Server::sqlQueue->Read("select * from player_spell_affects where player='" + ch->GetName() + "'");
    if(affectres.empty())
        return;

    Row row;
    StoreQueryResult::iterator i;
    for(i = affectres.begin(); i != affectres.end(); i++)
    {
        row = *i;

        Skill * sk = Game::GetGame()->GetSkill(row["skill"]);
        if(sk == nullptr)
        {
            LogFile::Log("error", "SpellAffect::Load, bad skill id");
            continue;
        }

        //ch->AddSpellAffect(row["is_debuff"], ch, sk->long_name, row["hidden"], row["stackable"], row["ticks"], row["duration"], sk); 
        //don't use AddSpellAffect since we don't want to call the _apply function
        SpellAffect * sa = new SpellAffect();
        sa->name = sk->long_name;
        sa->hidden = row["hidden"];
        sa->stackable = row["stackable"];
        sa->ticks = row["ticks"];
        sa->duration = row["duration"];
        sa->appliedTime = Utilities::GetTime() - ((double)row["duration"] - row["timeleft"]); //Game::currentTime isn't initialized until after one update...
        sa->skill = sk;
        sa->debuff = row["debuff"];
        sa->caster = nullptr;
        sa->casterName = row["caster"];
        sa->affectCategory = row["category"];
        double tick_interval = (double)row["duration"] / row["ticks"];
        sa->ticksRemaining = (int)(row["timeleft"] / tick_interval) + 1;

        if(sa->debuff)
        {
            sa->id = (int)ch->debuffs.size() + 1;
            ch->debuffs.push_front(sa);
        }
        else
        {
            sa->id = (int)ch->buffs.size() + 1;
            ch->buffs.push_front(sa);
        }
		std::string affect_data = (row["data"]).c_str();
        int first = 0, second = 0;
        while(first < (int)affect_data.length())
        {
            char data_type = affect_data[first];
            second = (int)affect_data.find(',', first+2);
			std::string data_name = affect_data.substr(first+2, second - (first+2));
            first = second+1;
            second = (int)affect_data.find(';', first);
            switch(data_type)
            {
                case 'i':
                {         
                    sa->affectDataInt[data_name] = Utilities::atoi(affect_data.substr(first, second - first));
                    break;
                }
                case 'd':
                {
                    sa->affectDataDouble[data_name] = Utilities::atof(affect_data.substr(first, second - first));
                    break;
                }
                case 's':
                {
                    sa->affectDataString[data_name] = affect_data.substr(first, second - first);
                    break;
                }
            }
            first = second + 1;
        }

		std::string aurastring = row["auras"].c_str();
        first = second = 0;
        while(first < (int)aurastring.length())
        {
            int affectid;
            int modifier;
            second = (int)aurastring.find(',', first);
            affectid = Utilities::atoi(aurastring.substr(first, second - first));
            first = second + 1;
            second = (int)aurastring.find(';', first);
            modifier = Utilities::atoi(aurastring.substr(first, second - first));
            first = second + 1;
            sa->ApplyAura(affectid, modifier);
        }
    }
	std::string affectsql = "DELETE FROM player_spell_affects WHERE player = '" + ch->GetName() + "'";
    Server::sqlQueue->Write(affectsql);
}

bool SpellAffect::CompareAuraByID::operator()(AuraAffect & elem) const
{
	return value == elem.auraID;
}
