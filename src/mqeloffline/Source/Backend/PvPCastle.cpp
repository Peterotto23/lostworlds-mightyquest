/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 08-02-2018
    License: MIT
    Notes:
        Provides matchmaking castles.
*/

#include "../Stdinclude.hpp"
#include "PvECastleDatabase.hpp"

namespace Backend
{
    namespace PvPCastle
    {
        std::unordered_map<uint64_t /* PlayerID */, MQEL_json::object_t> Castles;

        /*
            Normalize a Castle_<ID> file when the file is actually an extracted
            PVE settings export.

            Real runtime Castle_<ID> objects should be left alone.

            Our manually created Castle_100 is currently the raw PVE export, so
            it needs the same runtime normalization that BuildServerCastle()
            gives the PVE fallback path.
        */
        void NormalizeRuntimePVEcastle(
            MQEL_json::object_t &Castle)
        {
            /*
                Extracted PVE exports contain these settings fields.
                A normal runtime Castle_* object does not.
            */
            bool IsExtractedPVE =
                Castle.find("ActivationDate") !=
                    Castle.end() ||
                Castle.find("DeActivationDate") !=
                    Castle.end() ||
                Castle.find("CastleValidationDuration") !=
                    Castle.end();

            if (!IsExtractedPVE)
                return;

            Infoprint(
                "PvPCastle: detected extracted PVE data inside Castle_*; normalizing runtime fields.");

            /*
                The client expects a runtime UbisoftCastle.
            */
            Castle["$type"] =
                "HyperQuest.GameServer.Contracts.UbisoftCastle, HyperQuest.GameServer.Contracts";

            /*
                Extracted R01Q data uses LayoutId=0.
                Runtime attack castles use LayoutId=1.
            */
            if (Castle.find("LayoutId") ==
                    Castle.end() ||
                !Castle["LayoutId"].is_number_integer() ||
                Castle["LayoutId"].get<int64_t>() == 0)
            {
                Castle["LayoutId"] =
                    1;
            }

            /*
                Runtime castles need buildable level forcing enabled.
            */
            if (Castle.find("ForceCastleLevelOnBuildables") ==
                    Castle.end() ||
                !Castle["ForceCastleLevelOnBuildables"].is_boolean() ||
                !Castle["ForceCastleLevelOnBuildables"].get<bool>())
            {
                Castle["ForceCastleLevelOnBuildables"] =
                    true;
            }

            /*
                Use the same validation duration used by the working attack
                response.
            */
            if (Castle.find("CastleValidationDuration") ==
                    Castle.end() ||
                !Castle["CastleValidationDuration"].is_number() ||
                Castle["CastleValidationDuration"].get<double>() <= 0.0)
            {
                Castle["CastleValidationDuration"] =
                    44.23333;
            }

            /*
                The extracted PVE files have empty runtime dates.
            */
            if (Castle.find("CreationDate") ==
                    Castle.end() ||
                !Castle["CreationDate"].is_string() ||
                Castle["CreationDate"].get<std::string>().empty())
            {
                Castle["CreationDate"] =
                    "2016-01-01T00:00:00Z";
            }

            if (Castle.find("ModificationDate") ==
                    Castle.end() ||
                !Castle["ModificationDate"].is_string() ||
                Castle["ModificationDate"].get<std::string>().empty())
            {
                Castle["ModificationDate"] =
                    "2016-01-01T00:00:00Z";
            }

            /*
                Convert OasisName -> OasisNameId if necessary.
            */
            if (Castle.find("OasisNameId") ==
                    Castle.end() &&
                Castle.find("OasisName") !=
                    Castle.end() &&
                Castle["OasisName"].is_number())
            {
                Castle["OasisNameId"] =
                    Castle["OasisName"];
            }

            /*
                Runtime castle objects should not carry these settings-only
                fields.
            */
            Castle.erase("ActivationDate");
            Castle.erase("DeActivationDate");
            Castle.erase("CastleIconUrl");
            Castle.erase("IsBranded");
            Castle.erase("IsForSaleCastle");
            Castle.erase("IsHidden");
            Castle.erase("IsDisabled");
            Castle.erase("IsTutorialCastle");
            Castle.erase("OasisDescription");
            Castle.erase("OasisName");

            Infoprint(
                va(
                    "PvPCastle: normalized runtime castle AccountId=%llu Level=%u LayoutId=%u ForceCastleLevelOnBuildables=%s CastleValidationDuration=%.5f.",
                    (unsigned long long)
                        Castle["AccountId"]
                            .get<uint64_t>(),
                    Castle.find("Level") != Castle.end() &&
                            Castle["Level"].is_number()
                        ? Castle["Level"].get<uint32_t>()
                        : 0,
                    Castle["LayoutId"].get<uint32_t>(),
                    Castle["ForceCastleLevelOnBuildables"].get<bool>()
                        ? "true"
                        : "false",
                    Castle["CastleValidationDuration"].get<double>()));
        }

