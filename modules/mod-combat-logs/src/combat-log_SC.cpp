/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "LuaEngine.h"
#include "Spell.h"
#include "combatLogManager.h"

class CombatLog_PlayerScripts : public PlayerScript
{
public:
    CombatLog_PlayerScripts() : PlayerScript("CombatLog_PlayerScripts") { }


    void OnPlayerEnterCombat(Player* player, Unit* enemy)
    {

        if (!player)
            return;

        CombatLogManager::StartCombat(player);
    }

    void OnPlayerLeaveCombat(Player* player)
    {

        if (!player)
            return;

        CombatLogManager::EndCombat(player);
    }
    
};

class CombatLog_UnitScript : public UnitScript
{
public:
    CombatLog_UnitScript()
        : UnitScript("CombatLog_UnitScript", true)
    {
    }

    void OnHeal(Unit* healer, Unit* reciever, uint32& gain, SpellInfo const* spellInfo)
    {
        if (!healer)
            return;

        if (!reciever)
            return;

        if (healer->GetTypeId() == TYPEID_PLAYER)
            CombatLogManager::RecordLog(healer, reciever, gain, spellInfo, true);
    }

    void OnDamage(Unit* attacker, Unit* victim, uint32& damage, SpellInfo const* spellProto)
    {

        if (!attacker)
            return;

        if (!victim)
            return;

        if(attacker->GetTypeId() == TYPEID_PLAYER)
            CombatLogManager::RecordLog(attacker, victim, damage, spellProto, false);
    }
};

// Add all scripts in one
void AddSC_CombatLog()
{
    new CombatLog_UnitScript();
    new CombatLog_PlayerScripts();
}
