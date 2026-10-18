
#pragma once

#include "../Stdinclude.hpp"

#include <atomic>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <sstream>

namespace Backend
{
    namespace HeroItemCatalog
    {
        struct CatalogData
        {
            std::unordered_map<std::string, MQEL_json> Skus;
            std::unordered_map<uint32_t, MQEL_json> Templates;
        };

        inline CatalogData &GetData()
        {
            static CatalogData Data;
            return Data;
        }

        inline uint32_t GetUInt(
            const MQEL_json &Object,
            const char *Key,
            uint32_t DefaultValue = 0)
        {
            auto It = Object.find(Key);

            if (It == Object.end() ||
                !It->is_number_integer())
            {
                return DefaultValue;
            }

            try
            {
                return It->get<uint32_t>();
            }
            catch (...)
            {
                return DefaultValue;
            }
        }

        inline std::string GetString(
            const MQEL_json &Object,
            const char *Key,
            const std::string &DefaultValue = "")
        {
            auto It = Object.find(Key);

            if (It == Object.end() ||
                !It->is_string())
            {
                return DefaultValue;
            }

            return It->get<std::string>();
        }

        inline bool ReadJsonFile(
            const std::string &Filename,
            MQEL_json &Result)
        {
            const std::string Path =
                "Plugins/MQELOffline-Catalog/" + Filename;

            std::ifstream File(
                Path,
                std::ios::binary);

            if (!File)
            {
                Infoprint(
                    va(
                        "HeroItemCatalog: cannot open %s. Check the catalog folder and server working directory.",
                        Path.c_str()));

                return false;
            }

            std::ostringstream Contents;
            Contents << File.rdbuf();

            try
            {
                Result = MQEL_json::parse(
                    Contents.str());

                return true;
            }
            catch (std::exception &Error)
            {
                Infoprint(
                    va(
                        "HeroItemCatalog: invalid JSON in %s: %s",
                        Path.c_str(),
                        Error.what()));

                return false;
            }
        }

        inline bool LoadCatalog()
        {
            MQEL_json TemplateRoot;
            MQEL_json SkuRoot;

            if (!ReadJsonFile(
                    "HeroItemTemplates.json",
                    TemplateRoot))
            {
                return false;
            }

            if (!ReadJsonFile(
                    "ShopSkuBaseSettings.json",
                    SkuRoot))
            {
                return false;
            }

            if (!TemplateRoot["TemplateList"].is_array() ||
                !SkuRoot["Skus"].is_array())
            {
                Infoprint(
                    "HeroItemCatalog: required catalog arrays are missing.");

                return false;
            }

            CatalogData NewData;

            for (const auto &Template :
                 TemplateRoot["TemplateList"])
            {
                const uint32_t Id =
                    GetUInt(Template, "Id");

                if (Id == 0)
                    continue;

                if (!NewData.Templates.emplace(
                        Id,
                        Template).second)
                {
                    Infoprint(
                        va(
                            "HeroItemCatalog: duplicate TemplateId %u.",
                            Id));

                    return false;
                }
            }

            for (const auto &Sku :
                 SkuRoot["Skus"])
            {
                const std::string Code =
                    GetString(Sku, "Code");

                if (Code.empty())
                    continue;

                if (!NewData.Skus.emplace(
                        Code,
                        Sku).second)
                {
                    Infoprint(
                        va(
                            "HeroItemCatalog: duplicate SKU %s.",
                            Code.c_str()));

                    return false;
                }
            }

            uint32_t HeroItemCount = 0;
            uint32_t MissingTemplates = 0;

            for (const auto &Entry :
                 NewData.Skus)
            {
                const MQEL_json &Sku = Entry.second;

                if (GetUInt(Sku, "ItemType") != 3)
                    continue;

                ++HeroItemCount;

                const uint32_t ItemId =
                    GetUInt(Sku, "ItemId");

                if (NewData.Templates.find(ItemId) ==
                    NewData.Templates.end())
                {
                    ++MissingTemplates;

                    Infoprint(
                        va(
                            "HeroItemCatalog: SKU %s references missing TemplateId %u.",
                            Entry.first.c_str(),
                            ItemId));
                }
            }

            /*
                Publish the catalog only after both files have
                been parsed and indexed successfully.
            */
            GetData().Skus.swap(NewData.Skus);
            GetData().Templates.swap(NewData.Templates);

            Infoprint(
                va(
                    "HeroItemCatalog: loaded %u templates, %u shop SKUs, %u hero-item SKUs, %u missing templates.",
                    (unsigned)GetData().Templates.size(),
                    (unsigned)GetData().Skus.size(),
                    (unsigned)HeroItemCount,
                    (unsigned)MissingTemplates));

            return true;
        }

