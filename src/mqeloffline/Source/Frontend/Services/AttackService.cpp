/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 15-01-2018
    License: MIT
    Notes:
        Provides implementations for attack services.
*/

#pragma once
#include "../../Stdinclude.hpp"

uint64_t CurrentattackID;
constexpr uint64_t TutorialID1 = 12345;
constexpr uint64_t TutorialID2 = 54321;

// Helper.
uint32_t GetCastlelevel()
{
    return 1;
}

MQEL_json GetCastle(uint64_t AccountID, eCastletype Type)
{
    MQEL_json Result;

    // Get the castle-type.
    if (Type == eCastletype::Ubisoft)
        Result["$type"] =
            "HyperQuest.GameServer.Contracts.UbisoftCastle, HyperQuest.GameServer.Contracts";

    if (Type == eCastletype::User)
        Result["$type"] =
            "HyperQuest.GameServer.Contracts.UserCastle, HyperQuest.GameServer.Contracts";

    // Check tutorial status.
    if (AccountID <= 3)
        Result["IsTutorialCastle"] = true;

    std::string Castle =
        Backend::PvPCastle::Getcastle(AccountID).dump(4);

    std::string Insert =
        Result.dump(4);

    // Trim "{}".
    Castle = Castle.substr(1, Castle.size() - 2);
    Insert = Insert.substr(1, Insert.size() - 2);

    std::string Hackery;

    if (Castle.find_first_not_of(" \t\r\n") == std::string::npos)
        Hackery = "{" + Insert + "}";
    else
        Hackery = "{" + Insert + ", " + Castle + "}";

    return MQEL_json::parse(Hackery.c_str());
}

