/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Scripts for spells with SPELLFAMILY_WARLOCK and SPELLFAMILY_GENERIC spells used by warlock players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_warl_".
 */

#include "Pet.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "TemporarySummon.h"

enum WarlockSpells
{
    //THEIRS
    SPELL_WARLOCK_DRAIN_SOUL_R1                     = 1120,
    SPELL_WARLOCK_CREATE_SOULSHARD                  = 43836,
    SPELL_WARLOCK_CURSE_OF_DOOM_EFFECT              = 18662,
    SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON             = 48018,
    SPELL_WARLOCK_DEMONIC_CIRCLE_TELEPORT           = 48020,
    SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST         = 62388,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_SUCCUBUS      = 54435,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_VOIDWALKER    = 54443,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELGUARD      = 54508,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELHUNTER     = 54509,
    SPELL_WARLOCK_DEMONIC_EMPOWERMENT_IMP           = 54444,
    SPELL_WARLOCK_FEL_SYNERGY_HEAL                  = 54181,
    SPELL_WARLOCK_GLYPH_OF_DRAIN_SOUL_AURA          = 58070,
    SPELL_WARLOCK_GLYPH_OF_DRAIN_SOUL_PROC          = 58068,
    SPELL_WARLOCK_GLYPH_OF_SHADOWFLAME              = 63311,
    SPELL_WARLOCK_GLYPH_OF_SIPHON_LIFE              = 56216,
    //SPELL_WARLOCK_HAUNT                             = 48181,
    SPELL_WARLOCK_HAUNT_HEAL                        = 48210,
    SPELL_WARLOCK_IMPROVED_HEALTHSTONE_R1           = 18692,
    SPELL_WARLOCK_IMPROVED_HEALTHSTONE_R2           = 18693,
    SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_R1         = 18703,
    SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_R2         = 18704,
    SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R1    = 60955,
    SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R2    = 60956,
    SPELL_WARLOCK_LIFE_TAP_ENERGIZE                 = 31818,
    SPELL_WARLOCK_LIFE_TAP_ENERGIZE_2               = 32553,
    SPELL_WARLOCK_SOULSHATTER                       = 32835,
    SPELL_WARLOCK_SIPHON_LIFE_HEAL                  = 63106,
    SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL        = 31117,
    SPELL_WARLOCK_IMPROVED_DRAIN_SOUL_R1            = 18213,
    SPELL_WARLOCK_IMPROVED_DRAIN_SOUL_PROC          = 18371,

    //OURS
    SPELL_FELBOAR_CHARGE                            = 83005,
    SPELL_WARLOCK_AGONY                             = 83010,
    SPELL_WARLOCK_DARK_PACT                         = 59092,
    SPELL_WARLOCK_DARK_PACT_DAMAGE                  = 83011,
    SPELL_WARLOCK_DARK_PACT_SHIELD                  = 83012,
    SPELL_WARLOCK_HAUNT                             = 59164,
    SPELL_WARLOCK_SEED_OF_CORRUPTION                = 47836,
    SPELL_WARLOCK_SEED_OF_CORRUPTION_DETONATION     = 47834,
    SPELL_WARLOCK_SEED_OF_CORRUPTION_VISUAL         = 37826,
    SPELL_WARLOCK_SHADOWBURN                        = 47827,
    SPELL_WARLOCK_CONFLAGRATE                       = 17962,
    SPELL_WARLOCK_SOUL_FIRE                         = 47825,
    SPELL_WARLOCK_BURNING_RUSH_DAMAGE               = 83019,
    SPELL_WARLOCK_MALEFIC_RAPTURE_DAMAGE            = 83021,
    SPELL_WARLOCK_SOUL_STRIKE                       = 83039,
    SPELL_WARLOCK_IMMOLATE                          = 47811,
    TALENT_WARLOCK_SOUL_FLAME                       = 30054,
    TALENT_WARLOCK_MOLTEN_HAND                      = 47245,
    TALENT_WARLOCK_MOLTEN_HAND_LISTENER_R1          = 83073,
    TALENT_WARLOCK_MOLTEN_HAND_LISTENER_R2          = 83074,
    TALENT_WARLOCK_MOLTEN_HAND_LISTENER_R3          = 83075,
    TALENT_WARLOCK_MOLTEN_HAND_BUFF_R1              = 47383,
    TALENT_WARLOCK_MOLTEN_HAND_BUFF_R2              = 71162,
    TALENT_WARLOCK_MOLTEN_HAND_BUFF_R3              = 71165,
    TALENT_WARLOCK_MOLTEN_HAND_ADDITIONAL_R1        = 83076,
    TALENT_WARLOCK_MOLTEN_HAND_ADDITIONAL_R2        = 83077,
    TALENT_WARLOCK_MOLTEN_HAND_ADDITIONAL_R3        = 83078,

    SPELL_WARLOCK_GRIMOIRE_OF_SACRIFICE_DAMAGE      = 83055,
    SPELL_WARLOCK_GRIMOIRE_FELGUARD                 = 83031,
    SPELL_WARLOCK_CHARGE_FELGUARD                   = 25821,

    SPELL_WARLOCK_IMPLOSSION                        = 83038,
    SPELL_WARLOCK_DEMONBOLT_EMPOREWED               = 83041,

    SPELL_WARLOCK_HAVOC_AURA                        = 83062,
    SPELL_WARLOCK_HAVOC_DAMAGE                      = 83061,

    SPELL_MINION_STAT_DREAD_STALKER                 = 75001,
    SPELL_MINION_STAT_WILD_IMP                      = 75002,
    SPELL_MINION_STAT_DARKGLARE                     = 75003,
    SPELL_MINION_STAT_VILEFIEND                     = 75004,
    SPELL_MINION_STAT_DEMONIC_TYRANT                = 75005,
    SPELL_MINION_STAT_BOMBER                        = 74999,

    SPELL_MINION_INCREASE_DREAD_STALKER             = 1100009,
    SPELL_MINION_INCREASE_WILD_IMP                  = 1100010,
    SPELL_MINION_INCREASE_DARKGLARE                 = 1100011,
    SPELL_MINION_INCREASE_VILEFIEND                 = 1100012,
    SPELL_MINION_INCREASE_DEMONIC_TYRANT            = 1100013,
    SPELL_MINION_INCREASE_BOMBER                    = 1100014,
};

enum WarlockPets {

    PET_DARKGLARE = 600604,

    // Lesser
    PET_DARKHOUND = 600600,
    PET_FELBOAR = 600602,
    PET_WILDIMP = 600601,

    PET_FELGUARD_SUMMON = 600605,
    PET_GARGOYLE = 600607,

    PET_FELGUARD = 17252,

    // Greater
    PET_DEMONIC_TYRAN = 600603,
    NPC_PORTAL_SUMMON = 600606,
};

enum WarlockSpellIcons
{
    WARLOCK_ICON_ID_IMPROVED_LIFE_TAP               = 208,
    WARLOCK_ICON_ID_MANA_FEED                       = 1982
};

class spell_warl_eye_of_kilrogg : public AuraScript
{
    PrepareAuraScript(spell_warl_eye_of_kilrogg);

    void HandleAuraApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        PreventDefaultAction();
        if (Player* player = GetTarget()->ToPlayer())
        {
            player->UnsummonPetTemporaryIfAny();

            // Glyph of Kilrogg
            if (player->HasAura(58081))
                if (Unit* charm = player->GetCharm())
                {
                    charm->SetSpeed(MOVE_RUN, 2.14f, true);
                    if (charm->GetMapId() == 530 || charm->GetMapId() == 571)
                    {
                        charm->SetCanFly(true);
                        charm->SetSpeed(MOVE_FLIGHT, 2.14f, true);
                        charm->SendMovementFlagUpdate();
                    }
                }
        }
    }

    void HandleAuraRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* player = GetTarget()->ToPlayer())
        {
            if (Unit* charm = player->GetCharm())
                charm->ToTempSummon()->UnSummon();

            player->ResummonPetTemporaryUnSummonedIfAny();
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warl_eye_of_kilrogg::HandleAuraApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_warl_eye_of_kilrogg::HandleAuraRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_warl_shadowflame : public SpellScript
{
    PrepareSpellScript(spell_warl_shadowflame);

    void HandleSchoolDMG(SpellEffIndex /*effIndex*/)
    {
        if (Unit* target = GetHitUnit())
            GetCaster()->CastSpell(target, (GetSpellInfo()->Id == 47897 ? 47960 : 61291), true);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warl_shadowflame::HandleSchoolDMG, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_warl_seduction : public AuraScript
{
    PrepareAuraScript(spell_warl_seduction);

    void HandleAuraApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
        {
            if (Unit* owner = caster->GetOwner())
                if (owner->GetAuraEffectDummy(56250))
                {
                    Unit* target = GetTarget();
                    target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE, ObjectGuid::Empty, target->GetAura(32409)); // SW:D shall not be removed.
                    target->RemoveAurasByType(SPELL_AURA_PERIODIC_DAMAGE_PERCENT);
                    target->RemoveAurasByType(SPELL_AURA_PERIODIC_LEECH);
                }
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warl_seduction::HandleAuraApply, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_warl_improved_demonic_tactics : public AuraScript
{
    PrepareAuraScript(spell_warl_improved_demonic_tactics);

    bool Load() override
    {
        return GetUnitOwner()->GetTypeId() == TYPEID_PLAYER;
    }

    void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& isPeriodic, int32& amplitude)
    {
        isPeriodic = true;
        amplitude = 10 * IN_MILLISECONDS;
    }

    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool&  /*canBeRecalculated*/)
    {
        if (aurEff->GetEffIndex() == EFFECT_0)
            amount = CalculatePct<int32, float>(GetUnitOwner()->ToPlayer()->GetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + static_cast<uint8>(SPELL_SCHOOL_FROST)), GetSpellInfo()->Effects[EFFECT_0].CalcValue());
        else
            amount = CalculatePct<int32, float>(GetUnitOwner()->ToPlayer()->GetFloatValue(PLAYER_CRIT_PERCENTAGE), GetSpellInfo()->Effects[EFFECT_0].CalcValue());
    }

    void HandleEffectCalcSpellMod(AuraEffect const* aurEff, SpellModifier*& spellMod)
    {
        if (!spellMod)
        {
            spellMod = new SpellModifier(aurEff->GetBase());
            spellMod->op = SpellModOp(aurEff->GetMiscValue());
            spellMod->type = SPELLMOD_FLAT;
            spellMod->spellId = GetId();
            spellMod->mask = flag96(0x0, 0x2000, 0x0); // Pet Passive
        }

        spellMod->value = aurEff->GetAmount();
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        PreventDefaultAction();
        GetAura()->GetEffect(aurEff->GetEffIndex())->RecalculateAmount();
    }

    void Register() override
    {
        DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_warl_improved_demonic_tactics::CalcPeriodic, EFFECT_ALL, SPELL_AURA_DUMMY);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_improved_demonic_tactics::CalculateAmount, EFFECT_ALL, SPELL_AURA_DUMMY);
        DoEffectCalcSpellMod += AuraEffectCalcSpellModFn(spell_warl_improved_demonic_tactics::HandleEffectCalcSpellMod, EFFECT_ALL, SPELL_AURA_DUMMY);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_improved_demonic_tactics::HandlePeriodic, EFFECT_ALL, SPELL_AURA_DUMMY);
    }
};

