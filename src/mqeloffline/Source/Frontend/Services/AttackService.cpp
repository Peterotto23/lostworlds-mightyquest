/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 15-01-2018
    License: MIT
    Notes:
        Provides implementations for attack services.
*/

#pragma once
#include "../../Stdinclude.hpp"
#include "../../Backend/PvECastleDatabase.hpp"

uint64_t CurrentattackID;
uint64_t CurrentattackCastleID = 0;
eCastletype CurrentattackCastleType = eCastletype::Ubisoft;
bool CurrentattackDreadKnightQuest = false;
MQEL_json CurrentattackEquippedConsumables = MQEL_json::array();
bool CurrentattackConsumablesProcessed = false;

// State for the attack currently in progress.
MQEL_json CurrentattackCreatureLoot = MQEL_json::array();
int32_t CurrentattackInitialGold = 0;
uint32_t CurrentattackInitialXP = 0;

constexpr uint64_t TutorialCastleID = 2;
constexpr uint64_t BewarewichCastleID = 3;
constexpr uint64_t DreadKnightCastleID = 21;

constexpr uint64_t TutorialAttackID1 = 12345;
constexpr uint64_t TutorialAttackID2 = 54321;

uint32_t GetCastlelevel(
    uint64_t AccountID)
{
    const auto *Castle =
        Backend::PvECastle::Find(
            AccountID);

    if (Castle != nullptr)
    {
        Infoprint(
            va(
                "StartAttack: resolved PVE castle AccountId=%llu Level=%u.",
                (unsigned long long)AccountID,
                Castle->Level));

        return Castle->Level;
    }

    Infoprint(
        va(
            "StartAttack: unable to resolve PVE castle AccountId=%llu.",
            (unsigned long long)AccountID));

    return 0;
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
    if (AccountID == TutorialCastleID ||
        AccountID == BewarewichCastleID)
    {
        Result["IsTutorialCastle"] = true;
    }

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

MQEL_json GetCreaturelootForCastle(const MQEL_json &Castle)
{
    uint64_t AccountId = 0;

    if (Castle["AccountId"].is_number())
        AccountId = Castle["AccountId"].get<uint64_t>();

    // Preserve the existing bespoke drops for the first tutorial castle.
    if (AccountId == TutorialCastleID)
        return GetCreatureloot();

    if (!Castle["Rooms"].is_array())
        return GetCreatureloot();

    MQEL_json LootTable = MQEL_json::array();

    // Build loot entries using the creature INSTANCE IDs in this castle.
    for (const auto &Room : Castle["Rooms"])
    {
        if (!Room["Creatures"].is_array())
            continue;

        for (const auto &Creature : Room["Creatures"])
        {
            if (!Creature["Id"].is_number())
                continue;

            const uint32_t Id =
                Creature["Id"].get<uint32_t>();

            // Do not insert an instance ID twice.
            bool AlreadyAdded = false;

            for (const auto &Existing : LootTable)
            {
                if (Existing["Id"].is_number() &&
                    Existing["Id"].get<uint32_t>() == Id)
                {
                    AlreadyAdded = true;
                    break;
                }
            }

            if (AlreadyAdded)
                continue;

            const uint32_t Spec =
                Creature["SpecContainerId"].is_number()
                    ? Creature["SpecContainerId"].get<uint32_t>()
                    : 0;

            // These construction-point values are based on the supplied
            // CreatureDefinitions.json data for the creature types below.
            uint32_t ConstructionPoints = 1;
            uint32_t Fragments = 1;

            switch (Spec)
            {
            case 1000: // DERP_GRUNT
            case 1001: // SNOTTER_GRUNT
                ConstructionPoints = 1;
                Fragments = 4;
                break;

            case 1002: // TENTAKILL_GRUNT
                ConstructionPoints = 2;
                Fragments = 8;
                break;

            case 1003: // CYCLOPS_CAPTAIN
                ConstructionPoints = 6;
                Fragments = 24;
                break;

            case 1006: // BONEPUPPETEER_CAPTAIN
            case 1079: // GOATMAN_CAPTAIN
                ConstructionPoints = 3;
                Fragments = 12;
                break;

            case 1023: // SMELLYWARRIOR_CAPTAIN
            case 1029: // BATTERINGRAM_CAPTAIN
                ConstructionPoints = 4;
                Fragments = 16;
                break;

            case 1068: // BONEPUPPET_SPAWN
                ConstructionPoints = 1;
                Fragments = 1;
                break;

            case 1081: // CHICKEN
                ConstructionPoints = 1;
                Fragments = 1;
                break;

            case 1085: // CHICKEN_ELITE
            case 1155: // LD_CHICKEN_ELITE_PVEONLY
                ConstructionPoints = 8;
                Fragments = 32;
                break;

            default:
                // Give otherwise unlisted creatures a basic positive
                // reward instead of silently omitting their drops.
                ConstructionPoints = 1;
                Fragments = 1;
                break;
            }

            // XP follows the baseline seen in the existing creature table:
            // 1 CP = 2 XP, 4 CP = 8 XP, 6 CP = 12 XP.
            const uint32_t Xp =
                ConstructionPoints * 2;

            // Conservative deterministic fallback values for offline PvE.
            // These restore usable drops but do not reproduce the original
            // server's full randomized reward formula.
            const uint32_t Gold =
                ConstructionPoints >= 8 ? 3 : ConstructionPoints >= 3 ? 2
                                                                      : 1;

            const uint32_t LifeForce =
                ConstructionPoints >= 8 ? 5 : ConstructionPoints >= 3 ? 2
                                                                      : 1;

            LootTable.push_back({{"Id", Id},
                                 {"Gold", Gold},
                                 {"Xp", Xp},
                                 {"LifeForce", LifeForce},
                                 {"HealthOrbFragments", Fragments}});
        }
    }

    return LootTable.empty()
               ? GetCreatureloot()
               : LootTable;
}

void Fixclassloot(MQEL_json &Response)
{
    /*
        Original MQEL drops were not strictly class locked.

        The old offline patch replaced every reward with
        class-specific templates, which caused Mage/Wizard runs
        to receive unusable or missing equipment.

        Keep server generated loot intact.
        Only repair missing TemplateId/ArchetypeId fields.
    */

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
            if (InboxItem["HeroItem"].is_null())
                continue;

            auto &Item = InboxItem["HeroItem"];

            if (Item["TemplateId"].is_null())
            {
                Item["TemplateId"] = 37;
            }

            if (Item["ArchetypeId"].is_null())
            {
                Item["ArchetypeId"] = 9;
            }

            if (Item["ItemLevel"].is_null())
            {
                Item["ItemLevel"] = 1;
            }

            if (Item["IsSellable"].is_null())
            {
                Item["IsSellable"] = true;
            }

            Infoprint(
                va(
                    "Loot fix: keeping drop TemplateId=%u ArchetypeId=%u",
                    Item["TemplateId"].get<uint32_t>(),
                    Item["ArchetypeId"].get<uint32_t>()));
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
                std::string ObjectID;

                if (!InboxItem["ObjectId"].is_null())
                {
                    ObjectID =
                        InboxItem["ObjectId"].get<std::string>();
                }
                else if (!Notification["ObjectId"].is_null())
                {
                    ObjectID =
                        Notification["ObjectId"].get<std::string>();
                    InboxItem["ObjectId"] = ObjectID;
                }
                else
                {
                    Infoprint(
                        "PersistAttackInbox: item missing ObjectId.");
                    continue;
                }

                Infoprint(
                    va(
                        "PersistAttackInbox: storing reward %s",
                        ObjectID.c_str()));

                Backend::Inventory::Addinbox(InboxItem);
            }
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
    /*
        Match the currently advertised castle-selection loot:
        50 IGC * 0.30 treasure-room ratio = 15 gold.
    */
    return MQEL_json::parse(
        R"({"CurrencyType": 2,"Amount": 15})");
}