        // Populate the castle-map.
        void Loadcastles()
        {
            /*
                Load the original runtime Castle_* objects first.

                These are the existing/generated castles and therefore take
                priority over the PVE settings when the same AccountId exists
                in both places.
            */
            auto Filelist =
                Package::Findfiles("Castle_");

            for (auto &Item :
                 Filelist)
            {
                uint64_t CastleID{0};

                if (Item.rfind("Castle_", 0) != 0)
                    continue;

                try
                {
                    CastleID =
                        std::stoull(
                            Item.substr(7));
                }
                catch (...)
                {
                    Debugprint(
                        va(
                            "PvPCastle: invalid castle filename \"%s\"",
                            Item.c_str()));

                    continue;
                }

                if (CastleID == 0)
                    continue;

                auto &Entry =
                    Castles[CastleID];

                /*
                    Already loaded. Do not replace existing runtime castle
                    data with another copy.
                */
                if (Entry.size() > 0)
                    continue;

                Infoprint(
                    va(
                        "PvPCastle: loading %s for AccountId=%llu",
                        Item.c_str(),
                        (unsigned long long)CastleID));

                try
                {
                    std::string Data =
                        Package::Read(Item);

                    Infoprint(
                        va(
                            "PvPCastle: read %zu bytes from %s",
                            Data.size(),
                            Item.c_str()));

                    Entry =
                        MQEL_json::parse(
                            Data);

                    /*
                        Castle_3 is an actual runtime castle and therefore does not match
                        the extracted-PVE settings signature.

                        Castle_100 and other manually promoted PVE castles are raw extracted
                        PVE JSON placed into Castle_<ID>, so normalize those here.
                    */
                    NormalizeRuntimePVEcastle(
                        Entry);

                    Infoprint(
                        va(
                            "PvPCastle: loaded %s (AccountId=%llu, Rooms=%zu)",
                            Item.c_str(),
                            (unsigned long long)CastleID,
                            Entry["Rooms"].is_array()
                                ? Entry["Rooms"].size()
                                : 0));
                }
                catch (std::exception &e)
                {
                    Debugprint(
                        va(
                            "%s (\"%s\"): %s",
                            __FUNCTION__,
                            Item.c_str(),
                            e.what()));

                    Castles.erase(
                        CastleID);
                }
            }

            /*
                Load missing PVE castles from the extracted castle JSONs.

                This is the important part:

                    Castle_3  -> existing runtime castle wins
                    Castle_21 -> PVE JSON fills the missing entry
                    Castle_22 -> PVE JSON fills the missing entry
                    ...
                    Castle_104 -> PVE JSON fills the missing entry

                No individual castle IDs are hardcoded here.
            */
            try
            {
                const auto &PVECastles =
                    Backend::PvECastle::GetAll();

                uint32_t AddedCount = 0;

                for (const auto &PVECastle :
                     PVECastles)
                {
                    if (PVECastle.AccountId == 0)
                        continue;

                    /*
                        Never overwrite an existing runtime Castle_<id>.
                    */
                    if (Castles.find(
                            PVECastle.AccountId) !=
                        Castles.end())
                    {
                        continue;
                    }

                    MQEL_json PVEData =
                        Backend::PvECastle::BuildServerCastle(
                            PVECastle,
                            eCastletype::Ubisoft);

                    // if (PVECastle.AccountId != TutorialID1 &&
                    //     PVECastle.AccountId != TutorialID2)
                    // {
                    //     if (PVEData.find("IsTutorialCastle") != PVEData.end())
                    //     {
                    //         Infoprint(
                    //             va(
                    //                 "PvPCastle: removing IsTutorialCastle from normal PVE castle AccountId=%llu.",
                    //                 (unsigned long long)PVECastle.AccountId));
                    //
                    //         PVEData.erase("IsTutorialCastle");
                    //     }
                    // }

                    Infoprint(
                        va(
                            "PvPCastle: runtime AccountId=%llu Level=%u IsTutorialCastle=%s.",
                            (unsigned long long)
                                PVECastle.AccountId,
                            PVECastle.Level,
                            (PVEData.find("IsTutorialCastle") !=
                                 PVEData.end() &&
                             PVEData["IsTutorialCastle"].is_boolean() &&
                             PVEData["IsTutorialCastle"].get<bool>())
                                ? "true"
                                : "false"));

                    /*
                        Some older extracted PVE castle definitions do not contain
                        fields that the runtime UbisoftCastle contract expects.
                    */
                    if (PVEData.find("LayoutId") == PVEData.end())
                    {
                        PVEData["LayoutId"] = 1;

                        Infoprint(
                            va(
                                "PvPCastle: defaulted missing LayoutId for PVE castle AccountId=%llu.",
                                (unsigned long long)PVECastle.AccountId));
                    }

                    if (PVEData.find("ForceCastleLevelOnBuildables") ==
                        PVEData.end())
                    {
                        PVEData["ForceCastleLevelOnBuildables"] = true;

                        Infoprint(
                            va(
                                "PvPCastle: defaulted missing ForceCastleLevelOnBuildables for PVE castle AccountId=%llu.",
                                (unsigned long long)PVECastle.AccountId));
                    }

                    /*
                        The extracted PVE settings may contain OasisName
                        rather than the runtime OasisNameId field.
                    */
                    if (PVEData.find("OasisNameId") ==
                            PVEData.end() &&
                        PVEData.find("OasisName") !=
                            PVEData.end())
                    {
                        if (PVEData["OasisName"].is_number())
                        {
                            PVEData["OasisNameId"] =
                                PVEData["OasisName"];
                        }
                    }

                    if (PVEData.find("OasisName") !=
                        PVEData.end())
                    {
                        PVEData.erase(
                            "OasisName");
                    }

                    /*
                        Remove settings-only flags that are not needed in
                        the runtime castle object.
                    */
                    if (PVEData.find("IsHidden") !=
                        PVEData.end())
                    {
                        PVEData.erase(
                            "IsHidden");
                    }

                    if (PVEData.find("IsDisabled") !=
                        PVEData.end())
                    {
                        PVEData.erase(
                            "IsDisabled");
                    }

                    /*
                        Store the PVE definition in the same castle map used
                        by Getcastle().
                    */
                    Castles[PVECastle.AccountId] =
                        PVEData;

                    ++AddedCount;

                    Infoprint(
                        va(
                            "PvPCastle: added PVE fallback AccountId=%llu name=%s level=%u rooms=%u.",
                            (unsigned long long)
                                PVECastle.AccountId,
                            PVECastle.AccountDisplayName.empty()
                                ? "Unknown Castle"
                                : PVECastle.AccountDisplayName.c_str(),
                            PVECastle.Level,
                            PVEData["Rooms"].is_array()
                                ? (unsigned)
                                      PVEData["Rooms"]
                                          .size()
                                : 0));
                }

                Infoprint(
                    va(
                        "PvPCastle: added %u PVE fallback castles.",
                        AddedCount));
            }
            catch (std::exception &e)
            {
                Debugprint(
                    va(
                        "PvPCastle: PVE fallback loading failed: %s",
                        e.what()));
            }
        }