class spell_warl_ritual_of_summoning : public SpellScript
{
    PrepareSpellScript(spell_warl_ritual_of_summoning);

    SpellCastResult CheckCast()
    {
        if (GetCaster()->GetTypeId() == TYPEID_PLAYER)
            if (GetCaster()->ToPlayer()->InBattleground())
                return SPELL_FAILED_NOT_IN_BATTLEGROUND;
        return SPELL_CAST_OK;
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_warl_ritual_of_summoning::CheckCast);
    }
};

class spell_warl_demonic_aegis : public AuraScript
{
    PrepareAuraScript(spell_warl_demonic_aegis);

    void HandleAuraApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        // Remove Fel Armor and Demon Armor
        GetTarget()->RemoveAurasWithFamily(SPELLFAMILY_WARLOCK, 0, 0x20000020, 0, ObjectGuid::Empty);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_warl_demonic_aegis::HandleAuraApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_warl_demonic_knowledge : public AuraScript
{
    PrepareAuraScript(spell_warl_demonic_knowledge);

    void CalculateAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Unit* caster = GetCaster())
            amount = CalculatePct(caster->GetStat(STAT_STAMINA) + caster->GetStat(STAT_INTELLECT), aurEff->GetBaseAmount());
    }

    void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& isPeriodic, int32& amplitude)
    {
        isPeriodic = true;
        amplitude = 5 * IN_MILLISECONDS;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        PreventDefaultAction();
        GetEffect(aurEff->GetEffIndex())->RecalculateAmount();
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_demonic_knowledge::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_DONE);
        DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_warl_demonic_knowledge::CalcPeriodic, EFFECT_0, SPELL_AURA_MOD_DAMAGE_DONE);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_demonic_knowledge::HandlePeriodic, EFFECT_0, SPELL_AURA_MOD_DAMAGE_DONE);
    }
};


class spell_warl_all_minion_scaling : public AuraScript
{
    PrepareAuraScript(spell_warl_all_minion_scaling);

    void CalculateAPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Player* owner = GetCaster()->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            int32 fire = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
            int32 shadow = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);
            int32 maximum = (fire > shadow) ? fire : shadow;
            amount = CalculatePct(std::max<int32>(0, maximum), 57);
        }
    }

    void CalculateSPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Player* owner = GetCaster()->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            int32 fire = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
            int32 shadow = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);
            int32 maximum = (fire > shadow) ? fire : shadow;
            amount = maximum;
        }
    }

    void CalculateDamageAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        if (Player* owner = GetCaster()->GetCharmerOrOwnerPlayerOrPlayerItself())
        {
            int32 newAmount = 0;

            if(m_scriptSpellId == SPELL_MINION_STAT_DREAD_STALKER)
                newAmount = owner->GetAura(SPELL_MINION_INCREASE_DREAD_STALKER)->GetEffect(EFFECT_0)->GetAmount();
            if (m_scriptSpellId == SPELL_MINION_STAT_WILD_IMP)
                newAmount = owner->GetAura(SPELL_MINION_INCREASE_WILD_IMP)->GetEffect(EFFECT_0)->GetAmount();
            if (m_scriptSpellId == SPELL_MINION_STAT_DARKGLARE)
                newAmount = owner->GetAura(SPELL_MINION_INCREASE_DARKGLARE)->GetEffect(EFFECT_0)->GetAmount();
            if (m_scriptSpellId == SPELL_MINION_STAT_VILEFIEND)
                newAmount = owner->GetAura(SPELL_MINION_INCREASE_VILEFIEND)->GetEffect(EFFECT_0)->GetAmount();
            if (m_scriptSpellId == SPELL_MINION_STAT_DEMONIC_TYRANT)
                newAmount = owner->GetAura(SPELL_MINION_INCREASE_DEMONIC_TYRANT)->GetEffect(EFFECT_0)->GetAmount();
            if (m_scriptSpellId == SPELL_MINION_STAT_BOMBER)
                newAmount = owner->GetAura(SPELL_MINION_INCREASE_BOMBER)->GetEffect(EFFECT_0)->GetAmount();

            if (newAmount > 0) {
                amount = newAmount;
            }
        }
    }


    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_all_minion_scaling::CalculateDamageAmount, EFFECT_2, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_all_minion_scaling::CalculateAPAmount, EFFECT_1, SPELL_AURA_MOD_ATTACK_POWER);
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_all_minion_scaling::CalculateSPAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_DONE);
    }
};

class spell_warl_generic_scaling : public AuraScript
{
    PrepareAuraScript(spell_warl_generic_scaling);

    void CalculateResistanceAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: pet inherits 40% of resistance from owner and 35% of armor
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            SpellSchoolMask schoolMask = SpellSchoolMask(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            int32 modifier = schoolMask == SPELL_SCHOOL_MASK_NORMAL ? 35 : 40;
            amount = CalculatePct(std::max<int32>(0, owner->GetResistance(schoolMask)), modifier);
        }
    }

    void CalculateStatAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: by default warlock pet inherits 75% of stamina and 30% of intellect
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            Stats stat = Stats(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            int32 modifier = stat == STAT_STAMINA ? 75 : 30;
            amount = CalculatePct(std::max<int32>(0, owner->GetStat(stat)), modifier);
        }
    }

    void CalculateAPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: by default warlock pet inherits 57% of max(SP FIRE, SP SHADOW) as AP
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 fire  = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
            int32 shadow = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);
            int32 maximum  = (fire > shadow) ? fire : shadow;
            amount = CalculatePct(std::max<int32>(0, maximum), 57);
        }
    }

    void CalculateSPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: by default warlock pet inherits 15% of max(SP FIRE, SP SHADOW) as SP
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 fire  = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
            int32 shadow = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);
            int32 maximum  = (fire > shadow) ? fire : shadow;
            amount = CalculatePct(std::max<int32>(0, maximum), 15);

            // xinef: Update appropriate player field
            if (owner->GetTypeId() == TYPEID_PLAYER)
                owner->SetUInt32Value(PLAYER_PET_SPELL_POWER, (uint32)amount);
        }
    }

    void CalcPeriodic(AuraEffect const* /*aurEff*/, bool& isPeriodic, int32& amplitude)
    {
        isPeriodic = true;
        amplitude = 2 * IN_MILLISECONDS;
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        PreventDefaultAction();
        if (aurEff->GetAuraType() == SPELL_AURA_MOD_STAT && (aurEff->GetMiscValue() == STAT_STAMINA || aurEff->GetMiscValue() == STAT_INTELLECT))
        {
            int32 currentAmount = aurEff->GetAmount();
            int32 newAmount = GetEffect(aurEff->GetEffIndex())->CalculateAmount(GetCaster());
            if (newAmount != currentAmount)
            {
                if (aurEff->GetMiscValue() == STAT_STAMINA)
                {
                    uint32 actStat = GetUnitOwner()->GetHealth();
                    GetEffect(aurEff->GetEffIndex())->ChangeAmount(newAmount, false);
                    GetUnitOwner()->SetHealth(std::min<uint32>(GetUnitOwner()->GetMaxHealth(), actStat));
                }
                else
                {
                    uint32 actStat = GetUnitOwner()->GetPower(POWER_MANA);
                    GetEffect(aurEff->GetEffIndex())->ChangeAmount(newAmount, false);
                    GetUnitOwner()->SetPower(POWER_MANA, std::min<uint32>(GetUnitOwner()->GetMaxPower(POWER_MANA), actStat));
                }
            }
        }
        else
            GetEffect(aurEff->GetEffIndex())->RecalculateAmount();
    }

    void Register() override
    {
        if (m_scriptSpellId != 34947)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_generic_scaling::CalculateResistanceAmount, EFFECT_ALL, SPELL_AURA_MOD_RESISTANCE);

        if (m_scriptSpellId == 34947 || m_scriptSpellId == 34956)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_generic_scaling::CalculateStatAmount, EFFECT_ALL, SPELL_AURA_MOD_STAT);

        if (m_scriptSpellId == 34947)
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_generic_scaling::CalculateAPAmount, EFFECT_ALL, SPELL_AURA_MOD_ATTACK_POWER);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_generic_scaling::CalculateSPAmount, EFFECT_ALL, SPELL_AURA_MOD_DAMAGE_DONE);
        }

        DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_warl_generic_scaling::CalcPeriodic, EFFECT_ALL, SPELL_AURA_ANY);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_generic_scaling::HandlePeriodic, EFFECT_ALL, SPELL_AURA_ANY);
    }
};

class spell_warl_infernal_scaling : public AuraScript
{
    PrepareAuraScript(spell_warl_infernal_scaling);