        inline bool EnsureLoaded()
        {
            static std::once_flag LoadOnce;
            static bool LoadSucceeded = false;

            std::call_once(
                LoadOnce,
                []()
                {
                    LoadSucceeded = LoadCatalog();
                });

            return LoadSucceeded;
        }

        inline const MQEL_json *FindSku(
            const std::string &Code)
        {
            if (!EnsureLoaded())
                return nullptr;

            auto &Skus = GetData().Skus;

            auto It = Skus.find(Code);

            if (It == Skus.end())
                return nullptr;

            return &It->second;
        }

        inline const MQEL_json *FindTemplate(
            uint32_t TemplateId)
        {
            if (!EnsureLoaded())
                return nullptr;

            auto &Templates = GetData().Templates;

            auto It = Templates.find(TemplateId);

            if (It == Templates.end())
                return nullptr;

            return &It->second;
        }

        inline uint32_t ResolveItemLevel(
            const MQEL_json &Sku,
            const MQEL_json &Template)
        {
            /*
                Shop descriptions sometimes specify the purchased
                level more accurately than the template's LevelMin.
                For example, an existing Level 3 SKU maps to a
                template whose LevelMin is 2.
            */
            std::string Description =
                GetString(Sku, "InternalDescription");

            for (char &Character : Description)
            {
                Character = static_cast<char>(
                    std::tolower(
                        static_cast<unsigned char>(Character)));
            }

            const size_t Position =
                Description.find("level");

            if (Position != std::string::npos)
            {
                const char *Start =
                    Description.c_str() + Position + 5;

                while (*Start &&
                       !std::isdigit(
                           static_cast<unsigned char>(*Start)))
                {
                    ++Start;
                }

                if (*Start)
                {
                    char *End = nullptr;

                    const unsigned long Level =
                        std::strtoul(
                            Start,
                            &End,
                            10);

                    if (End != Start &&
                        Level > 0 &&
                        Level <= 100)
                    {
                        return static_cast<uint32_t>(Level);
                    }
                }
            }

            const uint32_t MinimumLevel =
                GetUInt(Template, "LevelMin", 1);

            return MinimumLevel > 0
                ? MinimumLevel
                : 1;
        }

        inline MQEL_json CreateEquipmentInstance(
            const std::string &SkuCode,
            const MQEL_json &Sku,
            const MQEL_json &Template)
        {
            const uint32_t TemplateId =
                GetUInt(
                    Sku,
                    "ItemId",
                    GetUInt(Template, "Id"));

            const uint32_t ArchetypeId =
                GetUInt(Template, "ArchetypeId");

            MQEL_json Item =
                MQEL_json::object();

            Item["$type"] =
                "HyperQuest.GameServer.Contracts.HeroEquipmentItem, HyperQuest.GameServer.Contracts";

            Item["Type"] =
                "HeroEquipmentItem";

            Item["TemplateId"] =
                TemplateId;

            Item["ItemLevel"] =
                ResolveItemLevel(
                    Sku,
                    Template);

            /*
                Some costumes and pets have no equipment archetype.
                Preserve that distinction rather than assigning a
                fake archetype to every catalog item.
            */
            if (ArchetypeId != 0)
            {
                Item["ArchetypeId"] =
                    ArchetypeId;

                /*
                    Keep the previous purchase behavior for now.
                    Authentic generated stat rolls are a separate
                    step using EquipmentGenerationSettings.json.
                */
                Item["PrimaryStatsModifiers"] =
                    {0.4, 0.4, 0.4};
            }

            auto Effects = Template.find("Effects");

            if (Effects != Template.end() &&
                Effects->is_array())
            {
                Item["Effects"] =
                    *Effects;
            }

            Item["IsSellable"] = true;
            Item["DyeTemplateId"] = 0;

            /*
                Use a timestamp plus a sequence number so rapid
                purchases do not reuse the same ObjectId.
            */
            static std::atomic<uint64_t> NextId{0};

            const auto Timestamp =
                std::chrono::high_resolution_clock::now()
                    .time_since_epoch()
                    .count();

            const uint64_t Sequence =
                ++NextId;

            Item["ObjectId"] =
                "purchased-" +
                SkuCode + "-" +
                std::to_string(Timestamp) + "-" +
                std::to_string(Sequence);

            return Item;
        }
    }
}
