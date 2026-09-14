/*
    Persistent inventory implementation.
*/

#include "../Stdinclude.hpp"

namespace Backend
{
    namespace Inventory
    {
        struct InventoryItem_t
        {
            std::string ObjectID;
            int Slot = -1;
            MQEL_json Item = MQEL_json::object();

            MQEL_json Serialize()
            {
                MQEL_json Object = MQEL_json::object();

                Object["ObjectId"] = ObjectID;
                Object["Slot"] = Slot;
                Object["Item"] = Item;

                return Object;
            }

            void Deserialize(MQEL_json &Object)
            {
                if (!Object["ObjectId"].is_null())
                    ObjectID = Object["ObjectId"].get<std::string>();

                if (!Object["Slot"].is_null())
                    Slot = Object["Slot"].get<int>();

                if (!Object["Item"].is_null())
                    Item = Object["Item"];
            }
        };

        std::vector<InventoryItem_t> Items;
        std::vector<InventoryItem_t> Inbox;

        void Save()
        {
            MQEL_json Object = MQEL_json::object();

            Object["Inventory"] = MQEL_json::array();
            for (auto &Item : Items)
                Object["Inventory"] += Item.Serialize();

            Object["Inbox"] = MQEL_json::array();
            for (auto &Item : Inbox)
                Object["Inbox"] += Item.Serialize();

            std::string Plaintext = Object.dump(4);
            Package::Write("Inventory.json", Plaintext);
        }

        void Load()
        {
            Items.clear();
            Inbox.clear();

            auto Filebuffer = Package::Read("Inventory.json");

            if (Filebuffer.empty())
                return;

            try
            {
                auto Object = MQEL_json::parse(Filebuffer);

                if (!Object["Inventory"].is_null())
                {
                    for (auto &Entry : Object["Inventory"])
                    {
                        InventoryItem_t Item;
                        Item.Deserialize(Entry);
                        Items.push_back(Item);
                    }
                }

                if (!Object["Inbox"].is_null())
                {
                    for (auto &Entry : Object["Inbox"])
                    {
                        InventoryItem_t Item;
                        Item.Deserialize(Entry);
                        Inbox.push_back(Item);
                    }
                }

                Infoprint(va(
                    "Inventory loaded: %u inventory items, %u inbox items.",
                    (unsigned)Items.size(),
                    (unsigned)Inbox.size()));
            }
            catch (std::exception &e)
            {
                Infoprint(va(
                    "Inventory load failed: %s",
                    e.what()));
            }
        }

        void Addinbox(MQEL_json Item)
        {
            std::string ObjectID;

            if (!Item["ObjectId"].is_null())
                ObjectID = Item["ObjectId"].get<std::string>();

            if (ObjectID.empty())
            {
                Infoprint("Inventory: refusing inbox item without ObjectId.");
                return;
            }

            for (auto &Existing : Inbox)
            {
                if (Existing.ObjectID == ObjectID)
                    return;
            }

            InventoryItem_t NewItem;
            NewItem.ObjectID = ObjectID;
            NewItem.Item = Item;
            NewItem.Slot = -1;

            Inbox.push_back(NewItem);

            Save();

            Infoprint(va(
                "Inventory: stored inbox item %s.",
                ObjectID.c_str()));
        }

        bool Collect(std::string ObjectID, int Slot)
        {
            for (auto Iterator = Inbox.begin();
                 Iterator != Inbox.end();
                 ++Iterator)
            {
                if (Iterator->ObjectID != ObjectID)
                    continue;

                InventoryItem_t Item = *Iterator;
                Item.Slot = Slot;

                Inbox.erase(Iterator);
                Items.push_back(Item);

                Save();

                Infoprint(va(
                    "Inventory: collected %s into slot %i.",
                    ObjectID.c_str(),
                    Slot));

                return true;
            }

            Infoprint(va(
                "Inventory: could not find inbox item %s.",
                ObjectID.c_str()));

            return false;
        }

        void Addpurchased(MQEL_json Item, int Slot)
        {
            std::string ObjectID;

            if (!Item["ObjectId"].is_null())
                ObjectID = Item["ObjectId"].get<std::string>();

            if (ObjectID.empty())
            {
                Infoprint("Inventory: purchased item has no ObjectId.");
                return;
            }

            // Do not create the same purchased object twice.
            for (auto &Existing : Items)
            {
                if (Existing.ObjectID == ObjectID)
                    return;
            }

            InventoryItem_t NewItem;
            NewItem.ObjectID = ObjectID;
            NewItem.Item = Item;
            NewItem.Slot = Slot;

            Items.push_back(NewItem);

            Save();

            Infoprint(va(
                "Inventory: stored purchased item %s in slot %i.",
                ObjectID.c_str(),
                Slot));
        }

