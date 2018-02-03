#ifndef CCHARACTER_H
#define CCHARACTER_H

class Server;
class User;
class SpellAffect;
class Reset;

class Character : public Listener, public ListenerManager
{
public:
    Character();
    Character(std::string name_, int id_);
    Character(std::string name_, User * user_);
    Character(const Character&);
    ~Character();
    void SetDefaults();
    void Notify(ListenerManager *);

    enum MessageType
    {
        MSG_CHAR, MSG_CHAR_VICT, MSG_ROOM, MSG_ROOM_NOTCHAR, MSG_ROOM_NOTVICT, MSG_ROOM_NOTCHARVICT
    };

    int id;
    int level;
    int gender;

    int agility;	//crit chance and avoidance
    int intellect;  //spell power... mana pool based completely off class level
    int strength;	//attack power
    int stamina;	//health
    int wisdom;		//mana regen

	enum ResourceType
	{
		RESOURCE_HEALTH = 1, RESOURCE_MANA, RESOURCE_ENERGY, RESOURCE_RAGE, RESOURCE_COMBO
	};
	int energy;
	int maxEnergy;
	int rage;
	int maxRage;
	int comboPoints;
	int maxComboPoints;
	Character * comboPointTarget;

    int health;     //current health
    int maxHealth;  //permanent health(stamina*5) + gear and buff bonuses
    int mana;
    int maxMana;
    static const int HEALTH_FROM_STAMINA = 10;
    static const int MANA_FROM_INTELLECT = 10; //todo: move intellect into class per level up!
	static constexpr double STRENGTH_DAMAGE_MODIFIER = 0.30;	//auto attack damage increased by 30% of strength
    std::string name;
    std::string title;
    Room * room;
    Player * player;

    int race; //index into Character::race_table
	enum Races
	{
		RACE_NONE, RACE_HUMAN, RACE_ELF, RACE_DWARF, RACE_ORC, RACE_GNOME, RACE_GOBLIN, RACE_UNDEAD, RACE_MINOTAUR, RACE_TROLL
	};
    //TODO, make this a class? std::list<Skill*> racials? starting stat bonuses?
    struct RaceType
    {
        int id;
        std::string name;
    };
    static RaceType race_table[];

    //npc only stuff
    struct DropData
    {
        std::vector<int> id;
        int percent;
    };
    std::list<DropData> drops; //TODO Random drops
    Reset * reset; //reset that spawned this npc, if any
    std::vector<Quest *> questStart;
    std::vector<Quest *> questEnd;
	std::string speechText;
	std::string keywords;
    bool changed;
    std::vector<int> flags; //a vector of constants to indicate flag is set

	enum Flags
    {
        FLAG_FRIENDLY,FLAG_NEUTRAL,FLAG_AGGRESSIVE,FLAG_GUILD,FLAG_VENDOR,FLAG_REPAIR,
        FLAG_TRAINER
    };
    struct flag_type
    {
        int flag;
        std::string flag_name;
    };
    static flag_type flag_table[];
    bool remove;

    //Movement
    double movementSpeed; 
    static constexpr double NORMAL_MOVE_SPEED = 1;						//default move speed in rooms per second
    static constexpr double COMBAT_MOVE_SPEED = NORMAL_MOVE_SPEED * .3; //30% of normal
    double lastMoveTime;
    std::deque<void(*)(Character *, std::string)> movementQueue;

    //Combat
    bool combat;
    bool meleeActive;
    //Character * target; //private
    double lastAutoAttack_off; //Time stamp for melee swing timer
    double lastAutoAttack_main;
    double npcAttackSpeed;
    int npcDamageLow;
    int npcDamageHigh;

    struct LeashData //Keep track of wander distance and leash distance
    { 
        int type;
        Room * room;
        Exit * next;
    };
    std::list<LeashData> leashData;
    std::map<int, Exit*> wanderPath;
    std::map<int, Exit*> leashPath;
    struct Threat
    {
        Character * ch;
        int threat;
    };
    std::list<Threat> threatList;

    //Spells/Skills
    struct DelayData
    {
        Character * caster;
        Skill * sk;
        Character * charTarget;
        //Object * objTarget;
    };
    double delay;
    DelayData delayData;
    bool delay_active;
    void (*delayFunction)(DelayData);
    std::list<SpellAffect *> buffs; //TODO http://www.boost.org/doc/libs/1_37_0/libs/ptr_container/doc/ptr_container.html
    std::list<SpellAffect *> debuffs;
    bool debuffs_invalid;
    bool buffs_invalid;
    std::map<std::string, Skill *> knownSkills;
    std::map<std::string, double> cooldowns; //TODO, save cooldowns
    double lastSpellCast;  //time stamp for mana regen 5 second rule

    std::map<int, Trigger> triggers;
    void AddTrigger(Trigger & trig);
    Trigger * GetTrigger(int id, int type = -1);
    /*enum TriggerType //moved to CTrigger.h
    {
        ENTER_CHAR, ENTER_NPC, ENTER_PC, EXIT_CHAR, EXIT_NPC, EXIT_PC, TIMER, TRIG_LAST
    };*/


