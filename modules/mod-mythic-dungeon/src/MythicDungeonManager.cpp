#include "MythicDungeonManager.h"
#include "Group.h"
#include "Config.h"
#include "ElunaIncludes.h"
#include "LuaEngine.h"
#include "boost/iterator/counting_iterator.hpp"

std::map<uint32, MythicDungeon> MythicDungeonManager::m_MythicDungeon = {};
std::map<uint32, std::vector<MythicRewardDungeon>> MythicDungeonManager::m_MythicRewardDungeon = {};
std::map<uint32, std::vector<DungeonBoss>> MythicDungeonManager::m_MythicDungeonBosses = {};
std::map<uint64, MythicKey> MythicDungeonManager::m_PlayerKey = {};
std::vector<Affixe> MythicDungeonManager::m_WeeklyAffixes = {};
std::map<uint32, MythicRun> MythicDungeonManager::m_MythicRun = {};

std::map<uint32, std::map<MythicTypeData, std::vector<MythicPlayerDataCompletion>>>
MythicDungeonManager::m_MythicDungeonPlayerDataCompletion = {};

Config MythicDungeonManager::m_Config = {};

void MythicDungeonManager::InitializeMythicKeys()
{
    m_PlayerKey = {};
    QueryResult result = CharacterDatabase.Query("SELECT * FROM character_mythic_key");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint64 guid = fields[0].Get<uint64>();
        uint32 mapId = fields[2].Get<uint32>();
        uint32 level = fields[3].Get<uint32>();
        bool enable = fields[4].Get<bool>();
        MythicKey dungeon = { mapId, level };
        m_PlayerKey[guid] = dungeon;
    } while (result->NextRow());
}

void MythicDungeonManager::InitializeWeeklyAffixes()
{
    m_WeeklyAffixes = {};
    QueryResult result = WorldDatabase.Query("SELECT * FROM dungeon_mythic_affixes");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 spellId = fields[0].Get<uint32>();
        uint32 level = fields[1].Get<uint32>();
        Affixe affixe = { spellId, level };
        m_WeeklyAffixes.push_back(affixe);
    } while (result->NextRow());
}

void MythicDungeonManager::InitializeConfig()
{
    std::vector<Config> configs = {};
    QueryResult result = WorldDatabase.Query("SELECT * FROM seasons ORDER by season");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        std::time_t startDate = time_t(fields[0].Get<uint32>());
        std::time_t endDate = time_t(fields[1].Get<uint32>());
        uint8 season = fields[2].Get<uint8>();
        Config config = { startDate, endDate, season };
        configs.push_back(config);
    } while (result->NextRow());

    if (configs.size() == 0)
        return;

    m_Config = configs.back();
}

void MythicDungeonManager::ApplyAffixesAndOtherUpgrade(Creature* creature, Map* map)
{
    auto it = m_MythicRun.find(map->GetInstanceId());

    if (it == m_MythicRun.end())
        return;

    if (it->second.done)
        return;

    uint32 level = it->second.level;

    int32 hp = level * 5;
    int32 damage = level * 10;
    bool requierement = IsCreatureNpc(creature) && !creature->HasAura(90000);

    if (requierement)
        creature->CastCustomSpell(creature, 90000, &damage, &hp, nullptr, true);

}

void MythicDungeonManager::HandleAffixes(Map* map)
{
   // depending of the level of the key we need to handle the affixes.
}

