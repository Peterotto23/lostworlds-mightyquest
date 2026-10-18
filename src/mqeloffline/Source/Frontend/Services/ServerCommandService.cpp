/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 14-01-2018
    License: MIT
    Notes:
        Provides implementations for ServerCommand services.
*/

#include "../../Stdinclude.hpp"
#include "../../Backend/HeroItemCatalog.hpp"

// Statistics / telemetry.
void Handletracking(Request_t Request)
{
    // Get the tracking-type.
    auto Trackinginfo = Request.Get("TrackingTag", Message_t());
    auto Type = Trackinginfo.Get("$type", "Invalid");
    Type = Type.substr(0, Type.find_first_of(','));
    Type = Type.substr(Type.find_last_of('.') + 1);

    // Handle the telemetry.
    switch (Hash::FNV1a_32(Type.c_str()))
    {
    // Client has started.
    case Hash::FNV1a_32("GameStartTracking"):
    {
        auto Version = Trackinginfo.Get("GameClientVersion", "");
        auto Timestamp = Trackinginfo.Get("CreationDate", "");
        auto MachineID = Trackinginfo.Get("MachineId", "");
        auto TagID = Trackinginfo.Get("TrackingTagId", 0);

        Infoprint(va(
            "Telemetry: Client started with game-version %s and HWID %s",
            Version.c_str(),
            MachineID.c_str()));

        break;
    }

    // Clients state has changed.
    case Hash::FNV1a_32("GameStateTracking"):
    {
        auto Totaltime = Trackinginfo.Get("GameStateTotalTime", 0);
        auto Nextstate = Trackinginfo.Get("NextGameStateType", 0);
        auto Idletime = Trackinginfo.Get("GameStateIdleTime", 0);
        auto Statetype = Trackinginfo.Get("GameStateType", 0);
        auto Timestamp = Trackinginfo.Get("CreationDate", "");
        auto StateID = Trackinginfo.Get("GameStateId", 0);
        auto TagID = Trackinginfo.Get("TrackingTagId", 0);

        Infoprint(va(
            "Telemetry: Gamestate %i updated",
            StateID));

        break;
    }

    // Client wants a new logfile / epoch.
    case Hash::FNV1a_32("GameInitializeTracking"):
    {
        auto Timestamp = Trackinginfo.Get("CreationDate", "");
        auto TagID = Trackinginfo.Get("TrackingTagId", 0);

        Infoprint("Telemetry: Epoch changed");

        break;
    }

    // Errors.
    case Hash::FNV1a_32("Invalid"):
        Infoprint("Could not parse telemetry.");
        break;

    default:
        Infoprint(va(
            "No handler for tracking-tag \"%s\".",
            Type.c_str()));

        break;
    }
}

void Handleidle(Request_t Request)
{
    auto Idletime = Request.Get("IdleTime", 0);

    Infoprint(va(
        "Telemetry: Client was idle for %i seconds.",
        Idletime));
}

// Questing.
void Handleassignmentupdate(Request_t Request)
{
    auto AssignmentID = Request.Get("AssignmentId", 0);
    auto ActionID = Request.Get("ActionIndex", 0);

    Backend::Quest::Update(
        AssignmentID,
        ActionID);
}

void Handleassignmentaccepted(Request_t Request)
{
    auto AssignmentID = Request.Get("AssignmentId", 0);

    Backend::Quest::Start(
        AssignmentID);
}

void Handleassignmentcomplete(Request_t Request)
{
    auto AssignmentID = Request.Get("AssignmentId", 0);

    Backend::Quest::Complete(
        AssignmentID);
}