    bool hasQuery;
	bool (*queryFunction)(Character *, std::string);
	void * queryData;
    std::string queryPrompt;

    enum EditState
    {
        ED_NONE, ED_ROOM, ED_SKILL, ED_NPC, ED_ITEM, ED_QUEST, ED_CLASS, ED_PLAYER, ED_HELP, ED_AREA
    };
    EditState editState;
    void * editData;

    //For search  Store a reference to all searchable class data by type
    std::map<std::string, std::string*> stringTable;
    std::map<std::string, int*> intTable;
    std::map<std::string, double*> doubleTable;

    //TODO sort these functions!
	void ResetMaxStats();
    void GeneratePrompt(double currentTime);
    void QueryClear();
	void SendBW(std::string str);
    void Send(std::string str);
	void Send(char * str);
	void SendGMCP(std::string str);
	void SendGMCP(char * str);
    void Message(const std::string & txt, MessageType msg_type, Character * vict = NULL);
    Character * GetCharacterRoom(std::string name);
    Character * GetCharacterAdjacentRoom(std::string name);
    Character * GetCharacterRoom(Character * target);
    Character * GetCharacterAdjacentRoom(Character * target);
    void SetTarget(Character * t);
    void ClearTarget();
    Character * GetTarget();
    void Move(int direction);
    bool ChangeRoomsID(int roomid);
    bool ChangeRooms(Room * room);
    static Character * LoadPlayer(std::string name, User * user);
    //static Character * LoadNPC(Server_ptr server, int id);
    void Save();
    void SetLevel(int level);
    int GetLevel();
	Player * GetPlayer();
    std::string GetName();
    std::string HisHer();
	bool CancelCast();
    void EnterCombat(Character * victim);
    void ExitCombat();
    bool InCombat();
    bool IsFighting(Character * target);
    void UpdateThreat(Character * ch, int value);
    int GetThreat(Character * ch);
    Character * GetTopThreat();
    bool HasThreat(Character * ch);
    void AutoAttack(Character * victim);
    void OneHit(Character * victim, int damage);
	double GetMainhandWeaponSpeed();
	double GetMainhandDamagePerSecond();
	int GetOffhandDamageRandomHit();
	double GetOffhandDamagePerSecond();
	double GetOffhandWeaponSpeed();
	int GetMainhandDamageRandomHit();
	int GetIntellect();
	int GetStrength();
    int GetHealth();
    int GetMana();
	int GetMaxMana();
	int GetMaxHealth();
	int GetEnergy();
	int GetRage();
	void GenerateRageOnAttack(int damage, double weapon_speed, bool mainhand, bool wascrit);
	void GenerateRageOnTakeDamage(int damage);
	int GetComboPoints();
	void SetComboPoints(int howmany);
	void GenerateComboPoint(Character * target);
	int SpendComboPoints(Character * target);
    bool HasResource(int which, int amount);
    void AdjustHealth(Character * source, int amount);
    void ConsumeMana(int amount);
	void ConsumeEnergy(int amount);
	void ConsumeRage(int amount);
    void SetMana(Character * source, int amount);
    void AdjustMana(Character * source, int amount);
	void AdjustEnergy(Character * source, int amount);
	void AdjustRage(Character * source, int amount);
    void ApplyExperience(int amount);
    void RemoveThreat(Character * ch, bool removeall);
    SpellAffect * AddSpellAffect(int isDebuff, Character * caster, std::string name,
                        bool hidden, bool stackable, int ticks, double duration, int category, Skill * sk, std::string affect_description);
    SpellAffect * HasSpellAffect(std::string name);
    int CleanseSpellAffect(Character * cleanser, int category, int howMany = -1);
    void RemoveSpellAffect(int isDebuff, int id);
    void RemoveSpellAffect(int isDebuff, std::string name);
    void RemoveAllSpellAffects();
    void SaveSpellAffects();
    void LoadSpellAffects();
    void SaveCooldowns();
    void LoadCooldowns();
    int GetAuraModifier(int affect, int whatModifier);
    int GetTotalAuraModifier(int affect);
    int GetSmallestAuraModifier(int affect);
    int GetLargestAuraModifier(int affect);
    bool CanMove();
    double GetMoveSpeed();
    void AddSkill(Skill * newskill);
    Skill * GetSkillShortName(std::string name);
    bool HasSkill(Skill * sk);
    bool HasSkillByName(std::string name);
    void RemoveSkill(Skill * sk);
    void RemoveSkill(std::string name);
    void SetCooldown(Skill * sk, std::string name, bool global, double length);
    double GetCooldownRemaining(Skill * sk);
    bool IsNPC();
    bool IsCorpse();
    bool IsGhost();
	void AddClassSkills();
    

private:

    Character * target;
	
};

#endif