void MythicDungeonManager::Update(Map* map, uint32 diff)
{
    auto it = m_MythicRun.find(map->GetInstanceId());

    if (it == m_MythicRun.end())
        return;

    MythicRun* run = &it->second;

    if (run->done || run->chestDecrapeted)
        return;

    if (!run->started) {
        Map::PlayerList const& playerList = map->GetPlayers();

        if (playerList.IsEmpty())
            return;

        run->startTimer -= diff;

        if (run->startTimer <= 0) {
            run->startTimer = 0;
            run->started = true;
            for (auto playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration) {
                if (Player* player = playerIteration->GetSource()) {
                    player->ClearUnitState(UNIT_STATE_ROOT);
                    player->SetControlled(false, UNIT_STATE_ROOT);
                    sEluna->SendStartMythicDungeon(player);
                }
            }
        }
    }
    else {
        run->elapsedTime += diff;

        if (run->elapsedTime >= run->timeToComplete && !run->chestDecrapeted && !run->done) {
            run->chestDecrapeted = true;
            Map::PlayerList const& playerList = map->GetPlayers();

            if (!playerList.IsEmpty())
                for (auto playerIteration = playerList.begin(); playerIteration != playerList.end(); ++playerIteration)
                    if (Player* player = playerIteration->GetSource())
                        sEluna->SendMythicUpdateChestDecrapeted(player);
        }
    }
}

void MythicDungeonManager::InitializeMythicDungeons()
{
    m_MythicDungeon = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM dungeon_mythic WHERE enable = 1");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 mapId = fields[0].Get<uint32>();
        uint32 timeToComplete = fields[1].Get<uint32>();
        uint32 totalCreatureToKill = fields[2].Get<uint32>();
        bool enabled = fields[3].Get<bool>();
        MythicDungeon dungeon = { mapId, timeToComplete, totalCreatureToKill, enabled };
        m_MythicDungeon[mapId] = dungeon;
    } while (result->NextRow());
}

void MythicDungeonManager::InitializeRewardsDungeons()
{
    m_MythicRewardDungeon = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM dungeon_mythic_rewards");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 mapId = fields[0].Get<uint32>();
        uint32 itemId = fields[1].Get<uint32>();
        uint32 level = fields[2].Get<uint32>();
        MythicRewardDungeon reward = { mapId, itemId, level };
        m_MythicRewardDungeon[mapId].push_back(reward);
    } while (result->NextRow());
}

void MythicDungeonManager::InitializeMythicDungeonBosses()
{
    m_MythicDungeonBosses = {};

    QueryResult result = WorldDatabase.Query("SELECT * FROM dungeon_mythic_bosses ORDER by `order` ASC");

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 mapId = fields[0].Get<uint32>();
        uint32 order = fields[1].Get<uint32>();
        uint32 bossId = fields[2].Get<uint32>();
        DungeonBoss boss = { mapId, order, bossId };
        m_MythicDungeonBosses[mapId].push_back(boss);
    } while (result->NextRow());

}

void MythicDungeonManager::HandleChangeDungeonDifficulty(Player* _player, uint8 mode)
{
    Group* group = _player->GetGroup();
    if (group)
    {
        if (group->IsLeader(_player->GetGUID()))
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
            {
                Player* groupGuy = itr->GetSource();
                if (!groupGuy)
                    continue;

                if (!groupGuy->IsInWorld())
                {
                    _player->SendDungeonDifficulty(group != nullptr);
                    return;
                }

                if (groupGuy->GetGUID() == _player->GetGUID() ? groupGuy->GetMap()->IsDungeon() : groupGuy->GetMap()->IsNonRaidDungeon())
                {
                    _player->SendDungeonDifficulty(group != nullptr);
                    return;
                }
            }

            group->ResetInstances(INSTANCE_RESET_CHANGE_DIFFICULTY, false, _player);
            group->SetDungeonDifficulty(Difficulty(mode));
        }
    }
    else {
        _player->SetDungeonDifficulty(Difficulty(mode));
        _player->SendDungeonDifficulty(false, DUNGEON_DIFFICULTY_EPIC);
        Player::ResetInstances(_player->GetGUID(), INSTANCE_RESET_CHANGE_DIFFICULTY, false);
    }
}

