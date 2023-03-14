#include "PetDefines.h"
#include "Player.h"
#include "Pet.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Totem.h"
#include "UnitAI.h"
#include "Log.h"

enum HunterSpells
{

    SPELL_HUNTER_ASPECT_OF_THE_BEAST = 13161,
    SPELL_HUNTER_ASPECT_OF_THE_DRAGONHAWK = 5118,
    SPELL_HUNTER_ASPECT_OF_THE_HAWK = 27044,
    SPELL_HUNTER_ASPECT_OF_THE_MONKEY = 13163,
    SPELL_HUNTER_ASPECT_OF_THE_WILD = 80130,

    SPELL_HUNTER_AIMED_SHOT = 49050,
    SPELL_HUNTER_ARCANE_SHOT = 49045,
    SPELL_HUNTER_BARBED_SHOT = 80172,
    SPELL_HUNTER_BARBED_SHOT_FRENZY = 80174,

    SPELL_HUNTER_BESTIAL_WRATH = 80133,
    SPELL_HUNTER_BESTIAL_WRATH_AURA = 80132,

    SPELL_HUNTER_EXPLOSIVE_TRAP = 80138,
    SPELL_HUNTER_FREEZING_TRAP = 80139,
    SPELL_HUNTER_FROST_TRAP = 13809,
    SPELL_HUNTER_IMMOLATION_TRAP = 49056,
    SPELL_HUNTER_IMMOLATION_TRAP_DOT = 49054,
    SPELL_HUNTER_SNAKE_TRAP = 34600,
    SPELL_HUNTER_SNAKE_TRAP_RANGED = 34601,

    SPELL_HUNTER_CALL_OF_THE_WILD = 80186,
    SPELL_HUNTER_DISENGAGE = 781,
    SPELL_HUNTER_EXHILARATION = 80161,
    SPELL_HUNTER_FEIGN_DEATH = 5384,

    SPELL_HUNTER_KILL_COMMAND = 80141,
    SPELL_HUNTER_KILL_SHOT = 61006,
    SPELL_HUNTER_WEAK_SPOT = 80160,
    SPELL_HUNTER_SERPENT_STING = 49001,

    SPELL_HUNTER_RABID_AURA = 53401,
    SPELL_HUNTER_RAPID_FIRE = 80146,
    SPELL_HUNTER_RAPID_FIRE_DAMAGE = 80147,


    RUNE_HUNTER_BULLSEYE_BUFF = 500044,

    RUNE_HUNTER_50CAL_DAMAGE = 500052,

    RUNE_HUNTER_SNIPER_TRAINING_CHECK = 500066,

    RUNE_HUNTER_SERPENT_TOUCH_DAMAGE = 500074,

    RUNE_HUNTER_POISON_INJECTION_DAMAGE = 500094,

    RUNE_HUNTER_REJUVENATING_WIND_HOT = 500258,

    RUNE_HUNTER_REST_IN_PEACE_HEAL = 500314,

    RUNE_HUNTER_KILLER_INSTINCT_DAMAGE = 500366,

    RUNE_HUNTER_PLAYING_WITH_MATCHES_AOE = 500352,

    RUNE_HUNTER_CLEAVE_COMMAND_DAMAGE = 500380,

    RUNE_HUNTER_RAZOR_FRAGMENTS_DAMAGE = 500442,

    RUNE_HUNTER_BEAST_CLEAVE_DAMAGE = 500480,

    RUNE_HUNTER_TRICK_SHOTS_LISTENER = 500518,

    RUNE_HUNTER_ASPECT_OF_THE_STORM_DAMAGE = 500538,

    RUNE_HUNTER_BLACK_MAMBA_DOT = 500570,
    RUNE_HUNTER_BLACK_MAMBA_DAMAGE = 500571,

    RUNE_HUNTER_STOMP_DAMAGE = 500730,

    RUNE_HUNTER_BLOODTHIRSTY_WRATH_HEAL = 500750,
    RUNE_HUNTER_BLOODTHIRSTY_WRATH_SHIELD = 500751,

    RUNE_HUNTER_THUNDERSLASH_DAMAGE = 500814,

    RUNE_HUNTER_POISON_FANGS_DOT = 500900,
    RUNE_HUNTER_POISON_FANGS_DAMAGE = 500901,

    RUNE_HUNTER_TRUE_AIMED_SHOT_BUFF = 500908,
    RUNE_HUNTER_TRUE_AIMED_SHOT_CHECK = 500909,

    RUNE_HUNTER_DOUBLE_IMPACT_DAMAGE = 500928,
};

class rune_hunter_exposed_weakness : public AuraScript
{
    PrepareAuraScript(rune_hunter_exposed_weakness);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_KILL_SHOT, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_exposed_weakness::HandleProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
    }
};

class rune_hunter_bullseye : public AuraScript
{
    PrepareAuraScript(rune_hunter_bullseye);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        int32 healthPct = victim->GetHealthPct();

        if (healthPct > GetAura()->GetEffect(EFFECT_1)->GetAmount())
            return;

        GetCaster()->CastSpell(GetCaster(), RUNE_HUNTER_BULLSEYE_BUFF, TRIGGERED_FULL_MASK);

        Aura* buff = GetCaster()->GetAura(RUNE_HUNTER_BULLSEYE_BUFF);
        int32 maxStack = aurEff->GetAmount();

        if (int32 currentStack = buff->GetStackAmount() > maxStack)
            buff->SetStackAmount(maxStack);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_bullseye::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_bullseye::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_50cal : public AuraScript
{
    PrepareAuraScript(rune_hunter_50cal);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(RUNE_HUNTER_50CAL_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_50cal::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_50cal::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_sniper_training : public AuraScript
{
    PrepareAuraScript(rune_hunter_sniper_training);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Aura* movementCheck = GetCaster()->GetAura(RUNE_HUNTER_SNIPER_TRAINING_CHECK);

        if (!movementCheck)
            return;

        int32 currentStacks = movementCheck->GetStackAmount();

        if (currentStacks < aurEff->GetAmount())
            return;

        int32 buffAura = GetAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;

        if (GetCaster()->HasAura(buffAura))
            return;

        GetCaster()->CastSpell(GetCaster(), buffAura, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_sniper_training::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

class rune_hunter_serpent_touch : public AuraScript
{
    PrepareAuraScript(rune_hunter_serpent_touch);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        if (!victim->HasAura(SPELL_HUNTER_SERPENT_STING))
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(RUNE_HUNTER_SERPENT_TOUCH_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_serpent_touch::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_serpent_touch::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_poison_injection : public SpellScript
{
    PrepareSpellScript(rune_hunter_poison_injection);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500082))
            return GetCaster()->GetAura(500082);

        if (GetCaster()->HasAura(500083))
            return GetCaster()->GetAura(500083);

        if (GetCaster()->HasAura(500084))
            return GetCaster()->GetAura(500084);

        if (GetCaster()->HasAura(500085))
            return GetCaster()->GetAura(500085);

        if (GetCaster()->HasAura(500086))
            return GetCaster()->GetAura(500086);

        if (GetCaster()->HasAura(500087))
            return GetCaster()->GetAura(500087);

        return nullptr;
    }

    void HandleProc()
    {
        if (!GetRuneAura())
            return;

        Unit* victim = GetExplTargetUnit();

        if (!victim)
            return;

        int32 latentPoisonID = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
        Aura* latentPoisonAura = victim->GetAura(latentPoisonID);

        if (!latentPoisonAura)
            return;

        int32 damagePct = latentPoisonAura->GetEffect(EFFECT_0)->GetAmount();
        int32 attackPower = std::max<int32>(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), GetCaster()->GetTotalAttackPowerValue(RANGED_ATTACK));
        float damage = int32(CalculatePct(attackPower, damagePct));
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(RUNE_HUNTER_POISON_INJECTION_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
        victim->RemoveAura(latentPoisonID);
    }

    void Register()
    {
        OnCast += SpellCastFn(rune_hunter_poison_injection::HandleProc);
    }
};

class rune_hunter_dance_with_death : public AuraScript
{
    PrepareAuraScript(rune_hunter_dance_with_death);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        int32 criticalChance = std::max<int32>(GetCaster()->GetUnitCriticalChance(BASE_ATTACK, GetCaster()), GetCaster()->GetUnitCriticalChance(RANGED_ATTACK, GetCaster()));
        uint32 random = urand(1, 100);

        return random <= criticalChance;
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_dance_with_death::CheckProc);
    }
};

class rune_hunter_might_of_the_beast : public AuraScript
{
    PrepareAuraScript(rune_hunter_might_of_the_beast);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(SPELL_HUNTER_ASPECT_OF_THE_BEAST);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = GetAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet)
            GetCaster()->AddAura(procSpell, pet);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            GetCaster()->AddAura(procSpell, unit);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_might_of_the_beast::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_might_of_the_beast::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_dragonhawk_focus : public AuraScript
{
    PrepareAuraScript(rune_hunter_dragonhawk_focus);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(SPELL_HUNTER_ASPECT_OF_THE_DRAGONHAWK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_dragonhawk_focus::CheckProc);
    }
};

class rune_hunter_hawk_quickness : public AuraScript
{
    PrepareAuraScript(rune_hunter_hawk_quickness);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(SPELL_HUNTER_ASPECT_OF_THE_HAWK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_hawk_quickness::CheckProc);
    }
};