MQEL_json GetTutorialcastle(eAttacktype Type)
{
    (void)Type;

    // First (wilderness) tutorial castle. This is the hardcoded castle
    // used by the original MQELOffline build because the local castle
    // package is intentionally empty.
    return MQEL_json::parse(R"JSON({"$type":"HyperQuest.GameServer.Contracts.UbisoftCastle, HyperQuest.GameServer.Contracts","IsTutorialCastle":true,"ForceCastleLevelOnBuildables":true,"AccountId":2,"OasisNameId":2325,"LayoutId":1,"Rooms":[{"Triggers":[{"SizeX":18,"SizeY":6,"RoomZoneId":1,"Y":18,"Orientation":1,"Id":1,"SpecContainerId":52}],"X":4,"Y":8,"Orientation":3,"Id":11,"SpecContainerId":7105},{"Creatures":[{"AggroPropagationOffsetX":2.539598,"AggroPropagationOffsetZ":-0.9580116,"RoomZoneId":1,"X":9,"Y":11,"Orientation":3,"Id":45,"SpecContainerId":1081},{"AggroPropagationOffsetX":-0.1254749,"AggroPropagationOffsetZ":-0.4284554,"RoomZoneId":1,"X":14,"Y":21,"Orientation":2,"Id":46,"SpecContainerId":1081},{"AggroPropagationOffsetX":-0.588815,"AggroPropagationOffsetZ":0.1549107,"RoomZoneId":1,"X":9,"Y":28,"Orientation":1,"Id":47,"SpecContainerId":1081},{"AggroPropagationOffsetX":1.657236,"AggroPropagationOffsetZ":-1.133854,"RoomZoneId":1,"X":22,"Y":7,"Orientation":2,"Id":48,"SpecContainerId":1081},{"AggroPropagationOffsetX":-1.982243,"AggroPropagationOffsetZ":-0.4284554,"RoomZoneId":1,"X":23,"Y":29,"Orientation":2,"Id":49,"SpecContainerId":1081},{"RoomZoneId":1,"X":26,"Y":18,"Orientation":2,"Id":50,"SpecContainerId":1081}],"Triggers":[{"SizeX":18,"SizeY":6,"RoomZoneId":1,"X":18,"Y":2,"Id":8,"SpecContainerId":52}],"Decorations":[{"RoomZoneId":1,"X":13,"Y":8,"Orientation":3,"Id":115,"SpecContainerId":224},{"RoomZoneId":1,"X":20,"Y":20,"Orientation":2,"Id":116,"SpecContainerId":224},{"RoomZoneId":1,"X":18,"Y":18,"Orientation":2,"Id":118,"SpecContainerId":224},{"RoomZoneId":1,"X":18,"Y":22,"Orientation":2,"Id":119,"SpecContainerId":224},{"RoomZoneId":1,"X":16,"Y":29,"Orientation":3,"Id":120,"SpecContainerId":224},{"RoomZoneId":1,"X":7,"Y":19,"Orientation":3,"Id":131,"SpecContainerId":223},{"RoomZoneId":1,"X":23,"Y":11,"Orientation":2,"Id":126,"SpecContainerId":224},{"RoomZoneId":1,"X":25,"Y":25,"Orientation":3,"Id":139,"SpecContainerId":223},{"RoomZoneId":1,"X":15,"Y":27,"RoomZoneId":1,"Id":166,"SpecContainerId":236},{"RoomZoneId":1,"X":30,"Y":11,"Orientation":2,"Id":167,"SpecContainerId":236},{"RoomZoneId":1,"X":33,"Y":22,"Orientation":2,"Id":169,"SpecContainerId":236},{"RoomZoneId":1,"X":6,"Y":17,"Orientation":3,"Id":170,"SpecContainerId":235},{"RoomZoneId":1,"X":29,"Y":5,"Orientation":3,"Id":171,"SpecContainerId":235},{"RoomZoneId":1,"X":22,"Y":33,"Orientation":3,"Id":172,"SpecContainerId":236}],"X":4,"Y":7,"Id":26,"SpecContainerId":5025},{"Traps":[{"RoomZoneId":4,"X":17,"Y":33,"Id":1,"SpecContainerId":79},{"RoomZoneId":4,"X":18,"Y":33,"Id":2,"SpecContainerId":79}],"Triggers":[{"SizeX":14,"SizeY":7,"RoomZoneId":1,"X":30,"Y":18,"Orientation":1,"Id":2,"SpecContainerId":52},{"SizeX":2,"SizeY":2,"RoomZoneId":1,"X":24,"Y":18,"Orientation":1,"Id":3,"SpecContainerId":52},{"SizeX":2,"SizeY":2,"RoomZoneId":1,"X":6,"Y":25,"Orientation":1,"Id":4,"SpecContainerId":52},{"SizeX":12,"SizeY":2,"RoomZoneId":1,"X":14,"Y":18,"Orientation":1,"Id":5,"SpecContainerId":52}],"Decorations":[{"RoomZoneId":4,"X":13,"Y":34,"Id":174,"SpecContainerId":236},{"RoomZoneId":4,"X":21,"Y":34,"Id":175,"SpecContainerId":235}],"Buildings":[{"Rank":1,"RoomZoneId":1,"Id":3,"SpecContainerId":1}],"X":4,"Y":3,"Orientation":3,"Id":23,"SpecContainerId":7104},{"Creatures":[{"RoomZoneId":1,"X":21,"Y":18,"Orientation":2,"Id":68,"SpecContainerId":1081},{"RoomZoneId":1,"X":18,"Y":34,"SpecContainerId":1,"Id":58,"SpecContainerId":1029},{"RoomZoneId":1,"X":16,"Y":17,"Id":65,"SpecContainerId":1081}],"Decorations":[{"RoomZoneId":1,"X":20,"Y":13,"Id":110,"SpecContainerId":223},{"RoomZoneId":1,"X":19,"Y":3,"Orientation":3,"Id":111,"SpecContainerId":223},{"RoomZoneId":1,"X":16,"Y":4,"Orientation":3,"Id":112,"SpecContainerId":223},{"RoomZoneId":1,"X":14,"Y":34,"Orientation":2,"Id":114,"SpecContainerId":224},{"RoomZoneId":1,"X":21,"Y":29,"Id":140,"SpecContainerId":224},{"RoomZoneId":1,"X":13,"Y":22,"Id":141,"SpecContainerId":224},{"RoomZoneId":1,"X":22,"Y":1,"Orientation":3,"Id":143,"SpecContainerId":224},{"RoomZoneId":1,"X":13,"Y":7,"Orientation":1,"Id":142,"SpecContainerId":224},{"RoomZoneId":1,"X":16,"Y":1,"Id":113,"SpecContainerId":223}],"X":4,"Y":6,"Orientation":3,"Id":29,"SpecContainerId":2006},{"Creatures":[{"RoomZoneId":1,"X":24,"Y":19,"Orientation":2,"Id":56,"SpecContainerId":1003},{"AggroPropagationOffsetX":-0.6984169,"AggroPropagationOffsetZ":-0.2310722,"RoomZoneId":1,"X":21,"Y":25,"Orientation":1,"Id":44,"SpecContainerId":1081},{"RoomZoneId":1,"X":20,"Y":13,"Orientation":1,"Id":69,"SpecContainerId":1081},{"RoomZoneId":1,"X":21,"Y":1,"Orientation":1,"Id":81,"SpecContainerId":1001}],"Decorations":[{"RoomZoneId":1,"X":13,"Y":14,"Id":153,"SpecContainerId":155},{"RoomZoneId":1,"X":14,"Y":25,"Orientation":2,"Id":154,"SpecContainerId":155},{"RoomZoneId":1,"X":22,"Y":25,"Orientation":2,"Id":155,"SpecContainerId":155},{"RoomZoneId":1,"X":18,"Y":24,"Orientation":2,"Id":157,"SpecContainerId":154},{"RoomZoneId":1,"X":10,"Y":20,"Orientation":2,"Id":158,"SpecContainerId":154},{"RoomZoneId":1,"X":17,"Y":11,"Orientation":2,"Id":159,"SpecContainerId":154},{"RoomZoneId":1,"X":28,"Y":21,"Orientation":2,"Id":160,"SpecContainerId":154},{"RoomZoneId":1,"X":1,"Y":18,"Orientation":1,"Id":144,"SpecContainerId":224},{"RoomZoneId":1,"X":21,"Y":29,"Orientation":3,"Id":165,"SpecContainerId":238}],"X":4,"Y":4,"Id":36,"SpecContainerId":5020},{"Creatures":[{"RoomZoneId":1,"X":16,"Y":10,"Id":70,"SpecContainerId":1081},{"RoomZoneId":1,"X":26,"Y":16,"Orientation":2,"Id":59,"SpecContainerId":1029},{"RoomZoneId":1,"X":25,"Y":21,"Orientation":1,"Id":57,"SpecContainerId":1029},{"RoomZoneId":1,"X":18,"Y":28,"Orientation":2,"Id":66,"SpecContainerId":1029}],"Triggers":[{"SizeX":14,"SizeY":7,"RoomZoneId":1,"X":17,"Y":32,"Id":6,"SpecContainerId":52},{"SizeX":14,"SizeY":7,"RoomZoneId":1,"X":17,"Y":2,"Id":7,"SpecContainerId":52}],"Decorations":[{"RoomZoneId":1,"X":18,"Y":16,"Orientation":1,"Id":146,"SpecContainerId":224},{"RoomZoneId":1,"X":17,"Y":24,"Orientation":1,"Id":145,"SpecContainerId":224},{"RoomZoneId":1,"X":13,"Y":17,"Orientation":3,"Id":147,"SpecContainerId":223},{"RoomZoneId":1,"X":22,"Y":6,"Orientation":3,"Id":173,"SpecContainerId":236}],"X":4,"Y":5,"Id":39,"SpecContainerId":5020}],"CreatureTiers":[{"SpecContainerId":1081},{"SpecContainerId":1029},{"SpecContainerId":1003},{"SpecContainerId":1001}],"TrapTiers":[{"SpecContainerId":79}],"ThemeId":25})JSON");
}

