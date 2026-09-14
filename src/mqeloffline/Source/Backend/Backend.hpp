/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 31-01-2018
    License: MIT
    Notes:
        Provides implementations for the frontend.
*/

#pragma once
#include "../Stdinclude.hpp"

namespace Backend
{
    // Quest tracking and management.
    namespace Quest
    {
        void Start(uint32_t QuestID);
        void Complete(uint32_t QuestID);
        std::vector<uint32_t> Getcompleted();
        void Update(uint32_t QuestID, uint32_t ActionID);
    }

    // Notification tracking and management.
    namespace Notification
    {
        std::vector<MQEL_json> Dequeuelocals();
        std::vector<MQEL_json> Dequeueglobals();
        void Enqueuelocal(MQEL_json Notification);
        void Enqueueglobal(MQEL_json Notification);
    }

    // Track the clients currency.
    namespace Wallet
    {
        // Modify the wallet state.
        template <typename T>
        int32_t Getamount(T Type);
        template <typename T>
        uint32_t Getcapacity(T Type);
        template <typename T>
        void Setcapacity(T Type, uint32_t Max);
        template <typename T>
        void Updateamount(T Type, int32_t Delta);

        // Serialize to game-readable JSON.
        template <typename T>
        MQEL_json Serialize(T Type);
    }

    // Persistent player inventory.
    namespace Inventory
    {
        // Load persistent inventory state.
        void Load();

        // Add an item to the persistent inbox.
        void Addinbox(MQEL_json Item);

        // Move an inbox item into the player's inventory.
        bool Collect(std::string ObjectID, int Slot);

        // Add a purchased item directly to inventory.
        void Addpurchased(MQEL_json Item, int Slot);

        // Move an inventory item to another slot.
        bool Swap(int SourceSlot, int DestinationSlot);

        // Equip an inventory item on the active hero.
        bool Equip(int SourceSlot, int DestinationSlot, int HeroID);

        // Remove an equipped item back into inventory.
        bool Unequip(int HeroID, int EquipmentSlot);

        // Return the current inventory state.
        MQEL_json Serialize();

        // Return persisted inbox state.
        MQEL_json Serializeinbox();
    }

    // Hero status and management.
    namespace Hero
    {
        // Fetch general properties.
        template <typename C = int>
        C Getheroclass();

        // Initialize a new character.
        template <typename C = int>
        void Createhero(C Class);

        // Serialize to game-readable JSON.
        template <typename C = int>
        MQEL_json Serialize(C Class = Getheroclass());

        // Save persistent hero data.
        void Savehero();

        // Modify the characters stats.
        template <typename C = int>
        void Increaselevel(uint32_t Level, C Class = Getheroclass());
        template <typename C = int>
        void Increasestats(Stat_t Delta, C Class = Getheroclass());
        template <typename C = int>
        void IncreaseXP(uint32_t XP, C Class = Getheroclass());
        template <typename C = int>
        void SetXP(uint32_t XP, C Class = Getheroclass());

        // Modify the characters equipment.Wh
        template <typename T = int, typename C = int>
        void Equipgear(T Slot, Equipment_t Item, C Class = Getheroclass());
        template <typename C = int>
        void Equipspell(Spell_t Spell, C Class = Getheroclass());
    }

    // Castle matchmaking.
    namespace PvPCastle
    {
        // Get a players castle.
        MQEL_json Getcastle(uint64_t AccountID);

        // Tutorial 'playerID' substitutes.
        constexpr uint64_t TutorialID1 = 0x2;
        constexpr uint64_t TutorialID2 = 0x3;
    }
}
