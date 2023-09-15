#include "AutobalanceManager.h"
#include "PlayerSpecialization.h"
#include "WorldSession.h"
#include "Chat.h"

std::map<uint8, AutobalanceScalingInfo> AutoBalanceManager::m_ScalingPerSpecialization = {};
std::map<uint32, AutobalanceScalingInfo> AutoBalanceManager::m_OverrideScalingPerCreatureId = {};
std::map<uint32, std::map<Difficulty, AutobalanceScalingInfo>> AutoBalanceManager::m_ScalingDungeonDifficulty = {};

std::list<Player*> AutoBalanceManager::GetPlayersMap(Map* map)
{
    std::list<Player*> players = {};

    Map::PlayerList const& playerList = map->GetPlayers();
    if (!playerList.IsEmpty())
        for (Map::PlayerList::const_iterator playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration)
            if (Player* playerHandle = playerIteration->GetSource())
                players.push_back(playerHandle);

    return players;
}

void AutoBalanceManager::SendMessageScalingInfo(Map* map)
{

    if (!map->IsDungeon() && !map->IsRaid())
        return;

    std::list<Player*> players = GetPlayersMap(map);

    if (players.size() == 0)
        return;

    std::string mapName = map->GetMapName();

    uint32 maxPlayers = map->IsRaid() && players.size() < 10 ? 10 : players.size();

    for (const auto& player : players)
    {
        ChatHandler(player->GetSession()).SendSysMessage(mapName + " is now scaling for " + std::to_string(maxPlayers) + " player(s).");
    }
}

Player* AutoBalanceManager::GetFirstPlayerMap(Map* map)
{
    std::list<Player*> players = GetPlayersMap(map);

    if (players.size() == 0)
        return nullptr;

    return players.front();
}

void AutoBalanceManager::InitializeScalingPerSpecialization()
{
    m_ScalingPerSpecialization = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM scalings_per_specialization");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint8 specialization = fields[0].Get<uint8>();
        float meleeDamage = fields[1].Get<float>();
        float healthModifier = fields[2].Get<float>();
        float spellDamageModifier = fields[3].Get<float>();
        float periodicDamageModifier = fields[4].Get<float>();
        AutobalanceScalingInfo info = { meleeDamage, healthModifier, spellDamageModifier, periodicDamageModifier };
        m_ScalingPerSpecialization[specialization] = info;
    } while (result->NextRow());
}

void AutoBalanceManager::InitializeScalingRaid()
{
    m_ScalingDungeonDifficulty = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM scaling_instance");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 mapId = fields[0].Get<uint32>();
        uint32 difficulty = fields[1].Get<uint32>();
        double healthModifier = fields[2].Get<double>();
        double damageModifier = fields[3].Get<double>();
        
        AutobalanceScalingInfo info = { damageModifier, healthModifier, 0, 0 };

        m_ScalingDungeonDifficulty[mapId].insert(std::make_pair(Difficulty(difficulty), info));
    } while (result->NextRow());
}

AutobalanceScalingInfo AutoBalanceManager::GetScalingInfo(Map* map, Unit* creature)
{
    if (!map->IsRaid() && !map->IsDungeon())
        return {};

    if (creature->isDead())
        return {};

    bool isRaid = map->IsRaid();
    Difficulty difficulty = map->GetDifficulty();
    uint8 playerCount = map->GetPlayers().getSize();

    if (playerCount <= 1 && !isRaid) {

        auto mapPlayer = map->GetPlayers().getFirst();
        Player* player = mapPlayer->GetSource();

        if (!player)
            return {};

        uint8 CurrentSpecialization = PlayerSpecialization::GetCurrentSpecId(player);

        if (CurrentSpecialization <= 0)
            return { 0.5, 0.3, 0, 0 };

        auto match = m_ScalingPerSpecialization.find(CurrentSpecialization);

        if (match != m_ScalingPerSpecialization.end())
            return match->second;

        return {};
    }

    uint32 creatureId = creature->GetEntry();

    auto match = m_OverrideScalingPerCreatureId.find(creatureId);

    if (match != m_OverrideScalingPerCreatureId.end())
        return match->second;

    if (playerCount > 1) {
        auto match = m_ScalingDungeonDifficulty.find(map->GetId());
        if (match != m_ScalingDungeonDifficulty.end()) {
            auto found = match->second.find(map->GetDifficulty());
            if (found != match->second.end())
                return found->second;
        }
    }

    return {};
}

void AutoBalanceManager::ApplyScalingHealthAndMana(Map* map, Creature* creature)
{
    if (!creature || !creature->GetMap())
        return;

    if (!creature->GetMap()->IsDungeon() && !creature->GetMap()->IsBattleground())
        return;

    if (((creature->IsHunterPet() || creature->IsPet() || creature->IsSummon()) && creature->IsControlledByPlayer()))
        return;

    if (!creature->IsAlive())
        return;

    uint8 playerCount = map->GetPlayers().getSize();
    bool isRaid = map->IsRaid();

    if (playerCount == 0)
        return;

    bool shouldRecalculate = (playerCount != creature->AutobalancePlayerCount) || creature->ShouldRecalculate == true;

    if (!shouldRecalculate)
        return;

    creature->AutobalancePlayerCount = playerCount;

    AutobalanceScalingInfo scaling = GetScalingInfo(map, creature);

    if (!scaling.healthModifier)
        return;

    CreatureTemplate const* creatureTemplate = creature->GetCreatureTemplate();

    if (!creatureTemplate)
        return;

    if (creature->prevMaxHealth <= 0)
        creature->prevMaxHealth = creature->GetMaxHealth();

    uint32 scaledHealth = 0;
    creature->ShouldRecalculate = false;

    if (playerCount == 1)
        scaledHealth = creature->prevMaxHealth * scaling.healthModifier;
    else {
        if (map->IsRaid())
        {
            double totalReduction = CalculateHealthRaidScaling(playerCount, scaling.healthModifier);
            scaledHealth = creature->prevMaxHealth - (totalReduction * creature->prevMaxHealth);
        }
        else
        {
            double totalReduction = CalculateHealthDungeonScaling(playerCount, scaling.healthModifier);
            scaledHealth = creature->prevMaxHealth - (totalReduction * creature->prevMaxHealth);
        }
    }

    creature->SetMaxHealth(scaledHealth);
    creature->SetCreateHealth(scaledHealth);
    creature->SetModifierValue(UNIT_MOD_HEALTH, BASE_VALUE, (float)scaledHealth);
    creature->ResetPlayerDamageReq();
    creature->SetHealth(scaledHealth);
}

float AutoBalanceManager::CalculateHealthRaidScaling(uint8 totalPlayerInRaid, double healthScaling)
{
    if (totalPlayerInRaid <= 10)
        return healthScaling / 3;
    if (totalPlayerInRaid > 10 && totalPlayerInRaid <= 15)
        return healthScaling / 2;
    if (totalPlayerInRaid > 15 && totalPlayerInRaid <= 20)
        return healthScaling;


    return 1.0f;
}

float AutoBalanceManager::CalculateHealthDungeonScaling(uint8 totalPlayerInDungeon, double healthScaling)
{
    if (totalPlayerInDungeon == 4)
        return healthScaling;
    if (totalPlayerInDungeon == 3)
        return healthScaling / 1.5;
    if (totalPlayerInDungeon == 2)
        return healthScaling / 1.8;

    // If we are more than 5 just return the normal amount;
    return 1.0f;
}