// Marketplace.
void Handlecastlebuy(Request_t Request)
{
    Printfunction();

    // Set the max capacity for gold and life.
    Backend::Wallet::Updateamount(
        eCurrencytype::IGC,
        2000);

    Backend::Wallet::Updateamount(
        eCurrencytype::Lifeforce,
        2000);

    /* TODO(Convery): Implement these notifications rather than hardcoding. */
    Backend::Notification::Enqueuelocal(
        MQEL_json::parse(
            R"({"$type":"HyperQuest.GameServer.Contracts.CastleBoughtNotification, HyperQuest.GameServer.Contracts","IsStartupCastle":true,"BuildInfo":{"Draft":{"AccountId":3142975,"LayoutId":1,"CreationDate":"2016-10-23T07:20:03Z","ModificationDate":"2016-10-24T00:20:50Z","Rooms":[{"X":4,"Y":3,"Id":1,"SpecContainerId":21},{"Buildings":[{"Rank":1,"RoomZoneId":12,"X":3,"Y":3,"Orientation":2,"Id":1,"SpecContainerId":1},{"Rank":1,"RoomZoneId":11,"Id":2,"SpecContainerId":3},{"Rank":1,"RoomZoneId":13,"Id":3,"SpecContainerId":4},{"RoomZoneId":7,"X":3,"Orientation":3,"Id":4,"SpecContainerId":13},{"RoomZoneId":4,"X":3,"Orientation":3,"Id":6,"SpecContainerId":10},{"RoomZoneId":6,"X":3,"Y":3,"Orientation":2,"Id":7,"SpecContainerId":8},{"RoomZoneId":3,"Y":3,"Orientation":1,"Id":8,"SpecContainerId":9}],"X":3,"Y":3,"Id":3,"SpecContainerId":25}],"ThemeId":21},"Level":1,"InventoryThemes":[2,21],"RoomNextIndex":4,"CreatureNextIndex":40,"TrapNextIndex":5,"DecorationNextIndex":32,"TriggerNextIndex":1,"BuildingNextIndex":9,"CastleStats":{"TotalConstructionPoints":56,"MaxConstructionPoints":56,"WinRatio":0.5,"WinRatioDifficulty":2},"CastleHeartRank":1},"NotificationType":86})"));

    Backend::Notification::Enqueuelocal(
        MQEL_json::parse(
            R"({"$type":"HyperQuest.GameServer.Contracts.SkusModifiersUpdatedNotification, HyperQuest.GameServer.Contracts","ShopSkuModifiers":[{"SkuCode":"15221","LimitedQuantityMax":15,"LimitedQuantityNextUnlockCastleLevelRequirement":3,"LimitedQuantityNextUnlockQuantity":10},{"SkuCode":"13911","LimitedQuantityMax":1,"LimitedQuantityNextUnlockCastleLevelRequirement":2,"LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15266","LimitedQuantityMax":2},{"SkuCode":"15226","LimitedQuantityMax":20,"LimitedQuantityNextUnlockCastleLevelRequirement":2,"LimitedQuantityNextUnlockQuantity":5},{"SkuCode":"15224","LimitedQuantityMax":2,"LimitedQuantityNextUnlockCastleLevelRequirement":2,"LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15502","LimitedQuantityMax":4},{"SkuCode":"15503","LimitedQuantityMax":4},{"SkuCode":"15504","LimitedQuantityMax":3},{"SkuCode":"15513","LimitedQuantityMax":3},{"SkuCode":"15262","LimitedQuantityNextUnlockQuantity":4},{"SkuCode":"15229","LimitedQuantityNextUnlockCastleLevelRequirement":3,"LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15220","LimitedQuantityNextUnlockCastleLevelRequirement":4,"LimitedQuantityNextUnlockQuantity":20},{"SkuCode":"15264","LimitedQuantityNextUnlockCastleLevelRequirement":4,"LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15230","LimitedQuantityNextUnlockCastleLevelRequirement":5,"LimitedQuantityNextUnlockQuantity":5},{"SkuCode":"15263","LimitedQuantityNextUnlockCastleLevelRequirement":6,"LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15268","LimitedQuantityNextUnlockCastleLevelRequirement":6,"LimitedQuantityNextUnlockQuantity":5},{"SkuCode":"15225","LimitedQuantityNextUnlockCastleLevelRequirement":7,"LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15467","LimitedQuantityNextUnlockCastleLevelRequirement":8,"LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15234","LimitedQuantityNextUnlockCastleLevelRequirement":9,"LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16070","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16071","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16072","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16073","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16074","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16075","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16076","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16077","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16078","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16079","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16080","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16049","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15805","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15810","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15811","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15806","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15835","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15807","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15812","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15808","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15804","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15809","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15813","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15825","LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15834","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15872","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15974","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15957","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15814","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15892","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16067","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15815","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15816","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16068","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15827","LimitedQuantityNextUnlockQuantity":6},{"SkuCode":"15828","LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15817","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15856","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16006","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16055","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16058","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15818","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15819","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15855","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15890","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16059","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16060","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15820","LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15821","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15874","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15981","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16064","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16057","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16061","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15822","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16010","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15961","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15853","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15984","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16062","LimitedQuantityNextUnlockQuantity":2},{"skuCode":"16063","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15823","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15830","LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15977","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16046","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16065","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16066","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15854","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16032","LimitedQuantityNextUnlockQuantity":1}],"NotificationType":90})"));
}

