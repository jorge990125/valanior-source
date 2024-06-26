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

#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "molten_core.h"

enum Spells
{
    SPELL_ARCANE_EXPLOSION = 19712,
    SPELL_SHAZZRAH_CURSE = 19713,
    SPELL_COUNTERSPELL = 19715,
    SPELL_SHAZZRAH_GATE_DUMMY = 23138, // Teleports to and attacks a random target. About every 45 seconds Shazzrah will blink to random target causing a wipe of the threat list (source: wowwwiki)
    SPELL_SHAZZRAH_GATE = 23139,

    SPELL_PERIODIC_ARCANE_EXPLOSION = 2000088,
    SPELL_ARCANIC_SLASH             = 2000082,
    SPELL_ARCANE_ZONE           = 2000083,
    SPELL_SOAKING_ARCANE_EXPLOSION  = 2000085,
    SPELL_SOAKING_ARCANE_ZONE_PERIODIC_TRIGGER = 2000086

};

enum Events
{
    EVENT_ARCANE_EXPLOSION              = 1,
    EVENT_SHAZZRAH_CURSE,
    EVENT_MAGIC_GROUNDING,
    EVENT_COUNTERSPELL,
    EVENT_SHAZZRAH_GATE,
    EVENT_ARCANIC_SLASH,
    EVENT_ZONE_ARCANE
};


enum Npc
{
    NPC_CLONE_OF_SHAZZRAH = 226226,
};

class boss_shazzrah : public CreatureScript
{
public:
    boss_shazzrah() : CreatureScript("boss_shazzrah") { }

    struct boss_shazzrahAI : public BossAI
    {
        boss_shazzrahAI(Creature* creature) : BossAI(creature, DATA_SHAZZRAH) {}