    void CalculateResistanceAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: pet inherits 40% of resistance from owner and 35% of armor
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            SpellSchoolMask schoolMask = SpellSchoolMask(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            int32 modifier = schoolMask == SPELL_SCHOOL_MASK_NORMAL ? 35 : 40;
            amount = CalculatePct(std::max<int32>(0, owner->GetResistance(schoolMask)), modifier);
        }
    }

    void CalculateStatAmount(AuraEffect const* aurEff, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: by default warlock pet inherits 75% of stamina and 30% of intellect
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            Stats stat = Stats(aurEff->GetSpellInfo()->Effects[aurEff->GetEffIndex()].MiscValue);
            int32 modifier = stat == STAT_STAMINA ? 75 : 30;
            amount = CalculatePct(std::max<int32>(0, owner->GetStat(stat)), modifier);
        }
    }

    void CalculateAPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: by default warlock pet inherits 57% of max(SP FIRE, SP SHADOW) as AP
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 fire  = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
            int32 shadow = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);
            int32 maximum  = (fire > shadow) ? fire : shadow;
            amount = CalculatePct(std::max<int32>(0, maximum), 57);
        }
    }

    void CalculateSPAmount(AuraEffect const*  /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        // xinef: by default warlock pet inherits 15% of max(SP FIRE, SP SHADOW) as SP
        if (Unit* owner = GetUnitOwner()->GetOwner())
        {
            int32 fire  = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_FIRE);
            int32 shadow = owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SHADOW);
            int32 maximum  = (fire > shadow) ? fire : shadow;
            amount = CalculatePct(std::max<int32>(0, maximum), 15);

            // xinef: Update appropriate player field
            if (owner->GetTypeId() == TYPEID_PLAYER)
                owner->SetUInt32Value(PLAYER_PET_SPELL_POWER, (uint32)amount);
        }
    }

    void HandleEffectApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, aurEff->GetAuraType(), true, SPELL_BLOCK_TYPE_POSITIVE);
        if (aurEff->GetAuraType() == SPELL_AURA_MOD_ATTACK_POWER)
            GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_ATTACK_POWER_PCT, true, SPELL_BLOCK_TYPE_POSITIVE);
        else if (aurEff->GetAuraType() == SPELL_AURA_MOD_STAT)
            GetUnitOwner()->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE, true, SPELL_BLOCK_TYPE_POSITIVE);
    }

    void Register() override
    {
        if (m_scriptSpellId != 36186)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_infernal_scaling::CalculateResistanceAmount, EFFECT_ALL, SPELL_AURA_MOD_RESISTANCE);

        if (m_scriptSpellId == 36186 || m_scriptSpellId == 36188)
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_infernal_scaling::CalculateStatAmount, EFFECT_ALL, SPELL_AURA_MOD_STAT);

        if (m_scriptSpellId == 36186)
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_infernal_scaling::CalculateAPAmount, EFFECT_ALL, SPELL_AURA_MOD_ATTACK_POWER);
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_infernal_scaling::CalculateSPAmount, EFFECT_ALL, SPELL_AURA_MOD_DAMAGE_DONE);
        }

        OnEffectApply += AuraEffectApplyFn(spell_warl_infernal_scaling::HandleEffectApply, EFFECT_ALL, SPELL_AURA_ANY, AURA_EFFECT_HANDLE_REAL);
    }
};

// -710 - Banish
class spell_warl_banish : public SpellScript
{
    PrepareSpellScript(spell_warl_banish);

    void HandleBanish(SpellMissInfo missInfo)
    {
        if (missInfo != SPELL_MISS_IMMUNE)
        {
            return;
        }

        if (Unit* target = GetHitUnit())
        {
            // Casting Banish on a banished target will remove applied aura
            if (Aura* banishAura = target->GetAura(GetSpellInfo()->Id, GetCaster()->GetGUID()))
            {
                banishAura->Remove();
            }
        }
    }

    void Register() override
    {
        BeforeHit += BeforeSpellHitFn(spell_warl_banish::HandleBanish);
    }
};

// 47193 - Demonic Empowerment
class spell_warl_demonic_empowerment : public SpellScript
{
    PrepareSpellScript(spell_warl_demonic_empowerment);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
            {
                SPELL_WARLOCK_DEMONIC_EMPOWERMENT_SUCCUBUS,
                SPELL_WARLOCK_DEMONIC_EMPOWERMENT_VOIDWALKER,
                SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELGUARD,
                SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELHUNTER,
                SPELL_WARLOCK_DEMONIC_EMPOWERMENT_IMP
            });
    }

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Creature* targetCreature = GetHitCreature())
        {
            if (targetCreature->IsPet())
            {
                CreatureTemplate const* ci = sObjectMgr->GetCreatureTemplate(targetCreature->GetEntry());
                switch (ci->family)
                {
                    case CREATURE_FAMILY_SUCCUBUS:
                        targetCreature->CastSpell(targetCreature, SPELL_WARLOCK_DEMONIC_EMPOWERMENT_SUCCUBUS, true);
                        break;
                    case CREATURE_FAMILY_VOIDWALKER:
                        {
                            SpellInfo const* spellInfo = sSpellMgr->AssertSpellInfo(SPELL_WARLOCK_DEMONIC_EMPOWERMENT_VOIDWALKER);
                            int32 hp = int32(targetCreature->CountPctFromMaxHealth(GetCaster()->CalculateSpellDamage(targetCreature, spellInfo, 0)));
                            targetCreature->CastCustomSpell(targetCreature, SPELL_WARLOCK_DEMONIC_EMPOWERMENT_VOIDWALKER, &hp, nullptr, nullptr, true);
                            //unitTarget->CastSpell(unitTarget, 54441, true);
                            break;
                        }
                    case CREATURE_FAMILY_FELGUARD:
                        targetCreature->CastSpell(targetCreature, SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELGUARD, true);
                        break;
                    case CREATURE_FAMILY_FELHUNTER:
                        targetCreature->CastSpell(targetCreature, SPELL_WARLOCK_DEMONIC_EMPOWERMENT_FELHUNTER, true);
                        break;
                    case CREATURE_FAMILY_IMP:
                        targetCreature->CastSpell(targetCreature, SPELL_WARLOCK_DEMONIC_EMPOWERMENT_IMP, true);
                        break;
                }
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warl_demonic_empowerment::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 6201 - Create Healthstone (and ranks)
class spell_warl_create_healthstone : public SpellScript
{
    PrepareSpellScript(spell_warl_create_healthstone);

    static uint32 const iTypes[8][3];

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_IMPROVED_HEALTHSTONE_R1, SPELL_WARLOCK_IMPROVED_HEALTHSTONE_R2 });
    }

    SpellCastResult CheckCast()
    {
        if (Player* caster = GetCaster()->ToPlayer())
        {
            uint8 spellRank = GetSpellInfo()->GetRank();
            ItemPosCountVec dest;
            InventoryResult msg = caster->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, iTypes[spellRank - 1][0], 1, nullptr);
            if (msg != EQUIP_ERR_OK)
                return SPELL_FAILED_TOO_MANY_OF_ITEM;
        }
        return SPELL_CAST_OK;
    }

    void HandleScriptEffect(SpellEffIndex effIndex)
    {
        if (Unit* unitTarget = GetHitUnit())
        {
            uint32 rank = 0;
            // Improved Healthstone
            if (AuraEffect const* aurEff = unitTarget->GetDummyAuraEffect(SPELLFAMILY_WARLOCK, 284, 0))
            {
                switch (aurEff->GetId())
                {
                    case SPELL_WARLOCK_IMPROVED_HEALTHSTONE_R1:
                        rank = 1;
                        break;
                    case SPELL_WARLOCK_IMPROVED_HEALTHSTONE_R2:
                        rank = 2;
                        break;
                    default:
                        LOG_ERROR("spells", "Unknown rank of Improved Healthstone id: {}", aurEff->GetId());
                        break;
                }
            }
            uint8 spellRank = GetSpellInfo()->GetRank();
            if (spellRank > 0 && spellRank <= 8)
                CreateItem(effIndex, iTypes[spellRank - 1][rank]);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warl_create_healthstone::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        OnCheckCast += SpellCheckCastFn(spell_warl_create_healthstone::CheckCast);
    }
};

uint32 const spell_warl_create_healthstone::spell_warl_create_healthstone::iTypes[8][3] =
{
    { 5512, 19004, 19005},              // Minor Healthstone
    { 5511, 19006, 19007},              // Lesser Healthstone
    { 5509, 19008, 19009},              // Healthstone
    { 5510, 19010, 19011},              // Greater Healthstone
    { 9421, 19012, 19013},              // Major Healthstone
    {22103, 22104, 22105},              // Master Healthstone
    {36889, 36890, 36891},              // Demonic Healthstone
    {36892, 36893, 36894}               // Fel Healthstone
};

// 47422 - Everlasting Affliction
class spell_warl_everlasting_affliction : public SpellScript
{
    PrepareSpellScript(spell_warl_everlasting_affliction);

    void HandleScriptEffect(SpellEffIndex /*effIndex*/)
    {
        if (Unit* unitTarget = GetHitUnit())
            // Refresh corruption on target
            if (AuraEffect* aur = unitTarget->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_WARLOCK, 0x2, 0, 0, GetCaster()->GetGUID()))
            {
                aur->GetBase()->RefreshTimersWithMods();
                aur->ChangeAmount(aur->CalculateAmount(aur->GetCaster()), false);
            }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warl_everlasting_affliction::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
    }
};