std::vector<std::string> MythicDungeonManager::GetDungeonsEnabled(Player* player)
{
    std::vector<std::string> elements = {};

    for (auto const& dungeon : m_MythicDungeon)
    {
        if (dungeon.second.enable) {
            Map* map = sMapMgr->FindBaseMap(dungeon.second.mapId);
            std::string fmt =
                std::to_string(dungeon.second.mapId)
                + ";" + map->GetMapName()
                + ";" + std::to_string(dungeon.second.timeToComplete)
                + ";" + std::to_string(dungeon.second.totalEnemyForces);
            elements.push_back(fmt);
        }
    }
    return elements;
}

std::vector<std::string> MythicDungeonManager::GetWeeklyAffixes(Player* player)
{
    std::vector<std::string> elements = {};

    for (auto const& dungeon : m_WeeklyAffixes)
    {
        std::string fmt =
            std::to_string(dungeon.spellId)
            + ";" + std::to_string(dungeon.level);
        elements.push_back(fmt);
    }
    return elements;
}

void MythicDungeonManager::StartMythicDungeon(Player* player, uint32 keyId, uint32 level)
{
    Map* map = player->GetMap();

    if (!map)
        return;

    if (map->GetId() != keyId)
        return;

    if (player->GetDungeonDifficulty() != DUNGEON_DIFFICULTY_EPIC)
        return;

    Group* group = player->GetGroup();

    std::vector<MythicDungeonBoss> bosses = {};
    std::vector<DungeonBoss> DungeonBosses = m_MythicDungeonBosses[keyId];

    MythicDungeon dungeon = GetMythicDungeonByMapId(keyId);


    if (!dungeon.mapId)
        return;

    for (auto const& Mythic : DungeonBosses)
        bosses.push_back({ Mythic.bossId, true });

    uint32 totalDeath = 0;
    bool started = false;
    bool chestDecrapeted = false;
    bool done = false;
    uint32 elapsedTime = 0;
    float enemyForces = 0.0f;
    int counting = 10 * IN_MILLISECONDS;
    MythicRun run = { keyId, level, dungeon.timeToComplete, started, chestDecrapeted, done, elapsedTime, bosses, enemyForces, totalDeath, counting };

    m_MythicRun[map->GetInstanceId()] = run;

    if (group) {

        if (group->GetLeaderGUID() != player->GetGUID())
            return;

        if (group->GetMembersCount() <= 1)
            return;

        auto const& allyList = group->GetMemberSlots();

        for (auto const& target : allyList)
        {
            Player* member = ObjectAccessor::FindPlayer(target.guid);
            if (member) {
                sEluna->SendBeginMythicDungeon(member);
                member->ClearUnitState(UNIT_STATE_ROOT);
                member->SetControlled(true, UNIT_STATE_ROOT);
                AreaTriggerTeleport const* at = sObjectMgr->GetMapEntranceTrigger(map->GetId());
                if (at) {
                    member->TeleportTo(at->target_mapId, at->target_X, at->target_Y, at->target_Z, at->target_Orientation);
                }
            }
        }
    }
    else {
        player->ClearUnitState(UNIT_STATE_ROOT);
        player->SetControlled(true, UNIT_STATE_ROOT);
        sEluna->SendBeginMythicDungeon(player);
        AreaTriggerTeleport const* at = sObjectMgr->GetMapEntranceTrigger(map->GetId());
        if (at) {
            player->TeleportTo(at->target_mapId, at->target_X, at->target_Y, at->target_Z, at->target_Orientation);
        }
    }
}

void MythicDungeonManager::OnKillBoss(Player* player, Creature* killed)
{
    Map* map = player->GetMap();

    auto it = m_MythicRun.find(map->GetInstanceId());

    if (it == m_MythicRun.end())
        return;

    if (!it->second.started)
        return;

    if (it->second.done)
        return;

    if (!killed->IsDungeonBoss())
        return;

    uint8 index = 1;
    for (auto ij = it->second.bosses.begin(); ij != it->second.bosses.end(); ++ij) {
        if (ij->creatureId == killed->GetEntry()) {
            ij->alive = false;
            if (Group* group = player->GetGroup()) {
                auto const& allyList = group->GetMemberSlots();

                for (auto const& target : allyList)
                {
                    Player* member = ObjectAccessor::FindPlayer(target.guid);
                    if (member) {
                        sEluna->SendMythicUpdateBossKill(member, index);
                    }
                }
            }
            else
                sEluna->SendMythicUpdateBossKill(player, index);
        }
        index++;
    }
    MythicRun* run = &it->second;
    if (MeetTheConditionsToCompleteTheDungeon(run)) {
        CompleteMythicDungeon(run, player);
    }
}

