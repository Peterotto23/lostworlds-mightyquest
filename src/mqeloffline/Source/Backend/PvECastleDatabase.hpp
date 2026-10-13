#pragma once

#include "../Stdinclude.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>

namespace Backend
{
    namespace PvECastle
    {
        struct CastleEntry
        {
            uint64_t AccountId = 0;
            uint32_t Level = 1;
            uint32_t ThemeId = 0;
            uint32_t OasisNameId = 0;

            std::string AccountDisplayName;
            std::string Filename;

            bool IsTutorialCastle = false;
            bool IsHidden = false;
            bool IsDisabled = false;

            MQEL_json Data = MQEL_json::object();
        };

        inline std::vector<CastleEntry> &Database()
        {
            static std::vector<CastleEntry> Entries;
            return Entries;
        }

        inline bool &Loaded()
        {
            static bool Value = false;
            return Value;
        }

        inline bool HasKey(
            const MQEL_json &Object,
            const char *Key)
        {
            return Object.find(Key) != Object.end();
        }

        inline bool ReadUint64(
            const MQEL_json &Object,
            const char *Key,
            uint64_t &Value)
        {
            if (!HasKey(Object, Key) ||
                Object[Key].is_null())
            {
                return false;
            }

            try
            {
                if (Object[Key].is_number_unsigned())
                {
                    Value =
                        Object[Key].get<uint64_t>();

                    return true;
                }

                if (Object[Key].is_number_integer())
                {
                    Value =
                        (uint64_t)Object[Key].get<int64_t>();

                    return true;
                }
            }
            catch (...)
            {
            }

            return false;
        }

        inline bool ReadUint32(
            const MQEL_json &Object,
            const char *Key,
            uint32_t &Value)
        {
            uint64_t Temporary = 0;

            if (!ReadUint64(
                    Object,
                    Key,
                    Temporary))
            {
                return false;
            }

            Value =
                (uint32_t)Temporary;

            return true;
        }

        inline std::string ReadString(
            const MQEL_json &Object,
            const char *Key)
        {
            if (!HasKey(Object, Key) ||
                Object[Key].is_null() ||
                !Object[Key].is_string())
            {
                return "";
            }

            return Object[Key].get<std::string>();
        }

        inline bool ReadBool(
            const MQEL_json &Object,
            const char *Key,
            bool DefaultValue = false)
        {
            if (!HasKey(Object, Key) ||
                Object[Key].is_null() ||
                !Object[Key].is_boolean())
            {
                return DefaultValue;
            }

            return Object[Key].get<bool>();
        }

        inline bool IsPVEFilename(
            const std::string &Filename)
        {
            std::string Lower =
                Filename;

            std::transform(
                Lower.begin(),
                Lower.end(),
                Lower.begin(),
                [](unsigned char Character)
                {
                    return (char)std::tolower(
                        Character);
                });

            if (Lower.find(
                    " - pve_") ==
                std::string::npos)
            {
                return false;
            }

            if (Lower.size() < 5 ||
                Lower.substr(
                    Lower.size() - 5) !=
                    ".json")
            {
                return false;
            }

            return true;
        }