// 18541 - Ritual of Doom Effect
class spell_warl_ritual_of_doom_effect : public SpellScript
{
    PrepareSpellScript(spell_warl_ritual_of_doom_effect);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        caster->CastSpell(caster, GetEffectValue(), true);
    }

    void Register() override
    {
        OnEffectHit += SpellEffectFn(spell_warl_ritual_of_doom_effect::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// -27285 - Seed of Corruption
class spell_warl_seed_of_corruption : public SpellScript
{
    PrepareSpellScript(spell_warl_seed_of_corruption);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        targets.remove_if([&](WorldObject const* target)
        {
            if (Unit const* unitTarget = target->ToUnit())
            {
                if (WorldLocation const* dest = GetExplTargetDest())
                {
                    if (!unitTarget->IsWithinLOS(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ()))
                    {
                        return true;
                    }
                }
            }

            return false;
        });
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_seed_of_corruption::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

// 29858 - Soulshatter
class spell_warl_soulshatter : public SpellScript
{
    PrepareSpellScript(spell_warl_soulshatter);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_SOULSHATTER });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Unit* caster = GetCaster();
        if (Unit* target = GetHitUnit())
        {
            if (target->CanHaveThreatList() && target->GetThreatMgr().GetThreat(caster) > 0.0f)
                caster->CastSpell(target, SPELL_WARLOCK_SOULSHATTER, true);
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warl_soulshatter::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

// 63108 - Siphon Life
class spell_warl_siphon_life : public AuraScript
{
    PrepareAuraScript(spell_warl_siphon_life);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_SIPHON_LIFE_HEAL, SPELL_WARLOCK_GLYPH_OF_SIPHON_LIFE });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {

        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
        {
            return false;
        }

        return GetTarget()->IsAlive();
    }

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        int32 amount = CalculatePct(static_cast<int32>(eventInfo.GetDamageInfo()->GetDamage()), aurEff->GetAmount());
        // Glyph of Siphon Life
        if (AuraEffect const* glyph = GetTarget()->GetAuraEffect(SPELL_WARLOCK_GLYPH_OF_SIPHON_LIFE, EFFECT_0))
            AddPct(amount, glyph->GetAmount());

        GetTarget()->CastCustomSpell(SPELL_WARLOCK_SIPHON_LIFE_HEAL, SPELLVALUE_BASE_POINT0, amount, GetTarget(), true, nullptr, aurEff);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_warl_siphon_life::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_warl_siphon_life::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_warl_havoc : public AuraScript
{
    PrepareAuraScript(spell_warl_havoc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();
        SpellInfo const* spellInfo = eventInfo.GetSpellInfo();

        if (!spellInfo || !damageInfo || !damageInfo->GetDamage() || spellInfo->Id == SPELL_WARLOCK_HAVOC_DAMAGE)
        {
            return false;
        }

        return true;
    }

    Unit* FindTargetHavoc()
    {
        Unit* havocTarget = nullptr;
        auto const& threatList = GetCaster()->getAttackers();
        for (auto const& threat : threatList)
        {
            if (Unit* target = ObjectAccessor::GetUnit(*GetCaster(), threat->GetGUID())) {
                if (target->HasAura(SPELL_WARLOCK_HAVOC_AURA))
                    havocTarget = target;
            }
        }

        return havocTarget;
    }

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();
        int32 damagePourcentage = aurEff->GetAmount();
        int32 totalDamage = CalculatePct(damageInfo->GetDamage(), damagePourcentage);
        Unit* havocTarget = FindTargetHavoc();
        GetCaster()->CastCustomSpell(SPELL_WARLOCK_HAVOC_DAMAGE, SPELLVALUE_BASE_POINT0, totalDamage, havocTarget, true);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_warl_havoc::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_warl_havoc::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_vampirism : public AuraScript
{
    PrepareAuraScript(spell_vampirism);

    uint32 healPct;
    uint32 spellId;

    bool Load() override
    {
        healPct = GetAura()->GetEffect(EFFECT_0)->GetAmount();
        spellId = GetSpellInfo()->Effects[EFFECT_0].TriggerSpell;
        return true;
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        DamageInfo* damageInfo = eventInfo.GetDamageInfo();
        if (!damageInfo || !damageInfo->GetDamage())
        {
            return false;
        }
        return GetTarget()->IsAlive();
    }

    void TriggerHeal(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (DamageInfo* damageInfo = eventInfo.GetDamageInfo()) {
            int32 amount = CalculatePct(static_cast<int32>(eventInfo.GetDamageInfo()->GetDamage()), healPct);
            GetCaster()->CastCustomSpell(spellId, SPELLVALUE_BASE_POINT0, amount, GetCaster(), true);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_vampirism::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_vampirism::TriggerHeal, EFFECT_1, SPELL_AURA_DUMMY);
    }
};

// -1454 - Life Tap
#define LIFE_TAP_COEFFICIENT 0.9F
class spell_warl_life_tap : public SpellScript
{
    PrepareSpellScript(spell_warl_life_tap);

    bool Load() override
    {
        return GetCaster()->GetTypeId() == TYPEID_PLAYER;
    }

    bool Validate(SpellInfo const* /*spell*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_LIFE_TAP_ENERGIZE, SPELL_WARLOCK_LIFE_TAP_ENERGIZE_2 });
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        Player* caster = GetCaster()->ToPlayer();
        if (Unit* target = GetHitUnit())
        {
            int32 damage = GetEffectValue() + LIFE_TAP_COEFFICIENT;
            int32 damage2Mana = GetEffectValue();
            int32 mana = int32(damage2Mana + (caster->GetInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + static_cast<uint8>(SPELL_SCHOOL_SHADOW)) * 0.5f));

            // Shouldn't Appear in Combat Log
            target->ModifyHealth(-damage);

            // Improved Life Tap mod
            if (AuraEffect const* aurEff = caster->GetDummyAuraEffect(SPELLFAMILY_WARLOCK, WARLOCK_ICON_ID_IMPROVED_LIFE_TAP, 0))
                AddPct(mana, aurEff->GetAmount());

            caster->CastCustomSpell(target, SPELL_WARLOCK_LIFE_TAP_ENERGIZE, &mana, nullptr, nullptr, false);

            // Mana Feed
            int32 manaFeedVal = 0;
            if (AuraEffect const* aurEff = caster->GetAuraEffect(SPELL_AURA_ADD_FLAT_MODIFIER, SPELLFAMILY_WARLOCK, WARLOCK_ICON_ID_MANA_FEED, 0))
                manaFeedVal = aurEff->GetAmount();

            if (manaFeedVal > 0)
            {
                ApplyPct(manaFeedVal, mana);
                caster->CastCustomSpell(caster, SPELL_WARLOCK_LIFE_TAP_ENERGIZE_2, &manaFeedVal, nullptr, nullptr, true, nullptr);
            }
        }
    }

    SpellCastResult CheckCast()
    {
        if ((int32(GetCaster()->GetHealth()) > int32(GetSpellInfo()->Effects[EFFECT_0].CalcValue() + (3.1 * GetSpellInfo()->BaseLevel) + LIFE_TAP_COEFFICIENT )))
            return SPELL_CAST_OK;
        return SPELL_FAILED_FIZZLE;
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warl_life_tap::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        OnCheckCast += SpellCheckCastFn(spell_warl_life_tap::CheckCast);
    }
};

// 48018 - Demonic Circle: Summon
class spell_warl_demonic_circle_summon : public AuraScript
{
    PrepareAuraScript(spell_warl_demonic_circle_summon);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST, SPELL_WARLOCK_DEMONIC_CIRCLE_TELEPORT });
    }

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes mode)
    {
        // If effect is removed by expire remove the summoned demonic circle too.
        if (!(mode & AURA_EFFECT_HANDLE_REAPPLY))
            GetTarget()->RemoveGameObject(GetId(), true);

        GetTarget()->RemoveAura(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST);
    }

    void HandleDummyTick(AuraEffect const* /*aurEff*/)
    {
        if (GameObject* circle = GetTarget()->GetGameObject(GetId()))
        {
            // Here we check if player is in demonic circle teleport range, if so add
            // WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST; allowing him to cast the WARLOCK_DEMONIC_CIRCLE_TELEPORT.
            // If not in range remove the WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST.

            SpellInfo const* spellInfo = sSpellMgr->AssertSpellInfo(SPELL_WARLOCK_DEMONIC_CIRCLE_TELEPORT);

            if (GetTarget()->IsWithinDist(circle, spellInfo->GetMaxRange(true)))
            {
                if (!GetTarget()->HasAura(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST))
                    GetTarget()->CastSpell(GetTarget(), SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST, true);
            }
            else
                GetTarget()->RemoveAura(SPELL_WARLOCK_DEMONIC_CIRCLE_ALLOW_CAST);
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_warl_demonic_circle_summon::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_demonic_circle_summon::HandleDummyTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_warl_soul_power : public AuraScript
{
    PrepareAuraScript(spell_warl_soul_power);

    void HandleDummyTick(AuraEffect const* aurEff)
    {
        int32 maxRegenerate = aurEff->GetAmount();
        int32 amountRegenerate = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();

        if(GetCaster()->GetPower(POWER_ENERGY) < maxRegenerate)
           GetCaster()->ModifyPower(POWER_ENERGY, amountRegenerate);
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster() && !GetCaster()->IsInCombat() && GetCaster()->IsAlive();
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_warl_soul_power::CheckProc);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_soul_power::HandleDummyTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
    }
};

// 48020 - Demonic Circle: Teleport
class spell_warl_demonic_circle_teleport : public AuraScript
{
    PrepareAuraScript(spell_warl_demonic_circle_teleport);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON });
    }

    void HandleTeleport(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Player* player = GetTarget()->ToPlayer())
        {
            if (GameObject* circle = player->GetGameObject(SPELL_WARLOCK_DEMONIC_CIRCLE_SUMMON))
            {
                player->NearTeleportTo(circle->GetPositionX(), circle->GetPositionY(), circle->GetPositionZ(), circle->GetOrientation(), false, false, false, true);
                player->RemoveAurasWithMechanic(1 << MECHANIC_SNARE);
            }
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warl_demonic_circle_teleport::HandleTeleport, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// -47230 - Fel Synergy
class spell_warl_fel_synergy : public AuraScript
{
    PrepareAuraScript(spell_warl_fel_synergy);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_FEL_SYNERGY_HEAL });
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        // Xinef: Added charm check

        DamageInfo* damageInfo = eventInfo.GetDamageInfo();

        if (!damageInfo || !damageInfo->GetDamage())
        {
            return false;
        }

        return (GetTarget()->GetGuardianPet() || GetTarget()->GetCharm()) && damageInfo->GetDamage();
    }

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        int32 heal = CalculatePct(static_cast<int32>(eventInfo.GetDamageInfo()->GetDamage()), aurEff->GetAmount());
        GetTarget()->CastCustomSpell(SPELL_WARLOCK_FEL_SYNERGY_HEAL, SPELLVALUE_BASE_POINT0, heal, (Unit*)nullptr, true, nullptr, aurEff); // TARGET_UNIT_PET
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_warl_fel_synergy::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_warl_fel_synergy::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// -48181 - Haunt
class spell_warl_haunt : public SpellScript
{
    PrepareSpellScript(spell_warl_haunt);

    void HandleAfterHit()
    {
        if (Aura* aura = GetHitAura())
            if (AuraEffect* aurEff = aura->GetEffect(EFFECT_1))
                aurEff->SetAmount(CalculatePct(aurEff->GetAmount(), GetHitDamage()));
    }

    void Register() override
    {
        AfterHit += SpellHitFn(spell_warl_haunt::HandleAfterHit);
    }
};

class spell_warl_haunt_aura : public AuraScript
{
    PrepareAuraScript(spell_warl_haunt_aura);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_HAUNT_HEAL });
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* caster = GetCaster())
        {
            int32 amount = aurEff->GetAmount();
            GetTarget()->CastCustomSpell(caster, SPELL_WARLOCK_HAUNT_HEAL, &amount, nullptr, nullptr, true, nullptr, aurEff, GetCasterGUID());
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_warl_haunt_aura::HandleRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};

// -30108 - Unstable Affliction
class spell_warl_unstable_affliction : public AuraScript
{
    PrepareAuraScript(spell_warl_unstable_affliction);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL });
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ModifyPower(POWER_ENERGY, aurEff->GetAmount());
    }

    void HandleDispel(DispelInfo* dispelInfo)
    {
        if (Unit* caster = GetCaster())
            if (AuraEffect const* aurEff = GetEffect(EFFECT_0))
            {
                int32 damage = aurEff->GetBaseAmount();
                damage = aurEff->GetSpellInfo()->Effects[EFFECT_0].CalcValue(caster, &damage, nullptr) * 9;
                // backfire damage and silence
                caster->CastCustomSpell(dispelInfo->GetDispeller(), SPELL_WARLOCK_UNSTABLE_AFFLICTION_DISPEL, &damage, nullptr, nullptr, true, nullptr, aurEff);
            }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_warl_unstable_affliction::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        AfterDispel += AuraDispelFn(spell_warl_unstable_affliction::HandleDispel);
    }
};