void MythicDungeonManager::OnKillMinion(Player* player, Creature* killed)
{
    Map* map = player->GetMap();

    auto it = m_MythicRun.find(map->GetInstanceId());

    if (it == m_MythicRun.end())
        return;

    if (it->second.done)
        return;

    if (!it->second.started)
        return;

    if (it->second.enemyForces >= 100)
        return;

    if (killed->IsDungeonBoss())
        return;

    MythicDungeon dungeon = m_MythicDungeon[map->GetId()];

    // From now all elite give the same.
    int32 point = 1;

    if (killed->isElite())
        point = 2;

    uint32 totalEnemyForces = dungeon.totalEnemyForces;
    double value = (double)point / (double)totalEnemyForces * 100.0;

    if ((it->second.enemyForces + value) > 100)
        it->second.enemyForces = 100;
    else
        it->second.enemyForces += value;

    if (Group* group = player->GetGroup()) {
        auto const& allyList = group->GetMemberSlots();

        for (auto const& target : allyList)
        {
            Player* member = ObjectAccessor::FindPlayer(target.guid);
            if (member) {
                sEluna->SendMythicUpdateEnemyForces(member, it->second.enemyForces);
            }
        }
    }
    else
        sEluna->SendMythicUpdateEnemyForces(player, it->second.enemyForces);

    MythicRun* run = &it->second;
    if (MeetTheConditionsToCompleteTheDungeon(run)) {
        CompleteMythicDungeon(run, player);
    }
}

void MythicDungeonManager::OnPlayerKilledByCreature(Creature* killer, Player* killed)
{
    Map* map = killed->GetMap();

    auto it = m_MythicRun.find(map->GetInstanceId());

    if (it == m_MythicRun.end())
        return;

    if (!it->second.started)
        return;

    it->second.deaths += 1;
    int32 totalDeath = it->second.deaths;
    int32 timeToReduce = std::max(((5 * IN_MILLISECONDS) * totalDeath), (30 * IN_MILLISECONDS));
    it->second.elapsedTime += timeToReduce;

    if (Group* group = killed->GetGroup()) {
        auto const& allyList = group->GetMemberSlots();

        for (auto const& target : allyList)
        {
            Player* member = ObjectAccessor::FindPlayer(target.guid);
            if (member) {
                sEluna->SendMythicUpdateDeath(member, totalDeath);
                sEluna->SendMythicUpdateTimer(member, timeToReduce);
            }
        }
    }
    else {
        sEluna->SendMythicUpdateDeath(killed, totalDeath);
        sEluna->SendMythicUpdateTimer(killed, timeToReduce);
    }
}

uint32 MythicDungeonManager::GetHighestRunId()
{
    return uint32();
}

void MythicDungeonManager::SaveRun(MythicRun* run, Player* player, uint32 increaseAmountKey, uint32 runId)
{
    if (runId > 0) {
        CharacterDatabase.Execute("INSERT INTO character_mythic_completed (`id`, `guid`, `mapId`, `level`, `timer`, `createdAt`) VALUES({}, {}, {}, {}, {}, NOW())"
            , runId, player->GetGUID().GetCounter(), run->mapId, run->level, increaseAmountKey);
    }
    else {
        CharacterDatabase.Execute("INSERT INTO character_mythic_completed (`guid`, `mapId`, `level`, `timer`, `createdAt`) VALUES({}, {}, {}, {}, NOW())"
            , player->GetGUID().GetCounter(), run->mapId, run->level, increaseAmountKey);
    }
}