MQEL_json GetCreatureloot()
{
    return MQEL_json::parse(
        R"([{"Id":45,"Gold":1,"Xp":2,"LifeForce":1,"HealthOrbFragments":1},{"Id":46,"Gold":9,"Xp":2,"LifeForce":1,"HealthOrbFragments":1},{"Id":47,"Gold":1,"Xp":2,"LifeForce":2,"HealthOrbFragments":1},{"Id":48,"Gold":1,"Xp":2,"HealthOrbFragments":1},{"Id":49,"Gold":1,"Xp":2,"LifeForce":1,"HealthOrbFragments":1},{"Id":50,"Gold":3,"Xp":2,"LifeForce":5,"HealthOrbFragments":1},{"Id":68,"Gold":2,"Xp":2,"HealthOrbFragments":1},{"Id":58,"Gold":2,"Xp":8,"LifeForce":4,"HealthOrbFragments":16},{"Id":65,"Gold":1,"Xp":2,"LifeForce":2,"HealthOrbFragments":1},{"Id":56,"Gold":1,"Xp":12,"LifeForce":1,"HealthOrbFragments":24,"InventoryItems":[{"$type":"HyperQuest.GameServer.Contracts.HeroEquipmentItem, HyperQuest.GameServer.Contracts","ItemLevel":1,"ArchetypeId":9,"PrimaryStatsModifiers":[0.354,0.998,0.021],"Effects":[{"Id":22,"Level":1}],"IsSellable":true,"TemplateId":37},{"$type":"HyperQuest.GameServer.Contracts.HeroEquipmentItem, HyperQuest.GameServer.Contracts","ItemLevel":1,"ArchetypeId":8,"PrimaryStatsModifiers":[0.375,0.364,0.821],"IsSellable":true,"TemplateId":40}]},{"Id":44,"Gold":1,"Xp":2,"LifeForce":5,"HealthOrbFragments":1},{"Id":69,"Gold":2,"Xp":2,"LifeForce":1,"HealthOrbFragments":1},{"Id":81,"Gold":1,"Xp":2,"LifeForce":1,"HealthOrbFragments":4},{"Id":70,"Gold":2,"Xp":2,"LifeForce":1,"HealthOrbFragments":1},{"Id":59,"Gold":3,"Xp":8,"LifeForce":2,"HealthOrbFragments":16},{"Id":57,"Gold":1,"Xp":8,"LifeForce":5,"HealthOrbFragments":16},{"Id":66,"Gold":2,"Xp":2,"LifeForce":1,"HealthOrbFragments":1}])");
}

void Fixclassloot(MQEL_json &Response)
{
    std::vector<std::pair<uint32_t, uint32_t>> Loot;

    switch (Backend::Hero::Getheroclass())
    {
    case (int)eHerotype::Knight:
        Loot = {
            {108, 2},
            {109, 8},
            {110, 8}
        };
        break;

    case (int)eHerotype::Archer:
        Loot = {
            {124, 2},
            {125, 8},
            {126, 8}
        };
        break;

    case (int)eHerotype::Mage:
        Loot = {
            {128, 9},
            {129, 8},
            {130, 8}
        };
        break;

    default:
        return;
    }

    uint32_t LootIndex = 0;

    if (Response["Notifications"].is_null())
        return;

    for (auto &Notification : Response["Notifications"])
    {
        if (Notification["$type"].is_null())
            continue;

        const std::string Type =
            Notification["$type"].get<std::string>();

        if (Type.find("InboxItemsAddedNotification") ==
            std::string::npos)
            continue;

        if (Notification["InboxItems"].is_null() ||
            !Notification["InboxItems"].is_array())
            continue;

        for (auto &InboxItem : Notification["InboxItems"])
        {
            if (InboxItem["$type"].is_null())
                continue;

            const std::string ItemType =
                InboxItem["$type"].get<std::string>();

            if (ItemType.find("InboxHeroEquipmentItem") ==
                std::string::npos)
                continue;

            if (LootIndex >= Loot.size())
                break;

            if (InboxItem["HeroItem"].is_null())
                continue;

            InboxItem["HeroItem"]["TemplateId"] =
                Loot[LootIndex].first;

            InboxItem["HeroItem"]["ArchetypeId"] =
                Loot[LootIndex].second;

            ++LootIndex;
        }
    }
}