class rune_hunter_monkey_business : public AuraScript
{
    PrepareAuraScript(rune_hunter_monkey_business);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(SPELL_HUNTER_ASPECT_OF_THE_MONKEY);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_monkey_business::CheckProc);
    }
};

class rune_hunter_ambuscade : public AuraScript
{
    PrepareAuraScript(rune_hunter_ambuscade);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_DISENGAGE, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_ambuscade::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_natural_mending : public AuraScript
{
    PrepareAuraScript(rune_hunter_natural_mending);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetSpellInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Aura* aura = GetAura();
        Player* target = GetTarget()->ToPlayer();

        if (!aura || !target)
            return;

        int32 spellFocus = eventInfo.GetSpellInfo()->ManaCost;

        if (spellFocus <= 0)
            return;

        int32 cooldownReduction = GetAura()->GetEffect(EFFECT_1)->GetAmount();
        int32 focusAccumulated = GetAura()->GetEffect(EFFECT_2)->GetAmount() + spellFocus;
        int32 focusThreshold = aurEff->GetAmount();

        if (focusAccumulated >= focusThreshold)
        {
            target->ModifySpellCooldown(SPELL_HUNTER_EXHILARATION, -cooldownReduction);
            aura->GetEffect(EFFECT_2)->SetAmount(focusAccumulated - focusThreshold);
            focusAccumulated = GetAura()->GetEffect(EFFECT_2)->GetAmount();

            if (focusAccumulated >= focusThreshold)
            {
                target->ModifySpellCooldown(SPELL_HUNTER_EXHILARATION, -cooldownReduction);
                aura->GetEffect(EFFECT_2)->SetAmount(focusAccumulated - focusThreshold);
            }
        }
        else
            aura->GetEffect(EFFECT_2)->SetAmount(focusAccumulated);
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_natural_mending::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_natural_mending::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_rejuvenating_wind : public AuraScript
{
    PrepareAuraScript(rune_hunter_rejuvenating_wind);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* target = GetCaster();

        if (!target)
            return;

        int32 maxHealth = target->GetMaxHealth();
        float heal = int32(CalculatePct(maxHealth, aurEff->GetAmount()));
        int32 maxTicks = sSpellMgr->GetSpellInfo(RUNE_HUNTER_REJUVENATING_WIND_HOT)->GetMaxTicks();
        int32 amount = heal / maxTicks;

        GetCaster()->CastCustomSpell(RUNE_HUNTER_REJUVENATING_WIND_HOT, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_rejuvenating_wind::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_trap_mastery : public AuraScript
{
    PrepareAuraScript(rune_hunter_trap_mastery);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = GetTarget()->ToPlayer())
        {
            target->ModifySpellCooldown(SPELL_HUNTER_EXPLOSIVE_TRAP, -aurEff->GetAmount());
            target->ModifySpellCooldown(SPELL_HUNTER_FREEZING_TRAP, -aurEff->GetAmount());
            target->ModifySpellCooldown(SPELL_HUNTER_FROST_TRAP, -aurEff->GetAmount());
            target->ModifySpellCooldown(SPELL_HUNTER_IMMOLATION_TRAP, -aurEff->GetAmount());
            target->ModifySpellCooldown(SPELL_HUNTER_SNAKE_TRAP, -aurEff->GetAmount());
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_trap_mastery::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_third_degree_burn : public AuraScript
{
    PrepareAuraScript(rune_hunter_third_degree_burn);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        int32 debuffSpell = GetAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->CastSpell(victim, debuffSpell, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_third_degree_burn::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_third_degree_burn::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_rest_in_peace : public AuraScript
{
    PrepareAuraScript(rune_hunter_rest_in_peace);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_FEIGN_DEATH, true);
    }

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Unit* target = GetCaster();

        if (!target)
            return;

        if (target->GetHealthPct() >= 100)
            return;

        if (!target->HasAura(SPELL_HUNTER_FEIGN_DEATH))
            return;

        int32 maxHealth = target->GetMaxHealth();
        int32 amount = int32(CalculatePct(maxHealth, aurEff->GetAmount()));

        target->CastCustomSpell(RUNE_HUNTER_REST_IN_PEACE_HEAL, SPELLVALUE_BASE_POINT0, amount, target, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_rest_in_peace::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_rest_in_peace::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
    }
};

class rune_hunter_resilience_of_the_hunter : public AuraScript
{
    PrepareAuraScript(rune_hunter_resilience_of_the_hunter);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500316))
            return GetCaster()->GetAura(500316);

        if (GetCaster()->HasAura(500317))
            return GetCaster()->GetAura(500317);

        if (GetCaster()->HasAura(500318))
            return GetCaster()->GetAura(500318);

        if (GetCaster()->HasAura(500319))
            return GetCaster()->GetAura(500319);

        if (GetCaster()->HasAura(500320))
            return GetCaster()->GetAura(500320);

        if (GetCaster()->HasAura(500321))
            return GetCaster()->GetAura(500321);

        return nullptr;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_resilience_of_the_hunter::HandleRemove, EFFECT_0, SPELL_AURA_FEIGN_DEATH, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_ice_skate : public SpellScript
{
    PrepareSpellScript(rune_hunter_ice_skate);

    Aura* GetRuneAura()
    {
        if (GetOriginalCaster()->HasAura(500328))
            return GetOriginalCaster()->GetAura(500328);

        if (GetOriginalCaster()->HasAura(500329))
            return GetOriginalCaster()->GetAura(500329);

        if (GetOriginalCaster()->HasAura(500330))
            return GetOriginalCaster()->GetAura(500330);

        if (GetOriginalCaster()->HasAura(500331))
            return GetOriginalCaster()->GetAura(500331);

        if (GetOriginalCaster()->HasAura(500332))
            return GetOriginalCaster()->GetAura(500332);

        if (GetOriginalCaster()->HasAura(500333))
            return GetOriginalCaster()->GetAura(500333);

        return nullptr;
    }

    void HandleApplyAura()
    {
        if (!GetRuneAura())
            return;

        Unit* unit = GetExplTargetUnit();

        if (!unit)
            return;

        Position dest = unit->GetPosition();
        int32 buffAreaAura = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();

        GetOriginalCaster()->CastSpell(dest.GetPositionX(), dest.GetPositionY(), dest.GetPositionZ(), buffAreaAura, true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(rune_hunter_ice_skate::HandleApplyAura);
    }
};

class rune_hunter_playing_with_matches : public SpellScript
{
    PrepareSpellScript(rune_hunter_playing_with_matches);

    Aura* GetRuneAura()
    {
        if (GetOriginalCaster()->HasAura(500346))
            return GetOriginalCaster()->GetAura(500346);

        if (GetOriginalCaster()->HasAura(500347))
            return GetOriginalCaster()->GetAura(500347);

        if (GetOriginalCaster()->HasAura(500348))
            return GetOriginalCaster()->GetAura(500348);

        if (GetOriginalCaster()->HasAura(500349))
            return GetOriginalCaster()->GetAura(500349);

        if (GetOriginalCaster()->HasAura(500350))
            return GetOriginalCaster()->GetAura(500350);

        if (GetOriginalCaster()->HasAura(500351))
            return GetOriginalCaster()->GetAura(500351);

        return nullptr;
    }

    void HandleApplyAura()
    {
        if (!GetRuneAura())
            return;

        Unit* unit = GetExplTargetUnit();

        if (!unit)
            return;

        Position dest = unit->GetPosition();

        GetOriginalCaster()->CastSpell(dest.GetPositionX(), dest.GetPositionY(), dest.GetPositionZ(), RUNE_HUNTER_PLAYING_WITH_MATCHES_AOE, true);
    }

    void Register() override
    {
        OnCast += SpellCastFn(rune_hunter_playing_with_matches::HandleApplyAura);
    }
};

class rune_hunter_playing_with_matches_proc : public SpellScript
{
    PrepareSpellScript(rune_hunter_playing_with_matches_proc);

    void HandleApplyAura(SpellEffIndex effIndex)
    {
        Unit* unit = GetExplTargetUnit();

        if (!unit)
            return;

        GetOriginalCaster()->CastSpell(unit, SPELL_HUNTER_IMMOLATION_TRAP_DOT, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectHitTarget += SpellEffectFn(rune_hunter_playing_with_matches_proc::HandleApplyAura, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class rune_hunter_killer_instinct : public SpellScript
{
    PrepareSpellScript(rune_hunter_killer_instinct);

    Aura* GetRuneAura()
    {
        Unit* owner = GetCaster()->GetOwner();

        if (owner->HasAura(500360))
            return owner->GetAura(500360);

        if (owner->HasAura(500361))
            return owner->GetAura(500361);

        if (owner->HasAura(500362))
            return owner->GetAura(500362);

        if (owner->HasAura(500363))
            return owner->GetAura(500363);

        if (owner->HasAura(500364))
            return owner->GetAura(500364);

        if (owner->HasAura(500365))
            return owner->GetAura(500365);

        return nullptr;
    }


    void HandleDummy(SpellEffIndex effIndex)
    {
        Aura* rune = GetRuneAura();

        if (!rune)
            return;

        Unit* victim = GetHitUnit();
        int32 pct = rune->GetEffect(EFFECT_0)->GetAmount();
        int32 healthThreshold = rune->GetEffect(EFFECT_1)->GetAmount();

        if (!victim->IsAlive())
            return;

        if (victim->GetHealthPct() <= healthThreshold) {
            int32 damage = GetEffectValue();
            int32 finalDamage = CalculatePct(damage, pct);
            GetCaster()->CastCustomSpell(RUNE_HUNTER_KILLER_INSTINCT_DAMAGE, SPELLVALUE_BASE_POINT0, finalDamage, victim, TRIGGERED_IGNORE_AURA_SCALING);
        }
    }

    void Register() override
    {
        OnEffectLaunchTarget += SpellEffectFn(rune_hunter_killer_instinct::HandleDummy, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
    }
};

class rune_hunter_cleave_command_proc : public AuraScript
{
    PrepareAuraScript(rune_hunter_cleave_command_proc);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet)
            GetCaster()->AddAura(procSpell, pet);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_cleave_command_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_cleave_command : public AuraScript
{
    PrepareAuraScript(rune_hunter_cleave_command);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        eventInfo.GetActor()->CastCustomSpell(RUNE_HUNTER_CLEAVE_COMMAND_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_cleave_command::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_cleave_command::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_strength_of_the_pack : public AuraScript
{
    PrepareAuraScript(rune_hunter_strength_of_the_pack);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        player->AddAura(procSpell, player);

        Pet* pet = player->GetPet();

        if (pet)
            GetCaster()->AddAura(procSpell, pet);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            GetCaster()->AddAura(procSpell, unit);
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_strength_of_the_pack::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_quick_shot : public SpellScript
{
    PrepareSpellScript(rune_hunter_quick_shot);

    Aura* GetRuneAura()
    {
        if (GetOriginalCaster()->HasAura(500394))
            return GetOriginalCaster()->GetAura(500394);

        if (GetOriginalCaster()->HasAura(500395))
            return GetOriginalCaster()->GetAura(500395);

        if (GetOriginalCaster()->HasAura(500396))
            return GetOriginalCaster()->GetAura(500396);

        if (GetOriginalCaster()->HasAura(500397))
            return GetOriginalCaster()->GetAura(500397);

        if (GetOriginalCaster()->HasAura(500398))
            return GetOriginalCaster()->GetAura(500398);

        if (GetOriginalCaster()->HasAura(500399))
            return GetOriginalCaster()->GetAura(500399);

        return nullptr;
    }

    void HandleProc()
    {
        if (!GetRuneAura())
            return;

        Unit* victim = GetExplTargetUnit();

        if (!victim)
            return;

        if (Player* caster = GetCaster()->ToPlayer())
        {
            int32 coolDown = caster->GetSpellCooldownDelay(SPELL_HUNTER_ARCANE_SHOT);
            int32 arcaneShotCooldown = sSpellMgr->GetSpellInfo(SPELL_HUNTER_ARCANE_SHOT)->GetRecoveryTime();
            int32 cooldownReduction = arcaneShotCooldown - coolDown;

            caster->CastSpell(victim, SPELL_HUNTER_ARCANE_SHOT, TRIGGERED_FULL_MASK);

            caster->ModifySpellCooldown(SPELL_HUNTER_ARCANE_SHOT, -cooldownReduction);
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(rune_hunter_quick_shot::HandleProc);
    }
};

class rune_hunter_cobra_senses : public AuraScript
{
    PrepareAuraScript(rune_hunter_cobra_senses);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_KILL_COMMAND, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_cobra_senses::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_deathblow : public AuraScript
{
    PrepareAuraScript(rune_hunter_deathblow);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        LOG_ERROR("error", "deathblow CHECK");
        // return eventInfo.GetDamageInfo();
        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        LOG_ERROR("error", "deathblow PROC");
        uint32 random = urand(1, 100);
        int32 procChance = 0;
        int32 spellID = eventInfo.GetDamageInfo()->GetSpellInfo()->Id;
        LOG_ERROR("error", "deathblow spellID = {}", spellID);
        if (spellID != SPELL_HUNTER_RAPID_FIRE_DAMAGE && spellID != SPELL_HUNTER_AIMED_SHOT)
            return;

        if (spellID == SPELL_HUNTER_RAPID_FIRE_DAMAGE)
            procChance = GetAura()->GetEffect(EFFECT_1)->GetAmount();

        if (spellID == SPELL_HUNTER_AIMED_SHOT)
            procChance = GetAura()->GetEffect(EFFECT_0)->GetAmount();
        LOG_ERROR("error", "deathblow procChance = {} , random = {}", procChance, random);
        if (random > procChance)
            return;
        LOG_ERROR("error", "deathblow PROC 2");
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_KILL_SHOT, true);
        LOG_ERROR("error", "deathblow cooldown REDUCED");
        GetCaster()->AddAura(SPELL_HUNTER_WEAK_SPOT, GetCaster());
        LOG_ERROR("error", "deathblow aura ADDED");
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_deathblow::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_deathblow::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_razor_fragments_trick_shots : public AuraScript
{
    PrepareAuraScript(rune_hunter_razor_fragments_trick_shots);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500430))
            return GetCaster()->GetAura(500430);

        if (GetCaster()->HasAura(500431))
            return GetCaster()->GetAura(500431);

        if (GetCaster()->HasAura(500432))
            return GetCaster()->GetAura(500432);

        if (GetCaster()->HasAura(500433))
            return GetCaster()->GetAura(500433);

        if (GetCaster()->HasAura(500434))
            return GetCaster()->GetAura(500434);

        if (GetCaster()->HasAura(500435))
            return GetCaster()->GetAura(500435);

        return nullptr;
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        int32 procAura = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();

        GetCaster()->AddAura(procAura, GetCaster());
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_razor_fragments_trick_shots::HandleRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_razor_fragments_weak_spot : public AuraScript
{
    PrepareAuraScript(rune_hunter_razor_fragments_weak_spot);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500430))
            return GetCaster()->GetAura(500430);

        if (GetCaster()->HasAura(500431))
            return GetCaster()->GetAura(500431);

        if (GetCaster()->HasAura(500432))
            return GetCaster()->GetAura(500432);

        if (GetCaster()->HasAura(500433))
            return GetCaster()->GetAura(500433);

        if (GetCaster()->HasAura(500434))
            return GetCaster()->GetAura(500434);

        if (GetCaster()->HasAura(500435))
            return GetCaster()->GetAura(500435);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        int32 procAura = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();

        GetCaster()->AddAura(procAura, GetCaster());
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_hunter_razor_fragments_weak_spot::HandleProc, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_razor_fragments : public AuraScript
{
    PrepareAuraScript(rune_hunter_razor_fragments);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 maxTicks = sSpellMgr->GetSpellInfo(RUNE_HUNTER_RAZOR_FRAGMENTS_DAMAGE)->GetMaxTicks();
        int32 amount = damage / maxTicks;

        GetCaster()->CastCustomSpell(RUNE_HUNTER_RAZOR_FRAGMENTS_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
        GetCaster()->RemoveAura(GetAura());
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_razor_fragments::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_razor_fragments::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_good_health : public AuraScript
{
    PrepareAuraScript(rune_hunter_good_health);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = GetAura()->GetSpellInfo()->GetEffect(EFFECT_1).TriggerSpell;
        int32 procPct = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (!pet)
            return;

        if (pet->HasAura(procSpell))
            pet->RemoveAura(procSpell);

        float health = pet->GetMaxHealth();
        int32 amount = CalculatePct(health, procPct);

        GetCaster()->CastCustomSpell(procSpell, SPELLVALUE_BASE_POINT0, amount, pet, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_good_health::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_beast_cleave_proc : public AuraScript
{
    PrepareAuraScript(rune_hunter_beast_cleave_proc);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet)
            GetCaster()->AddAura(procSpell, pet);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            GetCaster()->AddAura(procSpell, unit);
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_beast_cleave_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_beast_cleave : public AuraScript
{
    PrepareAuraScript(rune_hunter_beast_cleave);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        eventInfo.GetActor()->CastCustomSpell(RUNE_HUNTER_BEAST_CLEAVE_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_beast_cleave::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_beast_cleave::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_trick_shots_listener : public SpellScript
{
    PrepareSpellScript(rune_hunter_trick_shots_listener);

    void HandleApplyAura()
    {
        if (GetCaster()->HasAura(RUNE_HUNTER_TRICK_SHOTS_LISTENER))
            GetCaster()->RemoveAura(RUNE_HUNTER_TRICK_SHOTS_LISTENER);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(rune_hunter_trick_shots_listener::HandleApplyAura);
    }
};

class rune_hunter_trick_shots : public AuraScript
{
    PrepareAuraScript(rune_hunter_trick_shots);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 buffAura = aurEff->GetAmount();

        if (GetCaster()->HasAura(buffAura))
            return;

        GetCaster()->CastSpell(GetCaster(), RUNE_HUNTER_TRICK_SHOTS_LISTENER, TRIGGERED_FULL_MASK);

        Aura* listener = GetCaster()->GetAura(RUNE_HUNTER_TRICK_SHOTS_LISTENER);

        if (listener->GetStackAmount() < 3)
            return;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_trick_shots::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_trick_shots::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_aspect_of_the_storm : public AuraScript
{
    PrepareAuraScript(rune_hunter_aspect_of_the_storm);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        int32 attackPower = std::max<int32>(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), GetCaster()->GetTotalAttackPowerValue(RANGED_ATTACK));
        float damage = int32(CalculatePct(attackPower, aurEff->GetAmount()));

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet)
            pet->CastCustomSpell(RUNE_HUNTER_ASPECT_OF_THE_STORM_DAMAGE, SPELLVALUE_BASE_POINT0, damage, pet, TRIGGERED_FULL_MASK);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            unit->CastCustomSpell(RUNE_HUNTER_ASPECT_OF_THE_STORM_DAMAGE, SPELLVALUE_BASE_POINT0, damage, unit, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_aspect_of_the_storm::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_black_mamba_proc : public AuraScript
{
    PrepareAuraScript(rune_hunter_black_mamba_proc);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            int32 displayID = unit->GetNativeDisplayId();

            if (displayID != 6303 && displayID != 2958 && displayID != 1206 && displayID != 2957)
                continue;

            GetCaster()->AddAura(procSpell, unit);
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_black_mamba_proc::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_black_mamba : public AuraScript
{
    PrepareAuraScript(rune_hunter_black_mamba);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();
        Unit* pet = eventInfo.GetActor();
        int32 attackPower = std::max<int32>(player->GetTotalAttackPowerValue(BASE_ATTACK), player->GetTotalAttackPowerValue(RANGED_ATTACK));
        float damage = int32(CalculatePct(attackPower, aurEff->GetAmount()));

        if (!victim || !player || !pet)
            return;

        if (Aura* dot = victim->GetAura(RUNE_HUNTER_BLACK_MAMBA_DOT))
        {
            if (dot->GetStackAmount() >= 5)
                player->CastCustomSpell(RUNE_HUNTER_BLACK_MAMBA_DAMAGE, SPELLVALUE_BASE_POINT0, damage, victim, TRIGGERED_FULL_MASK);
        }

        player->CastCustomSpell(RUNE_HUNTER_BLACK_MAMBA_DOT, SPELLVALUE_BASE_POINT0, damage, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_black_mamba::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_black_mamba::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_it_had_to_be_snake : public AuraScript
{
    PrepareAuraScript(rune_hunter_it_had_to_be_snake);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        Position dest = victim->GetPosition();

        GetCaster()->CastSpell(dest.GetPositionX(), dest.GetPositionY(), dest.GetPositionZ(), SPELL_HUNTER_SNAKE_TRAP_RANGED, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_it_had_to_be_snake::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_mighty_snake_trap : public AuraScript
{
    PrepareAuraScript(rune_hunter_mighty_snake_trap);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            int32 displayID = unit->GetNativeDisplayId();

            if (displayID != 6303 && displayID != 2958 && displayID != 1206 && displayID != 2957)
                continue;

            GetCaster()->AddAura(procSpell, unit);
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_mighty_snake_trap::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_master_handler_aura : public AuraScript
{
    PrepareAuraScript(rune_hunter_master_handler_aura);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet && !pet->HasAura(procSpell))
            GetCaster()->AddAura(procSpell, pet);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (!unit->HasAura(procSpell))
                GetCaster()->AddAura(procSpell, unit);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->HasAura(procSpell))
            pet->RemoveAura(procSpell);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (unit->HasAura(procSpell))
                unit->RemoveAura(procSpell);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_master_handler_aura::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_master_handler_aura::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_master_handler : public AuraScript
{
    PrepareAuraScript(rune_hunter_master_handler);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = eventInfo.GetActor()->GetOwner()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_ASPECT_OF_THE_WILD, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_master_handler::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_ferocious_appetite : public AuraScript
{
    PrepareAuraScript(rune_hunter_ferocious_appetite);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* target = GetCaster()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_ASPECT_OF_THE_WILD, -aurEff->GetAmount());
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_ferocious_appetite::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_snake_bite : public AuraScript
{
    PrepareAuraScript(rune_hunter_snake_bite);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500652))
            return GetCaster()->GetAura(500652);

        if (GetCaster()->HasAura(500653))
            return GetCaster()->GetAura(500653);

        if (GetCaster()->HasAura(500654))
            return GetCaster()->GetAura(500654);

        if (GetCaster()->HasAura(500655))
            return GetCaster()->GetAura(500655);

        if (GetCaster()->HasAura(500656))
            return GetCaster()->GetAura(500656);

        if (GetCaster()->HasAura(500657))
            return GetCaster()->GetAura(500657);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(500658))
            GetCaster()->RemoveAura(500658);

        if (GetCaster()->HasAura(500659))
            GetCaster()->RemoveAura(500659);

        if (GetCaster()->HasAura(500660))
            GetCaster()->RemoveAura(500660);

        if (GetCaster()->HasAura(500661))
            GetCaster()->RemoveAura(500661);

        if (GetCaster()->HasAura(500662))
            GetCaster()->RemoveAura(500662);

        if (GetCaster()->HasAura(500663))
            GetCaster()->RemoveAura(500663);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_hunter_snake_bite::HandleProc, EFFECT_0, SPELL_AURA_MOD_CRIT_PCT, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_snake_bite::HandleRemove, EFFECT_0, SPELL_AURA_MOD_CRIT_PCT, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_war_orders : public AuraScript
{
    PrepareAuraScript(rune_hunter_war_orders);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_KILL_COMMAND, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_war_orders::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_scent_of_blood : public AuraScript
{
    PrepareAuraScript(rune_hunter_scent_of_blood);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500682))
            return GetCaster()->GetAura(500682);

        if (GetCaster()->HasAura(500683))
            return GetCaster()->GetAura(500683);

        if (GetCaster()->HasAura(500684))
            return GetCaster()->GetAura(500684);

        if (GetCaster()->HasAura(500685))
            return GetCaster()->GetAura(500685);

        if (GetCaster()->HasAura(500686))
            return GetCaster()->GetAura(500686);

        if (GetCaster()->HasAura(500687))
            return GetCaster()->GetAura(500687);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_BARBED_SHOT, true);

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(500688))
            GetCaster()->RemoveAura(500688);

        if (GetCaster()->HasAura(500689))
            GetCaster()->RemoveAura(500689);

        if (GetCaster()->HasAura(500690))
            GetCaster()->RemoveAura(500690);

        if (GetCaster()->HasAura(500691))
            GetCaster()->RemoveAura(500691);

        if (GetCaster()->HasAura(500692))
            GetCaster()->RemoveAura(500692);

        if (GetCaster()->HasAura(500693))
            GetCaster()->RemoveAura(500693);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_hunter_scent_of_blood::HandleProc, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_scent_of_blood::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_deaths_dance : public AuraScript
{
    PrepareAuraScript(rune_hunter_deaths_dance);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        int32 criticalChance = std::max<int32>(GetCaster()->GetUnitCriticalChance(BASE_ATTACK, eventInfo.GetActionTarget()), GetCaster()->GetUnitCriticalChance(RANGED_ATTACK, eventInfo.GetActionTarget()));
        uint32 random = urand(1, 100);

        return random <= criticalChance;
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_deaths_dance::CheckProc);
    }
};

class rune_hunter_brutal_companion : public AuraScript
{
    PrepareAuraScript(rune_hunter_brutal_companion);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        Pet* pet = GetCaster()->ToPlayer()->GetPet();

        if (!pet)
            return false;

        if (!pet->HasAura(SPELL_HUNTER_BARBED_SHOT_FRENZY))
            return false;

        if (pet->GetAura(SPELL_HUNTER_BARBED_SHOT_FRENZY)->GetStackAmount() < 3)
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetActionTarget();

        if (!victim)
            return;

        if (Player* caster = GetCaster()->ToPlayer())
        {
            int32 coolDown = caster->GetSpellCooldownDelay(SPELL_HUNTER_KILL_COMMAND);
            int32 arcaneShotCooldown = sSpellMgr->GetSpellInfo(SPELL_HUNTER_KILL_COMMAND)->GetRecoveryTime();
            int32 cooldownReduction = arcaneShotCooldown - coolDown;

            caster->CastSpell(victim, SPELL_HUNTER_KILL_COMMAND, TRIGGERED_FULL_MASK);

            caster->ModifySpellCooldown(SPELL_HUNTER_KILL_COMMAND, -cooldownReduction);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_brutal_companion::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_brutal_companion::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_stomp : public AuraScript
{
    PrepareAuraScript(rune_hunter_stomp);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        int32 attackPower = std::max<int32>(GetCaster()->GetTotalAttackPowerValue(BASE_ATTACK), GetCaster()->GetTotalAttackPowerValue(RANGED_ATTACK));
        float damage = int32(CalculatePct(attackPower, aurEff->GetAmount()));

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet)
            pet->CastCustomSpell(RUNE_HUNTER_STOMP_DAMAGE, SPELLVALUE_BASE_POINT0, damage, pet, TRIGGERED_FULL_MASK);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            unit->CastCustomSpell(RUNE_HUNTER_STOMP_DAMAGE, SPELLVALUE_BASE_POINT0, damage, unit, TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_stomp::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_bloodthirsty_wrath : public AuraScript
{
    PrepareAuraScript(rune_hunter_bloodthirsty_wrath);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500738))
            return GetCaster()->GetAura(500738);

        if (GetCaster()->HasAura(500739))
            return GetCaster()->GetAura(500739);

        if (GetCaster()->HasAura(500740))
            return GetCaster()->GetAura(500740);

        if (GetCaster()->HasAura(500741))
            return GetCaster()->GetAura(500741);

        if (GetCaster()->HasAura(500742))
            return GetCaster()->GetAura(500742);

        if (GetCaster()->HasAura(500743))
            return GetCaster()->GetAura(500743);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(500744))
            GetCaster()->RemoveAura(500744);

        if (GetCaster()->HasAura(500745))
            GetCaster()->RemoveAura(500745);

        if (GetCaster()->HasAura(500746))
            GetCaster()->RemoveAura(500746);

        if (GetCaster()->HasAura(500747))
            GetCaster()->RemoveAura(500747);

        if (GetCaster()->HasAura(500748))
            GetCaster()->RemoveAura(500748);

        if (GetCaster()->HasAura(500749))
            GetCaster()->RemoveAura(500749);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_hunter_bloodthirsty_wrath::HandleProc, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_bloodthirsty_wrath::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_bloodthirsty_wrath_heal : public AuraScript
{
    PrepareAuraScript(rune_hunter_bloodthirsty_wrath_heal);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);
        int32 maxHealth = GetCaster()->GetMaxHealth();
        int32 currentHealth = GetCaster()->GetHealth();

        if (currentHealth + amount <= maxHealth)
            GetCaster()->CastCustomSpell(RUNE_HUNTER_BLOODTHIRSTY_WRATH_HEAL, SPELLVALUE_BASE_POINT0, amount, GetCaster(), TRIGGERED_FULL_MASK);
        else
        {
            int32 newAmount = 0;

            if (currentHealth != maxHealth)
            {
                newAmount = maxHealth - currentHealth;
                GetCaster()->CastCustomSpell(RUNE_HUNTER_BLOODTHIRSTY_WRATH_HEAL, SPELLVALUE_BASE_POINT0, newAmount, GetCaster(), TRIGGERED_FULL_MASK);
            }

            int32 shieldAmount = amount - newAmount;
            int32 maxShieldThreshold = CalculatePct(maxHealth, GetAura()->GetEffect(EFFECT_1)->GetAmount());

            if (GetCaster()->HasAura(RUNE_HUNTER_BLOODTHIRSTY_WRATH_SHIELD))
                shieldAmount += GetCaster()->GetAura(RUNE_HUNTER_BLOODTHIRSTY_WRATH_SHIELD)->GetEffect(EFFECT_0)->GetAmount();

            int32 shield = std::min<int32>(shieldAmount, maxShieldThreshold);

            GetCaster()->CastCustomSpell(RUNE_HUNTER_BLOODTHIRSTY_WRATH_SHIELD, SPELLVALUE_BASE_POINT0, shield, GetCaster(), TRIGGERED_FULL_MASK);
        }
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_bloodthirsty_wrath_heal::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_bloodthirsty_wrath_heal::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_2wolves_1man : public SpellScript
{
    PrepareSpellScript(rune_hunter_2wolves_1man);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500752))
            return GetCaster()->GetAura(500752);

        if (GetCaster()->HasAura(500753))
            return GetCaster()->GetAura(500753);

        if (GetCaster()->HasAura(500754))
            return GetCaster()->GetAura(500754);

        if (GetCaster()->HasAura(500755))
            return GetCaster()->GetAura(500755);

        if (GetCaster()->HasAura(500756))
            return GetCaster()->GetAura(500756);

        if (GetCaster()->HasAura(500757))
            return GetCaster()->GetAura(500757);

        return nullptr;
    }

    void HandleBuff()
    {
        if (!GetRuneAura())
            return;

        int32 procChance = GetRuneAura()->GetEffect(EFFECT_0)->GetAmount();
        int32 procSpell = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;
        int32 random = urand(1, 100);

        if (random > procChance)
            return;

        GetCaster()->CastSpell(GetCaster(), procSpell, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        BeforeCast += SpellCastFn(rune_hunter_2wolves_1man::HandleBuff);
    }
};

class rune_hunter_2wolves_1man_summon : public SpellScript
{
    PrepareSpellScript(rune_hunter_2wolves_1man_summon);

    void PetBuffs(Creature* summon)
    {
        Unit* caster = GetCaster();

        summon->AddAura(34902, caster);
        summon->AddAura(34903, caster);
        summon->AddAura(34904, caster);
    }

    void HandlePet()
    {
        Unit* caster = GetCaster();
        Position const& pos = GetCaster()->GetPosition();
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        int32 duration = GetSpellInfo()->GetDuration();
        SpellValue const* value = GetSpellValue();
        uint32 summonAmount = value->EffectBasePoints[EFFECT_0];

        for (size_t i = 0; i < summonAmount; i++)
        {
            auto petId = GetSpellInfo()->Id;
            Creature* pet = GetCaster()->SummonCreature(petId, pos, TEMPSUMMON_TIMED_DESPAWN, duration, 0, properties);

            CreatureTemplate const* petCinfo = sObjectMgr->GetCreatureTemplate(petId);
            CreatureFamilyEntry const* petFamily = sCreatureFamilyStore.LookupEntry(petCinfo->family);

            PetBuffs(pet);

            if (pet && pet->IsAlive())
                pet->AI()->AttackStart(GetExplTargetUnit());
        }
    }

    void Register() override
    {
        OnCast += SpellCastFn(rune_hunter_2wolves_1man_summon::HandlePet);
    }
};

class rune_hunter_howl : public AuraScript
{
    PrepareAuraScript(rune_hunter_howl);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        Player* player = GetCaster()->ToPlayer();
        Pet* pet = player->GetPet();
        int32 increasedDuration = GetAura()->GetEffect(EFFECT_1)->GetAmount();
        int32 baseDuration = aurEff->GetAmount();
        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();


        if (Aura* auraEff = player->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA))
        {
            uint32 duration = (std::min<int32>(auraEff->GetDuration() + increasedDuration, auraEff->GetMaxDuration() + 10000));

            auraEff->SetDuration(duration);

            if (pet && pet->HasAura(SPELL_HUNTER_BESTIAL_WRATH_AURA))
                pet->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(duration);
            else
            {
                player->AddAura(SPELL_HUNTER_BESTIAL_WRATH_AURA, pet);
                pet->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(duration);
            }

            for (auto const& unit : summonedUnits)
            {
                if (unit->isDead())
                    continue;

                if (unit->HasAura(SPELL_HUNTER_BESTIAL_WRATH_AURA))
                    unit->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(duration);
                else
                {
                    player->AddAura(SPELL_HUNTER_BESTIAL_WRATH_AURA, unit);
                    unit->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(duration);
                }
            }
        }
        else
        {
            player->AddAura(SPELL_HUNTER_BESTIAL_WRATH_AURA, player);
            player->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(baseDuration);

            if (pet)
            {
                player->AddAura(SPELL_HUNTER_BESTIAL_WRATH_AURA, pet);
                pet->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(baseDuration);
            }

            for (auto const& unit : summonedUnits)
            {
                if (unit->isDead())
                    continue;

                player->AddAura(SPELL_HUNTER_BESTIAL_WRATH_AURA, unit);
                unit->GetAura(SPELL_HUNTER_BESTIAL_WRATH_AURA)->SetDuration(baseDuration);
            }
        }
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_howl::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_barbed_wrath : public AuraScript
{
    PrepareAuraScript(rune_hunter_barbed_wrath);

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_BESTIAL_WRATH, -aurEff->GetAmount());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_barbed_wrath::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_sustained_anger : public AuraScript
{
    PrepareAuraScript(rune_hunter_sustained_anger);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return GetCaster()->HasAura(SPELL_HUNTER_BESTIAL_WRATH_AURA);
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        Player* player = GetCaster()->ToPlayer();
        int32 procSpell = aurEff->GetAmount();

        if (!player)
            return;

        player->CastSpell(player, procSpell, TRIGGERED_FULL_MASK);
        Pet* pet = player->GetPet();

        if (pet)
            pet->CastSpell(pet, procSpell, TRIGGERED_FULL_MASK);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            unit->CastSpell(unit, procSpell, TRIGGERED_FULL_MASK);
        }
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_sustained_anger::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_sustained_anger::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_killer_cobra_apply : public AuraScript
{
    PrepareAuraScript(rune_hunter_killer_cobra_apply);

    Aura* GetRuneAura()
    {
        if (GetCaster()->HasAura(500784))
            return GetCaster()->GetAura(500784);

        if (GetCaster()->HasAura(500785))
            return GetCaster()->GetAura(500785);

        if (GetCaster()->HasAura(500786))
            return GetCaster()->GetAura(500786);

        if (GetCaster()->HasAura(500787))
            return GetCaster()->GetAura(500787);

        if (GetCaster()->HasAura(500788))
            return GetCaster()->GetAura(500788);

        if (GetCaster()->HasAura(500789))
            return GetCaster()->GetAura(500789);

        return nullptr;
    }

    void HandleProc(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (!GetRuneAura())
            return;

        int32 buffAura = GetRuneAura()->GetSpellInfo()->GetEffect(EFFECT_0).TriggerSpell;

        GetCaster()->AddAura(buffAura, GetCaster());
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        if (GetCaster()->HasAura(500790))
            GetCaster()->RemoveAura(500790);

        if (GetCaster()->HasAura(500791))
            GetCaster()->RemoveAura(500791);

        if (GetCaster()->HasAura(500792))
            GetCaster()->RemoveAura(500792);

        if (GetCaster()->HasAura(500793))
            GetCaster()->RemoveAura(500793);

        if (GetCaster()->HasAura(500794))
            GetCaster()->RemoveAura(500794);

        if (GetCaster()->HasAura(500795))
            GetCaster()->RemoveAura(500795);
    }

    void Register() override
    {
        OnEffectApply += AuraEffectApplyFn(rune_hunter_killer_cobra_apply::HandleProc, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_killer_cobra_apply::HandleRemove, EFFECT_0, SPELL_AURA_MOD_SCALE, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_killer_cobra : public AuraScript
{
    PrepareAuraScript(rune_hunter_killer_cobra);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_KILL_COMMAND, true);
    }

    void Register()
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_killer_cobra::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_thunderslash_aura : public AuraScript
{
    PrepareAuraScript(rune_hunter_thunderslash_aura);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet && !pet->HasAura(procSpell))
            player->AddAura(procSpell, pet);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (!unit->HasAura(procSpell))
                player->AddAura(procSpell, unit);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->HasAura(procSpell))
            pet->RemoveAura(procSpell);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (unit->HasAura(procSpell))
                unit->RemoveAura(procSpell);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_thunderslash_aura::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_thunderslash_aura::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_thunderslash : public AuraScript
{
    PrepareAuraScript(rune_hunter_thunderslash);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return false;

        if (!eventInfo.GetActor()->HasAura(SPELL_HUNTER_BESTIAL_WRATH_AURA))
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* actor = eventInfo.GetActor();
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        actor->CastCustomSpell(RUNE_HUNTER_THUNDERSLASH_DAMAGE, SPELLVALUE_BASE_POINT0, amount, actor, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_thunderslash::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_thunderslash::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_wild_instincts : public AuraScript
{
    PrepareAuraScript(rune_hunter_wild_instincts);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        if (!eventInfo.GetDamageInfo())
            return false;

        if (!GetCaster()->HasAura(SPELL_HUNTER_CALL_OF_THE_WILD))
            return false;

        if (eventInfo.GetSpellInfo()->ManaCost <= 0)
            return false;

        return true;
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        if (Player* caster = GetCaster()->ToPlayer())
            caster->RemoveSpellCooldown(SPELL_HUNTER_BARBED_SHOT, true);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_wild_instincts::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_wild_instincts::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_enduring_call_aura : public AuraScript
{
    PrepareAuraScript(rune_hunter_enduring_call_aura);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet && !pet->HasAura(procSpell))
            player->AddAura(procSpell, pet);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (!unit->HasAura(procSpell))
                player->AddAura(procSpell, unit);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->HasAura(procSpell))
            pet->RemoveAura(procSpell);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (unit->HasAura(procSpell))
                unit->RemoveAura(procSpell);
        }
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        Pet* pet = player->GetPet();
        int32 durationIncrease = aurEff->GetAmount();

        if (Aura* auraEff = player->GetAura(SPELL_HUNTER_CALL_OF_THE_WILD))
            auraEff->SetDuration(auraEff->GetDuration() + durationIncrease);

        if (!pet)
            return;

        if (Aura* auraEff = pet->GetAura(SPELL_HUNTER_RABID_AURA))
            auraEff->SetDuration(auraEff->GetDuration() + durationIncrease);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_enduring_call_aura::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_enduring_call_aura::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
        OnEffectProc += AuraEffectProcFn(rune_hunter_enduring_call_aura::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_enduring_call : public AuraScript
{
    PrepareAuraScript(rune_hunter_enduring_call);

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* actor = eventInfo.GetActor();
        Player* player = actor->GetOwner()->ToPlayer();
        int32 durationIncrease = aurEff->GetAmount();

        if (Aura* auraEff = player->GetAura(SPELL_HUNTER_CALL_OF_THE_WILD))
            auraEff->SetDuration(auraEff->GetDuration() + durationIncrease);

        if (Aura* auraEff = actor->GetAura(SPELL_HUNTER_RABID_AURA))
            auraEff->SetDuration(auraEff->GetDuration() + durationIncrease);
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_enduring_call::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_double_trouble_aura : public AuraScript
{
    PrepareAuraScript(rune_hunter_double_trouble_aura);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet && !pet->HasAura(procSpell))
            player->AddAura(procSpell, pet);
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->HasAura(procSpell))
            pet->RemoveAura(procSpell);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_double_trouble_aura::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_double_trouble_aura::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_double_trouble : public AuraScript
{
    PrepareAuraScript(rune_hunter_double_trouble);

    void PetBuffs(Creature* summon)
    {
        Unit* caster = GetCaster();

        summon->AddAura(34902, caster);
        summon->AddAura(34903, caster);
        summon->AddAura(34904, caster);
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* caster = GetCaster()->ToPlayer();
        Position const& pos = GetCaster()->GetPosition();
        SummonPropertiesEntry const* properties = sSummonPropertiesStore.LookupEntry(61);
        int32 duration = aurEff->GetAmount();
        Unit* pet = eventInfo.GetActor();
        int32 petId = pet->GetDisplayId();

        LOG_ERROR("error", "ID = {}", petId);

        Creature* guardian = GetCaster()->SummonCreature(petId, pos, TEMPSUMMON_TIMED_DESPAWN, duration, 0, properties);
        LOG_ERROR("error", "ID = {}", guardian->GetDisplayId());
        CreatureTemplate const* petCinfo = sObjectMgr->GetCreatureTemplate(petId);
        CreatureFamilyEntry const* petFamily = sCreatureFamilyStore.LookupEntry(petCinfo->family);

        PetBuffs(guardian);

        if (guardian && guardian->IsAlive())
            guardian->AI()->AttackStart(eventInfo.GetActionTarget());
    }

    void Register() override
    {
        OnEffectProc += AuraEffectProcFn(rune_hunter_double_trouble::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_poison_fangs_aura : public AuraScript
{
    PrepareAuraScript(rune_hunter_poison_fangs_aura);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet && !pet->HasAura(procSpell))
            player->AddAura(procSpell, pet);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (!unit->HasAura(procSpell))
                player->AddAura(procSpell, unit);
        }
    }

    void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
    {
        int32 procSpell = aurEff->GetAmount();
        Player* player = GetCaster()->ToPlayer();

        if (!player)
            return;

        Pet* pet = player->GetPet();

        if (pet && pet->HasAura(procSpell))
            pet->RemoveAura(procSpell);

        std::vector<Unit*> summonedUnits = player->GetSummonedUnits();

        if (summonedUnits.empty())
            return;

        for (auto const& unit : summonedUnits)
        {
            if (unit->isDead())
                continue;

            if (unit->HasAura(procSpell))
                unit->RemoveAura(procSpell);
        }
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_poison_fangs_aura::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        OnEffectRemove += AuraEffectRemoveFn(rune_hunter_poison_fangs_aura::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

class rune_hunter_poison_fangs : public AuraScript
{
    PrepareAuraScript(rune_hunter_poison_fangs);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Player* player = GetCaster()->ToPlayer();
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();
        Unit* pet = eventInfo.GetActor();
        int32 attackPower = std::max<int32>(player->GetTotalAttackPowerValue(BASE_ATTACK), player->GetTotalAttackPowerValue(RANGED_ATTACK));
        float damage = int32(CalculatePct(attackPower, aurEff->GetAmount()));

        if (!victim || !player || !pet)
            return;

        if (Aura* dot = victim->GetAura(RUNE_HUNTER_POISON_FANGS_DOT))
        {
            if (dot->GetStackAmount() >= 5)
                player->CastCustomSpell(RUNE_HUNTER_POISON_FANGS_DAMAGE, SPELLVALUE_BASE_POINT0, damage, victim, TRIGGERED_FULL_MASK);
        }
        player->CastCustomSpell(RUNE_HUNTER_POISON_FANGS_DOT, SPELLVALUE_BASE_POINT0, damage, victim, TRIGGERED_FULL_MASK);
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_poison_fangs::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_poison_fangs::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_true_aimed_shot : public AuraScript
{
    PrepareAuraScript(rune_hunter_true_aimed_shot);

    void HandlePeriodic(AuraEffect const* aurEff)
    {
        Aura* movementCheck = GetCaster()->GetAura(RUNE_HUNTER_TRUE_AIMED_SHOT_CHECK);

        if (!movementCheck)
            return;

        if (movementCheck->GetStackAmount() < aurEff->GetAmount())
            return;

        movementCheck->SetStackAmount(aurEff->GetAmount());

        if (GetCaster()->HasAura(RUNE_HUNTER_TRUE_AIMED_SHOT_BUFF))
            return;

        GetCaster()->CastSpell(GetCaster(), RUNE_HUNTER_TRUE_AIMED_SHOT_BUFF, TRIGGERED_FULL_MASK);
    }

    void Register() override
    {
        OnEffectPeriodic += AuraEffectPeriodicFn(rune_hunter_true_aimed_shot::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
    }
};

class rune_hunter_set_aim_shoot : public AuraScript
{
    PrepareAuraScript(rune_hunter_set_aim_shoot);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleEffectProc(AuraEffect const* aurEff, ProcEventInfo& /*eventInfo*/)
    {
        if (Player* target = GetTarget()->ToPlayer())
            target->ModifySpellCooldown(SPELL_HUNTER_AIMED_SHOT, -aurEff->GetAmount());
    }

    void Register() override
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_set_aim_shoot::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_set_aim_shoot::HandleEffectProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};

class rune_hunter_double_impact : public AuraScript
{
    PrepareAuraScript(rune_hunter_double_impact);

    bool CheckProc(ProcEventInfo& eventInfo)
    {
        return eventInfo.GetDamageInfo();
    }

    void HandleProc(AuraEffect const* aurEff, ProcEventInfo& eventInfo)
    {
        Unit* victim = eventInfo.GetDamageInfo()->GetVictim();

        if (!victim)
            return;

        float damageDealt = eventInfo.GetDamageInfo()->GetDamage();

        if (damageDealt <= 0)
            return;

        float damage = CalculatePct(int32(damageDealt), aurEff->GetAmount());
        int32 amount = std::max<int32>(0, damage);

        GetCaster()->CastCustomSpell(RUNE_HUNTER_DOUBLE_IMPACT_DAMAGE, SPELLVALUE_BASE_POINT0, amount, victim, TRIGGERED_FULL_MASK);
        GetCaster()->RemoveAura(GetAura());
    }

    void Register()
    {
        DoCheckProc += AuraCheckProcFn(rune_hunter_double_impact::CheckProc);
        OnEffectProc += AuraEffectProcFn(rune_hunter_double_impact::HandleProc, EFFECT_0, SPELL_AURA_DUMMY);
    }
};










void AddSC_hunter_perks_scripts()
{
    RegisterSpellScript(rune_hunter_exposed_weakness);
    RegisterSpellScript(rune_hunter_bullseye);
    RegisterSpellScript(rune_hunter_50cal);
    RegisterSpellScript(rune_hunter_sniper_training);
    RegisterSpellScript(rune_hunter_serpent_touch);
    RegisterSpellScript(rune_hunter_poison_injection);
    RegisterSpellScript(rune_hunter_dance_with_death);
    RegisterSpellScript(rune_hunter_might_of_the_beast);
    RegisterSpellScript(rune_hunter_dragonhawk_focus);
    RegisterSpellScript(rune_hunter_hawk_quickness);
    RegisterSpellScript(rune_hunter_monkey_business);
    RegisterSpellScript(rune_hunter_ambuscade);
    RegisterSpellScript(rune_hunter_natural_mending);
    RegisterSpellScript(rune_hunter_rejuvenating_wind);
    RegisterSpellScript(rune_hunter_trap_mastery);
    RegisterSpellScript(rune_hunter_third_degree_burn);
    RegisterSpellScript(rune_hunter_rest_in_peace);
    RegisterSpellScript(rune_hunter_resilience_of_the_hunter);
    RegisterSpellScript(rune_hunter_ice_skate);
    RegisterSpellScript(rune_hunter_playing_with_matches);
    RegisterSpellScript(rune_hunter_playing_with_matches_proc);
    RegisterSpellScript(rune_hunter_killer_instinct);
    RegisterSpellScript(rune_hunter_cleave_command_proc);
    RegisterSpellScript(rune_hunter_cleave_command);
    RegisterSpellScript(rune_hunter_strength_of_the_pack);
    RegisterSpellScript(rune_hunter_quick_shot);
    RegisterSpellScript(rune_hunter_cobra_senses);
    RegisterSpellScript(rune_hunter_deathblow);
    RegisterSpellScript(rune_hunter_razor_fragments_trick_shots);
    RegisterSpellScript(rune_hunter_razor_fragments_weak_spot);
    RegisterSpellScript(rune_hunter_razor_fragments);
    RegisterSpellScript(rune_hunter_good_health);
    RegisterSpellScript(rune_hunter_beast_cleave_proc);
    RegisterSpellScript(rune_hunter_beast_cleave);
    RegisterSpellScript(rune_hunter_trick_shots_listener);
    RegisterSpellScript(rune_hunter_trick_shots);
    RegisterSpellScript(rune_hunter_aspect_of_the_storm);
    RegisterSpellScript(rune_hunter_black_mamba_proc);
    RegisterSpellScript(rune_hunter_black_mamba);
    RegisterSpellScript(rune_hunter_it_had_to_be_snake);
    RegisterSpellScript(rune_hunter_mighty_snake_trap);
    RegisterSpellScript(rune_hunter_master_handler_aura);
    RegisterSpellScript(rune_hunter_master_handler);
    RegisterSpellScript(rune_hunter_ferocious_appetite);
    RegisterSpellScript(rune_hunter_snake_bite);
    RegisterSpellScript(rune_hunter_war_orders);
    RegisterSpellScript(rune_hunter_scent_of_blood);
    RegisterSpellScript(rune_hunter_deaths_dance);
    RegisterSpellScript(rune_hunter_brutal_companion);
    RegisterSpellScript(rune_hunter_stomp);
    RegisterSpellScript(rune_hunter_bloodthirsty_wrath);
    RegisterSpellScript(rune_hunter_bloodthirsty_wrath_heal);
    RegisterSpellScript(rune_hunter_2wolves_1man);
    RegisterSpellScript(rune_hunter_2wolves_1man_summon);
    RegisterSpellScript(rune_hunter_howl);
    RegisterSpellScript(rune_hunter_barbed_wrath);
    RegisterSpellScript(rune_hunter_sustained_anger);
    RegisterSpellScript(rune_hunter_killer_cobra_apply);
    RegisterSpellScript(rune_hunter_killer_cobra);
    RegisterSpellScript(rune_hunter_thunderslash_aura);
    RegisterSpellScript(rune_hunter_thunderslash);
    RegisterSpellScript(rune_hunter_wild_instincts);
    RegisterSpellScript(rune_hunter_enduring_call_aura);
    RegisterSpellScript(rune_hunter_enduring_call);
    RegisterSpellScript(rune_hunter_double_trouble_aura);
    RegisterSpellScript(rune_hunter_double_trouble);
    RegisterSpellScript(rune_hunter_poison_fangs_aura);
    RegisterSpellScript(rune_hunter_poison_fangs);
    RegisterSpellScript(rune_hunter_true_aimed_shot);
    RegisterSpellScript(rune_hunter_set_aim_shoot);
    RegisterSpellScript(rune_hunter_double_impact);




    
}