// -603 - Curse of Doom
class spell_warl_curse_of_doom : public AuraScript
{
    PrepareAuraScript(spell_warl_curse_of_doom);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_CURSE_OF_DOOM_EFFECT });
    }

    bool Load() override
    {
        return GetCaster() && GetCaster()->GetTypeId() == TYPEID_PLAYER;
    }

    void OnRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        if (!GetCaster())
            return;

        AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
        if (removeMode != AURA_REMOVE_BY_DEATH || !IsExpired())
            return;

        if (GetCaster()->ToPlayer()->isHonorOrXPTarget(GetTarget()))
            GetCaster()->CastSpell(GetTarget(), SPELL_WARLOCK_CURSE_OF_DOOM_EFFECT, true, nullptr, aurEff);
    }

    void Register() override
    {
        AfterEffectRemove += AuraEffectRemoveFn(spell_warl_curse_of_doom::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
    }
};

// -755 - Health Funnel
class spell_warl_health_funnel : public AuraScript
{
    PrepareAuraScript(spell_warl_health_funnel);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({
            SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_R2,
            SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R2,
            SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_R1,
            SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R1
            });
    }

    void ApplyEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (!caster)
            return;

        Unit* target = GetTarget();
        if (caster->HasAura(SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_R2))
            target->CastSpell(target, SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R2, true);
        else if (caster->HasAura(SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_R1))
            target->CastSpell(target, SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R1, true);
    }

    void RemoveEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* target = GetTarget();
        target->RemoveAurasDueToSpell(SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R1);
        target->RemoveAurasDueToSpell(SPELL_WARLOCK_IMPROVED_HEALTH_FUNNEL_BUFF_R2);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_warl_health_funnel::RemoveEffect, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
        OnEffectApply += AuraEffectApplyFn(spell_warl_health_funnel::ApplyEffect, EFFECT_0, SPELL_AURA_PERIODIC_HEAL, AURA_EFFECT_HANDLE_REAL);
    }
};

// -6229 - Shadow Ward
class spell_warl_shadow_ward : public AuraScript
{
    PrepareAuraScript(spell_warl_shadow_ward);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        canBeRecalculated = false;
        if (Unit* caster = GetCaster())
        {
            // +80.68% from sp bonus
            float bonus = 0.8068f;

            bonus *= caster->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask());
            bonus *= caster->CalculateLevelPenalty(GetSpellInfo());

            amount += int32(bonus);
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_shadow_ward::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
    }
};