void PersistAttackInbox(MQEL_json &Response)
{
    if (Response["Notifications"].is_null() ||
        !Response["Notifications"].is_array())
        return;

    for (auto &Notification : Response["Notifications"])
    {
        if (Notification.find("$type") == Notification.end() ||
            Notification["$type"].is_null())
            continue;

        const std::string Type =
            Notification["$type"].get<std::string>();

        if (Type.find("InboxItemsAddedNotification") ==
            std::string::npos)
            continue;

        if (Notification.find("InboxItems") != Notification.end() &&
            Notification["InboxItems"].is_array())
        {
            for (auto &InboxItem : Notification["InboxItems"])
            {
                if (InboxItem.find("ObjectId") == InboxItem.end() ||
                    InboxItem["ObjectId"].is_null())
                    continue;

                const std::string ObjectID =
                    InboxItem["ObjectId"].get<std::string>();

                Infoprint(
                    va(
                        "PersistAttackInbox: storing reward %s in persistent inbox.",
                        ObjectID.c_str()));

                Infoprint(
                    va(
                        "PersistAttackInbox: FOUND ITEM ObjectId=%s JSON=%s",
                        ObjectID.c_str(),
                        InboxItem.dump().c_str()));

                Backend::Inventory::Addinbox(InboxItem);
            }
        }

        if (Notification.find("ObjectId") != Notification.end() &&
            !Notification["ObjectId"].is_null() &&
            Notification.find("HeroItem") != Notification.end() &&
            !Notification["HeroItem"].is_null())
        {
            const std::string ObjectID =
                Notification["ObjectId"].get<std::string>();

            Infoprint(
                va(
                    "PersistAttackInbox: storing direct reward %s in persistent inbox.",
                    ObjectID.c_str()));

            Backend::Inventory::Addinbox(Notification);
        }
    }
}

MQEL_json GetTraploot()
{
    return MQEL_json::parse(
        R"([{"Id":1,"InventoryItems":[{"$type":"HyperQuest.GameServer.Contracts.HeroEquipmentItem, HyperQuest.GameServer.Contracts","ItemLevel":1,"ArchetypeId":2,"PrimaryStatsModifiers":[1,1,1],"IsSellable":true,"TemplateId":53}]},{"Id":2}])");
}

MQEL_json GetIGCreward()
{
    return MQEL_json::parse(
        R"({"CurrencyType": 2,"Amount": 10})");
}

MQEL_json GetLifereward()
{
    return MQEL_json::parse(
        R"({"CurrencyType": 4})");
}

MQEL_json GetMines()
{
    return MQEL_json::parse(
        R"([{"CastleBuildingId":9,"StealableAmount":67,"MaxStealableAmount":67}])");
}

MQEL_json GetSpells()
{
    return MQEL_json::parse(
        R"([{"SpellSpecContainerId":611,"Level":1},{"SpellSpecContainerId":608,"Level":1},{"SpellSpecContainerId":613,"Level":1}])");
}

MQEL_json GetAttackerconsumables()
{
    return MQEL_json::parse(
        R"([{"TemplateId":1002,"StackCount":2},{"TemplateId":1004,"StackCount":2},{"TemplateId":1011,"StackCount":1},{"TemplateId":1000,"StackCount":1}])");
}

MQEL_json GetDefenderconsumables()
{
    return MQEL_json::parse(
        R"([{"TemplateId":402,"ConsumableType":10}])");
}

