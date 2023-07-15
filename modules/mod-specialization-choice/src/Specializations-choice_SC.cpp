/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "RunesManager.h"
#include "LuaEngine.h"
#include "Spell.h"
#include "PlayerSpecialization.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Totem.h"
#include "UnitAI.h"
#include "Log.h"

 // Add player scripts
class SpecChoice_PlayerScripts : public PlayerScript
{
public:
    SpecChoice_PlayerScripts() : PlayerScript("SpecChoice_PlayerScripts") { }

    void OnUpdateMastery(Player* player, uint32 value)
    {
        PlayerSpecialization::UpdateMastery(player, value);
    }

    void OnLogin(Player* player)
    {
        if (player->getClass() == CLASS_PALADIN) {
            player->SetMaxPower(POWER_ENERGY, 5);
            player->SetPower(POWER_ENERGY, 0);
        }

        if (player->getClass() == CLASS_WARLOCK) {
            player->SetMaxPower(POWER_RUNIC_POWER, 250);
            player->SetPower(POWER_RUNIC_POWER, 0);
        }
    }

    void OnPlayerLearnTalents(Player* player, uint32 talentId, uint32 talentRank, uint32 spellid)
    {
        player->UpdateMastery();
    }
};


using namespace Acore::ChatCommands;

class SpecChoice_CommandsScript : public CommandScript
{
public:
    SpecChoice_CommandsScript() : CommandScript("SpecChoice_CommandsScript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable commandTable =
        {
            { "reload specs",  HandleReloadCommand, SEC_MODERATOR,     Console::No },
        };
        return commandTable;
    }

    static bool HandleReloadCommand(ChatHandler* handler, Optional<PlayerIdentifier> player)
    {
        LOG_INFO("Runes", "Reload specs...");
        PlayerSpecialization::LoadAllSpecsPlayers();
        PlayerSpecialization::LoadAllSpecsSpells();
        PlayerSpecialization::InitializeSpecializations();
        Eluna::ReloadEluna();
        return true;
    }
};

class spell_activate_specialization : public SpellScript
{
    PrepareSpellScript(spell_activate_specialization);

    void HandleProc()
    {
        Player* player = GetCaster()->ToPlayer();
        SpellValue const* value = GetSpellValue();
        uint32 newSpecId = value->EffectBasePoints[EFFECT_0];

        uint32 currentSpecId = PlayerSpecialization::GetCurrentSpecId(player);
        Specialization newSpec = PlayerSpecialization::m_Specializations[newSpecId];

        if (currentSpecId == HUNTER_MARSKMANSHIP)
            player->RemoveAura(80182);

        if (currentSpecId > 0) {
            for (auto const& spellId : PlayerSpecialization::m_SpecSpells[currentSpecId]) {
                player->removeSpell(spellId, SPEC_MASK_ALL, false, true);
                player->RemoveAura(spellId);
            }
        }

        for (auto const& spellId : PlayerSpecialization::m_SpecSpells[newSpecId])
            player->learnSpell(spellId, false, false);

        PlayerSpecialization::m_PlayersSpecialization[player->GetGUID().GetCounter()] = newSpecId;
        CharacterDatabase.Execute("UPDATE characters SET specId = {} WHERE guid = {}", newSpecId, player->GetGUID().GetCounter());
        sEluna->OnActivateSpec(player, "Specialization " + newSpec.name + " successfully activated!", true, newSpecId);
        player->UpdateMastery();
        player->SaveToDB(false, false);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_activate_specialization::HandleProc);
    }
};

class SpecChoice_WorldScript : public WorldScript
{
public:
    SpecChoice_WorldScript() : WorldScript("SpecChoice_WorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        PlayerSpecialization::LoadAllSpecsPlayers();
        PlayerSpecialization::LoadAllSpecsSpells();
        PlayerSpecialization::InitializeSpecializations();
    }
};


// Add all scripts in one
void AddSC_SpecChoices()
{
    new SpecChoice_PlayerScripts();
    new SpecChoice_WorldScript();
    new SpecChoice_CommandsScript();
    RegisterSpellScript(spell_activate_specialization);
}