void Handleitembuy(Request_t Request)
{
    Infoprint(va(
        "BuyHeroItemCommand received: %s",
        Request.toString().c_str()));

    auto Price = Request.Get(
        "ClientPrice",
        Message_t());

    auto Currencytype = Price.Get(
        "CurrencyType",
        0);

    auto Currencyamount = Price.Get(
        "Amount",
        0);

    auto SKUCode = Request.Get(
        "SkuCode",
        "");

    auto Slot = Request.Get(
        "SlotIndex",
        0);

    /*
        Step 1: Resolve the SKU before charging anything.
    */
    const auto *Sku =
        Backend::HeroItemCatalog::FindSku(
            SKUCode);

    if (Sku == nullptr)
    {
        Infoprint(va(
            "BuyHeroItemCommand: SKU %s was not found in the catalog. No currency charged.",
            SKUCode.c_str()));

        return;
    }

    /*
        ItemType 3 denotes the hero-item purchases handled here.
        Other SKU types must use their own purchase handlers.
    */
    const uint32_t ItemType =
        Backend::HeroItemCatalog::GetUInt(
            *Sku,
            "ItemType");

    if (ItemType != 3)
    {
        Infoprint(va(
            "BuyHeroItemCommand: SKU %s has unsupported ItemType %u. No currency charged.",
            SKUCode.c_str(),
            ItemType));

        return;
    }

    auto Active = Sku->find("IsActive");

    if (Active != Sku->end() &&
        Active->is_boolean() &&
        !Active->get<bool>())
    {
        Infoprint(va(
            "BuyHeroItemCommand: SKU %s is inactive. No currency charged.",
            SKUCode.c_str()));

        return;
    }

    /*
        Step 2: Resolve the real template from the SKU.
    */
    const uint32_t TemplateId =
        Backend::HeroItemCatalog::GetUInt(
            *Sku,
            "ItemId");

    const auto *Template =
        Backend::HeroItemCatalog::FindTemplate(
            TemplateId);

    if (Template == nullptr)
    {
        Infoprint(va(
            "BuyHeroItemCommand: SKU %s references missing TemplateId %u. No currency charged.",
            SKUCode.c_str(),
            TemplateId));

        return;
    }

    /*
        All current hero-equipment SKUs represent one item.
        A missing ItemCount in the source data defaults to one.
    */
    const uint32_t ItemCount =
        Backend::HeroItemCatalog::GetUInt(
            *Sku,
            "ItemCount",
            1);

    if (ItemCount != 1)
    {
        Infoprint(va(
            "BuyHeroItemCommand: SKU %s has unsupported ItemCount %u. No currency charged.",
            SKUCode.c_str(),
            ItemCount));

        return;
    }

    /*
        Step 3: Construct the instance using catalog data.
        This happens before deducting currency.
    */
    MQEL_json Item =
        Backend::HeroItemCatalog::CreateEquipmentInstance(
            SKUCode,
            *Sku,
            *Template);

    if (!Item.is_object() ||
        Item["ObjectId"].is_null() ||
        Item["TemplateId"].is_null())
    {
        Infoprint(va(
            "BuyHeroItemCommand: failed to create item for SKU %s. No currency charged.",
            SKUCode.c_str()));

        return;
    }

    /*
        Step 4: Preserve the existing wallet behavior.
        The catalog checks above happen before the deduction.
    */
    Backend::Wallet::Updateamount(
        (eCurrencytype)Currencytype,
        Currencyamount * -1);

    /*
        Step 5: Use the existing persistence system.
        Inventory.json and Addpurchased() remain unchanged.
    */
    Backend::Inventory::Addpurchased(
        Item,
        Slot);

    Infoprint(va(
        "Purchased SKU %s -> TemplateId %u ArchetypeId %u ItemLevel %u Slot %i.",
        SKUCode.c_str(),
        (unsigned)TemplateId,
        Backend::HeroItemCatalog::GetUInt(
            *Template,
            "ArchetypeId"),
        (unsigned)Item["ItemLevel"].get<uint32_t>(),
        Slot));

    Infoprint(va(
        "Purchased item JSON: %s",
        Item.dump().c_str()));
}

