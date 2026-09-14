/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 31-01-2018
    License: MIT
    Notes:
        Provides game structs.
*/

#pragma once
#include "Enumerations.hpp"
#include "../Stdinclude.hpp"

struct Stat_t
{
    uint32_t Creatureskilled;
    uint32_t Castleslooted;
    uint32_t Timesplayed;

    MQEL_json Serialize()
    {
        MQEL_json Object = MQEL_json::object();

        if (Creatureskilled)
            Object["TotalCreaturesKilled"] = Creatureskilled;
        if (Castleslooted)
            Object["TotalCastlesLooted"] = Castleslooted;
        if (Timesplayed)
            Object["TimePlayed"] = Timesplayed;

        return Object;
    }
    void Deserialize(MQEL_json &Object)
    {
        if (!Object["TotalCreaturesKilled"].is_null())
        {
            Creatureskilled = Object["TotalCreaturesKilled"];
        }
        if (!Object["TotalCastlesLooted"].is_null())
        {
            Castleslooted = Object["TotalCastlesLooted"];
        }
        if (!Object["TimePlayed"].is_null())
        {
            Timesplayed = Object["TimePlayed"];
        }
    }
};
struct Spell_t
{
    uint32_t ID;
    uint32_t Slot;

    MQEL_json Serialize()
    {
        MQEL_json Object = MQEL_json::object();

        if (Slot)
            Object["SlotIndex"] = Slot;
        Object["SpellSpecContainerId"] = ID;

        return Object;
    }
};
struct Effect_t
{
    uint32_t ID;
    uint32_t Level;

    MQEL_json Serialize()
    {
        MQEL_json Object = MQEL_json::object();

        Object["Id"] = ID;
        Object["Level"] = Level;

        return Object;
    }
};
struct Equipment_t
{                         // NOTE(Convery): Need to further test and RE these properties.
    uint32_t ID;          // Localized as 4968 + ID in oasis_en.json
    uint32_t Dye;         // DyeID, bought for premium currency.
    bool Branded;         // Named item.
    bool Sellable;        // Tradeable item.
    uint32_t Level;       // Item level.
    uint32_t Archetype;   // I have no idea about this. Knights weapon is 2, archer's the same, armor is 8 and mage staff is 9.
    std::string Itemtype; // Readable string of eInventoryItemType
    std::vector<double> Modifiers;
    std::vector<Effect_t> Effects;

    /*
        NOTE(Convery):
        Possibly contains:
        uint32_t EnchantmentLevel;
        array Stats;
    */

    MQEL_json Serialize()
    {
        MQEL_json Object = MQEL_json::object();

        Object["Type"] = Itemtype;
        Object["ItemLevel"] = Level;
        Object["ArchetypeId"] = Archetype;
        Object["PrimaryStatsModifiers"] = Modifiers;
        for (auto &Item : Effects)
            Object["Effects"] += Item.Serialize();
        if (Sellable)
            Object["IsSellable"] = Sellable;
        if (Branded)
            Object["IsBranded"] = Branded;
        Object["TemplateId"] = ID;
        Object["DyeTemplateId"] = Dye;

        return Object;
    }
    void Deserialize(MQEL_json &Object)
    {
        if (!Object.is_object())
            return;

        if (!Object["Type"].is_null() &&
            Object["Type"].is_string())
        {
            Itemtype = Object["Type"].get<std::string>();
        }

        if (!Object["ItemLevel"].is_null() &&
            Object["ItemLevel"].is_number())
        {
            Level = Object["ItemLevel"].get<uint32_t>();
        }

        if (!Object["ArchetypeId"].is_null() &&
            Object["ArchetypeId"].is_number())
        {
            Archetype = Object["ArchetypeId"].get<uint32_t>();
        }

        Modifiers.clear();

        if (!Object["PrimaryStatsModifiers"].is_null() &&
            Object["PrimaryStatsModifiers"].is_array())
        {
            for (auto &Item : Object["PrimaryStatsModifiers"])
            {
                if (Item.is_number())
                    Modifiers.push_back(Item.get<double>());
            }
        }

        Effects.clear();

        if (!Object["Effects"].is_null() &&
            Object["Effects"].is_array())
        {
            for (auto &Item : Object["Effects"])
            {
                if (!Item.is_object())
                    continue;

                uint32_t EffectID = 0;
                uint32_t EffectLevel = 0;

                if (!Item["Id"].is_null() &&
                    Item["Id"].is_number())
                {
                    EffectID = Item["Id"].get<uint32_t>();
                }

                if (!Item["Level"].is_null() &&
                    Item["Level"].is_number())
                {
                    EffectLevel = Item["Level"].get<uint32_t>();
                }

                Effects.push_back({EffectID, EffectLevel});
            }
        }

        if (!Object["IsSellable"].is_null() &&
            Object["IsSellable"].is_boolean())
        {
            Sellable = Object["IsSellable"].get<bool>();
        }

        if (!Object["IsBranded"].is_null() &&
            Object["IsBranded"].is_boolean())
        {
            Branded = Object["IsBranded"].get<bool>();
        }

        if (!Object["TemplateId"].is_null() &&
            Object["TemplateId"].is_number())
        {
            ID = Object["TemplateId"].get<uint32_t>();
        }

        if (!Object["DyeTemplateId"].is_null() &&
            Object["DyeTemplateId"].is_number())
        {
            Dye = Object["DyeTemplateId"].get<uint32_t>();
        }
    }
};
struct Consumable_t
{
    uint32_t ID;

    MQEL_json Serialize()
    {
        MQEL_json Object = MQEL_json::object();

        /* TODO(Convery): RE the other properties. */
        Object["TemplateId"] = ID;

        return Object;
    }
};
struct Attackregion_t
{
    uint32_t ID;
    eRegionstatus Status;

    MQEL_json Serialize()
    {
        MQEL_json Object = MQEL_json::object();

        Object["AttackRegionId"] = ID;
        Object["Status"] = (uint32_t)Status;

        return Object;
    }
};