// Endpoints.
void StartAttack(
    Gameserver *Server,
    std::string Request,
    std::string Body)
{
    auto Parsed = MQEL_json::parse(Body);

    Infoprint(
        va(
            "StartAttack Body: %s",
            Body.c_str()));

    eAttacksource Attacksource =
        (eAttacksource)Parsed["attackSource"].get<uint32_t>();

    eAttacktype Attacktype =
        (eAttacktype)Parsed["attackType"].get<uint32_t>();

    eCastletype Castletype =
        (eCastletype)Parsed["castleType"].get<uint32_t>();

    Infoprint(
        "StartAttack: checking completed quests.");

    auto CompletedQuests =
        Backend::Quest::Getcompleted();

    std::string QuestList;

    for (auto QuestID : CompletedQuests)
    {
        if (!QuestList.empty())
            QuestList += ", ";

        QuestList += std::to_string(QuestID);
    }

    Infoprint(
        va(
            "StartAttack: completed quests = [%s]",
            QuestList.c_str()));

    uint64_t CastleAccountId = 0;

    if (!Parsed["castleAccountId"].is_null())
        CastleAccountId =
            Parsed["castleAccountId"].get<uint64_t>();

    bool Tutorial =
        (CastleAccountId == Backend::PvPCastle::TutorialID1 ||
         CastleAccountId == Backend::PvPCastle::TutorialID2);

    Infoprint(
        va(
            "%s a %s-castle from %s",
            [Attacktype]() -> const char *
            {
                switch (Attacktype)
                {
                case eAttacktype::Competition:
                    return "Challenging";

                case eAttacktype::Progression:
                    return "Questing in";

                case eAttacktype::Revenge:
                    return "Taking revenge on";

                case eAttacktype::Validation:
                    return "Validating";

                case eAttacktype::Visit:
                    return "Visiting";

                case eAttacktype::None:
                    return "Querying";
                }

                return "";
            }(),
            [Castletype]() -> const char *
            {
                switch (Castletype)
                {
                case eCastletype::Ubisoft:
                    return "bot";

                case eCastletype::User:
                    return "player";
                }

                return "";
            }(),
            [Attacksource]() -> const char *
            {
                switch (Attacksource)
                {
                case eAttacksource::Chat:
                    return "chat-link";

                case eAttacksource::Competition:
                    return "challenge";

                case eAttacksource::Friends:
                    return "friendslist";

                case eAttacksource::Guild:
                    return "guildpost";

                case eAttacksource::Machinelearning:
                    return "suggestion";

                case eAttacksource::MOTD:
                    return "welcome page";

                case eAttacksource::News:
                    return "notification";

                case eAttacksource::Quest:
                    return "questlog";

                case eAttacksource::Regular:
                    return "worldmap";
                }

                return "";
            }()));

    if (!Tutorial)
        CurrentattackID = time(NULL);

    if (Tutorial && Attacktype == eAttacktype::None)
        CurrentattackID = TutorialID1;

    if (Tutorial && Attacktype == eAttacktype::Progression)
        CurrentattackID = TutorialID2;

    auto Response =
        MQEL_json::object();

    Response["Result"]["AttackId"] =
        va("%x", CurrentattackID);

    if (Tutorial && Attacktype == eAttacktype::None)
    {
        Response["Result"]["Castle"] =
            GetTutorialcastle(Attacktype);
    }

    if (Tutorial && Attacktype == eAttacktype::Progression)
    {
        Response["Result"]["Castle"] =
            GetCastle(
                Parsed["castleAccountId"],
                Castletype);
    }

    if (!Tutorial)
    {
        Response["Result"]["Castle"] =
            GetCastle(
                Parsed["castleAccountId"],
                Castletype);
    }

    if (Tutorial)
        Response["Result"]["CastleType"] =
            Parsed["castleType"].get<uint32_t>();

    Response["Result"]["Level"] =
        Tutorial ? 1 : GetCastlelevel();

    Response["Result"]["Hero"] =
        Backend::Hero::Serialize();

    Response["Result"]["AttackerDisplayName"] =
        "CatCastleMiau";

    Response["Result"]["CreatureLoot"] =
        GetCreatureloot();

    Response["Result"]["TrapLoot"] =
        GetTraploot();

    if (!Tutorial)
        Response["Result"]["FirstResurrectionCost"] =
            500;

    Response["Result"]["AttackRandomSeed"] =
        CurrentattackID;

    if (!Tutorial)
        Response["Result"]["UnlockedSpells"] =
            GetSpells();

    Response["Result"]["UnlockedEmotes"] =
        MQEL_json::parse(
            R"([ 1, 2, 3 ])");

    Response["Result"]["AttackUserSettings"] =
        MQEL_json::object();

    Response["Result"]["VictoryConditionRewardRatios"] =
        MQEL_json::parse(
            R"([ 1, 0.75, 0.5 ])");

    Response["Result"]["FreeInventorySlotsCount"] =
        42;

    if (!Tutorial)
        Response["Result"]["InventoryConsumablesInfo"] =
            GetAttackerconsumables();

    if (!Tutorial)
        Response["Result"]["DefenderActiveConsumables"] =
            GetDefenderconsumables();

    Response["Result"]["TreasureRoomStealableIGC"] =
        GetIGCreward();

    Response["Result"]["TreasureRoomStealableLifeForce"] =
        GetLifereward();

    if (!Tutorial)
        Response["Result"]["StealableMines"] =
            GetMines();

    if (Tutorial)
        Response["Result"]["IsTutorial"] =
            true;

    Response["Result"]["TreasureRoomGoldRatio"] =
        0.3;

    Response["Result"]["TreasureRoomLifeForceRatio"] =
        0.3;

    Response["Result"]["AttackType"] =
        Tutorial ? 5 : Parsed["attackType"].get<uint32_t>();

    if (!Tutorial)
        Response["Result"]["CastleValidationDuration"] =
            44.23333;

    Response["Result"]["IsResurrectionAllowed"] =
        true;

    Sendreply(
        Server,
        Response.dump());
}

void ApplyCollectedAttackLoot(
    MQEL_json &Request,
    MQEL_json &Response)
{
    uint32_t Gold = 0;
    uint32_t LifeForce = 0;

    auto LootTable =
        GetCreatureloot();

    auto CalculateLoot =
        [&Request, &LootTable](
            const char *Field,
            const char *RewardField) -> uint32_t
    {
        uint32_t Total = 0;

        if (Request[Field].is_null() ||
            !Request[Field].is_array())
        {
            return 0;
        }

        for (auto &CreatureID : Request[Field])
        {
            if (!CreatureID.is_number_integer())
                continue;

            uint32_t ID =
                CreatureID.get<uint32_t>();

            for (auto &Loot : LootTable)
            {
                if (Loot["Id"].is_null())
                    continue;

                if (Loot["Id"].get<uint32_t>() != ID)
                    continue;

                if (!Loot[RewardField].is_null())
                {
                    Total +=
                        Loot[RewardField].get<uint32_t>();
                }

                break;
            }
        }

        return Total;
    };

    Gold =
        CalculateLoot(
            "LootedGoldCreatureIds",
            "Gold");

    LifeForce =
        CalculateLoot(
            "LootedLifeForceCreatureIds",
            "LifeForce");

    Infoprint(
        va(
            "EndAttack collected loot: Gold=%u LifeForce=%u",
            Gold,
            LifeForce));

    if (Gold > 0)
    {
        Backend::Wallet::Updateamount(
            eCurrencytype::IGC,
            (int32_t)Gold);
    }

    if (LifeForce > 0)
    {
        Backend::Wallet::Updateamount(
            eCurrencytype::Lifeforce,
            (int32_t)LifeForce);
    }

    Response["Result"]["CollectedGold"] =
        Gold;

    Response["Result"]["CollectedLifeForce"] =
        LifeForce;
}