void MythicDungeonManager::UpdateOrCreateMythicKey(MythicRun* run, Player* player, uint32 increaseAmountKey)
{
    auto item = m_MythicDungeon.begin();
    std::advance(item, rand() % m_MythicDungeon.size());
    auto random_dungeon = item->second;
    uint32 mapId = random_dungeon.mapId;

    auto it = m_PlayerKey.find(player->GetGUID().GetCounter());

    if (it == m_PlayerKey.end())
        m_PlayerKey[player->GetGUID().GetCounter()] = { mapId, run->level };
    else
        it->second.level += increaseAmountKey;

    player->CreateMythicKey(mapId);
}

void MythicDungeonManager::CompleteMythicDungeon(MythicRun* run, Player* player)
{
    run->done = true;
    uint32 increasesAmount = 1;

    if (Group* group = player->GetGroup()) {
        auto const& allyList = group->GetMemberSlots();

        for (auto const& target : allyList)
        {
            Player* member = ObjectAccessor::FindPlayer(target.guid);
            if (member) {
                uint32 highestId = GetHighestRunId();
                SaveRun(run, member, increasesAmount, highestId);
                UpdateOrCreateMythicKey(run, member, increasesAmount);
                sEluna->SendCompletedMythicDungeon(member, run->elapsedTime);
            }
        }
    }
    else {
        SaveRun(run, player, increasesAmount);
        UpdateOrCreateMythicKey(run, player, increasesAmount);
        sEluna->SendCompletedMythicDungeon(player, run->elapsedTime);
    }
}

void MythicDungeonManager::OnPlayerRelease(Player* player)
{
    Map* map = player->GetMap();

    auto it = m_MythicRun.find(map->GetInstanceId());

    if (it == m_MythicRun.end())
        return;

    if (!it->second.started)
        return;

    AreaTriggerTeleport const* at = sObjectMgr->GetMapEntranceTrigger(map->GetId());

    player->ResurrectPlayer(50.f);

    if (at) {
        player->TeleportTo(at->target_mapId, at->target_X, at->target_Y, at->target_Z, at->target_Orientation);
    }
}

MythicDungeon MythicDungeonManager::GetMythicDungeonByMapId(uint32 mapId)
{
    auto it = m_MythicDungeon.find(mapId);

    if (it == m_MythicDungeon.end())
        return {};

    if (it->second.enable)
        return it->second;

    return {};
}


bool MythicDungeonManager::MeetTheConditionsToCompleteTheDungeon(MythicRun* run)
{
    bool allBossesAreDead = true;

    for (auto ij = run->bosses.begin(); ij != run->bosses.end(); ++ij)
        if (ij->alive)
            allBossesAreDead = false;

    return allBossesAreDead && run->enemyForces >= 100.0f;
}

bool MythicDungeonManager::IsCreatureNpc(Creature* creature)
{
    return creature->IsAlive() && !creature->IsPet() && !creature->IsControlledByPlayer();
}

std::vector<std::string> MythicDungeonManager::GetDataMythicRun(Player* player)
{
    auto it = m_MythicRun.find(player->GetMap()->GetInstanceId());

    if (it == m_MythicRun.end())
        return {};

    MythicRun run = it->second;

    std::vector<std::string> elements = {};
    std::string first =
        std::to_string(run.level)
        + ";" + std::to_string(run.enemyForces)
        + ";" + std::to_string(run.timeToComplete)
        + ";" + std::to_string(run.elapsedTime)
        + ";" + std::to_string(run.deaths)
        + ";" + player->GetMap()->GetMapName();

    elements.push_back(first);

    std::string bossStatus = "";
    for (auto const& boss : run.bosses) {
        CreatureTemplate const* creatureTemplate = sObjectMgr->GetCreatureTemplate(boss.creatureId);
        bossStatus += ";" + creatureTemplate->Name + "+" + std::to_string(boss.alive);
    }
    elements.push_back(bossStatus);
    return elements;
}