        inline void Reload()
        {
            Database().clear();

            const std::filesystem::path CastleDirectory =
                std::filesystem::path("Plugins") /
                "MQELOffline-Castles";

            if (!std::filesystem::exists(
                    CastleDirectory))
            {
                Infoprint(
                    "PvECastle: Plugins/MQELOffline-Castles does not exist.");

                Loaded() = true;
                return;
            }

            try
            {
                for (const auto &File :
                     std::filesystem::directory_iterator(
                         CastleDirectory))
                {
                    if (!File.is_regular_file())
                        continue;

                    const std::string Filename =
                        File.path().filename().string();

                    if (!IsPVEFilename(Filename))
                        continue;

                    try
                    {
                        std::ifstream Input(
                            File.path(),
                            std::ios::binary);

                        if (!Input)
                        {
                            Infoprint(
                                va(
                                    "PvECastle: failed to open %s.",
                                    Filename.c_str()));

                            continue;
                        }

                        std::string Contents(
                            (std::istreambuf_iterator<char>(
                                Input)),
                            std::istreambuf_iterator<char>());

                        MQEL_json Parsed =
                            MQEL_json::parse(
                                Contents);

                        CastleEntry Entry;

                        if (!ReadUint64(
                                Parsed,
                                "AccountId",
                                Entry.AccountId))
                        {
                            Infoprint(
                                va(
                                    "PvECastle: skipping %s because AccountId is missing.",
                                    Filename.c_str()));

                            continue;
                        }

                        Entry.Level = 1;

                        ReadUint32(
                            Parsed,
                            "Level",
                            Entry.Level);

                        ReadUint32(
                            Parsed,
                            "ThemeId",
                            Entry.ThemeId);

                        /*
                            Most castle JSONs use OasisNameId, while some of
                            the older PVE definitions only carry OasisName.
                        */
                        if (!ReadUint32(
                                Parsed,
                                "OasisNameId",
                                Entry.OasisNameId))
                        {
                            ReadUint32(
                                Parsed,
                                "OasisName",
                                Entry.OasisNameId);
                        }

                        Entry.AccountDisplayName =
                            ReadString(
                                Parsed,
                                "AccountDisplayName");

                        Entry.IsTutorialCastle =
                            ReadBool(
                                Parsed,
                                "IsTutorialCastle");

                        Entry.IsHidden =
                            ReadBool(
                                Parsed,
                                "IsHidden");

                        Entry.IsDisabled =
                            ReadBool(
                                Parsed,
                                "IsDisabled");

                        Entry.Filename =
                            Filename;

                        Entry.Data =
                            Parsed;

                        Database().push_back(
                            Entry);
                    }
                    catch (const std::exception &Exception)
                    {
                        Infoprint(
                            va(
                                "PvECastle: failed parsing %s: %s",
                                Filename.c_str(),
                                Exception.what()));
                    }
                }
            }
            catch (const std::exception &Exception)
            {
                Infoprint(
                    va(
                        "PvECastle: directory scan failed: %s",
                        Exception.what()));
            }

            std::sort(
                Database().begin(),
                Database().end(),
                [](const CastleEntry &A,
                   const CastleEntry &B)
                {
                    if (A.Level != B.Level)
                        return A.Level < B.Level;

                    return A.AccountId <
                           B.AccountId;
                });

            Infoprint(
                va(
                    "PvECastle: loaded %u PVE castle definitions.",
                    (unsigned)Database().size()));
        }

        inline const std::vector<CastleEntry> &GetAll()
        {
            static std::once_flag LoadOnce;

            std::call_once(
                LoadOnce,
                []()
                {
                    Reload();
                });

            return Database();
        }

        inline const CastleEntry *Find(
            uint64_t AccountId)
        {
            const auto &Entries =
                GetAll();

            for (const auto &Entry :
                 Entries)
            {
                if (Entry.AccountId ==
                    AccountId)
                {
                    return &Entry;
                }
            }

            return nullptr;
        }

        inline MQEL_json BuildServerCastle(
            const CastleEntry &Entry,
            eCastletype Type)
        {
            MQEL_json Result = Entry.Data;

            if (Type == eCastletype::User)
            {
                Result["$type"] =
                    "HyperQuest.GameServer.Contracts.UserCastle, HyperQuest.GameServer.Contracts";
            }
            else
            {
                Result["$type"] =
                    "HyperQuest.GameServer.Contracts.UbisoftCastle, HyperQuest.GameServer.Contracts";
            }

            Result["AccountId"] = Entry.AccountId;
            Result["AccountDisplayName"] = Entry.AccountDisplayName;
            Result["Level"] = Entry.Level;

            Result["LayoutId"] = 1;
            Result["ForceCastleLevelOnBuildables"] = true;

            /*
               The client expects runtime castle metadata rather than the
               empty settings-export values from R01Q definitions.

           */
            Result["CastleValidationDuration"] = 44.23333;

            if (!HasKey(Result, "CreationDate") ||
                !Result["CreationDate"].is_string() ||
                Result["CreationDate"].get<std::string>().empty())
            {
                Result["CreationDate"] =
                    "2016-01-01T00:00:00Z";
            }

            if (!HasKey(Result, "ModificationDate") ||
                !Result["ModificationDate"].is_string() ||
                Result["ModificationDate"].get<std::string>().empty())
            {
                Result["ModificationDate"] =
                    "2016-01-01T00:00:00Z";
            }

            if (Entry.OasisNameId != 0)
                Result["OasisNameId"] = Entry.OasisNameId;

            Result.erase("OasisName");

            /*
     Preserve the tutorial flag from the source PVE entry.

     Some early campaign castles, including Castle 101,
     explicitly carry IsTutorialCastle=true in their
     original definitions.

     Keep that value in the runtime response rather than
     unconditionally deleting it.
 */
            /*
      IsTutorialCastle is selection metadata here,
      not a runtime marker for generated campaign castles.
  */
            Result.erase("IsTutorialCastle");

            /*
                Remove the other settings-only properties that do
                not belong in the live attack castle.
            */
            Result.erase("IsDisabled");
            Result.erase("IsHidden");
            Result.erase("IsForSaleCastle");
            Result.erase("IsBranded");
            Result.erase("ActivationDate");
            Result.erase("DeActivationDate");
            Result.erase("CastleIconUrl");
            Result.erase("OasisDescription");

            return Result;
        }
    }
}