void EndAttack(
    Gameserver *Server,
    std::string Request,
    std::string Body)
{
    (void)Request;

    Infoprint(
        "========== ENDATTACK ENTERED ==========");

    Infoprint(
        va(
            "EndAttack Body: %s",
            Body.c_str()));

    /*
        The standalone client is currently sending a Content-Length that
        includes trailing garbage after the actual JSON object. Extract
        the first complete root JSON object rather than using rfind('}'),
        because garbage bytes can themselves contain braces.
    */
    std::string CleanBody = Body;

    const size_t JsonStart =
        CleanBody.find('{');

    if (JsonStart == std::string::npos)
    {
        Infoprint(
            "EndAttack: request contains no JSON object.");

        return;
    }

    size_t Depth = 0;
    bool InString = false;
    bool Escaped = false;
    size_t JsonEnd = std::string::npos;

    for (size_t Index = JsonStart;
         Index < CleanBody.size();
         ++Index)
    {
        const char Character =
            CleanBody[Index];

        if (InString)
        {
            if (Escaped)
            {
                Escaped = false;
                continue;
            }

            if (Character == '\\')
            {
                Escaped = true;
                continue;
            }

            if (Character == '"')
                InString = false;

            continue;
        }

        if (Character == '"')
        {
            InString = true;
            continue;
        }

        if (Character == '{')
        {
            ++Depth;
            continue;
        }

        if (Character == '}')
        {
            if (Depth > 0)
                --Depth;

            if (Depth == 0)
            {
                JsonEnd = Index;
                break;
            }
        }
    }

    if (JsonEnd == std::string::npos)
    {
        Infoprint(
            "EndAttack: could not find the end of the JSON object.");

        return;
    }

    if (JsonStart > 0)
    {
        Infoprint(
            va(
                "EndAttack: removing %u leading byte(s) before JSON.",
                (unsigned)JsonStart));
    }

    if (JsonEnd + 1 < CleanBody.size())
    {
        Infoprint(
            va(
                "EndAttack: removing %u trailing byte(s) after JSON.",
                (unsigned)(CleanBody.size() - JsonEnd - 1)));
    }

    CleanBody =
        CleanBody.substr(
            JsonStart,
            JsonEnd - JsonStart + 1);

    Infoprint(
        va(
            "EndAttack: extracted JSON body length=%u.",
            (unsigned)CleanBody.size()));

    MQEL_json Parsed =
        MQEL_json::parse(CleanBody);

    MQEL_json Response;

    // Handle tutorial fights.
    if (CurrentattackID == TutorialID1)
    {
        Response = MQEL_json::parse(
            R"({"Notifications":[{"$type":"HyperQuest.GameServer.Contracts, HyperQuest.GameServer.Contracts","Amounts":[{"CurrencyType":2,"Amount":33},{"CurrencyType":4,"Amount":33}],"NotificationType":24},{"$type":"HyperQuest.GameServer.Contracts.HeroXpChangedNotification, HyperQuest.GameServer.Contracts","HeroSpecContainerId":4,"XpAdded":60,"TotalXp":60,"Level":1,"NotificationType":43},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":1,"ArchetypeId":9,"PrimaryStatsModifiers":[0.354,0.998,0.021],"Effects":[{"Id":22,"Level":1}],"IsSellable":true,"TemplateId":37},"ItemType":3,"ObjectId":"580d50cd5c98320a9cb0e719"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":1,"ArchetypeId":8,"PrimaryStatsModifiers":[0.375,0.364,0.821],"IsSellable":true,"TemplateId":40},"ItemType":3,"ObjectId":"580d50cd5c98320a9cb0e71a"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":1,"ArchetypeId":2,"PrimaryStatsModifiers":[1,1,1],"IsSellable":true,"TemplateId":53},"ItemType":3,"ObjectId":"580d50cd5c98320a9cb0e71b"}],"NotificationType":111}],"Result":{"AttackId":"580d504f5c98320c1881325a","DefenderCastleId":2,"DefenderCastleType":1,"DefenderOasisNameId":2325,"Duration":119100,"InitialGold":1000,"HeroLevel":1,"TotalXp":60,"TotalGold":33,"TotalLifeForce":33,"KillsXp":60,"KillsGold":33,"KillsLifeForce":33,"CastleRatingFreePrize":{"CurrencyType":2,"Amount":50},"IsTutorial":true,"IsCastleAttackable":true,"UpdatedAccountStats":{"TotalCreaturesKilled":16,"AttackTotalIGCWon":33,"TotalCastlesLooted":1,"CastlesDefeated":{"Medium":1},"KilledCreatures":{"1081":12,"1029":3,"1003":1},"CurrencyAccumulation":{"IGC":33,"LifeForce":33},"DefeatCastleStrike":1,"TotalItemsLooted":3},"TrophyScoreVariation":{},"VictoryConditionRewardRatios":[1,0.75,0.5],"VictoryConditionLevel":1,"MaxVictoryConditionLevel":1,"EnterTreasureRoom":true,"VictoryConditionType":1}})");
    }
    else if (CurrentattackID == TutorialID2)
    {
        Response = MQEL_json::parse(
            R"({"Notifications":[{"$type":"HyperQuest.GameServer.Contracts, HyperQuest.GameServer.Contracts","Amounts":[{"CurrencyType":2},{"CurrencyType":4,"Amount":29}],"NotificationType":24},{"$type":"HyperQuest.GameServer.Contracts.HeroXpChangedNotification, HyperQuest.GameServer.Contracts","HeroSpecContainerId":4,"XpAdded":66,"TotalXp":1868,"Level":4,"NotificationType":43},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1000},"ItemType":4,"ObjectId":"58043d945c98320eb4f50727"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":2,"ArchetypeId":4,"PrimaryStatsModifiers":[0.566,0.342,0.122],"Effects":[{"Id":22,"Level":1}],"IsSellable":true,"TemplateId":17},"ItemType":3,"ObjectId":"58043d945c98320eb4f50728"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":3,"ArchetypeId":8,"PrimaryStatsModifiers":[0.98,0.24,0.777],"IsSellable":true,"TemplateId":44},"ObjectId":"58043d945c98320eb4f50729","NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":4,"ArchetypeId":3,"PrimaryStatsModifiers":[0.938,0.568,0.491],"IsSellable":true,"TemplateId":205},"ItemType":3,"ObjectId":"58043d945c98320eb4f5072a"}],"NotificationType":111}],"Result":{"AttackId":"58043d5d5c98320c1881325a","DefenderCastleId":3000947,"DefenderAccountDisplayName":"jonsie111","Duration":49933,"InitialXp":1802,"InitialGold":2000,"HeroLevel":4,"TotalXp":66,"TotalGold":103,"TotalLifeForce":29,"TreasureRoomGold":7,"PillagedIGCMinesAmount":67,"PillagedIGCMineCount":1,"CastleRatingFreePrize":{"CurrencyType":2,"Amount":50},"IsCastleAttackable":true,"UpdatedAccountStats":{"TotalCreaturesKilled":202,"AttackTotalIGCWon":833,"TotalCastlesLooted":10,"CastlesDefeated":{"Medium":5,"Easy":4,"Hard":1},"KilledCreatures":{"1081":86,"1029":7,"1003":2,"1000":21,"1006":3,"1023":4,"1079":8,"1155":1,"1001":24,"1026":9,"1024":7,"1016":5,"1009":6,"1015":13,"1002":6,"1028":3,"1007":4,"1090":8,"1087":8},"DefeatCastleStrike":4,"TotalPotionsConsumed":5},"TrophyScoreVariation":{},"VictoryConditionLevel":3,"DefenderLossIGC":7,"EnterTreasureRoom":true,"CastleShieldedByAttacker":true,"CastleValidationDuration":44233,"LastComment":{"AttackId":"580238555c98321778c363ba","AccountId":3140910,"Comment":{"Raw":"for beginners..."},"AvatarId":10}})");
    }
    else
    {
        Response = MQEL_json::parse(
            R"({"Notifications":[{"$type":"HyperQuest.GameServer.Contracts, HyperQuest.GameServer.Contracts","Amounts":[{"CurrencyType":2},{"CurrencyType":4,"Amount":165}],"NotificationType":24},{"$type":"HyperQuest.GameServer.Contracts.HeroXpChangedNotification, HyperQuest.GameServer.Contracts","HeroSpecContainerId":4,"XpAdded":333,"TotalXp":2201,"Level":4,"NotificationType":43},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":3,"ArchetypeId":10,"PrimaryStatsModifiers":[0.979,0.193,0.183],"IsSellable":true,"TemplateId":318},"ItemType":3,"ObjectId":"58043e625c98320eb4f507ba"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":3,"ArchetypeId":4,"PrimaryStatsModifiers":[0.381,0.771,0.188],"IsSellable":true,"TemplateId":45},"ItemType":3,"ObjectId":"58043e625c98320eb4f507bb"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.ObjectiveCompletedNotification, HyperQuest.GameServer.Contracts","ObjectiveId":302,"NotificationType":14},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1003},"ItemType":4,"ObjectId":"58043e625c98320eb4f507bc"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1003},"ItemType":4,"ObjectId":"58043d945c98320eb4f507bd"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1001},"ItemType":4,"ObjectId":"58043d945c98320eb4f507be"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1001},"ItemType":4,"ObjectId":"58043d945c98320eb4f507bf"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1000},"ItemType":4,"ObjectId":"58043d945c98320eb4f507c0"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.ObjectiveUnlockedNotification, HyperQuest.GameServer.Contracts","AccountObjective":{"ObjectiveId":303,"LastStatusDate":"2016-10-17T02:58:42Z","Status":1},"NotificationType":17}],"Result":{"AttackId":"58043dab5c98320eb4f50743","DefenderCastleId":101,"DefenderAccountDisplayName":"Fendrick's Farm","DefenderOasisNameId":16659,"Duration":172600,"InitialGold":2000,"HeroLevel":4,"TotalXp":333,"TotalGold":179,"TotalLifeForce":165,"KillsXp":333,"KillsGold":133,"KillsLifeForce":119,"TreasureRoomGold":46,"TreasureRoomLifeForce":46,"CastleRatingFreePrize":{"CurrencyType":2,"Amount":75},"IsCastleAttackable":true,"UpdatedAccountStats":{"TotalCreaturesKilled":243,"AttackTotalIGCWon":1012,"TotalCastlesLooted":11,"CastlesDefeated":{"Medium":5,"Easy":5,"Hard":1},"KilledCreatures":{"1081":103,"1029":7,"1003":5,"1000":24,"1006":3,"1079":8,"1155":1,"1001":26,"1026":9,"1024":7,"1016":5,"1009":6,"1015":13,"1002":6,"1028":3,"1007":4,"1090":8,"1087":8},"CurrencyAccumulation":{"IGC":1300,"LifeForce":1200},"DefeatCastleStrike":5,"TotalPotionsConsumed":5,"TotalItemsLooted":33},"TrophyScoreVariation":{},"VictoryConditionRewardRatios":[1,0.75,0.5],"VictoryConditionLevel":1,"MaxVictoryConditionLevel":1,"EnterTreasureRoom":true,"VictoryConditionType":1}})");
    }

    /*
        Calculate and persist the actual loot collected by the client.
    */
    ApplyCollectedAttackLoot(
        Parsed["endAttackParams"],
        Response);

    uint32_t CollectedGold = 0;
    uint32_t CollectedLifeForce = 0;

    if (!Response["Result"]["CollectedGold"].is_null())
    {
        CollectedGold =
            Response["Result"]["CollectedGold"].get<uint32_t>();
    }

    if (!Response["Result"]["CollectedLifeForce"].is_null())
    {
        CollectedLifeForce =
            Response["Result"]["CollectedLifeForce"].get<uint32_t>();
    }

    /*
        Keep the currency notification synchronized with the actual
        currency collected during this attack.
    */
    if (Response["Notifications"].is_array() &&
        !Response["Notifications"].empty())
    {
        auto &CurrencyNotification =
            Response["Notifications"][0];

        if (CurrencyNotification["Amounts"].is_array())
        {
            for (auto &Amount : CurrencyNotification["Amounts"])
            {
                if (Amount["CurrencyType"].is_null())
                    continue;

                const uint32_t CurrencyType =
                    Amount["CurrencyType"].get<uint32_t>();

                if (CurrencyType ==
                    (uint32_t)eCurrencytype::IGC)
                {
                    Amount["Amount"] =
                        CollectedGold;
                }
                else if (CurrencyType ==
                    (uint32_t)eCurrencytype::Lifeforce)
                {
                    Amount["Amount"] =
                        CollectedLifeForce;
                }
            }
        }
    }

    Fixclassloot(Response);

    Infoprint(
        va(
            "EndAttack: CurrentattackID=%llu",
            (unsigned long long)CurrentattackID));

    Infoprint(
        va(
            "EndAttack: Notifications=%u",
            Response["Notifications"].is_array()
                ? (unsigned)Response["Notifications"].size()
                : 0));

    Infoprint(
        va(
            "EndAttack: Response before persistence=%s",
            Response.dump().c_str()));

    PersistAttackInbox(Response);

    uint32_t XPReward = 0;

    if (CurrentattackID == TutorialID1)
        XPReward = 60;
    else if (CurrentattackID == TutorialID2)
        XPReward = 66;
    else
        XPReward = 333;

    auto HeroState =
        Backend::Hero::Serialize();

    uint32_t CurrentXP = 0;

    if (!HeroState["XP"].is_null())
    {
        CurrentXP =
            HeroState["XP"].get<uint32_t>();
    }

    Infoprint(
        va(
            "EndAttack XP: CurrentXP=%u XPReward=%u NewXP=%u",
            CurrentXP,
            XPReward,
            CurrentXP + XPReward));

    Backend::Hero::SetXP(
        CurrentXP + XPReward);

    Infoprint(
        "EndAttack XP: SetXP returned.");

    auto UpdatedHero =
        Backend::Hero::Serialize();

    Response["Result"]["TotalXp"] =
        UpdatedHero["XP"];

    Response["Result"]["HeroLevel"] =
        UpdatedHero["Level"];

    if (Response["Notifications"].is_array())
    {
        for (auto &Notification :
             Response["Notifications"])
        {
            if (Notification["$type"].is_null())
                continue;

            const std::string Type =
                Notification["$type"].get<std::string>();

            if (Type.find("HeroXpChangedNotification") ==
                std::string::npos)
                continue;

            Notification["XpAdded"] =
                XPReward;

            Notification["TotalXp"] =
                UpdatedHero["XP"];

            Notification["Level"] =
                UpdatedHero["Level"];

            break;
        }
    }

    /*
        Keep the server-generated attack ID.
    */
    Response["Result"]["AttackId"] =
        va("%x", CurrentattackID);

    /*
        Keep all currency fields synchronized with the actual
        collected creature loot.
    */
    Response["Result"]["TotalGold"] =
        CollectedGold;

    Response["Result"]["TotalLifeForce"] =
        CollectedLifeForce;

    Response["Result"]["KillsGold"] =
        CollectedGold;

    Response["Result"]["KillsLifeForce"] =
        CollectedLifeForce;

    Infoprint(
        "========== ENDATTACK SENDING RESPONSE ==========");

    Infoprint(
        va(
            "EndAttack final XP=%u",
            UpdatedHero["XP"].is_null()
                ? 0
                : UpdatedHero["XP"].get<uint32_t>()));

    Infoprint(
        va(
            "EndAttack final collected loot: Gold=%u LifeForce=%u",
            CollectedGold,
            CollectedLifeForce));

    Infoprint(
        va(
            "EndAttack final response=%s",
            Response.dump().c_str()));

    Sendreply(
        Server,
        Response.dump());
}