void Handleconsumable(Request_t Request)
{
    auto Materials = Request.Get(
        "ClientCraftingMaterials",
        Message_t());

    auto Consumed = Request.Get(
        "ConsumedHeroInventory",
        Message_t());

    auto Slots = Request.Get(
        "SlotIndexes",
        Message_t());

    auto Price = Request.Get(
        "ClientPrice",
        Message_t());

    const auto Currencytype = Price.Get(
        "CurrencyType",
        0);

    const auto Currencyamount = Price.Get(
        "Amount",
        0);

    const auto SKUCode = Request.Get(
        "SkuCode",
        "");

    /*
        Step 1: Resolve the SKU using the existing item catalog.
        Reject unknown or unsupported items before charging currency.
    */
    const auto *Sku =
        Backend::HeroItemCatalog::FindSku(SKUCode);

    if (Sku == nullptr)
    {
        Infoprint(va(
            "BuyConsumableCommand: SKU %s was not found. No currency charged.",
            SKUCode.c_str()));

        return;
    }

    const uint32_t ItemType =
        Backend::HeroItemCatalog::GetUInt(
            *Sku,
            "ItemType");

    if (ItemType != 4)
    {
        Infoprint(va(
            "BuyConsumableCommand: SKU %s is not a consumable (ItemType=%u). No currency charged.",
            SKUCode.c_str(),
            ItemType));

        return;
    }

    auto Active = Sku->find("IsActive");

    if (Active != Sku->end() &&
        Active->is_boolean() &&
        !Active->get<bool>())
    {
        Infoprint(va(
            "BuyConsumableCommand: SKU %s is inactive. No currency charged.",
            SKUCode.c_str()));

        return;
    }

    /*
        Step 2: Read the actual consumable template and stack size.
        SKU 16023 is catalogued as TemplateId 1, ItemCount 5.
    */
    const uint32_t TemplateId =
        Backend::HeroItemCatalog::GetUInt(
            *Sku,
            "ItemId");

    const uint32_t ItemCount =
        Backend::HeroItemCatalog::GetUInt(
            *Sku,
            "ItemCount",
            1);

    if (TemplateId == 0 ||
        ItemCount == 0 ||
        Currencyamount < 0)
    {
        Infoprint(va(
            "BuyConsumableCommand: SKU %s has invalid item, count, or price data. No currency charged.",
            SKUCode.c_str()));

        return;
    }

    /*
        Crafting and consumed-inventory purchases are not implemented.
        Refuse these variants rather than charging for an incomplete
        purchase.
    */
    if (!Materials.toJSON().empty() ||
        !Consumed.toJSON().empty())
    {
        Infoprint(va(
            "BuyConsumableCommand: SKU %s requires crafting or consumed inventory, which is not implemented. No currency charged.",
            SKUCode.c_str()));

        return;
    }

    /*
        Step 3: Read the destination inventory slot.
        The captured request uses SlotIndexes: {"0": 5}.
    */
    int Slot = -1;

    const auto SlotJSON = Slots.toJSON();

    if (SlotJSON.is_object())
    {
        for (auto Iterator = SlotJSON.begin();
             Iterator != SlotJSON.end();
             ++Iterator)
        {
            if (Iterator.value().is_number_integer())
            {
                Slot = Iterator.value().get<int>();
                break;
            }
        }
    }
    else if (SlotJSON.is_array() &&
             !SlotJSON.empty() &&
             SlotJSON[0].is_number_integer())
    {
        Slot = SlotJSON[0].get<int>();
    }
    else if (SlotJSON.is_number_integer())
    {
        Slot = SlotJSON.get<int>();
    }

    if (Slot < 0)
    {
        Infoprint(va(
            "BuyConsumableCommand: SKU %s has no valid destination slot. No currency charged.",
            SKUCode.c_str()));

        return;
    }

    /*
        Step 4: Create the inventory item in the format used by
        the existing consumable inventory code.
    */
    MQEL_json Item = MQEL_json::object();

    Item["$type"] =
        "HyperQuest.GameServer.Contracts.HeroConsumableItem, HyperQuest.GameServer.Contracts";

    Item["TemplateId"] = TemplateId;
    Item["StackCount"] = ItemCount;

    /*
        Give each purchased item its own ObjectId so the persistence
        layer can save and identify it.
    */
    static std::atomic<uint64_t> NextConsumableObjectId{0};

    const auto Timestamp =
        std::chrono::high_resolution_clock::now()
            .time_since_epoch()
            .count();

    const uint64_t Sequence =
        ++NextConsumableObjectId;

    Item["ObjectId"] =
        "purchased-consumable-" +
        SKUCode + "-" +
        std::to_string(Timestamp) + "-" +
        std::to_string(Sequence);

    /*
        Step 5: Preserve the existing currency deduction and save
        the purchased item through the persistent inventory system.
    */
    Backend::Wallet::Updateamount(
        (eCurrencytype)Currencytype,
        Currencyamount * -1);

    Backend::Inventory::Addpurchased(
        Item,
        Slot);

    Infoprint(va(
        "Purchased consumable SKU %s: TemplateId=%u StackCount=%u Slot=%i.",
        SKUCode.c_str(),
        TemplateId,
        ItemCount,
        Slot));

    Infoprint(va(
        "Purchased consumable JSON: %s",
        Item.dump().c_str()));
}