MythicKey MythicDungeonManager::GetCurrentMythicKey(Player* player)
{
    auto it = m_PlayerKey.find(player->GetGUID().GetCounter());

    if (it != m_PlayerKey.end())
        return it->second;

    return {};
}

uint32 MythicDungeonManager::GetMythicScore(Player* player)
{
    uint32 score = 0;
    return score;
}

void MythicDungeonManager::InitHighestCompletedDungeonThisWeek(Player* player)
{
    uint64 guid = player->GetGUID().GetCounter();
    m_MythicDungeonPlayerDataCompletion[guid][HIGHEST_WEEKLY_COMPLETION] = {};
    QueryResult result = CharacterDatabase.Query("SELECT guid, mapId, MAX(level), timer FROM character_mythic_completed WHERE createdAt >= DATE_ADD(DATE_SUB(CURDATE(), INTERVAL IF(WEEKDAY(CURDATE()) >= 2, WEEKDAY(CURDATE()) - 1, WEEKDAY(CURDATE()) + 6) DAY), INTERVAL 4 HOUR) AND createdAt < DATE_ADD(DATE_SUB(CURDATE(), INTERVAL IF(WEEKDAY(CURDATE()) >= 2, WEEKDAY(CURDATE()) - 1, WEEKDAY(CURDATE()) + 6) DAY), INTERVAL 7 DAY) AND guid = {} GROUP BY mapId, `level`", player->GetGUID().GetCounter());

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 guid = fields[0].Get<uint32>();
        uint32 mapId = fields[1].Get<uint32>();
        uint32 level = fields[2].Get<uint32>();
        uint8 timer = fields[3].Get<uint8>();
        MythicPlayerDataCompletion data = { guid, mapId, level, timer, HIGHEST_WEEKLY_COMPLETION };
        m_MythicDungeonPlayerDataCompletion[guid][HIGHEST_WEEKLY_COMPLETION].push_back(data);
    } while (result->NextRow());
}

void MythicDungeonManager::InitHighestCompletedDungeonThisSeason(Player* player)
{
    uint64 guid = player->GetGUID().GetCounter();
    m_MythicDungeonPlayerDataCompletion[guid][HIGHEST_SEASONAL_COMPLETION] = {};
    QueryResult result = CharacterDatabase.Query("SELECT guid, mapId, MAX(level), timer FROM character_mythic_completed WHERE createdAt BETWEEN {} and {} AND guid = {} GROUP BY mapId, `level`", m_Config.start, m_Config.end, player->GetGUID().GetCounter());

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 guid = fields[0].Get<uint32>();
        uint32 mapId = fields[1].Get<uint32>();
        uint32 level = fields[2].Get<uint32>();
        uint8 timer = fields[3].Get<uint8>();
        MythicPlayerDataCompletion data = { guid, mapId, level, timer, HIGHEST_SEASONAL_COMPLETION };
        m_MythicDungeonPlayerDataCompletion[guid][HIGHEST_SEASONAL_COMPLETION].push_back(data);
    } while (result->NextRow());
}


void MythicDungeonManager::InitHighestCompletedDungeonAllTime(Player* player)
{
    uint64 guid = player->GetGUID().GetCounter();
    m_MythicDungeonPlayerDataCompletion[guid][HIGHEST_ALL_TIME_COMPLETION] = {};
    QueryResult result = CharacterDatabase.Query("SELECT guid, mapId, MAX(level), timer FROM character_mythic_completed WHERE guid = {} GROUP BY mapId, `level`", player->GetGUID().GetCounter());

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 guid = fields[0].Get<uint32>();
        uint32 mapId = fields[1].Get<uint32>();
        uint32 level = fields[2].Get<uint32>();
        uint8 timer = fields[3].Get<uint8>();
        MythicPlayerDataCompletion data = { guid, mapId, level, timer, HIGHEST_ALL_TIME_COMPLETION };
        m_MythicDungeonPlayerDataCompletion[guid][HIGHEST_ALL_TIME_COMPLETION].push_back(data);
    } while (result->NextRow());
}