void RateCastle(
    Gameserver *Server,
    std::string Request,
    std::string Body)
{
    (void)Request;
    (void)Body;

    Backend::Wallet::Updateamount(
        eCurrencytype::IGC,
        50);

    Sendreply(
        Server,
        Backend::Wallet::Serialize(
            eCurrencytype::IGC)
            .dump());
}

void Resurrect(
    Gameserver *Server,
    std::string Request,
    std::string Body)
{
    (void)Request;
    (void)Body;

    Response_t Response;

    Response.Set(
        "NextResurrectionCost",
        750);

    Response.Set(
        "ResurrectionCost",
        500);

    Response.Set(
        "ResurrectionCount",
        1);

    Backend::Wallet::Updateamount(
        eCurrencytype::IGC,
        -500);

    Sendreply(
        Server,
        Response.toString());
}

// Add the services to the gameserver on startup.
namespace
{
    struct Startup
    {
        Startup()
        {
            Mapservice(
                "/AttackService.hqs/StartAttack",
                StartAttack);

            Mapservice(
                "/AttackService.hqs/EndAttack",
                EndAttack);

            Mapservice(
                "/AttackService.hqs/RateCastle",
                RateCastle);

            Mapservice(
                "/AttackService.hqs/Resurrect",
                Resurrect);
        }
    };

    static Startup Loader{};
}