// Hero modification.
void Handleequipment(Request_t Request)
{
    auto DSTSlot = Request.Get("DestinationSlot", 0);
    auto SRCSlot = Request.Get("SourceSlotId", 0);
    auto HeroID = Request.Get("HeroId", 0);

    const char *HeroName = [&HeroID]() -> const char *
    {
        switch ((eHerotype)HeroID)
        {
        case eHerotype::Archer:
            return "Archer";
        case eHerotype::Knight:
            return "Knight";
        case eHerotype::Mage:
            return "Mage";
        case eHerotype::Runaway:
            return "Runaway";
        default:
            return "???";
        }
    }();

    Infoprint(va(
        "HeroEquipmentEquipCommand: %s inventory slot %i -> equipment slot %i.",
        HeroName, SRCSlot, DSTSlot));

    if (!Backend::Inventory::Equip(SRCSlot, DSTSlot, HeroID))
    {
        Infoprint(va(
            "HeroEquipmentEquipCommand failed: %s inventory slot %i -> equipment slot %i.",
            HeroName, SRCSlot, DSTSlot));
    }
}

void Handleequipconsumable(Request_t Request)
{
    const int HeroID =
        Request.Get("HeroId", Backend::Hero::Getheroclass());

    const int EquippedSlotIndex =
        Request.Get("EquippedSlotIndex", 0);

    const int TemplateId =
        Request.Get("TemplateId", 0);

    if (HeroID < 0 ||
        EquippedSlotIndex < 0 ||
        TemplateId < 0)
    {
        Infoprint(
            "HeroEquipConsumableCommand rejected invalid values.");
        return;
    }

    if (TemplateId != 0)
    {
        const auto Inventory = Backend::Inventory::Serialize();
        bool Found = false;

        for (const auto &Entry : Inventory)
        {
            const auto EntryItem = Entry.find("Item");

            if (EntryItem == Entry.end() ||
                !EntryItem->is_object())
            {
                continue;
            }

            const auto &Item = *EntryItem;

            const auto Template = Item.find("TemplateId");
            const auto Type = Item.find("$type");

            if (Template == Item.end() ||
                !Template->is_number() ||
                Template->get<uint32_t>() !=
                    static_cast<uint32_t>(TemplateId))
            {
                continue;
            }

            if (Type == Item.end() ||
                !Type->is_string() ||
                Type->get<std::string>().find(
                    "HeroConsumableItem") == std::string::npos)
            {
                continue;
            }

            uint32_t Count = 1;
            const auto Stack = Item.find("StackCount");

            if (Stack != Item.end() && Stack->is_number())
            {
                Count = Stack->get<uint32_t>();
            }

            if (Count > 0)
            {
                Found = true;
                break;
            }
        }

        if (!Found)
        {
            Infoprint(va(
                "HeroEquipConsumableCommand rejected TemplateId=%i: no owned consumable stack found.",
                TemplateId));
            return;
        }
    }

    Backend::Hero::Equipconsumable(
        Consumable_t(
            static_cast<uint32_t>(TemplateId),
            static_cast<uint32_t>(EquippedSlotIndex)),
        HeroID);
}