        // Get a player's castle.
        // Get a player's castle.
        MQEL_json Getcastle(
            uint64_t AccountID)
        {
            /*
                Load the shared castle map once.

                The map is not modified by a recurring background task.
                call_once also makes concurrent first lookups wait until
                the initial map has finished loading.
            */
            static std::once_flag CastleInitialization;

            std::call_once(
                CastleInitialization,
                []()
                {
                    Loadcastles();
                });

            auto Iterator =
                Castles.find(
                    AccountID);

            if (Iterator ==
                Castles.end())
            {
                Infoprint(
                    va(
                        "PvPCastle: no castle data found for AccountId=%llu.",
                        (unsigned long long)AccountID));

                return MQEL_json::object();
            }

    /*Normalize a copy before returning it.

     This ensures an extracted Castle_100 definition
     receives runtime fields even if its stored map
     entry was loaded before normalization was applied.
     The shared map remains unchanged.*/
            MQEL_json::object_t RuntimeCastle =
                Iterator->second;

            NormalizeRuntimePVEcastle(
                RuntimeCastle);

            Infoprint(
                va(
                    "PvPCastle: returning castle AccountId=%llu.",
                    (unsigned long long)AccountID));

            return RuntimeCastle;
        }

        // Initialize the castles on startup.
        namespace
        {
            struct Startup
            {
                Startup() {};
            };

            static Startup Loader{};
        }
    }
}