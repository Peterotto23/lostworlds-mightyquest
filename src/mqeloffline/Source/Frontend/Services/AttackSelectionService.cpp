/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 15-01-2018
    License: MIT
    Notes:
        Provides implementations for matchmaking services.
*/

#pragma once

#include "../../Stdinclude.hpp"
#include "../../Backend/PvECastleDatabase.hpp"

#include <map>

/*
    Query-string helper declaration.
*/
uint64_t GetQueryUint64(
    const std::string &Request,
    const char *Key,
    uint64_t Default);

/*
    Read a uint64 query-string parameter.

    Example:
        ?castleId=101&foo=1
*/
uint64_t GetQueryUint64(
    const std::string &Request,
    const char *Key,
    uint64_t Default)
{
    std::string Needle =
        std::string(Key) + "=";

    size_t Position =
        Request.find(
            Needle);

    if (Position == std::string::npos)
        return Default;

    Position +=
        Needle.size();

    size_t End =
        Request.find(
            '&',
            Position);

    std::string Value =
        Request.substr(
            Position,
            End == std::string::npos
                ? std::string::npos
                : End - Position);

    try
    {
        return std::stoull(
            Value);
    }
    catch (...)
    {
        return Default;
    }
}

/*
    Returns whether an AccountId belongs to the normal PVE campaign
    progression.

    These are the campaign castles represented by the early progression
    chain. Other extracted PVE castles remain available to the server
    but are not advertised through the normal progression selection.
*/
bool IsCampaignPVEcastle(
    uint64_t AccountId)
{
    switch (AccountId)
    {
    /*
        Level 1.
    */
    case 3:

    /*
        Level 2.
    */
    case 100:

    /*
        Level 3.
    */
    case 101:
    case 102:
    case 103:

    /*
        Level 4.
    */
    case 104:

    /*
        Level 5.
    */
    case 21:
    case 32:

    /*
        Level 6.
    */
    case 22:
    case 33:

        return true;

    default:
        return false;
    }
}