void Handleinbox(Request_t Request)
{
    auto Slots = Request.Get(
        "SlotIndexes",
        Message_t());

    auto Item = Request.Get(
        "ObjectId",
        "");

    int DestinationSlot = -1;

    auto SlotJSON = Slots.toJSON();

    if (SlotJSON.is_array() && !SlotJSON.empty())
    {
        DestinationSlot =
            SlotJSON[0].get<int>();
    }
    else if (SlotJSON.is_number_integer())
    {
        DestinationSlot =
            SlotJSON.get<int>();
    }

    Infoprint(va(
        "INBOX COLLECT REQUEST: ObjectId=%s DestinationSlot=%i",
        Item.c_str(),
        DestinationSlot));

    bool Collected =
        Backend::Inventory::Collect(
            Item,
            DestinationSlot);

    Infoprint(va(
        "INBOX COLLECT RESULT: ObjectId=%s Collected=%s",
        Item.c_str(),
        Collected ? "true" : "false"));

    if (!Collected)
    {
        Infoprint(va(
            "INBOX COLLECT FAILED: item %s was not present in backend inbox.",
            Item.c_str()));

        return;
    }
}

void Handleinventoryswap(Request_t Request)
{
    auto DSTSlot = Request.Get(
        "DestinationSlotId",
        0);

    auto SRCSlot = Request.Get(
        "SourceSlotId",
        0);

    Infoprint(va(
        "Inventory swap: %i -> %i.",
        SRCSlot,
        DSTSlot));

    if (!Backend::Inventory::Swap(
            SRCSlot,
            DSTSlot))
    {
        Infoprint(va(
            "Inventory swap failed: %i -> %i.",
            SRCSlot,
            DSTSlot));
    }
}

void Handlelogout(Request_t Request)
{
    Infoprint(
        "Client logout command received.");
}