        void EnterCombat(Unit* /*target*/) override
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_ARCANE_EXPLOSION, urand(2000, 4000));
            events.ScheduleEvent(EVENT_SHAZZRAH_CURSE, urand(7000, 11000));
            events.ScheduleEvent(EVENT_COUNTERSPELL, urand(9000, 10000));
            events.ScheduleEvent(EVENT_SHAZZRAH_GATE, 30000);
            events.ScheduleEvent(EVENT_ARCANIC_SLASH, 5500);
            events.ScheduleEvent(EVENT_ZONE_ARCANE, 35000);
        }

        void ExecuteEvent(uint32 eventId) override
        {
            switch (eventId)
            {
                case EVENT_ARCANIC_SLASH:
                {
                    DoCastVictim(SPELL_ARCANIC_SLASH);
                    events.RepeatEvent(12500);
                    break;
                }
                case EVENT_ARCANE_EXPLOSION:
                {
                    DoCastVictim(SPELL_ARCANE_EXPLOSION);
                    events.RepeatEvent(urand(4000, 5000));
                    break;
                }
                case EVENT_SHAZZRAH_CURSE:
                {

                    std::list<Unit*> targets;
                    SelectTargetList(targets, 3, SelectTargetMethod::MinThreat, 100.0f, true);
                    if (!targets.empty())
                        for (std::list<Unit*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                            if ((*itr) != me->GetVictim()) {
                                me->CastSpell(*itr, SPELL_SHAZZRAH_CURSE, true);
                            }

                    events.RepeatEvent(urand(23000, 26000));
                    break;
                }
                case EVENT_SHAZZRAH_GATE:
                {
                    TempSummon* summon = me->SummonCreature(NPC_CLONE_OF_SHAZZRAH, me->GetPosition(), TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 90000);

                    if (summon) {
                        summon->StopMoving();
                        summon->SetUnitFlag(UNIT_FLAG_DISABLE_MOVE);
                        summon->SetUnitFlag(UNIT_FLAG_NON_ATTACKABLE);
                        summon->AddAura(SPELL_PERIODIC_ARCANE_EXPLOSION, summon);
                    }

                    DoCastAOE(SPELL_SHAZZRAH_GATE_DUMMY);
                    events.RescheduleEvent(EVENT_ARCANE_EXPLOSION, urand(3000, 6000));
                    events.RepeatEvent(45000);
                    break;
                }   
                case EVENT_ZONE_ARCANE:
                {
                    std::list<Unit*> targets;
                    SelectTargetList(targets, 3, SelectTargetMethod::Random);

                    for (std::list<Unit*>::iterator itr = targets.begin(); itr != targets.end(); ++itr)
                    {
                        Unit* unit = (*itr);

                        if (!unit)
                            continue;

                        Position position = unit->GetNearPosition(urand(8, 15), 1);
                        TempSummon* summon = me->SummonCreature(WORLD_TRIGGER, position, TEMPSUMMON_TIMED_DESPAWN, 10000);
                        if (summon) {
                            summon->SetFaction(FACTION_MONSTER);
                            summon->CastSpell(summon, SPELL_ARCANE_ZONE);
                            summon->AddAura(SPELL_SOAKING_ARCANE_ZONE_PERIODIC_TRIGGER, summon);
                        }
                    }

                    events.RepeatEvent(35000);
                    break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return GetMoltenCoreAI<boss_shazzrahAI>(creature);
    }
};

// 23138 - Gate of Shazzrah
class spell_shazzrah_gate_dummy : public SpellScriptLoader
{
public:
    spell_shazzrah_gate_dummy() : SpellScriptLoader("spell_shazzrah_gate_dummy") {}

    class spell_shazzrah_gate_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_shazzrah_gate_dummy_SpellScript);

        bool Validate(SpellInfo const* /*spellInfo*/) override
        {
            return ValidateSpellInfo({ SPELL_SHAZZRAH_GATE });
        }

        void FilterTargets(std::list<WorldObject*>& targets)
        {
            Unit* caster = GetCaster();
            if (!targets.empty())
            {
                targets.remove_if([caster](WorldObject const* target) -> bool
                {
                    Player const* plrTarget = target->ToPlayer();
                    // Should not target non player targets
                    if (!plrTarget)
                    {
                        return true;
                    }

                    // Should skip current victim
                    if (caster->GetVictim() == plrTarget)
                    {
                        return true;
                    }

                    // Should not target enemies within melee range
                    if (plrTarget->IsWithinMeleeRange(caster))
                    {
                        return true;
                    }

                    return false;
                });
            }

            if (!targets.empty())
            {
                Acore::Containers::RandomResize(targets, 1);
            }
        }

        void HandleScript(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitUnit();

            if (caster && target)
            {
                target->CastSpell(caster, SPELL_SHAZZRAH_GATE, true);
                caster->CastSpell(nullptr, SPELL_ARCANE_EXPLOSION);

                if (Creature* creatureCaster = caster->ToCreature())
                {
                    creatureCaster->GetThreatMgr().ResetAllThreat();
                    creatureCaster->GetThreatMgr().AddThreat(target, 1);
                    creatureCaster->AI()->AttackStart(target); // Attack the target which caster will teleport to.
                }
            }
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_shazzrah_gate_dummy_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_shazzrah_gate_dummy_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new spell_shazzrah_gate_dummy_SpellScript();
    }
};

class spell_soaking_arcane_zone_finder : public SpellScript
{
    PrepareSpellScript(spell_soaking_arcane_zone_finder);

    void SelectTarget(std::list<WorldObject*>& targets)
    {
        Unit* caster = GetCaster();

        if (targets.empty())
        {
            caster->CastSpell(caster, SPELL_SOAKING_ARCANE_EXPLOSION);
        }

        // caster->ToTempSummon()->DespawnOrUnsummon();
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_soaking_arcane_zone_finder::SelectTarget, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};

void AddSC_boss_shazzrah()
{
    new boss_shazzrah();

    // Spells
    new spell_shazzrah_gate_dummy();
    RegisterSpellScript(spell_soaking_arcane_zone_finder);
}