// 63310 - Glyph of Shadowflame
class spell_warl_glyph_of_shadowflame : public AuraScript
{
    PrepareAuraScript(spell_warl_glyph_of_shadowflame);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo({ SPELL_WARLOCK_GLYPH_OF_SHADOWFLAME });
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        GetTarget()->CastSpell(eventInfo.GetProcTarget(), SPELL_WARLOCK_GLYPH_OF_SHADOWFLAME, true, nullptr, aurEff);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_warl_glyph_of_shadowflame::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

// -1120 - Drain Soul
class spell_warl_drain_soul : public AuraScript
{
    PrepareAuraScript(spell_warl_drain_soul);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
        {
            SPELL_WARLOCK_IMPROVED_DRAIN_SOUL_R1,
            SPELL_WARLOCK_IMPROVED_DRAIN_SOUL_PROC,
            SPELL_WARLOCK_CREATE_SOULSHARD,
            SPELL_WARLOCK_GLYPH_OF_DRAIN_SOUL_AURA,
            SPELL_WARLOCK_GLYPH_OF_DRAIN_SOUL_PROC
        });
    }

    void RemoveEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetTarget();
        if (!(GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH && caster && target && caster->IsPlayer() && caster->ToPlayer()->isHonorOrXPTarget(target)))
        {
            PreventDefaultAction();
        }
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        // Drain Soul's proc tries to happen each time the warlock lands a killing blow on a unit while channeling.
        // Make sure that the dying unit is afflicted by the caster's Drain Soul debuff in order to avoid a false positive.

        Unit* caster = GetCaster();
        Unit* victim = eventInfo.GetProcTarget();

        if (caster && victim)
        {
            return victim->GetAuraApplicationOfRankedSpell(SPELL_WARLOCK_DRAIN_SOUL_R1, caster->GetGUID()) != 0;
        }

        return false;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        if (Unit* caster = eventInfo.GetActor())
        {
            // Improved Drain Soul.
            if (Aura const* impDrainSoul = caster->GetAuraOfRankedSpell(SPELL_WARLOCK_IMPROVED_DRAIN_SOUL_R1, caster->GetGUID()))
            {
                int32 amount = CalculatePct(caster->GetMaxPower(POWER_MANA), impDrainSoul->GetSpellInfo()->Effects[EFFECT_2].CalcValue());
                caster->CastCustomSpell(SPELL_WARLOCK_IMPROVED_DRAIN_SOUL_PROC, SPELLVALUE_BASE_POINT0, amount, caster, true, nullptr, aurEff, caster->GetGUID());
            }
        }
    }

    void HandleTick(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();
        Unit* target = GetTarget();

        if (caster && caster->GetTypeId() == TYPEID_PLAYER && caster->ToPlayer()->isHonorOrXPTarget(target))
        {
            if (roll_chance_i(20))
            {
                caster->CastSpell(caster, SPELL_WARLOCK_CREATE_SOULSHARD, aurEff);
                // Glyph of Drain Soul - chance to create an additional Soul Shard.
                if (AuraEffect* aur = caster->GetAuraEffect(SPELL_WARLOCK_GLYPH_OF_DRAIN_SOUL_AURA, EFFECT_0))
                {
                    if (roll_chance_i(aur->GetMiscValue()))
                    {
                        caster->CastSpell(caster, SPELL_WARLOCK_CREATE_SOULSHARD, aur);
                    }
                }
            }
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_warl_drain_soul::RemoveEffect, EFFECT_0, SPELL_AURA_CHANNEL_DEATH_ITEM, AURA_EFFECT_HANDLE_REAL);
        DoCheckProc += AuraCheckProcFn(spell_warl_drain_soul::CheckProc);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_drain_soul::HandleTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
        OnEffectProc += AuraEffectProcFn(spell_warl_drain_soul::HandleProc, EFFECT_2, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

// 29341 - Shadowburn
class spell_warl_shadowburn : public AuraScript
{
    PrepareAuraScript(spell_warl_shadowburn);

    void RemoveEffect(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        Unit* target = GetTarget();
        if (!(GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH && caster && target && caster->IsPlayer() && caster->ToPlayer()->isHonorOrXPTarget(target)))
        {
            PreventDefaultAction();
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_warl_shadowburn::RemoveEffect, EFFECT_0, SPELL_AURA_CHANNEL_DEATH_ITEM, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_warl_glyph_of_felguard : public AuraScript
{
    PrepareAuraScript(spell_warl_glyph_of_felguard);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            if (Pet* pet = player->GetPet())
            {
                if (pet->GetEntry() == NPC_FELGUARD)
                {
                    pet->HandleStatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_PCT, aurEff->GetAmount(), true);
                }
            }
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        if (Player* player = GetCaster()->ToPlayer())
        {
            if (Pet* pet = player->GetPet())
            {
                if (pet->GetEntry() == NPC_FELGUARD)
                {
                    pet->HandleStatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_PCT, aurEff->GetAmount(), false);
                }
            }
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warl_glyph_of_felguard::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_warl_glyph_of_felguard::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

// 83000
class spell_warlock_summon_darkhound : public SpellScript
{
    PrepareSpellScript(spell_warlock_summon_darkhound);

    void HandleCast()
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* target = GetExplTargetUnit();

        int32 totalSummons = GetSpellInfo()->GetEffect(EFFECT_0).CalcValue(player);

        for (size_t i = 0; i < totalSummons; i++)
        {
            int32 duration = GetSpellInfo()->GetDuration();
            TempSummon* summon = GetCaster()->SummonCreatureGuardian(PET_DARKHOUND, player, player, duration, WARLOCK_PET_DARK_HOUND_DIST + i, PET_FOLLOW_ANGLE);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_warlock_summon_darkhound::HandleCast);
    }
};

class spell_warlock_summon_gargoyle : public SpellScript
{
    PrepareSpellScript(spell_warlock_summon_gargoyle);

    SpellCastResult CheckCast()
    {
        if (Player* player = GetCaster()->ToPlayer())
            if (player->GetSelectedUnit())
                return SPELL_CAST_OK;

        return SPELL_FAILED_NO_VALID_TARGETS;
    }

    void HandleCast()
    {
        Player* player = GetCaster()->ToPlayer();

        int32 duration = GetSpellInfo()->GetDuration();
        TempSummon* summon = GetCaster()->SummonCreatureGuardian(PET_GARGOYLE, player->GetSelectedUnit(), player, duration, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
        if (summon)
            summon->AI()->AttackStart(player->GetSelectedUnit());

    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_warlock_summon_gargoyle::CheckCast);
        OnCast += SpellCastFn(spell_warlock_summon_gargoyle::HandleCast);
    }
};

class spell_warlock_soul_strike : public SpellScript
{
    PrepareSpellScript(spell_warlock_soul_strike);

    Creature* GetPet()
    {
        Creature* controlledUnit = nullptr;
        if (Player* player = GetCaster()->ToPlayer())
            for (Unit::ControlSet::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
                if (Creature* pet = (*itr)->ToCreature())
                    if (pet->IsAlive() &&
                        pet->GetOwnerGUID() == player->GetGUID() &&
                        pet->GetEntry() == PET_FELGUARD &&
                        pet->IsWithinDist(player, 100.0f, false))
                        controlledUnit = pet;

        return controlledUnit;
    }

    SpellCastResult CheckCast()
    {
        Creature* pet = GetPet();

        if (pet)
            return SPELL_CAST_OK;

        return SPELL_FAILED_NO_PET;
    }

    void HandleCast()
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* target = player->GetSelectedUnit();
        Creature* pet = GetPet();
        uint32 energy = sSpellMgr->AssertSpellInfo(SPELL_WARLOCK_SOUL_STRIKE)->GetEffect(EFFECT_1).CalcValue(GetCaster());

        if (pet && target)
        {
            pet->AI()->AttackStart(target);
            pet->CastSpell(target, SPELL_WARLOCK_SOUL_STRIKE, true, nullptr, nullptr, player->GetGUID());
            player->ModifyPower(POWER_ENERGY, energy);
        }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_warlock_soul_strike::CheckCast);
        OnCast += SpellCastFn(spell_warlock_soul_strike::HandleCast);
    }
};

class spell_warlock_summon_felboar : public SpellScript
{
    PrepareSpellScript(spell_warlock_summon_felboar);

    void HandleCast()
    {
        Player* player = GetCaster()->ToPlayer();
        int32 totalSummons = GetSpellInfo()->GetEffect(EFFECT_0).CalcValue(player);
        Unit* target = GetExplTargetUnit();

        for (size_t i = 0; i < totalSummons; i++)
        {
            int32 duration = GetSpellInfo()->GetDuration();
            TempSummon* summon = GetCaster()->SummonCreatureGuardian(PET_FELBOAR, player, player, duration, WARLOCK_PET_FELBOAR + i, PET_FOLLOW_ANGLE);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_warlock_summon_felboar::HandleCast);
    }
};

class spell_warlock_summon_felguard : public SpellScript
{
    PrepareSpellScript(spell_warlock_summon_felguard);

    void HandleCast()
    {
        Player* player = GetCaster()->ToPlayer();

        int32 duration = GetSpellInfo()->GetDuration();
        TempSummon* summon = GetCaster()->SummonCreatureGuardian(PET_FELGUARD_SUMMON, player, player, duration, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_warlock_summon_felguard::HandleCast);
    }
};

class spell_warlock_implosion : public SpellScript
{
    PrepareSpellScript(spell_warlock_implosion);

    void HandleCast()
    {
        Player* player = GetCaster()->ToPlayer();
        for (auto itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
        {
            if (Unit* pet = *itr)
            {
                if (pet->GetEntry() == PET_WILDIMP)
                {
                    pet->JumpTo(GetExplTargetUnit(), 0.5f);
                    pet->CastSpell(GetExplTargetUnit(), SPELL_WARLOCK_IMPLOSSION, true, nullptr, nullptr, player->GetGUID());
                    pet->ToTempSummon()->DespawnOrUnsummon();
                }
            }
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_warlock_implosion::HandleCast);
    }
};

class spell_warlock_summon_darkglare : public SpellScript
{
    PrepareSpellScript(spell_warlock_summon_darkglare);

    void HandleCast()
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* target = GetExplTargetUnit();

        int32 duration = GetSpellInfo()->GetDuration();
        TempSummon* summon = GetCaster()->SummonCreatureGuardian(PET_DARKGLARE, player, player, duration, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        for (auto const& target : targets)
        {
            if (Unit const* unitTarget = target->ToUnit())
            {
                auto const& auras = unitTarget->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);
                for (Unit::AuraEffectList::const_iterator itr = auras.begin(); itr != auras.end(); ++itr)
                {
                    if ((*itr)->GetCasterGUID() == GetCaster()->GetGUID())
                    {
                        (*itr)->GetBase()->RefreshDuration();
                        (*itr)->ResetTicks();
                    }
                }
            }
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_warlock_summon_darkglare::HandleCast);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warlock_summon_darkglare::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class spell_warlock_cataclysm : public SpellScript
{
    PrepareSpellScript(spell_warlock_cataclysm);

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        for (auto const& target : targets)
        {
            if (Unit* unitTarget = target->ToUnit())
            {
                if (!unitTarget->HasAura(SPELL_WARLOCK_IMMOLATE))
                    GetCaster()->CastSpell(unitTarget, SPELL_WARLOCK_IMMOLATE, true);
            }
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warlock_cataclysm::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class spell_warlock_summon_demonic_tyrant : public SpellScript
{
    PrepareSpellScript(spell_warlock_summon_demonic_tyrant);

    void HandleCast()
    {
        Player* player = GetCaster()->ToPlayer();

        int32 timerIncrease = GetSpellInfo()->GetEffect(EFFECT_0).CalcValue(player);

        int32 duration = GetSpellInfo()->GetDuration();
        GetCaster()->SummonCreatureGuardian(PET_DEMONIC_TYRAN, player, player, duration, WARLOCK_PET_TYRANT, PET_FOLLOW_ANGLE);

        for (auto itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
            if (Unit* pet = *itr)
                if (TempSummon* summon = pet->ToTempSummon())
                {
                    if (summon->GetEntry() == PET_FELBOAR || summon->GetEntry() == PET_DARKHOUND)
                        summon->SetTimer(summon->GetTimer() + timerIncrease);
                }
                    

    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_warlock_summon_demonic_tyrant::HandleCast);
    }
};

// 83002
class spell_warlock_hand_of_guldan : public SpellScript
{
    PrepareSpellScript(spell_warlock_hand_of_guldan);

    void HandleHitTarget(SpellEffIndex /*effIndex*/)
    {
        int32 runicPower = GetCaster()->GetPower(POWER_ENERGY);
        uint8 maxSummon = std::min(runicPower, 3);
        Player* player = GetCaster()->ToPlayer();

        player->ModifyPower(POWER_ENERGY, -maxSummon);

        if (Unit* target = GetHitUnit()) {
            for (size_t i = 0; i < maxSummon; i++)
            {
                TempSummon* summon = GetCaster()->SummonCreatureGuardian(PET_WILDIMP, target, player, 30000, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE + i);
                if (AuraEffect const* aurEff = GetCaster()->GetAuraEffectOfRankedSpell(TALENT_WARLOCK_MOLTEN_HAND, EFFECT_0))
                {
                    uint32 rank = aurEff->GetAmount();

                    if (rank == 1)
                        player->AddAura(TALENT_WARLOCK_MOLTEN_HAND_LISTENER_R1, summon);
                    if (rank == 2)
                        player->AddAura(TALENT_WARLOCK_MOLTEN_HAND_LISTENER_R2, summon);
                    if (rank == 3)
                        player->AddAura(TALENT_WARLOCK_MOLTEN_HAND_LISTENER_R3, summon);
                }
            }
        }
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if(!targets.empty())
            targets.resize(1);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warlock_hand_of_guldan::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
        OnEffectHitTarget += SpellEffectFn(spell_warlock_hand_of_guldan::HandleHitTarget, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
    }
};

class spell_warlock_summon_nether_portal : public SpellScript
{
    PrepareSpellScript(spell_warlock_summon_nether_portal);

    void HandleCast()
    {
        Player* player = GetCaster()->ToPlayer();

        int32 timerIncrease = GetSpellInfo()->GetEffect(EFFECT_0).CalcValue(player);

        int32 duration = GetSpellInfo()->GetDuration();
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(83);
        Position pos = GetCaster()->GetNearPosition(PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
        TempSummon* summon = GetCaster()->GetMap()->SummonCreature(NPC_PORTAL_SUMMON, pos, properties, duration, GetCaster(), GetSpellInfo()->Id);
        summon->AddAura(40280, summon);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_warlock_summon_nether_portal::HandleCast);
    }
};

class spell_warl_nether_portal_proc: public AuraScript
{
    PrepareAuraScript(spell_warl_nether_portal_proc);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo() && eventInfo.GetSpellInfo()->ManaCost > 0;
    }


    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 soulPowerSpent = eventInfo.GetSpellInfo()->ManaCost;

        int32 currentSoulPowerSpent = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();
        int32 soulPowerThreshold = aurEff->GetAmount();

        if ((soulPowerSpent + currentSoulPowerSpent) >= soulPowerThreshold)
        {
            Player* player = GetCaster()->ToPlayer();
            Unit* portal = player->FindNearestCreature(NPC_PORTAL_SUMMON, 40.f, true);
            if (portal) {
               player->SummonCreatureGuardian(PET_WILDIMP, portal, player, 30000, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE + urand(1, 5));
            }
            aurEff->GetBase()->GetEffect(EFFECT_0)->SetAmount(0);
        }
        else
            aurEff->GetBase()->GetEffect(EFFECT_0)->SetAmount(currentSoulPowerSpent + soulPowerSpent);

    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_warl_nether_portal_proc::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_warl_nether_portal_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_warl_agony : public AuraScript
{
    PrepareAuraScript(spell_warl_agony);

    void OnApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_WARLOCK_AGONY);
        uint32 initialStack = value->GetEffect(EFFECT_2).CalcValue(GetCaster());

        aurEff->GetBase()->SetStackAmount(initialStack);
    }

    void OnReApply(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_WARLOCK_AGONY);
        uint32 maxStack = value->GetEffect(EFFECT_1).CalcValue(GetCaster());
        if (aurEff->GetBase()->GetStackAmount() > maxStack)
            aurEff->GetBase()->SetStackAmount(maxStack);
    }

    void OnPeriodic(AuraEffect const* aurEff)
    {
        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_WARLOCK_AGONY);
        uint32 maxStack = value->GetEffect(EFFECT_1).CalcValue(GetCaster());
        uint32 currentStack = aurEff->GetBase()->GetStackAmount();
        if (currentStack < maxStack)
            aurEff->GetBase()->SetStackAmount(currentStack + 1);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warl_agony::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        OnEffectApply += AuraEffectApplyFn(spell_warl_agony::OnReApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAPPLY);
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_agony::OnPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

class spell_warlock_dark_pact : public SpellScript
{
    PrepareSpellScript(spell_warlock_dark_pact);

    uint32 shieldAmount = 0;

    void HandleBeforeCast()
    {
        Unit* caster = GetCaster();
        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_WARLOCK_DARK_PACT);
        int32 sacrificePct = CalculatePct(caster->GetHealth(), value->GetEffect(EFFECT_0).CalcValue(caster));
        shieldAmount = CalculatePct(sacrificePct, value->GetEffect(EFFECT_1).CalcValue(caster)) + CalculatePct(caster->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask()), value->GetEffect(EFFECT_2).CalcValue(caster));

        caster->CastCustomSpell(SPELL_WARLOCK_DARK_PACT_DAMAGE, SPELLVALUE_BASE_POINT0, sacrificePct, caster, TRIGGERED_FULL_MASK);
    }

    void HandleAfterCast()
    {
        Unit* caster = GetCaster();
        caster->CastCustomSpell(SPELL_WARLOCK_DARK_PACT_SHIELD, SPELLVALUE_BASE_POINT0, shieldAmount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(spell_warlock_dark_pact::HandleBeforeCast);
        AfterCast += SpellCastFn(spell_warlock_dark_pact::HandleAfterCast);
    }
};

class spell_warl_demon_armor : public AuraScript
{
    PrepareAuraScript(spell_warl_demon_armor);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = GetUnitOwner()->CountPctFromMaxHealth(amount);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_demon_armor::CalculateAmount, EFFECT_2, SPELL_AURA_MOD_INCREASE_HEALTH);
    }
};

class spell_warl_fel_armor : public AuraScript
{
    PrepareAuraScript(spell_warl_fel_armor);

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {  
        amount = CalculatePct(GetCaster()->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask()), amount);
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_fel_armor::CalculateAmount, EFFECT_2, SPELL_AURA_MOD_DAMAGE_DONE);
    }
};

class spell_warl_health_funnel_new : public AuraScript
{
    PrepareAuraScript(spell_warl_health_funnel_new);

    bool Load() override
    {
        Unit* caster = GetCaster();
        return caster && caster->GetTypeId() == TYPEID_PLAYER;
    }

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& canBeRecalculated)
    {
        if (Unit* caster = GetCaster())
        {
            amount = CalculatePct(GetCaster()->ToPlayer()->GetMaxHealth(), amount) * 2 / 5;
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_warl_health_funnel_new::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
    }
};

class spell_warl_haunt_reset : public AuraScript
{
    PrepareAuraScript(spell_warl_haunt_reset);


    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(SPELL_WARLOCK_HAUNT, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_warl_haunt_reset::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_warl_seed_of_corruption_handler : public AuraScript
{
    PrepareAuraScript(spell_warl_seed_of_corruption_handler);

    void HandleAfterEffect(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        int32 amount = CalculatePct(GetCaster()->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask()), aurEff->GetAmount());
        aurEff->GetBase()->GetEffect(EFFECT_0)->SetAmount(amount);
    }

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0 && eventInfo.GetSpellInfo() && eventInfo.GetActor()->GetGUID() == GetCaster()->GetGUID() && GetCaster()->IsAlive();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = GetAura()->GetOwner()->ToUnit();
        Unit* caster = GetCaster();
        AuraEffect* effect1 = aurEff->GetBase()->GetEffect(EFFECT_1);
        int32 threshold = aurEff->GetAmount();
        int32 savedAmount = effect1->GetAmount();
        int32 damageTaken = eventInfo.GetDamageInfo()->GetDamage();

        if (damageTaken >= threshold || savedAmount >= threshold || eventInfo.GetSpellInfo()->Id == SPELL_WARLOCK_SEED_OF_CORRUPTION_DETONATION)
        {
            target->RemoveAura(SPELL_WARLOCK_SEED_OF_CORRUPTION);
        }
        else
        {
            effect1->SetAmount(savedAmount + damageTaken);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* target = GetTarget();
        Unit* caster = GetCaster();

        caster->CastSpell(target, SPELL_WARLOCK_SEED_OF_CORRUPTION_DETONATION, TRIGGERED_FULL_MASK);
        target->CastSpell(target, SPELL_WARLOCK_SEED_OF_CORRUPTION_VISUAL, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        AfterEffectApply += AuraEffectApplyFn(spell_warl_seed_of_corruption_handler::HandleAfterEffect, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        DoCheckProc += AuraCheckProcFn(spell_warl_seed_of_corruption_handler::CheckProc);
        OnEffectProc += AuraEffectProcFn(spell_warl_seed_of_corruption_handler::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(spell_warl_seed_of_corruption_handler::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_warl_drain_soul_energy : public AuraScript
{
    PrepareAuraScript(spell_warl_drain_soul_energy);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ModifyPower(POWER_ENERGY, aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_warl_drain_soul_energy::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_warl_chaos_bolt : public SpellScript
{
    PrepareSpellScript(spell_warl_chaos_bolt);

    void HandleHit(SpellEffIndex effIndex)
    {
        Unit* caster = GetCaster();
        int32 damageRatio = GetEffectValue();
        int32 critChance = caster->GetFloatValue(static_cast<uint16>(PLAYER_SPELL_CRIT_PERCENTAGE1) + SPELL_SCHOOL_FIRE);
        int32 damage = CalculatePct(caster->SpellBaseDamageBonusDone(GetSpellInfo()->GetSchoolMask()), damageRatio);
        damage = AddPct(damage, critChance);

        if (Unit* target = GetHitUnit())
        {
            damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }

        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warl_chaos_bolt::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_warl_conflagrate_energy : public SpellScript
{
    PrepareSpellScript(spell_warl_conflagrate_energy);

    void HandleCast()
    {
        Unit* caster = GetCaster();
        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_WARLOCK_CONFLAGRATE);
        uint32 energyAmount = value->GetEffect(EFFECT_1).CalcValue(caster);
        GetCaster()->ModifyPower(POWER_ENERGY, energyAmount);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_warl_conflagrate_energy::HandleCast);
    }
};

class spell_warl_immolate_energy : public AuraScript
{
    PrepareAuraScript(spell_warl_immolate_energy);

    void OnPeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster();

        if (!caster || !caster->IsAlive())
            return;

        caster->ModifyPower(POWER_ENERGY, 1);

        uint32 procChance = aurEff->GetAmount();

        if (roll_chance_f(procChance) && caster->GetTypeId() == TYPEID_PLAYER)
        {
            caster->ModifyPower(POWER_ENERGY, 1);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_immolate_energy::OnPeriodic, EFFECT_2, SPELL_AURA_PERIODIC_DAMAGE);
    }
};

class spell_warl_incinerate_energy : public AuraScript
{
    PrepareAuraScript(spell_warl_incinerate_energy);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ModifyPower(POWER_ENERGY, aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_warl_incinerate_energy::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_warl_shadowburn_death : public AuraScript
{
    PrepareAuraScript(spell_warl_shadowburn_death);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        caster->ModifyPower(POWER_ENERGY, aurEff->GetAmount());
        caster->ToPlayer()->RemoveSpellCooldown(SPELL_WARLOCK_SHADOWBURN, true);

    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_warl_shadowburn_death::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_warl_soul_fire_energy : public SpellScript
{
    PrepareSpellScript(spell_warl_soul_fire_energy);

    void HandleCast()
    {
        Unit* caster = GetCaster();
        SpellInfo const* value = sSpellMgr->AssertSpellInfo(SPELL_WARLOCK_SOUL_FIRE);
        uint32 energyAmount = value->GetEffect(EFFECT_1).CalcValue(caster);
        GetCaster()->ModifyPower(POWER_ENERGY, energyAmount);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_warl_soul_fire_energy::HandleCast);
    }
};

class spell_warl_power_siphon : public SpellScript
{
    PrepareSpellScript(spell_warl_power_siphon);


    Creature* GetPet()
    {
        Creature* controlledUnit = nullptr;
        if (Player* player = GetCaster()->ToPlayer())
            for (Unit::ControlSet::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
                if (Creature* pet = (*itr)->ToCreature())
                    if (pet->IsAlive() &&
                        pet->GetOwnerGUID() == player->GetGUID() &&
                        pet->GetEntry() == PET_WILDIMP &&
                        pet->IsWithinDist(player, 100.0f, false))
                        controlledUnit = pet;

        return controlledUnit;
    }

    SpellCastResult CheckCast()
    {
        Creature* pet = GetPet();

        if (pet)
            return SPELL_CAST_OK;

        return SPELL_FAILED_NO_PET;
    }

    void HandleCast()
    {
        Unit* caster = GetCaster();
        uint32 maxSacrifice = GetSpellInfo()->GetEffect(EFFECT_0).CalcValue(caster);
        uint8 totalSacrifice = 0;
        if (Player* player = GetCaster()->ToPlayer())
            for (Unit::ControlSet::const_iterator itr = player->m_Controlled.begin(); itr != player->m_Controlled.end(); ++itr)
            {
                if (Creature* pet = (*itr)->ToCreature())
                {
                    if (pet->IsAlive() && pet->GetOwnerGUID() == player->GetGUID() && pet->GetEntry() == PET_WILDIMP && pet->IsWithinDist(player, 100.0f, false))
                    {
                        if (totalSacrifice >= maxSacrifice)
                            return;
                        else {
                            if (Aura* aura = caster->GetAura(SPELL_WARLOCK_DEMONBOLT_EMPOREWED))
                                aura->SetCharges(aura->GetCharges() + 1);
                            else
                            {
                                pet->ToTempSummon()->UnSummon();
                                caster->CastCustomSpell(SPELL_WARLOCK_DEMONBOLT_EMPOREWED, SPELLVALUE_AURA_CHARGE, 1, caster, true);
                                totalSacrifice++;
                            }
                        }
                    }
                }
            }
    }

    void Register() override
    {
        OnCheckCast += SpellCheckCastFn(spell_warl_power_siphon::CheckCast);
        OnCast += SpellCastFn(spell_warl_power_siphon::HandleCast);
    }
};

class spell_warl_burning_rush : public AuraScript
{
    PrepareAuraScript(spell_warl_burning_rush);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* caster = GetCaster()->ToPlayer();
        uint32 healthPct = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount();
        uint32 damage = CalculatePct(caster->GetMaxHealth(), healthPct);
        caster->CastCustomSpell(SPELL_WARLOCK_BURNING_RUSH_DAMAGE, SPELLVALUE_BASE_POINT0, damage, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(spell_warl_burning_rush::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class spell_warl_malefic_rapture : public SpellScript
{
    PrepareSpellScript(spell_warl_malefic_rapture);

    void HandleCast()
    {
        Unit* caster = GetCaster();
        auto const& threatList = caster->getAttackers();
        for (auto const& threat : threatList)
        {
            if (Unit* target = ObjectAccessor::GetUnit(*caster, threat->GetGUID())) {
                auto targetAuras = target->GetAppliedAuras();
                for (auto itr = targetAuras.begin(); itr != targetAuras.end(); ++itr) {
                    if (Aura* aura = itr->second->GetBase())
                    {
                        if (caster->GetGUID() != aura->GetCasterGUID())
                            continue;

                        SpellInfo const* auraInfo = aura->GetSpellInfo();

                        if (auraInfo->SpellFamilyFlags[2] & 0x80000000 && auraInfo->SpellFamilyName == SPELLFAMILY_WARLOCK)
                            caster->CastSpell(target, SPELL_WARLOCK_MALEFIC_RAPTURE_DAMAGE, TRIGGERED_FULL_MASK);
                    }
                }
            }
        }
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_warl_malefic_rapture::HandleCast);
    }
};

class spell_mage_soul_swap : public AuraScript
{
    PrepareAuraScript(spell_mage_soul_swap);

    struct CopyAura {
        uint32 spellId;
        int32 duration;
    };

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        Player* player = GetCaster()->ToPlayer();

        if (!player || player->isDead())
            return;

        Unit* target = player->GetSelectedUnit();

        if (target) {
            auto effects = target->GetAuraEffectsByType(SPELL_AURA_PERIODIC_DAMAGE);

            for (auto effect : effects) {
                uint32 spellId = effect->GetSpellInfo()->Id;
                int32 duration = effect->GetBase()->GetDuration();
                int32 amount = effect->GetAmount();
                CopyAura aura = { spellId, duration };
                aurasCopied.push_back(aura);
            }
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes  mode)
    {
        Player* player = GetCaster()->ToPlayer();
        AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
        Unit* target = player->GetSelectedUnit();

        if (target) {
            for (auto aura : aurasCopied) {
                CustomSpellValues values;
                values.AddSpellMod(SPELLVALUE_AURA_DURATION, aura.duration);
                GetCaster()->CastCustomSpell(aura.spellId, values, target, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_mage_soul_swap::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        AfterEffectRemove += AuraEffectRemoveFn(spell_mage_soul_swap::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }

private:
    std::list<CopyAura> aurasCopied;
};

class spell_warl_grimoire_of_sacrifice : public AuraScript
{
    PrepareAuraScript(spell_warl_grimoire_of_sacrifice);

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        int32 amount = GetCaster()->CalculateSpellDamageWithRatio(SPELL_SCHOOL_MASK_SHADOW, 0.375f);
        int32 chance = aurEff->GetAmount();

        if (roll_chance_i(chance)) {
            GetCaster()->CastCustomSpell(SPELL_WARLOCK_GRIMOIRE_OF_SACRIFICE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, GetCaster()->GetVictim(), true, nullptr, aurEff);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_warl_grimoire_of_sacrifice::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_warl_channel_demonfire : public SpellScript
{
    PrepareSpellScript(spell_warl_channel_demonfire);

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        if (GetCaster() && GetHitUnit())
            if(!GetHitUnit()->HasAura(47811))
                GetCaster()->CastSpell(GetHitUnit(), 47811, false);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_warl_channel_demonfire::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_warl_xavians_teachings : public AuraScript
{
    PrepareAuraScript(spell_warl_xavians_teachings);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 spell = aurEff->GetAmount();

        GetCaster()->CastSpell(GetCaster()->ToPlayer()->GetSelectedUnit(), spell, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_warl_xavians_teachings::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_warl_agonizing_corruption : public SpellScript
{
    PrepareSpellScript(spell_warl_agonizing_corruption);

    void FindTargets(std::list<WorldObject*>& targets)
    {
        if (targets.size() > 0)
        {
            for (auto const& target : targets)
                if (Aura* agony = target->ToUnit()->GetAura(SPELL_WARLOCK_AGONY))
                {
                    uint32 increase = sSpellMgr->AssertSpellInfo(SPELL_WARLOCK_SEED_OF_CORRUPTION_DETONATION)->GetEffect(EFFECT_1).CalcValue(GetCaster());
                    agony->ModStackAmount(increase);
                }
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_warl_agonizing_corruption::FindTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

class spell_warl_soul_flame : public AuraScript
{
    PrepareAuraScript(spell_warl_soul_flame);

    void HandleProc(ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        AuraEffect const* aurEff = caster->GetAuraEffectOfRankedSpell(TALENT_WARLOCK_SOUL_FLAME, EFFECT_0);
        uint32 spell = aurEff->GetAmount();

        caster->CastSpell(eventInfo.GetProcTarget(), spell, TRIGGERED_FULL_MASK); 
    }

    void Register() override
    {
        DoPrepareProc += AuraProcFn(spell_warl_soul_flame::HandleProc);
    }
};

class spell_warl_molten_hand_listener : public AuraScript
{
    PrepareAuraScript(spell_warl_molten_hand_listener);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        uint32 rank = aurEff->GetAmount();
        Unit* owner = GetAura()->GetOwner()->ToUnit();
        if (rank == 1)
            owner->CastSpell(owner, TALENT_WARLOCK_MOLTEN_HAND_BUFF_R1, TRIGGERED_FULL_MASK);
        if (rank == 2)
            owner->CastSpell(owner, TALENT_WARLOCK_MOLTEN_HAND_BUFF_R2, TRIGGERED_FULL_MASK);
        if (rank == 3)
            owner->CastSpell(owner, TALENT_WARLOCK_MOLTEN_HAND_BUFF_R3, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_warl_molten_hand_listener::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_warl_felstorm_molten_hand : public SpellScript
{
    PrepareSpellScript(spell_warl_felstorm_molten_hand);

    void HandleCast()
    {
        if (Aura* aura = GetCaster()->GetAura(TALENT_WARLOCK_MOLTEN_HAND_BUFF_R1))
            aura->ModStackAmount(-1);
        else if (Aura* aura = GetCaster()->GetAura(TALENT_WARLOCK_MOLTEN_HAND_BUFF_R2))
            aura->ModStackAmount(-1);
        else if (Aura* aura = GetCaster()->GetAura(TALENT_WARLOCK_MOLTEN_HAND_BUFF_R3))
            aura->ModStackAmount(-1);
    }

    void Register() override
    {
        OnCast += SpellCastFn(spell_warl_felstorm_molten_hand::HandleCast);
    }
};

class spell_warl_molten_hand_aura : public AuraScript
{
    PrepareAuraScript(spell_warl_molten_hand_aura);

    void HandleApply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (AuraEffect const* aurEff = caster->GetAuraEffectOfRankedSpell(TALENT_WARLOCK_MOLTEN_HAND, EFFECT_0))
        {
            uint32 rank = aurEff->GetAmount();

            if (rank == 1)
                caster->CastSpell(caster, TALENT_WARLOCK_MOLTEN_HAND_ADDITIONAL_R1, TRIGGERED_FULL_MASK);
            if (rank == 2)
                caster->CastSpell(caster, TALENT_WARLOCK_MOLTEN_HAND_ADDITIONAL_R2, TRIGGERED_FULL_MASK);
            if (rank == 3)
                caster->CastSpell(caster, TALENT_WARLOCK_MOLTEN_HAND_ADDITIONAL_R3, TRIGGERED_FULL_MASK);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
    {
        Unit* caster = GetCaster();
        if (AuraEffect const* aurEff = caster->GetAuraEffectOfRankedSpell(TALENT_WARLOCK_MOLTEN_HAND, EFFECT_0))
        {
            uint32 rank = aurEff->GetAmount();

            if (rank == 1)
                caster->RemoveAura(TALENT_WARLOCK_MOLTEN_HAND_ADDITIONAL_R1);
            if (rank == 2)
                caster->RemoveAura(TALENT_WARLOCK_MOLTEN_HAND_ADDITIONAL_R2);
            if (rank == 3)
                caster->RemoveAura(TALENT_WARLOCK_MOLTEN_HAND_ADDITIONAL_R3);
        }
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_warl_molten_hand_aura::HandleApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_warl_molten_hand_aura::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_warlock_spell_scripts()
{
    RegisterSpellScript(spell_warl_eye_of_kilrogg);
    RegisterSpellScript(spell_warl_shadowflame);
    RegisterSpellScript(spell_warl_seduction);
    RegisterSpellScript(spell_warl_improved_demonic_tactics);
    RegisterSpellScript(spell_warl_ritual_of_summoning);
    RegisterSpellScript(spell_warl_demonic_aegis);
    RegisterSpellScript(spell_warl_demonic_knowledge);
    RegisterSpellScript(spell_warl_generic_scaling);
    RegisterSpellScript(spell_warl_infernal_scaling);
    RegisterSpellScript(spell_warl_banish);
    RegisterSpellScript(spell_warl_create_healthstone);
    RegisterSpellScript(spell_warl_curse_of_doom);
    RegisterSpellScript(spell_warl_demonic_circle_summon);
    RegisterSpellScript(spell_warl_demonic_circle_teleport);
    RegisterSpellScript(spell_warl_demonic_empowerment);
    RegisterSpellScript(spell_warl_everlasting_affliction);
    RegisterSpellScript(spell_warl_fel_synergy);
    RegisterSpellScript(spell_warl_glyph_of_shadowflame);
    //RegisterSpellAndAuraScriptPair(spell_warl_haunt, spell_warl_haunt_aura);
    //RegisterSpellScript(spell_warl_health_funnel);
    //RegisterSpellScript(spell_warl_life_tap);
    RegisterSpellScript(spell_warl_ritual_of_doom_effect);
    RegisterSpellScript(spell_warl_seed_of_corruption);
    RegisterSpellScript(spell_warl_shadow_ward);
    RegisterSpellScript(spell_warl_siphon_life);
    RegisterSpellScript(spell_warl_soulshatter);
    RegisterSpellScript(spell_warl_unstable_affliction);
    //RegisterSpellScript(spell_warl_drain_soul);
    //RegisterSpellScript(spell_warl_shadowburn);
    RegisterSpellScript(spell_warl_glyph_of_felguard);
    RegisterSpellScript(spell_warlock_summon_darkglare);
    RegisterSpellScript(spell_warlock_summon_darkhound);
    RegisterSpellScript(spell_warlock_hand_of_guldan);
    RegisterSpellScript(spell_warlock_summon_felboar);
    RegisterSpellScript(spell_warlock_summon_felguard);
    RegisterSpellScript(spell_warlock_summon_demonic_tyrant);
    RegisterSpellScript(spell_warl_agony);
    RegisterSpellScript(spell_warlock_dark_pact);
    RegisterSpellScript(spell_warl_demon_armor);
    RegisterSpellScript(spell_warl_fel_armor);
    RegisterSpellScript(spell_warl_health_funnel_new);
    RegisterSpellScript(spell_warl_haunt_reset);
    RegisterSpellScript(spell_warl_seed_of_corruption_handler);
    RegisterSpellScript(spell_warl_drain_soul_energy);
    RegisterSpellScript(spell_warl_chaos_bolt);
    RegisterSpellScript(spell_warl_conflagrate_energy);
    RegisterSpellScript(spell_warl_immolate_energy);
    RegisterSpellScript(spell_warl_incinerate_energy);
    RegisterSpellScript(spell_warl_shadowburn_death);
    RegisterSpellScript(spell_warl_soul_fire_energy);
    RegisterSpellScript(spell_warl_burning_rush);
    RegisterSpellScript(spell_warl_malefic_rapture);
    RegisterSpellScript(spell_warl_grimoire_of_sacrifice);
    RegisterSpellScript(spell_warl_channel_demonfire);
    RegisterSpellScript(spell_warlock_soul_strike);
    RegisterSpellScript(spell_warl_nether_portal_proc);
    RegisterSpellScript(spell_warlock_summon_nether_portal);
    RegisterSpellScript(spell_warlock_implosion);
    RegisterSpellScript(spell_warl_power_siphon);
    RegisterSpellScript(spell_warlock_summon_gargoyle);
    RegisterSpellScript(spell_warl_havoc);
    RegisterSpellScript(spell_warl_soul_power);
    RegisterSpellScript(spell_mage_soul_swap);
    RegisterSpellScript(spell_warlock_cataclysm);
    RegisterSpellScript(spell_warl_xavians_teachings);
    RegisterSpellScript(spell_warl_agonizing_corruption);
    RegisterSpellScript(spell_warl_soul_flame);
    RegisterSpellScript(spell_warl_molten_hand_listener);
    RegisterSpellScript(spell_warl_felstorm_molten_hand);
    RegisterSpellScript(spell_warl_molten_hand_aura);
}