// Endpoints.
void SendCommand(
    Gameserver *Server,
    std::string Request,
    std::string Body)
{
    Request_t Parsed{
        Body};

    // Deserialize the commands.
    for (auto &Item : Parsed.Get(
             "commands",
             std::vector<Message_t>{}))
    {
        // Get the command-type.
        auto Type = Item.Get(
            "$type",
            "Invalid");

        Type = Type.substr(
            0,
            Type.find_first_of(','));

        Type = Type.substr(
            Type.find_last_of('.') + 1);

        Infoprint(va(
            "ServerCommandService: received command \"%s\": %s",
            Type.c_str(),
            Item.toString().c_str()));

        // Trigger the callback.
        switch (Hash::FNV1a_32(Type.c_str()))
        {
        // Statistics / telemetry.
        case Hash::FNV1a_32("TrackingCommand"):
            Handletracking(Item);
            break;

        case Hash::FNV1a_32("ClientIdleCommand"):
            Handleidle(Item);
            break;

        // Questing.
        case Hash::FNV1a_32("StartAssignmentCommand"):
            Handleassignmentaccepted(Item);
            break;

        case Hash::FNV1a_32("CompleteAssignmentCommand"):
            Handleassignmentcomplete(Item);
            break;

        case Hash::FNV1a_32("ExecuteAssignmentActionCommand"):
            Handleassignmentupdate(Item);
            break;

        // Marketplace.
        case Hash::FNV1a_32("BuyCommand"):
            Handlecastlebuy(Item);
            break;

        case Hash::FNV1a_32("BuyHeroItemCommand"):
            Handleitembuy(Item);
            break;

        case Hash::FNV1a_32("BuyConsumableCommand"):
            Handleconsumable(Item);
            break;

        case Hash::FNV1a_32("HeroEquipmentEquipCommand"):
            Handleequipment(Item);
            break;

        case Hash::FNV1a_32("HeroEquipConsumableCommand"):
            Handleequipconsumable(Item);
            break;

        case Hash::FNV1a_32("InboxCollectToHeroInventoryCommand"):
            Handleinbox(Item);
            break;

        case Hash::FNV1a_32("InventorySwapItemCommand"):
            Handleinventoryswap(Item);
            break;

        case Hash::FNV1a_32("LogoutCommand"):
            Handlelogout(Item);
            break;

        // Errors.
        case Hash::FNV1a_32("Invalid"):
            Infoprint(
                "Could not parse command.");

            break;

        default:
            Infoprint(va(
                "No handler for command \"%s\".",
                Type.c_str()));

            break;
        }
    }

    /*
    Send the authoritative wallet and XP state on every command poll.

    Do not drain notification queues here. This sends only the current
    saved values and cannot replay tutorial or quest notifications.
*/
    const int32_t CurrentGold =
        Backend::Wallet::Getamount(eCurrencytype::IGC);

    const int32_t CurrentLifeForce =
        Backend::Wallet::Getamount(eCurrencytype::Lifeforce);

    const auto HeroState =
        Backend::Hero::Serialize();

    const uint32_t CurrentXP =
        HeroState["XP"].is_number()
            ? HeroState["XP"].get<uint32_t>()
            : 0;

    const uint32_t CurrentLevel =
        HeroState["Level"].is_number()
            ? HeroState["Level"].get<uint32_t>()
            : 1;

    uint32_t HeroSpecContainerId =
        (uint32_t)Backend::Hero::Getheroclass();

    if (HeroState["HeroSpecContainerId"].is_number())
    {
        HeroSpecContainerId =
            HeroState["HeroSpecContainerId"].get<uint32_t>();
    }
    MQEL_json Response =
        MQEL_json::object();

    Response["Notifications"] =
        MQEL_json::array();

    MQEL_json WalletNotification =
        MQEL_json::object();

    WalletNotification["$type"] =
        "HyperQuest.GameServer.Contracts.WalletUpdatedNotification, HyperQuest.GameServer.Contracts";

    WalletNotification["Amounts"] =
        MQEL_json::array();

    WalletNotification["Amounts"].push_back(
        Backend::Wallet::Serialize(eCurrencytype::IGC));

    WalletNotification["Amounts"].push_back(
        Backend::Wallet::Serialize(eCurrencytype::Lifeforce));

    WalletNotification["NotificationType"] = 24;

    Response["Notifications"].push_back(
        WalletNotification);

    MQEL_json XPNotification =
        MQEL_json::object();

    XPNotification["$type"] =
        "HyperQuest.GameServer.Contracts.HeroXpChangedNotification, HyperQuest.GameServer.Contracts";

    XPNotification["HeroSpecContainerId"] =
        HeroSpecContainerId;

    // Synchronize current XP without awarding the reward again.
    XPNotification["XpAdded"] = 0;
    XPNotification["TotalXp"] = CurrentXP;
    XPNotification["Level"] = CurrentLevel;
    XPNotification["NotificationType"] = 43;

    Response["Notifications"].push_back(
        XPNotification);

    Infoprint(
        va(
            "SendCommand global state snapshot: Gold=%d LifeForce=%d XP=%u Level=%u.",
            CurrentGold,
            CurrentLifeForce,
            CurrentXP,
            CurrentLevel));

    Sendreply(
        Server,
        Response.dump());
}

// Add the services to the gameserver on startup.
namespace
{
    struct Startup
    {
        Startup()
        {
            Mapservice(
                "/ServerCommandService.hqs/SendCommands",
                SendCommand);
        };
    };

    static Startup Loader{};
}