void GetAttackSelectionList(
    Gameserver *Server,
    std::string Request,
    std::string Body)
{
    (void)Request;
    (void)Body;

    /*
        Warm the existing castle map so StartAttack and GetCastleInfo
        can resolve the same PVE castles.
    */
    (void)Backend::PvPCastle::Getcastle(
        3);

    MQEL_json Response =
        MQEL_json::object();

    try
    {
        /*
            ------------------------------------------------------------
            BOSS CASTLE
            ------------------------------------------------------------
        */

        Response["Result"]["BossCastle"]
                ["DefenderAccountSummary"]
                ["Id"] =
                    14;

        Response["Result"]["BossCastle"]
                ["DefenderAccountSummary"]
                ["DisplayName"] =
                    "Snotter_King";

        Response["Result"]["BossCastle"]
                ["DefenderAccountSummary"]
                ["OasisNameId"] =
                    2645;

        Response["Result"]["BossCastle"]
                ["DefenderAccountSummary"]
                ["AvatarId"] =
                    10;

        Response["Result"]["BossCastle"]
                ["DefenderAccountSummary"]
                ["CastleLevel"] =
                    4;

        Response["Result"]["BossCastle"]["CastleType"] =
            1;

        Response["Result"]["BossCastle"]["Level"] =
            4;

        Response["Result"]["BossCastle"]["IsNew"] =
            true;

        Response["Result"]["BossCastle"]["IsCastleAttackable"] =
            true;

        Response["Result"]["BossCastle"]["CastleThemeId"] =
            5;

        Response["Result"]["BossCastle"]["CastleHeartRank"] =
            4;

        Response["Result"]["BossCastle"]["WinRatioDifficulty"] =
            2;

        /*
            ------------------------------------------------------------
            PLAYER LEVEL / PVE EXPOSURE
            ------------------------------------------------------------

            Hero Level determines how far through the campaign the
            client is allowed to see.

            The currently implemented campaign progression is capped
            at Level 6.

                Hero Level 1 -> expose Level 1
                Hero Level 2 -> expose Levels 1-2
                Hero Level 3 -> expose Levels 1-3
                Hero Level 4 -> expose Levels 1-4
                Hero Level 5 -> expose Levels 1-5
                Hero Level 6+ -> expose Levels 1-6

            This controls visibility in the attack-selection response.

            Quest-based progression locking is intentionally not
            hardcoded here. Campaign castles are sent to the client,
            including castles such as 100 and 101 that are marked
            IsTutorialCastle in the extracted data.
        */

        const uint32_t MaxExposedPVELevel =
            6;

        uint32_t PlayerLevel =
            1;

        MQEL_json Hero =
            Backend::Hero::Serialize();

        if (Hero.find("Level") !=
                Hero.end() &&
            Hero["Level"].is_number())
        {
            PlayerLevel =
                Hero["Level"].get<uint32_t>();
        }

        if (PlayerLevel == 0)
            PlayerLevel = 1;

        uint32_t ExposureLevel =
            PlayerLevel;

        if (ExposureLevel >
            MaxExposedPVELevel)
        {
            ExposureLevel =
                MaxExposedPVELevel;
        }

        Infoprint(
            va(
                "GetAttackSelectionList: player Hero Level=%u, exposing campaign PVE through Level %u.",
                PlayerLevel,
                ExposureLevel));

        /*
            ------------------------------------------------------------
            PVE CASTLES
            ------------------------------------------------------------
        */

        Response["Result"]["CastlesByLevel"] =
            MQEL_json::array();

        std::map<uint32_t, MQEL_json>
            CastlesByLevel;

        const auto &PVECastles =
            Backend::PvECastle::GetAll();

        uint32_t AdvertisedCastleCount =
            0;

        for (const auto &Castle :
             PVECastles)
        {
            /*
                Invalid entry.
            */
            if (Castle.AccountId == 0)
                continue;

            /*
                AccountId 2 is the special tutorial-only castle.
            */
            if (Castle.AccountId == 2)
                continue;

            /*
                AccountId 14 is the boss castle returned separately.
            */
            if (Castle.AccountId == 14)
                continue;

            /*
                Only advertise castles belonging to the actual early
                campaign progression chain.

                This is deliberately checked BEFORE IsTutorialCastle,
                because extracted campaign castles such as 100 and 101
                can carry the tutorial flag despite being real campaign
                progression castles.
            */
            if (!IsCampaignPVEcastle(
                    Castle.AccountId))
            {
                continue;
            }

            /*
                Ignore invalid zero-level entries.
            */
            if (Castle.Level == 0)
                continue;

            /*
                Explicit hidden/disabled data is still respected for
                campaign castles.
            */
            if (Castle.IsHidden ||
                Castle.IsDisabled)
            {
                continue;
            }

            /*
                Hero level is the exposure gate.
            */

            /*
                Hero level is the exposure gate.
            */
            const bool DreadKnightQuestActive =
                Backend::Quest::IsStarted(120);

            if (Castle.Level >
                    ExposureLevel &&
                !(Castle.AccountId == 21 &&
                  DreadKnightQuestActive))
            {
                continue;
            }

            const uint32_t Level =
                Castle.Level;

            /*
                Create the level bucket if necessary.
            */
            if (CastlesByLevel.find(Level) ==
                CastlesByLevel.end())
            {
                CastlesByLevel[Level] =
                    MQEL_json::object();

                CastlesByLevel[Level]["Level"] =
                    Level;

                CastlesByLevel[Level]["Castles"] =
                    MQEL_json::array();
            }

            /*
                Build the castle selection entry.
            */
            MQEL_json Entry =
                MQEL_json::object();

            /*
                Defender account summary.
            */
            Entry["DefenderAccountSummary"]
                 ["Id"] =
                     Castle.AccountId;

            if (Castle.AccountDisplayName.empty())
            {
                Entry["DefenderAccountSummary"]
                     ["DisplayName"] =
                         va(
                             "PVE Castle %llu",
                             (unsigned long long)
                                 Castle.AccountId);
            }
            else
            {
                Entry["DefenderAccountSummary"]
                     ["DisplayName"] =
                         Castle.AccountDisplayName;
            }

            if (Castle.OasisNameId != 0)
            {
                Entry["DefenderAccountSummary"]
                     ["OasisNameId"] =
                         Castle.OasisNameId;
            }

            Entry["DefenderAccountSummary"]
                 ["AvatarId"] =
                     10;

            Entry["DefenderAccountSummary"]
                 ["CastleLevel"] =
                     Level;

            /*
                Castle information.
            */
            Entry["CastleType"] =
                1;

            Entry["Level"] =
                Level;

            Entry["IsNew"] =
                true;

            /*
                Preserve the existing behavior of the selection
                response.

                The campaign castles are presented to the client;
                the client's progression UI can determine which ones
                are locked based on progression state.
            */
            Entry["IsCastleAttackable"] =
                true;

            Entry["CastleThemeId"] =
                Castle.ThemeId;

            Entry["CastleHeartRank"] =
                Level;

            Entry["WinRatioDifficulty"] =
                2;

            /*
                Add the castle to its level bucket.
            */
            CastlesByLevel[Level]
                          ["Castles"]
                              .push_back(
                                  Entry);

            ++AdvertisedCastleCount;

            Infoprint(
                va(
                    "GetAttackSelectionList: advertising campaign castle AccountId=%llu name=%s level=%u.",
                    (unsigned long long)
                        Castle.AccountId,
                    Castle.AccountDisplayName.empty()
                        ? "Unknown"
                        : Castle.AccountDisplayName.c_str(),
                    Level));
        }

        /*
            std::map keeps the level groups in ascending order.
        */
        for (const auto &Pair :
             CastlesByLevel)
        {
            Response["Result"]
                    ["CastlesByLevel"]
                        .push_back(
                            Pair.second);
        }

        Infoprint(
            va(
                "GetAttackSelectionList: advertised %u campaign PVE castles across %u exposed levels.",
                (unsigned)AdvertisedCastleCount,
                (unsigned)CastlesByLevel.size()));
    }
    catch (const std::exception &Exception)
    {
        Debugprint(
            va(
                "GetAttackSelectionList: failed building PVE list: %s",
                Exception.what()));
    }

    /*
        Refresh the HUD when the attack-selection list is requested.
        Build all notifications before sending the response.
    */
    const auto HeroState =
        Backend::Hero::Serialize();

    const int32_t CurrentGold =
        Backend::Wallet::Getamount(eCurrencytype::IGC);

    const int32_t CurrentLifeForce =
        Backend::Wallet::Getamount(eCurrencytype::Lifeforce);

    const uint32_t CurrentXP =
        HeroState["XP"].is_number()
            ? HeroState["XP"].get<uint32_t>()
            : 0;

    const uint32_t CurrentLevel =
        HeroState["Level"].is_number()
            ? HeroState["Level"].get<uint32_t>()
            : 1;

    const uint32_t HeroSpecContainerId =
        HeroState["HeroSpecContainerId"].is_number()
            ? HeroState["HeroSpecContainerId"].get<uint32_t>()
            : (uint32_t)Backend::Hero::Getheroclass();

    /*
        Mirror the live wallet fields returned by
        AccountInformationService::GetAccountInformation.

        This supplements the existing castle-selection Result;
        it does not replace the castle list or award currency.
    */
    Response["Result"]["Wallet"]["InGameCoin"] =
        CurrentGold;

    Response["Result"]["Wallet"]["LifeForce"] =
        CurrentLifeForce;

    Response["Result"]["Wallet"]["InGameCoinStorageCapacity"] =
        Backend::Wallet::Getcapacity(eCurrencytype::IGC);

    Response["Result"]["Wallet"]["LifeForceStorageCapacity"] =
        Backend::Wallet::Getcapacity(eCurrencytype::Lifeforce);

    // Keep the existing notification mechanism unchanged.
    Response["Notifications"] = MQEL_json::array();

    auto AddWalletNotification =
        [&Response](eCurrencytype Currency)
    {
        MQEL_json Notification =
            MQEL_json::object();

        Notification["$type"] =
            "HyperQuest.GameServer.Contracts.WalletUpdatedNotification, HyperQuest.GameServer.Contracts";

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

    MQEL_json XPNotification =
        MQEL_json::object();

    XPNotification["$type"] =
        "HyperQuest.GameServer.Contracts.HeroXpChangedNotification, HyperQuest.GameServer.Contracts";

    XPNotification["HeroSpecContainerId"] =
        HeroSpecContainerId;

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

void GetCastleInfo(
    Gameserver *Server,
    std::string Request,
    std::string Body)
{

    MQEL_json Response =
        MQEL_json::object();

    /*
        The client may identify the castle as castleId or
        castleAccountId depending on which map path requested it.
    */
    uint64_t CastleID =
        GetQueryUint64(
            Request,
            "castleId",
            0);

    if (CastleID == 0)
    {
        CastleID =
            GetQueryUint64(
                Request,
                "castleAccountId",
                0);
    }

    /*
        Also accept a JSON body fallback.
    */
    if (CastleID == 0 &&
        !Body.empty())
    {
        try
        {
            MQEL_json Parsed =
                MQEL_json::parse(
                    Body);

            if (!Parsed["castleId"].is_null())
            {
                CastleID =
                    Parsed["castleId"]
                        .get<uint64_t>();
            }
            else if (
                !Parsed["castleAccountId"].is_null())
            {
                CastleID =
                    Parsed["castleAccountId"]
                        .get<uint64_t>();
            }
        }
        catch (...)
        {
            /*
                Body was not JSON.
            */
        }
    }

    Infoprint(
        va(
            "GetCastleInfo: requested castle AccountId=%llu.",
            (unsigned long long)
                CastleID));

    /*
        We need an actual castle ID.
    */
    if (CastleID == 0)
    {
        Infoprint(
            "GetCastleInfo: no castle ID supplied.");

        Sendreply(
            Server,
            Response.dump());

        return;
    }

    /*
        Get the actual PVE castle.
    */
    MQEL_json Castle =
        Backend::PvPCastle::Getcastle(
            CastleID);

    if (Castle.empty())
    {
        Infoprint(
            va(
                "GetCastleInfo: castle AccountId=%llu not found.",
                (unsigned long long)
                    CastleID));

        Sendreply(
            Server,
            Response.dump());

        return;
    }

    /*
        ------------------------------------------------------------
        DEFENDER SUMMARY
        ------------------------------------------------------------
    */

    Response["Result"]
            ["DefenderAccountSummary"]
            ["Id"] =
                CastleID;

    if (Castle.find("AccountDisplayName") !=
        Castle.end())
    {
        Response["Result"]
                ["DefenderAccountSummary"]
                ["DisplayName"] =
                    Castle["AccountDisplayName"];
    }
    else
    {
        Response["Result"]
                ["DefenderAccountSummary"]
                ["DisplayName"] =
                    va(
                        "PVE Castle %llu",
                        (unsigned long long)
                            CastleID);
    }

    if (Castle.find("OasisNameId") !=
        Castle.end())
    {
        Response["Result"]
                ["DefenderAccountSummary"]
                ["OasisNameId"] =
                    Castle["OasisNameId"];
    }
    else if (
        Castle.find("OasisName") !=
            Castle.end() &&
        Castle["OasisName"].is_number())
    {
        Response["Result"]
                ["DefenderAccountSummary"]
                ["OasisNameId"] =
                    Castle["OasisName"];
    }

    Response["Result"]
            ["DefenderAccountSummary"]
            ["AvatarId"] =
                10;

    /*
        ------------------------------------------------------------
        CASTLE LEVEL
        ------------------------------------------------------------
    */

    uint32_t CastleLevel =
        1;

    if (Castle.find("Level") !=
            Castle.end() &&
        Castle["Level"].is_number())
    {
        CastleLevel =
            Castle["Level"]
                .get<uint32_t>();
    }

    Response["Result"]
            ["DefenderAccountSummary"]
            ["CastleLevel"] =
                CastleLevel;

    Response["Result"]
            ["CastleType"] =
                1;

    Response["Result"]
            ["Level"] =
                CastleLevel;

    Response["Result"]
            ["CastleHeartRank"] =
                CastleLevel;

    /*
        ------------------------------------------------------------
        ROOM COUNT
        ------------------------------------------------------------
    */

    if (Castle.find("Rooms") !=
            Castle.end() &&
        Castle["Rooms"].is_array())
    {
        Response["Result"]
                ["RoomCount"] =
                    (uint32_t)
                        Castle["Rooms"]
                            .size();
    }
    else
    {
        Response["Result"]
                ["RoomCount"] =
                    0;
    }

    /*
        ------------------------------------------------------------
        THEME
        ------------------------------------------------------------
    */

    if (Castle.find("ThemeId") !=
            Castle.end() &&
        Castle["ThemeId"].is_number())
    {
        Response["Result"]
                ["CastleThemeId"] =
                    Castle["ThemeId"];
    }
    else
    {
        Response["Result"]
                ["CastleThemeId"] =
                    1;
    }

    /*
        ------------------------------------------------------------
        DIFFICULTY / LOOT
        ------------------------------------------------------------
    */

    Response["Result"]
            ["Difficulty"] =
                2;

    Response["Result"]
            ["PotentialLoot"]
            ["Xp"] =
                90;

    Response["Result"]
            ["PotentialLoot"]
            ["TreasureRoomStealableIGC"] =
                15;

    Response["Result"]
            ["PotentialLoot"]
            ["TreasureRoomStealableLifeForce"] =
                15;

    Response["Result"]
            ["PotentialLoot"]
            ["IGC"] =
                50;

    Response["Result"]
            ["PotentialLoot"]
            ["LifeForce"] =
                50;

    Response["Result"]
            ["IsNew"] =
                true;

    Response["Result"]
            ["IsCastleAttackable"] =
                true;

    Response["Result"]
            ["AttackabilityStatus"] =
                1;

    Response["Result"]
            ["AttackType"] =
                5;

    /*
        ------------------------------------------------------------
        STATS
        ------------------------------------------------------------
    */

    Response["Result"]
            ["Stats"]
            ["TotalConstructionPoints"] =
                58;

    Response["Result"]
            ["Stats"]
            ["MaxConstructionPoints"] =
                58;

    Response["Result"]
            ["Stats"]
            ["TrapCount"] =
                13;

    Response["Result"]
            ["Stats"]
            ["WinRatio"] =
                0.5;

    Response["Result"]
            ["Stats"]
            ["WinRatioDifficulty"] =
                2;

    Response["Result"]
            ["VictoryConditionRewardRatios"] =
                MQEL_json::parse(
                    R"([1,0.75,0.5])");

    Infoprint(
        va(
            "GetCastleInfo: returning AccountId=%llu name=%s level=%u rooms=%u.",
            (unsigned long long)
                CastleID,
            Response["Result"]
                    ["DefenderAccountSummary"]
                    ["DisplayName"]
                        .get<std::string>()
                        .c_str(),
            CastleLevel,
            Response["Result"]
                    ["RoomCount"]
                        .get<uint32_t>()));

    Sendreply(
        Server,
        Response.dump());
}

/*
    Add the services to the gameserver on startup.
*/
namespace
{
    struct Startup
    {
        Startup()
        {
            Mapservice(
                "/AttackSelectionService.hqs/GetAttackSelectionList",
                GetAttackSelectionList);

            Mapservice(
                "/AttackSelectionService.hqs/GetCastleInfo",
                GetCastleInfo);
        }
    };

    static Startup Loader{};
}