        bool Swap(int SourceSlot, int DestinationSlot)
        {
            InventoryItem_t *Source = nullptr;
            InventoryItem_t *Destination = nullptr;

            for (auto &Item : Items)
            {
                if (Item.Slot == SourceSlot)
                    Source = &Item;

                if (Item.Slot == DestinationSlot)
                    Destination = &Item;
            }

            if (!Source)
                return false;

            if (Destination)
            {
                std::swap(Source->Slot, Destination->Slot);
            }
            else
            {
                Source->Slot = DestinationSlot;
            }

            Save();

            Infoprint(va(
                "Inventory: moved item %s from slot %i to slot %i.",
                Source->ObjectID.c_str(),
                SourceSlot,
                DestinationSlot));

            return true;
        }

        bool Equip(int SourceSlot, int DestinationSlot, int HeroID)
        {
            for (auto Iterator = Items.begin();
                 Iterator != Items.end();
                 ++Iterator)
            {
                if (Iterator->Slot != SourceSlot)
                    continue;

                Infoprint(va(
                    "Inventory: found item %s in slot %i.",
                    Iterator->ObjectID.c_str(),
                    SourceSlot));

                Infoprint(va(
                    "Inventory: Item JSON type = %s.",
                    Iterator->Item.type_name()));

                Infoprint(va(
                    "Inventory: Item JSON = %s.",
                    Iterator->Item.dump().c_str()));

                if (!Iterator->Item.is_object())
                {
                    Infoprint("Inventory: ERROR - stored item is not a JSON object.");
                    return false;
                }

                Equipment_t Equipment{};

                try
                {
                    Infoprint("Inventory: starting Equipment.Deserialize().");

                    Equipment.Deserialize(Iterator->Item);

                    Infoprint(va(
                        "Inventory: Equipment.Deserialize() OK. TemplateId=%u ArchetypeId=%u.",
                        Equipment.ID,
                        Equipment.Archetype));
                }
                catch (std::exception &e)
                {
                    Infoprint(va(
                        "Inventory: Equipment.Deserialize() FAILED: %s",
                        e.what()));
                    return false;
                }

                Infoprint(va(
                    "Inventory: equipping item %s from inventory slot %i to hero %i equipment slot %i.",
                    Iterator->ObjectID.c_str(),
                    SourceSlot,
                    HeroID,
                    DestinationSlot));

                try
                {
                    Backend::Hero::Equipgear(
                        (eItemslot)DestinationSlot,
                        Equipment,
                        (eHerotype)HeroID);

                    Infoprint("Inventory: Hero::Equipgear() OK.");
                }
                catch (std::exception &e)
                {
                    Infoprint(va(
                        "Inventory: Hero::Equipgear() FAILED: %s",
                        e.what()));
                    return false;
                }

                Items.erase(Iterator);

                Save();

                Infoprint(va(
                    "Inventory: equipped item and removed it from inventory slot %i.",
                    SourceSlot));

                return true;
            }

            Infoprint(va(
                "Inventory: could not find item in inventory slot %i to equip.",
                SourceSlot));

            return false;
        }

        bool Unequip(int HeroID, int EquipmentSlot)
        {
            (void)HeroID;
            (void)EquipmentSlot;

            /*
                Equipment synchronization will be added after the
                persistent inventory itself is working.

                We intentionally do not fake an unequip here yet.
            */

            return false;
        }

        MQEL_json Serialize()
        {
            MQEL_json Result = MQEL_json::array();

            for (auto &Item : Items)
            {
                MQEL_json Object = Item.Item;

                Object["ObjectId"] = Item.ObjectID;
                Object["SlotIndex"] = Item.Slot;

                Result += Object;
            }

            return Result;
        }

        MQEL_json Serializeinbox()
        {
            MQEL_json Result = MQEL_json::array();

            for (auto &Item : Inbox)
            {
                MQEL_json Object = Item.Item;

                Object["ObjectId"] = Item.ObjectID;
                Result += Object;
            }

            return Result;
        }
    }
}