MQEL_json GetLifereward()
{
    /*
        Life Force was missing Amount entirely, which explains
        why the client reported zero from the treasure room.
    */
    return MQEL_json::parse(
        R"({"CurrencyType": 4,"Amount": 15})");
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
    // Read the active hero's equipped consumable templates.
    const MQEL_json Hero =
        Backend::Hero::Serialize();

    // Read the actual persistent inventory.
    // Entries contain SlotIndex and StackCount around Item.
    const MQEL_json Inventory =
        Backend::Inventory::Serialize();

    MQEL_json Result = MQEL_json::array();

    const auto &Equipped =
        Hero["EquippedConsumables"];

    if (!Equipped.is_array())
    {
        Infoprint(
            "AttackService: no equipped consumables array found.");

        return Result;
    }

    // Build attack consumable counts from the saved inventory,
    // rather than returning hard-coded template IDs.
    for (const auto &EquippedItem : Equipped)
    {
        if (!EquippedItem.is_object() ||
            !EquippedItem["TemplateId"].is_number())
        {
            continue;
        }

        const uint32_t TemplateId =
            EquippedItem["TemplateId"].get<uint32_t>();

        if (TemplateId == 0)
            continue;

        // Only include each template once in the response.
        bool AlreadyAdded = false;

        for (const auto &Existing : Result)
        {
            if (Existing["TemplateId"].is_number() &&
                Existing["TemplateId"].get<uint32_t>() ==
                    TemplateId)
            {
                AlreadyAdded = true;
                break;
            }
        }

        if (AlreadyAdded)
            continue;

        uint32_t TotalStackCount = 0;

        // Count every saved inventory stack for this consumable.
        for (const auto &InventoryEntry : Inventory)
        {
            if (!InventoryEntry.is_object() ||
                !InventoryEntry["Item"].is_object())
            {
                continue;
            }

            const auto &Item =
                InventoryEntry["Item"];

            if (!Item["TemplateId"].is_number() ||
                Item["TemplateId"].get<uint32_t>() !=
                    TemplateId)
            {
                continue;
            }

            const std::string ItemType =
                Item["$type"].is_string()
                    ? Item["$type"].get<std::string>()
                    : "";

            // Do not count equipment with a coincidentally
            // matching TemplateId.
            if (ItemType.find("HeroConsumableItem") ==
                std::string::npos)
            {
                continue;
            }

            uint32_t StackCount = 1;

            if (Item["StackCount"].is_number())
            {
                StackCount =
                    Item["StackCount"].get<uint32_t>();
            }
            else if (InventoryEntry["StackCount"].is_number())
            {
                StackCount =
                    InventoryEntry["StackCount"].get<uint32_t>();
            }

            TotalStackCount += StackCount;
        }

        MQEL_json ConsumableInfo =
            MQEL_json::object();

        ConsumableInfo["TemplateId"] = TemplateId;
        ConsumableInfo["StackCount"] = TotalStackCount;

        Result.push_back(ConsumableInfo);
    }

    Infoprint(
        va(
            "AttackService: InventoryConsumablesInfo=%s",
            Result.dump().c_str()));

    return Result;
}

// Attach owned inventory counts to equipped consumables in the
// temporary Hero snapshot returned when an attack starts.
void AttachInventoryCountsToAttackHero(
    MQEL_json &Hero,
    const MQEL_json &InventoryConsumablesInfo)
{
    auto &Equipped = Hero["EquippedConsumables"];

    if (!Equipped.is_array() ||
        !InventoryConsumablesInfo.is_array())
    {
        return;
    }

    for (auto &EquippedItem : Equipped)
    {
        if (!EquippedItem.is_object() ||
            !EquippedItem["TemplateId"].is_number())
        {
            continue;
        }

        const uint32_t TemplateId =
            EquippedItem["TemplateId"].get<uint32_t>();

        for (const auto &InventoryItem : InventoryConsumablesInfo)
        {
            if (!InventoryItem.is_object() ||
                !InventoryItem["TemplateId"].is_number() ||
                InventoryItem["TemplateId"].get<uint32_t>() != TemplateId ||
                !InventoryItem["StackCount"].is_number())
            {
                continue;
            }

            const uint32_t InventoryStackCount =
                InventoryItem["StackCount"].get<uint32_t>();

            if (InventoryStackCount > 0)
            {
                // TemplateId 1 is the Minor Health Potion.
                // The captured game model reports 3 uses available per attack.
                uint32_t AttackStackCount = InventoryStackCount;

                if (TemplateId == 1 && AttackStackCount > 3)
                    AttackStackCount = 3;

                // Cap only the temporary combat entry.
                // InventoryConsumablesInfo still reports the actual owned stack.
                EquippedItem["StackCount"] = AttackStackCount;

                Infoprint(va(
                    "StartAttack: attack Hero consumable TemplateId=%u SlotIndex=%u InventoryStackCount=%u AttackStackCount=%u.",
                    TemplateId,
                    EquippedItem["SlotIndex"].is_number()
                        ? EquippedItem["SlotIndex"].get<uint32_t>()
                        : 0,
                    InventoryStackCount,
                    AttackStackCount));
            }

            break;
        }
    }
}

