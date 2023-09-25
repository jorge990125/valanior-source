#include "PetDefines.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Totem.h"
#include "UnitAI.h"
#include "Log.h"


enum SpellsWarrior {
    SPELL_WARR_THUNDERCLAP = 47502,
    SPELL_WARR_THUNDERCLAP_SON_OF_THUNDER = 84663,
    SPELL_WARR_IGNORE_PAIN = 80004,
    SPELL_WARR_SHIELD_SLAM = 47488,
    SPELL_WARR_REND = 47465,
    SPELL_WARR_RAGING_BLOW = 80008,
    SPELL_WARR_SHIELD_BLOCK = 2565,
    SPELL_WARR_SHIELD_WALL = 871,
    SPELL_WARR_COLOSSAL_THRUST = 84557,
    SPELL_WARR_SECOND_CRUSHING_STRIKE = 84664,
    SPELL_WARR_HULN_FURY = 84653,
    SPELL_WARR_BATTLE_TRANCE = 84560,
    SPELL_WARR_DEVASTATOR = 200868,
    SPELL_WARR_TEST_OF_MIGHT = 200887,
};

class spell_cut_the_veins : public AuraScript
{
    PrepareAuraScript(spell_cut_the_veins);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200038))
            return GetCaster()->GetAura(200038);

        if (GetCaster()->HasAura(200039))
            return GetCaster()->GetAura(200039);

        if (GetCaster()->HasAura(200040))
            return GetCaster()->GetAura(200040);

        if (GetCaster()->HasAura(200041))
            return GetCaster()->GetAura(200041);

        if (GetCaster()->HasAura(200042))
            return GetCaster()->GetAura(200042);

        if (GetCaster()->HasAura(200043))
            return GetCaster()->GetAura(200043);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints;
    }

    int GetDamagePct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();

        if (!GetCaster())
            return;

        if (GetRuneAura())
        {
            int32 totalTicks = sSpellMgr->AssertSpellInfo(GetProcSpell())->GetMaxTicks();
            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), GetDamagePct()) / totalTicks);
            int32 maxAmount = int32(CalculatePct(GetCaster()->GetMaxHealth(), 50));

            if (AuraEffect* protEff = eventInfo.GetProcTarget()->GetAuraEffect(GetProcSpell(), 0))
            {
                int32 remainingTicks = totalTicks - protEff->GetTickNumber();
                int32 remainingAmount = protEff->GetAmount() * remainingTicks;
                int32 remainingAmountPerTick = remainingAmount / totalTicks;

                amount = (std::min<int32>(amount + remainingAmountPerTick, maxAmount));
            }
            eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), GetProcSpell(), SPELL_AURA_PERIODIC_DAMAGE, amount, TRIGGERED_IGNORE_AURA_SCALING);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_cut_the_veins::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


class spell_the_art_of_war : public AuraScript
{
    PrepareAuraScript(spell_the_art_of_war);

    int GetRagePct()
    {
        return GetAura()->GetEffect(EFFECT_0)->GetAmount();
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        Aura* aura = GetAura();

        if (!aura)
            return;

        int32 spellRage=  eventInfo.GetSpellInfo()->CalcPowerCost(GetCaster(), SpellSchoolMask(eventInfo.GetSpellInfo()->SchoolMask));
        int32 rageAccumulated = GetAura()->GetEffect(EFFECT_1)->GetAmount() + spellRage;

        if (spellRage <= 0)
            return;

        if (rageAccumulated >= GetRagePct())
        {
            GetCaster()->CastSpell(GetCaster(), 200093, TRIGGERED_FULL_MASK);
            aura->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - GetRagePct());
        }
        else
        {
            aura->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_the_art_of_war::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


// crash...
class spell_tide_of_blood : public AuraScript
{
    PrepareAuraScript(spell_tide_of_blood);

    void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
    {
        Aura* aura = GetAura();
        Unit* caster = GetCaster();
        Unit* target = GetTarget();

        if (!target || GetCaster()->isDead())
            return;

        if (!aura)
            return;

        Aura* auraStack = caster->GetAura(200107);

        if (!auraStack)
            return;

        uint32 stacksAmount = auraStack->GetStackAmount();
        uint32 requireStacks = aura->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;

        if (stacksAmount < requireStacks)
            return;

        int32 damage = aura->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints + 1;
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        damage = caster->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, 0);
        damage = target->SpellDamageBonusTaken(caster, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);

        caster->CastCustomSpell(200106, SPELLVALUE_BASE_POINT0, damage, caster, TRIGGERED_FULL_MASK);
        caster->RemoveAura(200107);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_tide_of_blood::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_vein_cutter : public AuraScript
{
    PrepareAuraScript(spell_vein_cutter);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200108))
            return GetCaster()->GetAura(200108);

        if (GetCaster()->HasAura(200109))
            return GetCaster()->GetAura(200109);

        if (GetCaster()->HasAura(200110))
            return GetCaster()->GetAura(200110);

        if (GetCaster()->HasAura(200111))
            return GetCaster()->GetAura(200111);

        if (GetCaster()->HasAura(200112))
            return GetCaster()->GetAura(200112);

        if (GetCaster()->HasAura(200113))
            return GetCaster()->GetAura(200113);

        return nullptr;
    }