void ConsumeUsedAttackConsumables(
    const MQEL_json &EndAttackParams)
{
    if (CurrentattackConsumablesProcessed)
    {
        Infoprint(
            "EndAttack consumables: usage already processed for this attack.");
        return;
    }

    const auto UsedIt =
        EndAttackParams.find("ConsumableUsed");

    if (UsedIt == EndAttackParams.end() ||
        !UsedIt->is_array() ||
        UsedIt->empty())
    {
        Infoprint(
            "EndAttack consumables: no ConsumableUsed entries to deduct.");
        return;
    }

    uint32_t UseCount = 0;

    for (const auto &UsedEntry : *UsedIt)
    {
        if (UsedEntry.is_number_integer() &&
            UsedEntry.get<int64_t>() > 0)
        {
            ++UseCount;
        }
    }

    if (UseCount == 0)
    {
        Infoprint(
            "EndAttack consumables: no valid usage entries.");
        return;
    }

    std::vector<uint32_t> EquippedTemplateIds;

    if (CurrentattackEquippedConsumables.is_array())
    {
        for (const auto &EquippedItem :
             CurrentattackEquippedConsumables)
        {
            if (!EquippedItem.is_object() ||
                !EquippedItem["TemplateId"].is_number_integer())
            {
                continue;
            }

            const uint32_t TemplateId =
                EquippedItem["TemplateId"].get<uint32_t>();

            if (TemplateId == 0)
                continue;

            bool AlreadyAdded = false;

            for (const auto ExistingId : EquippedTemplateIds)
            {
                if (ExistingId == TemplateId)
                {
                    AlreadyAdded = true;
                    break;
                }
            }

            if (!AlreadyAdded)
                EquippedTemplateIds.push_back(TemplateId);
        }
    }

    if (EquippedTemplateIds.size() != 1)
    {
        Infoprint(va(
            "EndAttack consumables: refusing to guess which item to deduct; used=%u distinctEquippedTemplateIds=%u.",
            UseCount,
            (unsigned)EquippedTemplateIds.size()));
        return;
    }

    const uint32_t TemplateId =
        EquippedTemplateIds.front();

    CurrentattackConsumablesProcessed = true;

    const uint32_t Deducted =
        Backend::Inventory::Consumeconsumable(
            TemplateId,
            UseCount);

    Infoprint(va(
        "EndAttack consumables: TemplateId=%u reportedUses=%u deducted=%u.",
        TemplateId,
        UseCount,
        Deducted));
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

    CurrentattackInitialGold =
        Backend::Wallet::Getamount(eCurrencytype::IGC);

    const auto HeroAtStart =
        Backend::Hero::Serialize();

    CurrentattackInitialXP =
        HeroAtStart["XP"].is_number()
            ? HeroAtStart["XP"].get<uint32_t>()
            : 0;

    CurrentattackCreatureLoot = MQEL_json::array();

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

    uint64_t RequestedCastleAccountId = 0;

    if (!Parsed["castleAccountId"].is_null())
        RequestedCastleAccountId =
            Parsed["castleAccountId"].get<uint64_t>();

    uint64_t CastleAccountId =
        RequestedCastleAccountId;

    /*
        Assignment 120 / Knight of the Dread.

        The original client request uses Castle 2 as the quest/tutorial
        placeholder. The actual playable dungeon for this assignment is
        Castle 21 ("The Dead Legion" / "Enter the Archer").
    */
    const bool DreadKnightQuestAttack =
        Attacksource == eAttacksource::Quest &&
        RequestedCastleAccountId == TutorialCastleID &&
        Backend::Quest::IsStarted(120);

    CurrentattackDreadKnightQuest =
        DreadKnightQuestAttack;

    if (DreadKnightQuestAttack)
    {
        CastleAccountId =
            DreadKnightCastleID;

        Infoprint(
            "Knight of the Dread quest attack: routing Castle 2 request to Castle 21.");
    }

    /*
        EndAttack needs the ACTUAL dungeon that was played.
    */
    CurrentattackCastleID =
        CastleAccountId;

    CurrentattackCastleType =
        Castletype;

    Infoprint(
        va(
            "StartAttack: stored attack castle AccountId=%llu CastleType=%u.",
            (unsigned long long)
                CurrentattackCastleID,
            (unsigned)
                CurrentattackCastleType));

    /*
        Quest 120 must retain the tutorial/progression response
        contract even though its actual castle is Castle 21.
    */
    /*
     Preserve the two existing special cases.

     Ordinary PVE castles use a separate compatibility flag.
     They must not use fixed tutorial attack IDs or fixed
     tutorial reward responses.
 */
    const bool ActualTutorialCastle =
        RequestedCastleAccountId == TutorialCastleID;

    /*
        Ordinary local PVE castles use the Bewarewich-compatible
        StartAttack response, except for the Dreadknight quest castle.

        Keep the real PVE CastleEntry so we can report its actual level.
    */
    const Backend::PvECastle::CastleEntry *PVECompatibilityEntry =
        nullptr;

    if (!DreadKnightQuestAttack &&
        !ActualTutorialCastle)
    {
        PVECompatibilityEntry =
            Backend::PvECastle::Find(
                CastleAccountId);
    }

    const bool PVECompatibilityCastle =
        PVECompatibilityEntry != nullptr;

    const bool Tutorial =
        ActualTutorialCastle ||
        PVECompatibilityCastle ||
        DreadKnightQuestAttack;

    if (PVECompatibilityCastle)
    {
        Infoprint(
            va(
                "StartAttack: using Bewarewich-compatible PVE response for AccountId=%llu Level=%u.",
                (unsigned long long)CastleAccountId,
                PVECompatibilityEntry->Level));
    }

    Infoprint(
        va(
            "StartAttack: requested CastleAccountId=%llu resolved CastleAccountId=%llu Tutorial=%s DreadKnightQuest=%s.",
            (unsigned long long)
                RequestedCastleAccountId,
            (unsigned long long)
                CastleAccountId,
            Tutorial
                ? "true"
                : "false",
            DreadKnightQuestAttack
                ? "true"
                : "false"));

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

    /*
     Only the actual tutorial castles get the fixed tutorial
     attack IDs.

     Other progression dungeons get the tutorial/progression
     response shape, but retain a normal unique attack ID so
     EndAttack does not enter the hardcoded tutorial reward paths.
 */

    if (!DreadKnightQuestAttack &&
        ActualTutorialCastle &&
        Attacktype == eAttacktype::None)
    {
        CurrentattackID =
            TutorialAttackID1;
    }
    else if (!DreadKnightQuestAttack &&
             ActualTutorialCastle &&
             Attacktype == eAttacktype::Progression)
    {
        CurrentattackID =
            TutorialAttackID2;
    }
    else
    {
        CurrentattackID =
            time(NULL);
    }

    auto Response =
        MQEL_json::object();

    Response["Result"]["AttackId"] =
        va("%x", CurrentattackID);

    if (DreadKnightQuestAttack)
    {
        Response["Result"]["Castle"] =
            GetCastle(
                CastleAccountId,
                eCastletype::Ubisoft);
    }
    else if (PVECompatibilityCastle)
    {
        Response["Result"]["Castle"] =
            GetCastle(
                CastleAccountId,
                Castletype);

        /*
            Preserve the compatibility response and actual castle data,
            but do not activate the tutorial UI for a regular PVE castle.
        */
        Response["Result"]["Castle"]["IsTutorialCastle"] =
            false;
    }
    else if (Tutorial &&
             Attacktype == eAttacktype::None)
    {
        Response["Result"]["Castle"] =
            GetTutorialcastle(
                Attacktype);
    }
    else if (Tutorial &&
             Attacktype == eAttacktype::Progression)
    {
        Response["Result"]["Castle"] =
            GetCastle(
                RequestedCastleAccountId,
                Castletype);
    }
    else if (Attacktype == eAttacktype::Progression)
    {
        uint64_t DungeonID =
            Backend::Quest::GetCurrentDungeon(
                CastleAccountId);

        Infoprint(
            va(
                "Progression dungeon selected: %llu",
                (unsigned long long)DungeonID));

        Response["Result"]["Castle"] =
            GetCastle(
                DungeonID,
                eCastletype::Ubisoft);
    }
    else if (!Tutorial)
    {
        Response["Result"]["Castle"] =
            GetCastle(
                CastleAccountId,
                Castletype);
    }

    Infoprint(
        va(
            "StartAttack CastleAccount=%llu AttackType=%u Tutorial=%s",
            (unsigned long long)CastleAccountId,
            (unsigned)Attacktype,
            Tutorial ? "true" : "false"));

    if (Tutorial || DreadKnightQuestAttack)
        Response["Result"]["CastleType"] =
            Parsed["castleType"].get<uint32_t>();

    if (DreadKnightQuestAttack)
    {
        Response["Result"]["Level"] =
            5;
    }
    else if (PVECompatibilityCastle)
    {
        Response["Result"]["Level"] =
            PVECompatibilityEntry->Level;

        Infoprint(
            va(
                "StartAttack: compatibility Result.Level=%u for PVE AccountId=%llu.",
                PVECompatibilityEntry->Level,
                (unsigned long long)CastleAccountId));
    }
    else if (Tutorial)
    {
        Response["Result"]["Level"] =
            1;
    }
    else
    {
        const auto *PVECastle =
            Backend::PvECastle::Find(
                CastleAccountId);

        if (PVECastle != nullptr)
        {
            Response["Result"]["Level"] =
                PVECastle->Level;
        }
        else if (
            Response["Result"]["Castle"].find("Level") !=
                Response["Result"]["Castle"].end() &&
            Response["Result"]["Castle"]["Level"].is_number())
        {
            Response["Result"]["Level"] =
                Response["Result"]["Castle"]["Level"];
        }
        else
        {
            Response["Result"]["Level"] =
                GetCastlelevel(
                    CastleAccountId);
        }
    }

    Infoprint(
        va(
            "Castle payload size=%u",
            (unsigned)Response["Result"]["Castle"].dump().size()));
    // Build the raid-specific Hero snapshot and attach owned stack counts to
    // equipped consumables. Keep Backend::Hero::Serialize() unchanged because
    // the extra StackCount belongs to the transient combat model, not the save.
    const MQEL_json InventoryConsumablesInfo =
        GetAttackerconsumables();

    MQEL_json AttackHero =
        Backend::Hero::Serialize();

    AttachInventoryCountsToAttackHero(
        AttackHero,
        InventoryConsumablesInfo);

    CurrentattackEquippedConsumables =
        AttackHero["EquippedConsumables"].is_array()
            ? AttackHero["EquippedConsumables"]
            : MQEL_json::array();
    CurrentattackConsumablesProcessed = false;

    Response["Result"]["Hero"] =
        std::move(AttackHero);

    Response["Result"]["AttackerDisplayName"] =
        "CatCastleMiau";

    CurrentattackCreatureLoot =
        GetCreaturelootForCastle(Response["Result"]["Castle"]);

    Response["Result"]["CreatureLoot"] =
        CurrentattackCreatureLoot;

    Infoprint(
        va(
            "StartAttack: advertised %u creature loot entries for castle AccountId=%llu.",
            (unsigned)CurrentattackCreatureLoot.size(),
            (unsigned long long)CurrentattackCastleID));

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

    // Every attack type must receive the attacker's consumable inventory.
    // Tutorial and Dread Knight quest attacks also need this data so
    // the combat system can determine whether equipped potions are available.
    Response["Result"]["InventoryConsumablesInfo"] =
        InventoryConsumablesInfo;

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

    if (Tutorial &&
        !PVECompatibilityCastle &&
        !DreadKnightQuestAttack)
    {
        Response["Result"]["IsTutorial"] =
            true;
    }

    Response["Result"]["TreasureRoomGoldRatio"] =
        0.3;

    Response["Result"]["TreasureRoomLifeForceRatio"] =
        0.3;

    Response["Result"]["AttackType"] =
        (Tutorial || DreadKnightQuestAttack)
            ? 5
            : Parsed["attackType"].get<uint32_t>();

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
    const auto LootTable =
        CurrentattackCreatureLoot.is_array() &&
                !CurrentattackCreatureLoot.empty()
            ? CurrentattackCreatureLoot
            : GetCreatureloot();

    auto CalculateLoot =
        [&Request, &LootTable](
            const char *Field,
            const char *RewardField) -> uint32_t
    {
        uint32_t Total = 0;

        const auto FieldIt =
            Request.find(Field);

        if (FieldIt == Request.end() ||
            !FieldIt->is_array())
        {
            return 0;
        }

        for (const auto &CreatureID : *FieldIt)
        {
            if (!CreatureID.is_number_integer())
                continue;

            const uint32_t ID =
                CreatureID.get<uint32_t>();

            for (const auto &Loot : LootTable)
            {
                if (Loot["Id"].is_null() ||
                    !Loot["Id"].is_number_integer())
                {
                    continue;
                }

                if (Loot["Id"].get<uint32_t>() != ID)
                    continue;

                if (Loot[RewardField].is_number())
                {
                    Total +=
                        Loot[RewardField].get<uint32_t>();
                }

                Infoprint(
                    va(
                        "Loot lookup: Field=%s CreatureId=%u matched %s=%u.",
                        Field,
                        ID,
                        RewardField,
                        Loot[RewardField].is_number()
                            ? Loot[RewardField].get<uint32_t>()
                            : 0));

                break;
            }
        }

        return Total;
    };

    /*
        Keep creature rewards separate from chest rewards so the
        results screen can display them correctly.
    */
    uint32_t CreatureGold =
        CalculateLoot(
            "LootedGoldCreatureIds",
            "Gold");

    uint32_t CreatureLifeForce =
        CalculateLoot(
            "LootedLifeForceCreatureIds",
            "LifeForce");

    const uint32_t CreatureXP =
        CalculateLoot(
            "KilledCreatureIds",
            "Xp");

    // Work around the current client reporting empty pickup-ID lists.
    // If it reports no creature currency, use the actual killed-creature IDs
    // so a completed raid does not award only the treasure-room currency.
    if (CreatureGold == 0)
    {
        CreatureGold =
            CalculateLoot(
                "KilledCreatureIds",
                "Gold");
    }

    if (CreatureLifeForce == 0)
    {
        CreatureLifeForce =
            CalculateLoot(
                "KilledCreatureIds",
                "LifeForce");
    }

    Response["Result"]["KillsXp"] =
        CreatureXP;

    /*
        Only credit treasure-room values the client reported, and
        clamp them to the amounts advertised by StartAttack.
    */
    auto ReadCappedAmount =
        [&Request](
            const char *Field,
            uint32_t Maximum) -> uint32_t
    {
        const auto It =
            Request.find(Field);

        if (It == Request.end() ||
            !It->is_number())
        {
            return 0;
        }

        const double Value =
            It->get<double>();

        if (Value <= 0.0)
            return 0;

        if (Value >= static_cast<double>(Maximum))
            return Maximum;

        return static_cast<uint32_t>(Value);
    };

    const uint32_t MaximumTreasureGold =
        GetIGCreward()["Amount"].get<uint32_t>();

    const uint32_t MaximumTreasureLifeForce =
        GetLifereward()["Amount"].get<uint32_t>();

    const uint32_t TreasureGold =
        ReadCappedAmount(
            "TreasureRoomLootedGold",
            MaximumTreasureGold);

    const uint32_t TreasureLifeForce =
        ReadCappedAmount(
            "TreasureRoomLootedLifeForce",
            MaximumTreasureLifeForce);

    const uint32_t TotalGold =
        CreatureGold + TreasureGold;

    const uint32_t TotalLifeForce =
        CreatureLifeForce + TreasureLifeForce;

    Infoprint(
        va(
            "EndAttack loot breakdown: CreatureGold=%u TreasureGold=%u TotalGold=%u CreatureLifeForce=%u TreasureLifeForce=%u TotalLifeForce=%u.",
            CreatureGold,
            TreasureGold,
            TotalGold,
            CreatureLifeForce,
            TreasureLifeForce,
            TotalLifeForce));

    /*
        Persist the combined actual currency reward.
    */
    if (TotalGold > 0)
    {
        Backend::Wallet::Updateamount(
            eCurrencytype::IGC,
            static_cast<int32_t>(TotalGold));
    }

    if (TotalLifeForce > 0)
    {
        Backend::Wallet::Updateamount(
            eCurrencytype::Lifeforce,
            static_cast<int32_t>(TotalLifeForce));
    }

    /*
     Report the totals calculated in this function.

     TotalGold and TotalLifeForce include the creature and
     treasure-room portions. The Kills fields remain creature-only.
    */

    // Total values include creature loot plus treasure-room loot.
    Response["Result"]["TotalGold"] =
        TotalGold;

    Response["Result"]["TotalLifeForce"] =
        TotalLifeForce;

    Response["Result"]["InitialGold"] =
        CurrentattackInitialGold;

    Response["Result"]["InitialXp"] =
        CurrentattackInitialXP;

    Response["Result"]["TreasureRoomGold"] =
        TreasureGold;

    Response["Result"]["TreasureRoomLifeForce"] =
        TreasureLifeForce;
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
    /*
    The client frequently sends a valid EndAttack JSON object
    followed by a large amount of non-JSON data.

    Do not trust a brace counter to locate the JSON boundary.
    Instead, try each possible closing brace until we find the
    first prefix that is itself valid JSON.

    This also handles braces/quotes inside trailing garbage.
*/
    std::string CleanBody;

    const size_t JsonStart =
        Body.find('{');

    if (JsonStart == std::string::npos)
    {
        Infoprint(
            "EndAttack: request contains no JSON object.");

        return;
    }

    size_t JsonEnd =
        std::string::npos;

    for (size_t Index = JsonStart;
         Index < Body.size();
         ++Index)
    {
        if (Body[Index] != '}')
            continue;

        const size_t CandidateLength =
            Index - JsonStart + 1;

        std::string Candidate =
            Body.substr(
                JsonStart,
                CandidateLength);

        if (!MQEL_json::accept(Candidate))
            continue;

        JsonEnd =
            Index;

        CleanBody =
            std::move(Candidate);

        break;
    }

    if (JsonEnd == std::string::npos)
    {
        Infoprint(
            "EndAttack: could not find a valid JSON object in request.");

        return;
    }

    if (JsonStart > 0)
    {
        Infoprint(
            va(
                "EndAttack: removing %u leading byte(s) before JSON.",
                (unsigned)JsonStart));
    }

    if (JsonEnd + 1 < Body.size())
    {
        Infoprint(
            va(
                "EndAttack: removing %u trailing byte(s) after JSON.",
                (unsigned)(Body.size() - JsonEnd - 1)));
    }

    Infoprint(
        va(
            "EndAttack: extracted valid JSON body length=%u.",
            (unsigned)CleanBody.size()));

    MQEL_json Parsed =
        MQEL_json::parse(CleanBody);

    ConsumeUsedAttackConsumables(
        Parsed["endAttackParams"]);

    MQEL_json Response;

    // Handle tutorial fights.
    if (CurrentattackID == TutorialAttackID1 &&
        CurrentattackCastleID == TutorialCastleID &&
        !CurrentattackDreadKnightQuest)
    {
        Response = MQEL_json::parse(
            R"({"Notifications":[{"$type":"HyperQuest.GameServer.Contracts.WalletUpdatedNotification, HyperQuest.GameServer.Contracts","Amounts":[{"CurrencyType":2,"Amount":33},{"CurrencyType":4,"Amount":33}],"NotificationType":24},{"$type":"HyperQuest.GameServer.Contracts.HeroXpChangedNotification, HyperQuest.GameServer.Contracts","HeroSpecContainerId":4,"XpAdded":60,"TotalXp":60,"Level":1,"NotificationType":43},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":1,"ArchetypeId":9,"PrimaryStatsModifiers":[0.354,0.998,0.021],"Effects":[{"Id":22,"Level":1}],"IsSellable":true,"TemplateId":37},"ItemType":3,"ObjectId":"580d50cd5c98320a9cb0e719"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":1,"ArchetypeId":8,"PrimaryStatsModifiers":[0.375,0.364,0.821],"IsSellable":true,"TemplateId":40},"ItemType":3,"ObjectId":"580d50cd5c98320a9cb0e71a"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":1,"ArchetypeId":2,"PrimaryStatsModifiers":[1,1,1],"IsSellable":true,"TemplateId":53},"ItemType":3,"ObjectId":"580d50cd5c98320a9cb0e71b"}],"NotificationType":111}],"Result":{"AttackId":"580d504f5c98320c1881325a","DefenderCastleId":2,"DefenderCastleType":1,"DefenderOasisNameId":2325,"Duration":119100,"InitialGold":1000,"HeroLevel":1,"TotalXp":60,"TotalGold":33,"TotalLifeForce":33,"KillsXp":60,"KillsGold":33,"KillsLifeForce":33,"CastleRatingFreePrize":{"CurrencyType":2,"Amount":50},"IsTutorial":true,"IsCastleAttackable":true,"UpdatedAccountStats":{"TotalCreaturesKilled":16,"AttackTotalIGCWon":33,"TotalCastlesLooted":1,"CastlesDefeated":{"Medium":1},"KilledCreatures":{"1081":12,"1029":3,"1003":1},"CurrencyAccumulation":{"IGC":33,"LifeForce":33},"DefeatCastleStrike":1,"TotalItemsLooted":3},"TrophyScoreVariation":{},"VictoryConditionRewardRatios":[1,0.75,0.5],"VictoryConditionLevel":1,"MaxVictoryConditionLevel":1,"EnterTreasureRoom":true,"VictoryConditionType":1}})");
    }
    else if (CurrentattackID == TutorialAttackID2 &&
             CurrentattackCastleID == TutorialCastleID &&
             !CurrentattackDreadKnightQuest)
    {
        Response = MQEL_json::parse(
            R"({"Notifications":[{"$type":"HyperQuest.GameServer.Contracts.WalletUpdatedNotification, HyperQuest.GameServer.Contracts","Amounts":[{"CurrencyType":2},{"CurrencyType":4,"Amount":29}],"NotificationType":24},{"$type":"HyperQuest.GameServer.Contracts.HeroXpChangedNotification, HyperQuest.GameServer.Contracts","HeroSpecContainerId":4,"XpAdded":66,"TotalXp":1868,"Level":4,"NotificationType":43},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1000},"ItemType":4,"ObjectId":"58043d945c98320eb4f50727"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":2,"ArchetypeId":4,"PrimaryStatsModifiers":[0.566,0.342,0.122],"Effects":[{"Id":22,"Level":1}],"IsSellable":true,"TemplateId":17},"ItemType":3,"ObjectId":"58043d945c98320eb4f50728"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":3,"ArchetypeId":8,"PrimaryStatsModifiers":[0.98,0.24,0.777],"IsSellable":true,"TemplateId":44},"ItemType":3,"ObjectId":"58043d945c98320eb4f50729","NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":4,"ArchetypeId":3,"PrimaryStatsModifiers":[0.938,0.568,0.491],"IsSellable":true,"TemplateId":205},"ItemType":3,"ObjectId":"58043d945c98320eb4f5072a"}],"NotificationType":111}],"Result":{"AttackId":"58043d5d5c98320c1881325a","DefenderCastleId":3000947,"DefenderAccountDisplayName":"jonsie111","Duration":49933,"InitialXp":1802,"InitialGold":2000,"HeroLevel":4,"TotalXp":66,"TotalGold":103,"TotalLifeForce":29,"TreasureRoomGold":7,"PillagedIGCMinesAmount":67,"PillagedIGCMineCount":1,"CastleRatingFreePrize":{"CurrencyType":2,"Amount":50},"IsCastleAttackable":true,"UpdatedAccountStats":{"TotalCreaturesKilled":202,"AttackTotalIGCWon":833,"TotalCastlesLooted":10,"CastlesDefeated":{"Medium":5,"Easy":4,"Hard":1},"KilledCreatures":{"1081":86,"1029":7,"1003":2,"1000":21,"1006":3,"1023":4,"1079":8,"1155":1,"1001":24,"1026":9,"1024":7,"1016":5,"1009":6,"1015":13,"1002":6,"1028":3,"1007":4,"1090":8,"1087":8},"CurrencyAccumulation":{"IGC":1300,"LifeForce":1200},"DefeatCastleStrike":4,"TotalPotionsConsumed":5},"TrophyScoreVariation":{},"VictoryConditionLevel":3,"DefenderLossIGC":7,"EnterTreasureRoom":true,"CastleShieldedByAttacker":true,"CastleValidationDuration":44233,"LastComment":{"AttackId":"580238555c98321778c363ba","AccountId":3140910,"Comment":{"Raw":"for beginners..."},"AvatarId":10}})");
    }
    else
    {
        Response = MQEL_json::parse(
            R"({"Notifications":[{"$type":"HyperQuest.GameServer.Contracts.WalletUpdatedNotification, HyperQuest.GameServer.Contracts","Amounts":[{"CurrencyType":2},{"CurrencyType":4,"Amount":165}],"NotificationType":24},{"$type":"HyperQuest.GameServer.Contracts.HeroXpChangedNotification, HyperQuest.GameServer.Contracts","HeroSpecContainerId":4,"XpAdded":333,"TotalXp":2201,"Level":4,"NotificationType":43},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":3,"ArchetypeId":10,"PrimaryStatsModifiers":[0.979,0.193,0.183],"IsSellable":true,"TemplateId":318},"ItemType":3,"ObjectId":"58043e625c98320eb4f507ba"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":3,"ArchetypeId":4,"PrimaryStatsModifiers":[0.381,0.771,0.188],"IsSellable":true,"TemplateId":45},"ItemType":3,"ObjectId":"58043e625c98320eb4f507bb"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.ObjectiveCompletedNotification, HyperQuest.GameServer.Contracts","ObjectiveId":302,"NotificationType":14},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1003},"ItemType":4,"ObjectId":"58043e625c98320eb4f507bc"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1003},"ItemType":4,"ObjectId":"58043d945c98320eb4f507bd"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1001},"ItemType":4,"ObjectId":"58043d945c98320eb4f507be"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1001},"ItemType":4,"ObjectId":"58043d945c98320eb4f507bf"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1000},"ItemType":4,"ObjectId":"58043d945c98320eb4f507c0"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.ObjectiveUnlockedNotification, HyperQuest.GameServer.Contracts","AccountObjective":{"ObjectiveId":303,"LastStatusDate":"2016-10-17T02:58:42Z","Status":1},"NotificationType":17}],"Result":{"AttackId":"58043dab5c98320eb4f50743","DefenderCastleId":101,"DefenderAccountDisplayName":"Fendrick's Farm","DefenderOasisNameId":16659,"Duration":172600,"InitialGold":2000,"HeroLevel":4,"TotalXp":333,"TotalGold":179,"TotalLifeForce":165,"KillsXp":333,"KillsGold":133,"KillsLifeForce":119,"TreasureRoomGold":46,"TreasureRoomLifeForce":46,"CastleRatingFreePrize":{"CurrencyType":2,"Amount":75},"IsCastleAttackable":true,"UpdatedAccountStats":{"TotalCreaturesKilled":243,"AttackTotalIGCWon":1012,"TotalCastlesLooted":11,"CastlesDefeated":{"Medium":5,"Easy":5,"Hard":1},"KilledCreatures":{"1081":103,"1029":7,"1003":5,"1000":24,"1006":3,"1079":8,"1155":1,"1001":26,"1026":9,"1024":7,"1016":5,"1009":6,"1015":13,"1002":6,"1028":3,"1007":4,"1090":8,"1087":8},"CurrencyAccumulation":{"IGC":1300,"LifeForce":1200},"DefeatCastleStrike":5,"TotalPotionsConsumed":5,"TotalItemsLooted":33},"TrophyScoreVariation":{},"VictoryConditionRewardRatios":[1,0.75,0.5],"VictoryConditionLevel":1,"MaxVictoryConditionLevel":1,"EnterTreasureRoom":true,"VictoryConditionType":1}})");
    }

    // Exit/Escape requests must not reuse the generic successful-attack
    // response's EnterTreasureRoom=true flag.
    const auto CompletionTypeIt =
        Parsed["endAttackParams"].find("CompletionType");

    if (CompletionTypeIt != Parsed["endAttackParams"].end() &&
        CompletionTypeIt->is_number_integer())
    {
        const uint32_t CompletionType =
            CompletionTypeIt->get<uint32_t>();

        // AttackCompletionType: Exit=2, Escape=3.
        if (CompletionType == 2 || CompletionType == 3)
        {
            Response["Result"]["EnterTreasureRoom"] = false;

            // The static successful-attack template also reports a victory.
            // An Exit/Escape is not a victory, so clear those stale fields too.
            // VictoryConditionType::None is enum value 0 in the client model.
            Response["Result"]["VictoryConditionType"] = 0;
            Response["Result"]["VictoryConditionLevel"] = 0;
            Response["Result"]["MaxVictoryConditionLevel"] = 0;

            Infoprint(va(
                "EndAttack: CompletionType=%u; clearing treasure-room and victory fields for exit/escape.",
                CompletionType));
        }
    }

    /*
      The generic EndAttack response historically contained the
      Fendrick's Farm progression data (Castle 101).

      That is wrong when the player actually attacked another
      PVE castle such as Tybalt's Farm (100).

      Rebind the defender identity to the castle from StartAttack.
  */
    if (CurrentattackCastleID != 0)
    {
        const auto *PVECastle =
            Backend::PvECastle::Find(
                CurrentattackCastleID);

        Response["Result"]["DefenderCastleId"] =
            CurrentattackCastleID;

        Response["Result"]["DefenderCastleType"] =
            (uint32_t)
                CurrentattackCastleType;

        if (PVECastle != nullptr)
        {
            if (!PVECastle->AccountDisplayName.empty())
            {
                Response["Result"]["DefenderAccountDisplayName"] =
                    PVECastle->AccountDisplayName;
            }

            if (PVECastle->OasisNameId != 0)
            {
                Response["Result"]["DefenderOasisNameId"] =
                    PVECastle->OasisNameId;
            }

            Infoprint(
                va(
                    "EndAttack: rebound defender to AccountId=%llu name=%s OasisNameId=%u.",
                    (unsigned long long)
                        CurrentattackCastleID,
                    PVECastle->AccountDisplayName.empty()
                        ? "Unknown Castle"
                        : PVECastle->AccountDisplayName.c_str(),
                    PVECastle->OasisNameId));
        }
        else
        {
            Infoprint(
                va(
                    "EndAttack: rebound defender AccountId=%llu but no PVE database entry was found.",
                    (unsigned long long)
                        CurrentattackCastleID));
        }
    }

    if (CurrentattackDreadKnightQuest)
    {
        /*
            Dreadknight was actually played as Castle 21.

            Do not convert the completed attack back into the
            Castle 2 tutorial identity. The EndAttack result must
            describe the castle that was actually played.
        */
        const auto *DreadCastle =
            Backend::PvECastle::Find(
                DreadKnightCastleID);

        Response["Result"]["DefenderCastleId"] =
            DreadKnightCastleID;

        Response["Result"]["DefenderCastleType"] =
            (uint32_t)
                CurrentattackCastleType;

        if (DreadCastle != nullptr)
        {
            if (!DreadCastle->AccountDisplayName.empty())
            {
                Response["Result"]["DefenderAccountDisplayName"] =
                    DreadCastle->AccountDisplayName;
            }

            if (DreadCastle->OasisNameId != 0)
            {
                Response["Result"]["DefenderOasisNameId"] =
                    DreadCastle->OasisNameId;
            }

            Infoprint(
                va(
                    "EndAttack: Dread result bound to Castle 21 name=%s OasisNameId=%u.",
                    DreadCastle->AccountDisplayName.empty()
                        ? "Unknown Castle"
                        : DreadCastle->AccountDisplayName.c_str(),
                    DreadCastle->OasisNameId));
        }
        else
        {
            Infoprint(
                "EndAttack: Dread Castle 21 was not found in the PVE database.");
        }
    }

    /*
        The generic EndAttack template contains Fendrick's
        progression notifications.

        Do not send those when the player attacked another castle.
    */
    if (CurrentattackCastleID != 101 &&
        Response["Notifications"].is_array())
    {
        for (size_t Index =
                 Response["Notifications"].size();
             Index > 0;
             --Index)
        {
            auto &Notification =
                Response["Notifications"][Index - 1];

            if (Notification["$type"].is_null() ||
                !Notification["$type"].is_string())
            {
                continue;
            }

            const std::string Type =
                Notification["$type"].get<std::string>();

            /*
                ObjectiveCompletedNotification stores ObjectiveId
                directly on the notification.
            */
            if (Type.find("ObjectiveCompletedNotification") !=
                    std::string::npos &&
                !Notification["ObjectiveId"].is_null() &&
                Notification["ObjectiveId"].is_number_integer())
            {
                const uint32_t ObjectiveId =
                    Notification["ObjectiveId"].get<uint32_t>();

                if (ObjectiveId == 302)
                {
                    Infoprint(
                        "EndAttack: removing hardcoded Fendrick Objective 302.");

                    Response["Notifications"].erase(
                        Response["Notifications"].begin() +
                        (Index - 1));

                    continue;
                }
            }

            /*
                ObjectiveUnlockedNotification stores ObjectiveId
                inside AccountObjective.
            */
            if (Type.find("ObjectiveUnlockedNotification") !=
                    std::string::npos &&
                !Notification["AccountObjective"].is_null() &&
                !Notification["AccountObjective"]["ObjectiveId"].is_null() &&
                Notification["AccountObjective"]["ObjectiveId"].is_number_integer())
            {
                const uint32_t ObjectiveId =
                    Notification["AccountObjective"]["ObjectiveId"].get<uint32_t>();

                if (ObjectiveId == 303)
                {
                    Infoprint(
                        "EndAttack: removing hardcoded Fendrick Objective 303.");

                    Response["Notifications"].erase(
                        Response["Notifications"].begin() +
                        (Index - 1));
                }
            }
        }
    }

    /*
    Calculate and persist the actual loot collected by the client.
    */
    ApplyCollectedAttackLoot(
        Parsed["endAttackParams"],
        Response);

    const uint32_t CollectedGold =
        Response["Result"]["TotalGold"].is_number()
            ? Response["Result"]["TotalGold"].get<uint32_t>()
            : 0;

    const uint32_t CollectedLifeForce =
        Response["Result"]["TotalLifeForce"].is_number()
            ? Response["Result"]["TotalLifeForce"].get<uint32_t>()
            : 0;

    /*
        Replace the combined currency event with one event per
        currency, matching Backend::Wallet::NotifyWalletamountupdate().
    */
    const std::string WalletNotificationType =
        "HyperQuest.GameServer.Contracts.WalletUpdatedNotification, HyperQuest.GameServer.Contracts";

    if (!Response["Notifications"].is_array())
    {
        Response["Notifications"] = MQEL_json::array();
    }

    // Remove the old combined wallet notification(s).
    for (size_t Index = Response["Notifications"].size();
         Index > 0;
         --Index)
    {
        const auto &Notification =
            Response["Notifications"][Index - 1];

        if (Notification["$type"].is_null() ||
            !Notification["$type"].is_string())
        {
            continue;
        }

        if (Notification["$type"].get<std::string>() !=
            WalletNotificationType)
        {
            continue;
        }

        Response["Notifications"].erase(
            Response["Notifications"].begin() + (Index - 1));
    }

    // Emit one absolute-balance notification for each currency.
    auto AddWalletNotification =
        [&Response, &WalletNotificationType](eCurrencytype Currency)
    {
        MQEL_json Notification =
            MQEL_json::object();

        Notification["$type"] =
            WalletNotificationType;

        Notification["Amounts"] =
            MQEL_json::array();

        Notification["Amounts"].push_back(
            Backend::Wallet::Serialize(Currency));

        Notification["NotificationType"] = 24;

        Response["Notifications"].push_back(
            Notification);
    };

    AddWalletNotification(eCurrencytype::IGC);
    AddWalletNotification(eCurrencytype::Lifeforce);

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

    // Preserve the existing rewards for the two real tutorial attacks.
    if (CurrentattackCastleID == TutorialCastleID &&
        CurrentattackID == TutorialAttackID1 &&
        !CurrentattackDreadKnightQuest)
    {
        XPReward = 60;
    }
    else if (CurrentattackCastleID == TutorialCastleID &&
             CurrentattackID == TutorialAttackID2 &&
             !CurrentattackDreadKnightQuest)
    {
        XPReward = 66;
    }
    else if (Response["Result"]["KillsXp"].is_number())
    {
        // Regular PvE raids use the XP calculated from the killed creatures.
        XPReward =
            Response["Result"]["KillsXp"].get<uint32_t>();
    }
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

    // Result.TotalXp is XP GAINED during this raid.
    // The notification below still carries the hero's lifetime XP.
    Response["Result"]["InitialXp"] =
        CurrentattackInitialXP;

    Response["Result"]["TotalXp"] =
        XPReward;

    Response["Result"]["KillsXp"] =
        XPReward;

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

            // Ensure the XP notification targets the actual active hero.
            Notification["HeroSpecContainerId"] =
                UpdatedHero["HeroSpecContainerId"].is_number()
                    ? UpdatedHero["HeroSpecContainerId"].get<uint32_t>()
                    : (uint32_t)Backend::Hero::Getheroclass();

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