    int GetProcSpell()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).BasePoints;
    }

    int GetDamagePct()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).BasePoints + 1;
    }

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        if (GetRuneAura())
        {
            int32 totalTicks = sSpellMgr->AssertSpellInfo(GetProcSpell())->GetMaxTicks();
            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), GetDamagePct()) / totalTicks);
            int32 maxAmount = int32(CalculatePct(GetCaster()->GetMaxHealth(), 50));

            if (AuraEffect* protEff = eventInfo.GetProcTarget()->GetAuraEffect(GetProcSpell(), 0))
            {
                int32 remainingTicks = totalTicks - protEff->GetTickNumber();
                int32 remainingAmount = protEff->GetAmount() * remainingTicks;
                int32 remainingAmountPerTick = remainingAmount / totalTicks;

                amount = (std::min<int32>(amount + remainingAmountPerTick, maxAmount));
            }
            eventInfo.GetProcTarget()->CastDelayedSpellWithPeriodicAmount(eventInfo.GetActor(), GetProcSpell(), SPELL_AURA_PERIODIC_DAMAGE, amount, TRIGGERED_IGNORE_AURA_SCALING);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_vein_cutter::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_battlelord : public AuraScript
{
    PrepareAuraScript(spell_battlelord);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* caster = GetTarget()->ToPlayer())
        {
            caster->RemoveSpellCooldown(47486, true);
            caster->RemoveSpellCooldown(47520, true);
            /*caster->ModifySpellCooldown(47486, -aurEff->GetAmount());
            caster->ModifySpellCooldown(47520, -aurEff->GetAmount());*/
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_battlelord::HandleEffectProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_tactician : public AuraScript
{
    PrepareAuraScript(spell_tactician);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 spellRage=  eventInfo.GetSpellInfo()->CalcPowerCost(GetCaster(), SpellSchoolMask(eventInfo.GetSpellInfo()->SchoolMask));
        float procPctPerRagePoint = aurEff->GetSpellInfo()->Effects[EFFECT_0].DamageMultiplier;

        if (spellRage <= 0)
            return;

        float procChance = spellRage * procPctPerRagePoint;
        uint32 random = urand(1, 100);

        if (random <= procChance)
            GetCaster()->ToPlayer()->ModifySpellCooldown(7384, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_tactician::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_dreadnaught : public SpellScript
{
    PrepareSpellScript(spell_dreadnaught);

    void HandleHit(SpellEffIndex effIndex)
    {
        int32 damage = GetEffectValue();
        ApplyPct(damage, GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK));

        if (Unit* target = GetHitUnit())
        {
            damage = GetCaster()->SpellDamageBonusDone(target, GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE, effIndex);
            damage = target->SpellDamageBonusTaken(GetCaster(), GetSpellInfo(), uint32(damage), SPELL_DIRECT_DAMAGE);
        }
        SetHitDamage(damage);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(spell_dreadnaught::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class spell_blood_and_thunder : public SpellScript
{
    PrepareSpellScript(spell_blood_and_thunder);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200193))
            return GetCaster()->GetAura(200193);

        if (GetCaster()->HasAura(200194))
            return GetCaster()->GetAura(200194);

        if (GetCaster()->HasAura(200195))
            return GetCaster()->GetAura(200195);

        if (GetCaster()->HasAura(200196))
            return GetCaster()->GetAura(200196);

        if (GetCaster()->HasAura(200197))
            return GetCaster()->GetAura(200197);

        if (GetCaster()->HasAura(200198))
            return GetCaster()->GetAura(200198);

        return nullptr;
    }

    void FindTargets(std::list<WorldObject*>& targets)
    {
        if (!GetRuneAura())
            return;

        if (targets.size() > 0)
        {
            for (auto const& target : targets)
                if (Unit* unit = target->ToUnit())
                    if(!unit->HasAura(47465))
                        GetCaster()->CastSpell(unit, 200237, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_blood_and_thunder::FindTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class spell_anger_management : public AuraScript
{
    PrepareAuraScript(spell_anger_management);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 spellRage=  eventInfo.GetSpellInfo()->CalcPowerCost(GetCaster(), SpellSchoolMask(eventInfo.GetSpellInfo()->SchoolMask));
        int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount() + spellRage;
        int32 rageThreshold = aurEff->GetAmount();

        if (spellRage <= 0)
            return;

        if (rageAccumulated >= rageThreshold)
        {
            GetCaster()->ToPlayer()->ModifySpellCooldown(46924, -aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount());
            aurEff->GetBase()->GetEffect(EFFECT_2)->SetAmount(rageAccumulated - rageThreshold);
        }
        else
            aurEff->GetBase()->GetEffect(EFFECT_2)->SetAmount(rageAccumulated);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_anger_management::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_blood_dance : public AuraScript
{
    PrepareAuraScript(spell_blood_dance);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetProcTarget();

        if (!target || target->isDead())
            return;

        if (!target->HasAura(47465))
            return;

        AuraEffect* rendEff = target->GetAuraEffect(47465, 0);
        int32 tickDamage = rendEff->GetAmount();
        int32 remainingTicks = rendEff->GetSpellInfo()->GetMaxTicks() - rendEff->GetTickNumber();
        int32 remainingDamage = tickDamage * remainingTicks;

        int32 amount = CalculatePct(remainingDamage, aurEff->GetAmount());

        GetCaster()->CastCustomSpell(200217, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetActionTarget(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_blood_dance::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_keep_spinnin : public AuraScript
{
    PrepareAuraScript(spell_keep_spinnin);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->ModifySpellCooldown(46924, -aurEff->GetBase()->GetEffect(EFFECT_0)->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_keep_spinnin::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sweeping_rage : public AuraScript
{
    PrepareAuraScript(spell_sweeping_rage);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (!GetCaster()->HasAura(12328))
            return;

        int32 spellRage=  eventInfo.GetSpellInfo()->CalcPowerCost(GetCaster(), SpellSchoolMask(eventInfo.GetSpellInfo()->SchoolMask));

        if (spellRage <= 0)
            return;

        int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
        int32 rageThreshold = aurEff->GetAmount();

        if (rageAccumulated >= rageThreshold)
        {
            GetCaster()->CastSpell(GetCaster(), 200244, TRIGGERED_FULL_MASK);
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
        }
        else
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sweeping_rage::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sweeping_rage_proc : public AuraScript
{
    PrepareAuraScript(spell_sweeping_rage_proc);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster()->HasAura(200244))
            return;

        int32 stackAmount = GetCaster()->GetAura(200244)->GetStackAmount();

        GetCaster()->RemoveAura(200244);
        GetCaster()->CastSpell(GetCaster(), 200245, TRIGGERED_FULL_MASK);
        GetCaster()->GetAura(200245)->SetStackAmount(stackAmount);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_sweeping_rage_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_collateral_damage : public AuraScript
{
    PrepareAuraScript(spell_collateral_damage);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(12328);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_collateral_damage::CheckProc);
    }
};

class spell_collateral_damage_proc : public AuraScript
{
    PrepareAuraScript(spell_collateral_damage_proc);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200246))
            return GetCaster()->GetAura(200246);

        if (GetCaster()->HasAura(200247))
            return GetCaster()->GetAura(200247);

        if (GetCaster()->HasAura(200248))
            return GetCaster()->GetAura(200248);

        if (GetCaster()->HasAura(200249))
            return GetCaster()->GetAura(200249);

        if (GetCaster()->HasAura(200250))
            return GetCaster()->GetAura(200250);

        if (GetCaster()->HasAura(200251))
            return GetCaster()->GetAura(200251);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster()->HasAura(200252))
            return;

        int32 stackAmount = GetCaster()->GetAura(200252)->GetStackAmount();
        int32 damageAmount = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();

        GetCaster()->RemoveAura(200252);
        GetCaster()->CastCustomSpell(200253, SPELLVALUE_BASE_POINT0, damageAmount, GetCaster(), TRIGGERED_FULL_MASK);
        GetCaster()->GetAura(200253)->SetStackAmount(stackAmount);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_collateral_damage_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_vicious_comtempt : public AuraScript
{
    PrepareAuraScript(spell_vicious_comtempt);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(23881, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_vicious_comtempt::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_charging_for_blood : public AuraScript
{
    PrepareAuraScript(spell_charging_for_blood);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(23881, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_charging_for_blood::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_bloodcraze : public AuraScript
{
    PrepareAuraScript(spell_bloodcraze);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->CastSpell(eventInfo.GetActionTarget(), 23881, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_bloodcraze::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class spell_fervor_of_battle : public SpellScript
{
    PrepareSpellScript(spell_fervor_of_battle);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200370))
            return GetCaster()->GetAura(200370);

        if (GetCaster()->HasAura(200371))
            return GetCaster()->GetAura(200371);

        if (GetCaster()->HasAura(200372))
            return GetCaster()->GetAura(200372);

        if (GetCaster()->HasAura(200373))
            return GetCaster()->GetAura(200373);

        if (GetCaster()->HasAura(200374))
            return GetCaster()->GetAura(200374);

        if (GetCaster()->HasAura(200375))
            return GetCaster()->GetAura(200375);

        return nullptr;
    }

    void FindTargets(std::list<WorldObject*>& targets)
    {
        if (!GetRuneAura())
            return;

        if (targets.size() > 0)
        {
            for (auto const& target : targets)
                if (Unit* unit = target->ToUnit())
                {
                    GetCaster()->CastSpell(unit, 47475, TRIGGERED_FULL_MASK);
                    return;
                }
        }
        else {
            GetCaster()->CastSpell(GetExplTargetUnit(), 47475, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_fervor_of_battle::FindTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class spell_storm_of_swords : public AuraScript
{
    PrepareAuraScript(spell_storm_of_swords);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (GetCaster()->HasAura(1719))
        {
            float remainingDuration = GetCaster()->GetAura(1719)->GetDuration();
            GetCaster()->GetAura(1719)->SetDuration(remainingDuration + 2000);
        }
        else
        {
            GetCaster()->CastSpell(GetCaster(), 1719, TRIGGERED_FULL_MASK);
            GetCaster()->GetAura(1719)->SetDuration(5000);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_storm_of_swords::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_spinning_grip : public SpellScript
{
    PrepareSpellScript(spell_spinning_grip);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200388))
            return GetCaster()->GetAura(200388);

        if (GetCaster()->HasAura(200389))
            return GetCaster()->GetAura(200389);

        if (GetCaster()->HasAura(200390))
            return GetCaster()->GetAura(200390);

        if (GetCaster()->HasAura(200391))
            return GetCaster()->GetAura(200391);

        if (GetCaster()->HasAura(200392))
            return GetCaster()->GetAura(200392);

        if (GetCaster()->HasAura(200393))
            return GetCaster()->GetAura(200393);

        return nullptr;
    }

    void FindTargets(std::list<WorldObject*>& targets)
    {
        if (!GetRuneAura())
            return;

        if (targets.size() > 0)
        {
            for (auto const& target : targets)
                if (Unit* unit = target->ToUnit())
                    unit->CastSpell(GetCaster(), 200394, TRIGGERED_FULL_MASK);
        }
    }


    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_spinning_grip::FindTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    }
};

class spell_spinning_grip_jump : public SpellScript
{
    PrepareSpellScript(spell_spinning_grip_jump);

    void HandleSpecial(SpellEffIndex effIndex)
    {
        PreventHitDefaultEffect(effIndex);

        float x = GetHitUnit()->GetPositionX();
        float y = GetHitUnit()->GetPositionY();
        float z = GetHitUnit()->GetPositionZ() + 0.1f;
        float speedXY, speedZ;

        if (GetSpellInfo()->Effects[effIndex].MiscValue)
            speedZ = float(GetSpellInfo()->Effects[effIndex].MiscValue) / 10;
        else if (GetSpellInfo()->Effects[effIndex].MiscValueB)
            speedZ = float(GetSpellInfo()->Effects[effIndex].MiscValueB) / 10;
        else
            speedZ = 5.0f;
        speedXY = GetCaster()->GetExactDist2d(x, y) * 10.0f / speedZ;

        GetCaster()->GetMotionMaster()->MoveJump(x, y, z, speedXY, speedZ);
    }

    void Register() override
    {
        OnEffectLaunchTarget += SpellEffectFn(spell_spinning_grip_jump::HandleSpecial, EFFECT_0, SPELL_EFFECT_JUMP);
    }
};

class spell_depths_of_insanity : public AuraScript
{
    PrepareAuraScript(spell_depths_of_insanity);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 spellRage =  eventInfo.GetSpellInfo()->CalcPowerCost(GetCaster(), SpellSchoolMask(eventInfo.GetSpellInfo()->SchoolMask));

        if (spellRage <= 0)
            return;

        int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
        int32 rageThreshold = aurEff->GetAmount();

        if (rageAccumulated >= rageThreshold)
        {
            GetCaster()->ToPlayer()->ModifySpellCooldown(1719, -aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount());
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
        }
        else
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_depths_of_insanity::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_berserkers_tourment : public AuraScript
{
    PrepareAuraScript(spell_berserkers_tourment);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200401))
            return GetCaster()->GetAura(200401);

        if (GetCaster()->HasAura(200402))
            return GetCaster()->GetAura(200402);

        if (GetCaster()->HasAura(200403))
            return GetCaster()->GetAura(200403);

        if (GetCaster()->HasAura(200404))
            return GetCaster()->GetAura(200404);

        if (GetCaster()->HasAura(200405))
            return GetCaster()->GetAura(200405);

        if (GetCaster()->HasAura(200406))
            return GetCaster()->GetAura(200406);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->AddAura(200407, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster()->HasAura(200407))
            return;

        GetCaster()->RemoveAura(200407);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_berserkers_tourment::HandleProc, EFFECT_0, SPELL_AURA_MOD_RAGE_FROM_DAMAGE_DEALT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_berserkers_tourment::HandleRemove, EFFECT_0, SPELL_AURA_MOD_RAGE_FROM_DAMAGE_DEALT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_berserkers_tourment_proc : public AuraScript
{
    PrepareAuraScript(spell_berserkers_tourment_proc);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (!GetCaster()->HasAura(1719))
            return;

        uint32 damage = CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount());
        GetCaster()->DealDamage(GetCaster(), GetCaster(), damage, nullptr, DIRECT_DAMAGE);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_berserkers_tourment_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_reckless_abandon : public AuraScript
{
    PrepareAuraScript(spell_reckless_abandon);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (!GetCaster()->HasAura(1719))
            return;

        int32 spellRage=  eventInfo.GetSpellInfo()->CalcPowerCost(GetCaster(), SpellSchoolMask(eventInfo.GetSpellInfo()->SchoolMask));

        if (spellRage <= 0)
            return;

        int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
        int32 rageThreshold = aurEff->GetAmount();

        if (rageAccumulated >= rageThreshold)
        {
            GetCaster()->CastSpell(GetCaster(), 200414, TRIGGERED_FULL_MASK);
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
        }
        else
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_reckless_abandon::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_reckless_abandon_proc : public AuraScript
{
    PrepareAuraScript(spell_reckless_abandon_proc);

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster()->HasAura(200414))
            return;

        int32 stackAmount = GetCaster()->GetAura(200414)->GetStackAmount();

        GetCaster()->RemoveAura(200414);
        GetCaster()->CastSpell(GetCaster(), 200415, TRIGGERED_FULL_MASK);
        GetCaster()->GetAura(200415)->SetStackAmount(stackAmount);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_reckless_abandon_proc::HandleProc, EFFECT_0, SPELL_AURA_MOD_RAGE_FROM_DAMAGE_DEALT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_true_rage : public AuraScript
{
    PrepareAuraScript(spell_true_rage);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (GetCaster()->GetShapeshiftForm() != FORM_BERSERKERSTANCE)
            return;

        if (GetCaster()->HasAura(200004))
        {
            float remainingDuration = GetCaster()->GetAura(200004)->GetDuration();
            GetCaster()->GetAura(200004)->SetDuration(remainingDuration + 4000);
        }
        else
        {
            GetCaster()->CastSpell(GetCaster(), 200004, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_true_rage::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_relentless : public AuraScript
{
    PrepareAuraScript(spell_relentless);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200428))
            return GetCaster()->GetAura(200428);

        if (GetCaster()->HasAura(200429))
            return GetCaster()->GetAura(200429);

        if (GetCaster()->HasAura(200430))
            return GetCaster()->GetAura(200430);

        if (GetCaster()->HasAura(200431))
            return GetCaster()->GetAura(200431);

        if (GetCaster()->HasAura(200432))
            return GetCaster()->GetAura(200432);

        if (GetCaster()->HasAura(200433))
            return GetCaster()->GetAura(200433);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        int32 stackThreshold = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();

        GetCaster()->CastSpell(GetCaster(), 200434, TRIGGERED_FULL_MASK);

        if (GetCaster()->GetAura(200434)->GetStackAmount() < stackThreshold)
            return;

        GetCaster()->RemoveAura(200434);
        GetCaster()->CastSpell(GetCaster(), 200435, TRIGGERED_FULL_MASK);
        GetCaster()->RemoveAura(25771);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_relentless::HandleProc, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};

class spell_raging_death : public AuraScript
{
    PrepareAuraScript(spell_raging_death);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 spellRage=  eventInfo.GetSpellInfo()->CalcPowerCost(GetCaster(), SpellSchoolMask(eventInfo.GetSpellInfo()->SchoolMask));

        if (spellRage <= 0)
            return;

        int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
        int32 rageThreshold = aurEff->GetAmount();
        if (rageAccumulated >= rageThreshold)
        {
            GetCaster()->ToPlayer()->ModifySpellCooldown(12292, -aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount());
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
        }
        else
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_raging_death::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_son_of_thunder : public AuraScript
{
    PrepareAuraScript(spell_son_of_thunder);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (GetCaster()->GetShapeshiftForm() != FORM_DEFENSIVESTANCE)
            return;

        GetCaster()->CastSpell(GetCaster(), SPELL_WARR_THUNDERCLAP_SON_OF_THUNDER, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_son_of_thunder::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_best_served_cold : public AuraScript
{
    PrepareAuraScript(spell_best_served_cold);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        Unit* target = GetTarget();

        if (!target || GetCaster()->isDead())
            return;

        uint32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));

        Aura* shield = caster->GetAura(200496);

        if (shield)
            amount += shield->GetEffect(EFFECT_0)->GetAmount();

        caster->CastCustomSpell(200496, SPELLVALUE_BASE_POINT0, amount, caster, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_best_served_cold::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_side_step : public AuraScript
{
    PrepareAuraScript(spell_side_step);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200510))
            return GetCaster()->GetAura(200510);

        if (GetCaster()->HasAura(200511))
            return GetCaster()->GetAura(200511);

        if (GetCaster()->HasAura(200512))
            return GetCaster()->GetAura(200512);

        if (GetCaster()->HasAura(200513))
            return GetCaster()->GetAura(200513);

        if (GetCaster()->HasAura(200514))
            return GetCaster()->GetAura(200514);

        if (GetCaster()->HasAura(200515))
            return GetCaster()->GetAura(200515);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->CastSpell(GetCaster(), GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_side_step::HandleProc, EFFECT_0, SPELL_AURA_MOD_BLOCK_PERCENT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_shield_parry : public AuraScript
{
    PrepareAuraScript(spell_shield_parry);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200522))
            return GetCaster()->GetAura(200522);

        if (GetCaster()->HasAura(200523))
            return GetCaster()->GetAura(200523);

        if (GetCaster()->HasAura(200524))
            return GetCaster()->GetAura(200524);

        if (GetCaster()->HasAura(200525))
            return GetCaster()->GetAura(200525);

        if (GetCaster()->HasAura(200526))
            return GetCaster()->GetAura(200526);

        if (GetCaster()->HasAura(200527))
            return GetCaster()->GetAura(200527);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->CastSpell(GetCaster(), GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_shield_parry::HandleProc, EFFECT_0, SPELL_AURA_MOD_BLOCK_PERCENT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_magic_barrier : public AuraScript
{
    PrepareAuraScript(spell_magic_barrier);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200534))
            return GetCaster()->GetAura(200534);

        if (GetCaster()->HasAura(200535))
            return GetCaster()->GetAura(200535);

        if (GetCaster()->HasAura(200536))
            return GetCaster()->GetAura(200536);

        if (GetCaster()->HasAura(200537))
            return GetCaster()->GetAura(200537);

        if (GetCaster()->HasAura(200538))
            return GetCaster()->GetAura(200538);

        if (GetCaster()->HasAura(200539))
            return GetCaster()->GetAura(200539);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->CastSpell(GetCaster(), GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        uint32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        if (GetCaster()->HasAura(buffAura))
            GetCaster()->RemoveAura(buffAura);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_magic_barrier::HandleProc, EFFECT_0, SPELL_AURA_MOD_BLOCK_PERCENT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_magic_barrier::HandleRemove, EFFECT_0, SPELL_AURA_MOD_BLOCK_PERCENT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_enduring_defenses_shield_block : public AuraScript
{
    PrepareAuraScript(spell_enduring_defenses_shield_block);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200546))
            return GetCaster()->GetAura(200546);

        if (GetCaster()->HasAura(200547))
            return GetCaster()->GetAura(200547);

        if (GetCaster()->HasAura(200548))
            return GetCaster()->GetAura(200548);

        if (GetCaster()->HasAura(200549))
            return GetCaster()->GetAura(200549);

        if (GetCaster()->HasAura(200550))
            return GetCaster()->GetAura(200550);

        if (GetCaster()->HasAura(200551))
            return GetCaster()->GetAura(200551);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->CastSpell(GetCaster(), GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        uint32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;

        if (GetCaster()->HasAura(buffAura))
            GetCaster()->RemoveAura(buffAura);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_enduring_defenses_shield_block::HandleProc, EFFECT_0, SPELL_AURA_MOD_BLOCK_PERCENT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_enduring_defenses_shield_block::HandleRemove, EFFECT_0, SPELL_AURA_MOD_BLOCK_PERCENT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_enduring_defenses_shield_slam : public AuraScript
{
    PrepareAuraScript(spell_enduring_defenses_shield_slam);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Aura* shieldBlock = GetCaster()->GetAura(2565);

        if (!shieldBlock)
            return;

        shieldBlock->SetDuration(shieldBlock->GetDuration() + aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_enduring_defenses_shield_slam::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_barrier_lord : public AuraScript
{
    PrepareAuraScript(spell_barrier_lord);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->ModifySpellCooldown(2565, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_barrier_lord::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_sword_and_board : public AuraScript
{
    PrepareAuraScript(spell_sword_and_board);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* caster = GetTarget()->ToPlayer())
            caster->RemoveSpellCooldown(47488, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_sword_and_board::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_violent_outburst : public AuraScript
{
    PrepareAuraScript(spell_violent_outburst);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200600))
            return GetCaster()->GetAura(200600);

        if (GetCaster()->HasAura(200601))
            return GetCaster()->GetAura(200601);

        if (GetCaster()->HasAura(200602))
            return GetCaster()->GetAura(200602);

        if (GetCaster()->HasAura(200603))
            return GetCaster()->GetAura(200603);

        if (GetCaster()->HasAura(200604))
            return GetCaster()->GetAura(200604);

        if (GetCaster()->HasAura(200605))
            return GetCaster()->GetAura(200605);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster())
            return;

        if (!GetRuneAura())
            return;

        if (GetCaster()->GetAura(200612)->GetStackAmount() < GetRuneAura()->GetEffect(EFFECT_0)->GetAmount())
            return;

        uint32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;

        GetCaster()->RemoveAura(200612);
        GetCaster()->CastSpell(GetCaster(), buffAura, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_violent_outburst::HandleProc, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
    }
};

class spell_defenders_aegis : public AuraScript
{
    PrepareAuraScript(spell_defenders_aegis);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 spellRage=  eventInfo.GetSpellInfo()->CalcPowerCost(GetCaster(), SpellSchoolMask(eventInfo.GetSpellInfo()->SchoolMask));

        if (spellRage <= 0)
            return;

        if (!GetCaster()->HasAura(871))
            return;

        int32 newDuration = GetCaster()->GetAura(871)->GetDuration() + aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount();
        int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
        int32 rageThreshold = aurEff->GetAmount();

        if (rageAccumulated >= rageThreshold)
        {
            GetCaster()->GetAura(871)->SetDuration(newDuration);
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
        }
        else
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_defenders_aegis::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_offensive_wall : public AuraScript
{
    PrepareAuraScript(spell_offensive_wall);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* caster = GetTarget()->ToPlayer())
            caster->ModifySpellCooldown(871, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_offensive_wall::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_recuperation : public AuraScript
{
    PrepareAuraScript(spell_recuperation);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200631))
            return GetCaster()->GetAura(200631);

        if (GetCaster()->HasAura(200632))
            return GetCaster()->GetAura(200632);

        if (GetCaster()->HasAura(200633))
            return GetCaster()->GetAura(200633);

        if (GetCaster()->HasAura(200634))
            return GetCaster()->GetAura(200634);

        if (GetCaster()->HasAura(200635))
            return GetCaster()->GetAura(200635);

        if (GetCaster()->HasAura(200636))
            return GetCaster()->GetAura(200636);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        uint32 amount = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();
        GetCaster()->CastCustomSpell(200637, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_recuperation::HandleProc, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_rageful_stand : public AuraScript
{
    PrepareAuraScript(spell_rageful_stand);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 spellRage=  eventInfo.GetSpellInfo()->CalcPowerCost(GetCaster(), SpellSchoolMask(eventInfo.GetSpellInfo()->SchoolMask));

        if (spellRage <= 0)
            return;

        int32 rageAccumulated = aurEff->GetBase()->GetEffect(EFFECT_1)->GetAmount() + spellRage;
        int32 rageThreshold = aurEff->GetAmount();
        if (rageAccumulated >= rageThreshold)
        {
            GetCaster()->ToPlayer()->ModifySpellCooldown(12975, -aurEff->GetBase()->GetEffect(EFFECT_2)->GetAmount());
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated - rageThreshold);
        }
        else
            aurEff->GetBase()->GetEffect(EFFECT_1)->SetAmount(rageAccumulated);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_rageful_stand::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_unnerving_focus : public AuraScript
{
    PrepareAuraScript(spell_unnerving_focus);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200644))
            return GetCaster()->GetAura(200644);

        if (GetCaster()->HasAura(200645))
            return GetCaster()->GetAura(200645);

        if (GetCaster()->HasAura(200646))
            return GetCaster()->GetAura(200646);

        if (GetCaster()->HasAura(200647))
            return GetCaster()->GetAura(200647);

        if (GetCaster()->HasAura(200648))
            return GetCaster()->GetAura(200648);

        if (GetCaster()->HasAura(200649))
            return GetCaster()->GetAura(200649);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->CastSpell(GetCaster(), GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        uint32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        if (GetCaster()->HasAura(buffAura))
            GetCaster()->RemoveAura(buffAura);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_unnerving_focus::HandleProc, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_unnerving_focus::HandleRemove, EFFECT_0, SPELL_AURA_MOD_INCREASE_HEALTH, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_preparation : public AuraScript
{
    PrepareAuraScript(spell_preparation);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* caster = GetTarget()->ToPlayer())
            caster->ModifySpellCooldown(12809, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_preparation::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_inspiring_wall : public AuraScript
{
    PrepareAuraScript(spell_inspiring_wall);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200674))
            return GetCaster()->GetAura(200674);

        if (GetCaster()->HasAura(200675))
            return GetCaster()->GetAura(200675);

        if (GetCaster()->HasAura(200676))
            return GetCaster()->GetAura(200676);

        if (GetCaster()->HasAura(200677))
            return GetCaster()->GetAura(200677);

        if (GetCaster()->HasAura(200678))
            return GetCaster()->GetAura(200678);

        if (GetCaster()->HasAura(200679))
            return GetCaster()->GetAura(200679);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        GetCaster()->CastSpell(GetCaster(), GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell, TRIGGERED_FULL_MASK);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        uint32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        if (GetCaster()->HasAura(buffAura))
            GetCaster()->RemoveAura(buffAura);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_inspiring_wall::HandleProc, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_inspiring_wall::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
    }
};

class spell_inspiring_wall_heal : public AuraScript
{
    PrepareAuraScript(spell_inspiring_wall_heal);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetDamage() > 0 && aurEff) {
            if (eventInfo.GetHitMask() != PROC_EX_CRITICAL_HIT)
                return;

            int32 amount = int32(CalculatePct(eventInfo.GetDamageInfo()->GetDamage(), aurEff->GetAmount()));

            if (amount <= 0)
                return;

            GetCaster()->CastCustomSpell(200686, SPELLVALUE_BASE_POINT0, amount, eventInfo.GetActor(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_inspiring_wall_heal::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_defensive_quickness : public AuraScript
{
    PrepareAuraScript(spell_defensive_quickness);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->GetShapeshiftForm() == FORM_DEFENSIVESTANCE;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_defensive_quickness::CheckProc);
    }
};

class spell_undying_will : public AuraScript
{
    PrepareAuraScript(spell_undying_will);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->GetShapeshiftForm() == FORM_DEFENSIVESTANCE;
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(spell_undying_will::CheckProc);
    }
};

class spell_rumbling_earth : public SpellScript
{
    PrepareSpellScript(spell_rumbling_earth);

    uint32 auraShockwaveStack = 80009;

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200714))
            return GetCaster()->GetAura(200714);

        if (GetCaster()->HasAura(200715))
            return GetCaster()->GetAura(200715);

        if (GetCaster()->HasAura(200716))
            return GetCaster()->GetAura(200716);

        if (GetCaster()->HasAura(200717))
            return GetCaster()->GetAura(200717);

        if (GetCaster()->HasAura(200718))
            return GetCaster()->GetAura(200718);

        if (GetCaster()->HasAura(200719))
            return GetCaster()->GetAura(200719);

        return nullptr;
    }

    void HandleOnHit()
    {
        if (!GetRuneAura())
            return;

        if (GetHitUnit() && GetHitUnit()->GetTypeId() == TYPEID_UNIT && GetCaster()) {
            if (Aura* aura = GetCaster()->GetAura(auraShockwaveStack))
            {
                if (aura->GetStackAmount() >= 2)
                {
                    int32 cooldown = aura->GetDuration();
                    GetCaster()->ToPlayer()->ModifySpellCooldown(46968, -GetRuneAura()->GetEffect(EFFECT_2)->GetAmount());
                    GetCaster()->RemoveAura(auraShockwaveStack);
                    return;
                }
                aura->ModStackAmount(1);
            }
            else {
                GetCaster()->AddAura(auraShockwaveStack, GetCaster());
            }
        }
    }

    void HandleOnCast()
    {
        if (Aura* aura = GetCaster()->GetAura(auraShockwaveStack))
            GetCaster()->RemoveAura(auraShockwaveStack);
    }

    void Register() override
    {
        OnHit += SpellHitFn(spell_rumbling_earth::HandleOnHit);
        OnCast += SpellCastFn(spell_rumbling_earth::HandleOnCast);
    }
};

class spell_blade_turning : public AuraScript
{
    PrepareAuraScript(spell_blade_turning);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetBlock()) {
            float blockValue = eventInfo.GetDamageInfo()->GetBlock();

            if (blockValue <= 0)
                return;

            uint32 amount = int32(CalculatePct(blockValue, aurEff->GetAmount()));

            if (GetCaster()->HasAura(200726))
                amount += GetCaster()->GetAura(200726)->GetEffect(EFFECT_0)->GetAmount();

            GetCaster()->CastCustomSpell(200726, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_blade_turning::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_block_spike : public AuraScript
{
    PrepareAuraScript(spell_block_spike);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetBlock()) {

            float blockValue = eventInfo.GetDamageInfo()->GetBlock();

            if (blockValue <= 0)
                return;

            uint32 amount = int32(CalculatePct(blockValue, aurEff->GetAmount()));

            GetCaster()->CastCustomSpell(200733, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
        }

    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_block_spike::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class spell_healing_block : public AuraScript
{
    PrepareAuraScript(spell_healing_block);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (eventInfo.GetDamageInfo() && eventInfo.GetDamageInfo()->GetBlock()) {

            float blockValue = eventInfo.GetDamageInfo()->GetBlock();

            if (blockValue <= 0)
                return;

            uint32 amount = int32(CalculatePct(blockValue, aurEff->GetAmount()));

            GetCaster()->CastCustomSpell(200740, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(spell_healing_block::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};
// Crash ..
class spell_hurricane : public AuraScript
{
    PrepareAuraScript(spell_hurricane);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200796))
            return GetCaster()->GetAura(200796);

        if (GetCaster()->HasAura(200797))
            return GetCaster()->GetAura(200797);

        if (GetCaster()->HasAura(200798))
            return GetCaster()->GetAura(200798);

        if (GetCaster()->HasAura(200799))
            return GetCaster()->GetAura(200799);

        if (GetCaster()->HasAura(200800))
            return GetCaster()->GetAura(200800);

        if (GetCaster()->HasAura(200801))
            return GetCaster()->GetAura(200801);

        return nullptr;
    }

    int GetProcAura()
    {
        return GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster())
            return;

        if (!GetRuneAura())
            return;

        GetCaster()->AddAura(GetProcAura(), GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetCaster())
            return;

        if (!GetRuneAura())
            return;

        if (!GetCaster()->HasAura(GetProcAura()))
            return;

        GetCaster()->RemoveAura(GetProcAura());
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(spell_hurricane::HandleProc, EFFECT_1, SPELL_AURA_MECHANIC_IMMUNITY_MASK, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(spell_hurricane::HandleRemove, EFFECT_1, SPELL_AURA_MECHANIC_IMMUNITY_MASK, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_improved_execute : public SpellScript
{
    PrepareSpellScript(rune_improved_execute);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200838))
            return GetCaster()->GetAura(200838);

        if (GetCaster()->HasAura(200839))
            return GetCaster()->GetAura(200839);

        if (GetCaster()->HasAura(200840))
            return GetCaster()->GetAura(200840);

        if (GetCaster()->HasAura(200841))
            return GetCaster()->GetAura(200841);

        if (GetCaster()->HasAura(200842))
            return GetCaster()->GetAura(200842);

        if (GetCaster()->HasAura(200843))
            return GetCaster()->GetAura(200843);

        return nullptr;
    }

    void OnAfterCast()
    {
        Unit* caster = GetCaster();
        uint32 currentRage = caster->GetPower(POWER_RAGE);
        uint32 leftRage = rageBeforeCast - currentRage;

        if (Unit* target = GetExplTargetUnit()) {
            if (target->IsAlive())
            {
                if (Aura* aura = GetRuneAura()) {
                    int32 pct = aura->GetEffect(EFFECT_0)->GetAmount();
                    uint32 newRage = CalculatePct(leftRage, pct);
                    caster->ModifyPower(POWER_RAGE, newRage);
                }
            }
        }
    }

    void OnBeforeCast()
    {
        Unit* caster = GetCaster();
        rageBeforeCast = caster->GetPower(POWER_RAGE);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(rune_improved_execute::OnAfterCast);
        BeforeCast += SpellCastFn(rune_improved_execute::OnBeforeCast);
    }

private:
    uint32 rageBeforeCast;

};

class rune_thunder_weapon : public AuraScript
{
    PrepareAuraScript(rune_thunder_weapon);

   

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Unit* target = eventInfo.GetActionTarget())
        {
           target->CastSpell(target, SPELL_WARR_THUNDERCLAP_SON_OF_THUNDER, true, nullptr, nullptr, GetCaster()->GetGUID());
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_thunder_weapon::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_improved_heroic_throw : public SpellScript
{
    PrepareSpellScript(rune_improved_heroic_throw);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200850))
            return GetCaster()->GetAura(200850);

        if (GetCaster()->HasAura(200851))
            return GetCaster()->GetAura(200851);

        if (GetCaster()->HasAura(200852))
            return GetCaster()->GetAura(200852);

        if (GetCaster()->HasAura(200853))
            return GetCaster()->GetAura(200853);

        if (GetCaster()->HasAura(200854))
            return GetCaster()->GetAura(200854);

        if (GetCaster()->HasAura(200855))
            return GetCaster()->GetAura(200855);

        return nullptr;
    }

    void OnHitTarget(SpellEffIndex effIndex)
    {
        if (Unit* target = GetHitUnit())
        {
            if (Aura* improvedHeroicThrow = GetRuneAura()) {
                Player* player = GetCaster()->ToPlayer();
                float amount = 8.0f + player->GetMastery();
                player->CastCustomSpell(200002, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(rune_improved_heroic_throw::OnHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class rune_brutal_vitality : public AuraScript
{
    PrepareAuraScript(rune_brutal_vitality);

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
        float pct = aurEff->GetAmount();
        int32 calculatedAmount = CalculatePct(static_cast<int32>(eventInfo.GetDamageInfo()->GetDamage()), pct);

        if (Aura* aura = GetCaster()->GetAura(SPELL_WARR_IGNORE_PAIN))
        {
            int32 amount = aura->GetEffect(EFFECT_0)->GetAmount();
            int32 newAmount = amount += calculatedAmount;
            LOG_ERROR("", "current Amount {}", amount);
            aura->GetEffect(EFFECT_0)->SetAmount(newAmount);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_brutal_vitality::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_brutal_vitality::OnProc, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
    }
};

class rune_brutal_devastator : public AuraScript
{
    PrepareAuraScript(rune_brutal_devastator);


    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        PreventDefaultAction();
        Unit* caster = GetCaster();
        float totalAttackPower = caster->GetTotalAttackPowerValue(BASE_ATTACK);

        int32 amountPourcentage = aurEff->GetAmount();
        int32 attackPower = int32(CalculatePct(totalAttackPower, amountPourcentage));

        Unit* target = eventInfo.GetActionTarget();

        caster->CastCustomSpell(SPELL_WARR_DEVASTATOR, SPELLVALUE_BASE_POINT0, attackPower, target, TRIGGERED_FULL_MASK);

        if (roll_chance_f(20.f))
            caster->ToPlayer()->RemoveSpellCooldown(SPELL_WARR_SHIELD_SLAM, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_brutal_devastator::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_battle_scarred_veteran : public AuraScript
{
    PrepareAuraScript(rune_battle_scarred_veteran);

public:
    rune_battle_scarred_veteran() { }
private:

    void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
    {
        amount = -1;
    }

    void Absorb(AuraEffect* aurEff, DamageInfo& dmgInfo, uint32& absorbAmount)
    {
        Unit* victim = GetTarget();
        int32 remainingHealth = victim->GetHealth() - dmgInfo.GetDamage();
        uint32 allowedHealth = victim->CountPctFromMaxHealth(30.f);

        uint32 battleScaredProcTriggerSpell = aurEff->GetBase()->GetEffect(EFFECT_0)->GetAmount();

        if ((remainingHealth < int32(allowedHealth)) && !victim->ToPlayer()->HasSpellCooldown(battleScaredProcTriggerSpell))
        {
            victim->CastSpell(victim, battleScaredProcTriggerSpell);
            victim->ToPlayer()->AddSpellCooldown(battleScaredProcTriggerSpell, 0, 180000);
        }
    }

    void Register() override
    {
        DoEffectCalcAmount += AuraEffectCalcAmountFn(rune_battle_scarred_veteran::CalculateAmount, EFFECT_2, SPELL_AURA_SCHOOL_ABSORB);
        OnEffectAbsorb += AuraEffectAbsorbFn(rune_battle_scarred_veteran::Absorb, EFFECT_2);
    }
};

class rune_test_of_might : public AuraScript
{
    PrepareAuraScript(rune_test_of_might);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        int32 rage = eventInfo.GetSpellInfo()->ManaCost / 10;

        if (rage <= 0)
            return;

        int32 rageAccumulated = aurEff->GetAmount() + rage;

        aurEff->GetBase()->GetEffect(EFFECT_0)->SetAmount(rageAccumulated);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_test_of_might::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


class rune_test_of_might_effect_remove : public AuraScript
{
    PrepareAuraScript(rune_test_of_might_effect_remove);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(200881))
            return GetCaster()->GetAura(200881);

        if (GetCaster()->HasAura(200882))
            return GetCaster()->GetAura(200882);

        if (GetCaster()->HasAura(200883))
            return GetCaster()->GetAura(200883);

        if (GetCaster()->HasAura(200884))
            return GetCaster()->GetAura(200884);

        if (GetCaster()->HasAura(200885))
            return GetCaster()->GetAura(200885);

        if (GetCaster()->HasAura(200886))
            return GetCaster()->GetAura(200886);

        return nullptr;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        Unit* caster = GetCaster();
        int32 rageAccumulated = aurEff->GetAmount();
        if (Aura* rune = GetRuneAura()) {
            int32 threadshold = rune->GetEffect(EFFECT_0)->GetAmount();
            int32 calculatedStack = static_cast<int32>(rageAccumulated / threadshold);
            caster->CastCustomSpell(SPELL_WARR_TEST_OF_MIGHT, SPELLVALUE_AURA_STACK, calculatedStack, caster, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_test_of_might_effect_remove::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_in_for_the_kill : public SpellScript
{
    PrepareSpellScript(rune_in_for_the_kill);

    void HandleHit(SpellEffIndex effIndex)
    {
        if (Unit* target = GetHitUnit())
        {
            if (target->GetHealthPct() < 35.f) {
                
            }
            else {
               
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(rune_in_for_the_kill::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class rune_strength_of_arms : public SpellScript
{
    PrepareSpellScript(rune_strength_of_arms);

    void HandleHit(SpellEffIndex effIndex)
    {
        // Hit with Overpower check if we have the rune first
        if (Unit* target = GetHitUnit())
        {
            if (target->GetHealthPct() < 35.f) {
                // Give X rage depending on the rune rank.
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(rune_strength_of_arms::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class rune_bonegrinder : public AuraScript
{
    PrepareAuraScript(rune_bonegrinder);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        // Get rune and spell trigger depending on the rune rank and cast it on the player.
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_bonegrinder::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};


class rune_bloodletting : public SpellScript
{
    PrepareSpellScript(rune_bloodletting);

    void HandleHit(SpellEffIndex effIndex)
    {
        // We hit with Mortal strike, we check if we have the rune first.
        if (Unit* target = GetHitUnit())
        {
            if (target->GetHealthPct() < 35.f) {
                GetCaster()->CastSpell(target, SPELL_WARR_REND);
            }
        }
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(rune_bloodletting::HandleHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};



class rune_cold_steel_hot_blood : public AuraScript
{
    PrepareAuraScript(rune_cold_steel_hot_blood);

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
        Unit* target = eventInfo.GetActionTarget();

    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_cold_steel_hot_blood::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_cold_steel_hot_blood::OnProc, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
    }
};

class rune_invigorating_fury : public SpellScript
{
    PrepareSpellScript(rune_invigorating_fury);

    void HandleCast()
    {
        // On Cast Enraged Regeneration Check if the player has the rune, yes, get the spell trigger depending on the rank.
    }

    void Register() override
    {
        OnCast += SpellCastFn(rune_invigorating_fury::HandleCast);
    }
};


class rune_hack_and_slash : public AuraScript
{
    PrepareAuraScript(rune_hack_and_slash);

    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(SPELL_WARR_RAGING_BLOW, true);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hack_and_slash::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


class rune_merciless : public AuraScript
{
    PrepareAuraScript(rune_merciless);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        // Get rune and spell trigger depending on the rune rank and cast it on the player. 
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_merciless::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};


class rune_seismic_reverberation : public SpellScript
{
    PrepareSpellScript(rune_seismic_reverberation);

    void HandleOnHit()
    {
        if (GetHitUnit() && GetHitUnit()->GetTypeId() == TYPEID_UNIT && GetCaster()) {
            if (Aura* aura = GetCaster()->GetAura(0))
            {
                if (aura->GetStackAmount() >= 2)
                {
                    // Add aura
                }
                aura->ModStackAmount(1);
            }
            else {
                GetCaster()->AddAura(0, GetCaster());
            }
        }
    }


    void HandleOnCast()
    {
        if (Aura* aura = GetCaster()->GetAura(0)) {

        }
    }



    void Register() override
    {
        OnHit += SpellHitFn(rune_seismic_reverberation::HandleOnHit);
        OnCast += SpellCastFn(rune_seismic_reverberation::HandleOnCast);
    }
};


class rune_heavy_blocks : public AuraScript
{
    PrepareAuraScript(rune_heavy_blocks);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();
        if (Aura* aura = caster->GetAura(SPELL_WARR_SHIELD_BLOCK)) {

        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_heavy_blocks::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_impenetrable_shield : public AuraScript
{
    PrepareAuraScript(rune_impenetrable_shield);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster();

        // Reduce the remaning cooldown of shield wall depending on the rune.
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_impenetrable_shield::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


class rune_bolster : public SpellScript
{
    PrepareSpellScript(rune_bolster);

    void HandleOnCast()
    {
        // On Cast LastStand give shield block depending on the rune.
    }


    void Register() override
    {
        OnCast += SpellCastFn(rune_bolster::HandleOnCast);
    }
};


class rune_fierce_striking : public AuraScript
{
    PrepareAuraScript(rune_fierce_striking);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* caster = GetCaster()->ToPlayer();
        // Your crushing strike hit reduce the remaning cooldown of Colossal thrust by X sec depending on the rune.
        caster->ModifySpellCooldown(SPELL_WARR_COLOSSAL_THRUST, 0);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_fierce_striking::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_flurry_of_strikes : public AuraScript
{
    PrepareAuraScript(rune_flurry_of_strikes);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();
        GetCaster()->CastSpell(target, SPELL_WARR_SECOND_CRUSHING_STRIKE);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_flurry_of_strikes::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_heavy_impact : public AuraScript
{
    PrepareAuraScript(rune_heavy_impact);

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
        float pct = 0.0f; // Get the amount through the rune effect 1
        int32 calculatedAmount = CalculatePct(static_cast<int32>(eventInfo.GetDamageInfo()->GetDamage()), pct);
        // cast custom spell 
       
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_heavy_impact::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_heavy_impact::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


class rune_raging_fury : public AuraScript
{
    PrepareAuraScript(rune_raging_fury);


    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        Aura* aura = GetAura();
        Player* caster = GetCaster()->ToPlayer();


        int32 spellRage=  eventInfo.GetSpellInfo()->CalcPowerCost(GetCaster(), SpellSchoolMask(eventInfo.GetSpellInfo()->SchoolMask));
        int32 rageAccumulated = GetAura()->GetEffect(EFFECT_1)->GetAmount() + spellRage;

        if (spellRage <= 0)
            return;

        // Every X rages reduce the remaning cooldown by X sec of  SPELL_WARR_HULN_FURY

        caster->ModifySpellCooldown(SPELL_WARR_HULN_FURY, 0);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_raging_fury::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_spartan_aegis : public AuraScript
{
    PrepareAuraScript(rune_spartan_aegis);


    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        float pct = 0; // Get the amount through the rune effect_1
        uint32 absorbAmount = CalculatePct(GetCaster()->GetHealth(), pct);

        // Cast custom spell that give absorb shit

    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_spartan_aegis::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_furious_onslaught: public AuraScript
{
    PrepareAuraScript(rune_furious_onslaught);

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        GetCaster()->ToPlayer()->RemoveSpellCooldown(SPELL_WARR_COLOSSAL_THRUST);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_furious_onslaught::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_impenetrable_barrier : public AuraScript
{
    PrepareAuraScript(rune_impenetrable_barrier);


    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        float pct = 0; // Get the amount through the rune effect_1
        uint32 absorbAmount = CalculatePct(GetCaster()->GetHealth(), pct);

        // Cast custom spell that give absorb shit

    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_impenetrable_barrier::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};


class rune_masterful_swipe : public SpellScript
{
    PrepareSpellScript(rune_masterful_swipe);

    void HandleOnHit()
    {
        if (GetHitUnit() && GetHitUnit()->GetTypeId() == TYPEID_UNIT && GetCaster()) {
            if (Aura* aura = GetCaster()->GetAura(0))
            {
                if (aura->GetStackAmount() >= 2)
                {
                    // Add aura
                }
                aura->ModStackAmount(1);
            }
            else {
                GetCaster()->AddAura(0, GetCaster());
            }
        }
    }


    void HandleOnCast()
    {
        if (Aura* aura = GetCaster()->GetAura(0)) {

        }
    }



    void Register() override
    {
        OnHit += SpellHitFn(rune_masterful_swipe::HandleOnHit);
        OnCast += SpellCastFn(rune_masterful_swipe::HandleOnCast);
    }
};


class rune_the_depth_of_rage : public AuraScript
{
    PrepareAuraScript(rune_the_depth_of_rage);

    void HandleProc(AuraEffect const*  /*aurEff*/, ProcEventInfo& eventInfo)
    {
        int32 spellRage=  eventInfo.GetSpellInfo()->CalcPowerCost(GetCaster(), SpellSchoolMask(eventInfo.GetSpellInfo()->SchoolMask));
        int32 rageAccumulated = GetAura()->GetEffect(EFFECT_1)->GetAmount() + spellRage;

        if (spellRage <= 0)
            return;

    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_the_depth_of_rage::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_gladiator_tourment : public AuraScript
{
    PrepareAuraScript(rune_gladiator_tourment);


    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(SPELL_WARR_BATTLE_TRANCE);
    }


    void OnProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = eventInfo.GetActionTarget();
        float pct = 0.f; // get amount through the rune effect_1
        uint32 amount = CalculatePct(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), pct);

        // Cast Custom spell
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_gladiator_tourment::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_gladiator_tourment::OnProc, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
    }
};


class spell_calculated_abandon : public AuraScript
{
    PrepareAuraScript(spell_calculated_abandon);

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
       
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(spell_calculated_abandon::HandleRemove, EFFECT_0, SPELL_AURA_MOD_RAGE_FROM_DAMAGE_DEALT, AURA_EFFECT_HANDLE_REAL);
    }
};



void AddSC_warrior_perks_scripts()
{

    RegisterSpellScript(rune_improved_execute);
    RegisterSpellScript(rune_thunder_weapon);
    RegisterSpellScript(rune_improved_heroic_throw);
    RegisterSpellScript(rune_brutal_vitality);
    RegisterSpellScript(rune_brutal_devastator);
    RegisterSpellScript(rune_battle_scarred_veteran);
    RegisterSpellScript(rune_test_of_might);
    RegisterSpellScript(rune_test_of_might_effect_remove);
    RegisterSpellScript(rune_strength_of_arms);
    RegisterSpellScript(rune_in_for_the_kill);
    RegisterSpellScript(rune_bonegrinder);
    RegisterSpellScript(rune_bloodletting);
    RegisterSpellScript(rune_invigorating_fury);
    RegisterSpellScript(rune_merciless);
    RegisterSpellScript(rune_hack_and_slash);
    RegisterSpellScript(rune_seismic_reverberation);
    RegisterSpellScript(rune_heavy_blocks);
    RegisterSpellScript(rune_impenetrable_shield);
    RegisterSpellScript(rune_bolster);
    RegisterSpellScript(rune_fierce_striking);
    RegisterSpellScript(rune_flurry_of_strikes);
    RegisterSpellScript(rune_heavy_impact);
    RegisterSpellScript(rune_raging_fury);
    RegisterSpellScript(rune_spartan_aegis);
    RegisterSpellScript(rune_furious_onslaught);
    RegisterSpellScript(rune_impenetrable_barrier);
    RegisterSpellScript(rune_masterful_swipe);
    RegisterSpellScript(rune_the_depth_of_rage);
    RegisterSpellScript(rune_gladiator_tourment);
    RegisterSpellScript(spell_calculated_abandon);

    RegisterSpellScript(spell_cut_the_veins);
    RegisterSpellScript(spell_the_art_of_war);
    RegisterSpellScript(spell_tide_of_blood);
    RegisterSpellScript(spell_vein_cutter);
    RegisterSpellScript(spell_battlelord);
    RegisterSpellScript(spell_tactician);
    RegisterSpellScript(spell_dreadnaught);
    RegisterSpellScript(spell_blood_and_thunder);
    RegisterSpellScript(spell_anger_management);
    RegisterSpellScript(spell_blood_dance);
    RegisterSpellScript(spell_keep_spinnin);
    RegisterSpellScript(spell_sweeping_rage);
    RegisterSpellScript(spell_sweeping_rage_proc);
    RegisterSpellScript(spell_collateral_damage);
    RegisterSpellScript(spell_collateral_damage_proc);
    RegisterSpellScript(spell_vicious_comtempt);
    RegisterSpellScript(spell_charging_for_blood);
    RegisterSpellScript(spell_bloodcraze);
    RegisterSpellScript(spell_fervor_of_battle);
    RegisterSpellScript(spell_storm_of_swords);
    RegisterSpellScript(spell_spinning_grip);
    RegisterSpellScript(spell_spinning_grip_jump);
    RegisterSpellScript(spell_depths_of_insanity);
    RegisterSpellScript(spell_berserkers_tourment);
    RegisterSpellScript(spell_berserkers_tourment_proc);
    RegisterSpellScript(spell_reckless_abandon);
    RegisterSpellScript(spell_reckless_abandon_proc);
    RegisterSpellScript(spell_true_rage);
    RegisterSpellScript(spell_relentless);
    RegisterSpellScript(spell_raging_death);
    RegisterSpellScript(spell_son_of_thunder);
    RegisterSpellScript(spell_best_served_cold);
    RegisterSpellScript(spell_side_step);
    RegisterSpellScript(spell_shield_parry);
    RegisterSpellScript(spell_magic_barrier);
    RegisterSpellScript(spell_enduring_defenses_shield_block);
    RegisterSpellScript(spell_enduring_defenses_shield_slam);
    RegisterSpellScript(spell_barrier_lord);
    RegisterSpellScript(spell_sword_and_board);
    RegisterSpellScript(spell_violent_outburst);
    RegisterSpellScript(spell_defenders_aegis);
    RegisterSpellScript(spell_offensive_wall);
    RegisterSpellScript(spell_recuperation);
    RegisterSpellScript(spell_rageful_stand);
    RegisterSpellScript(spell_unnerving_focus);
    RegisterSpellScript(spell_preparation);
    RegisterSpellScript(spell_inspiring_wall);
    RegisterSpellScript(spell_inspiring_wall_heal);
    RegisterSpellScript(spell_defensive_quickness);
    RegisterSpellScript(spell_undying_will);
    RegisterSpellScript(spell_rumbling_earth);
    RegisterSpellScript(spell_blade_turning);
    RegisterSpellScript(spell_block_spike);
    RegisterSpellScript(spell_healing_block);
    RegisterSpellScript(spell_hurricane);
}