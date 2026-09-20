/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 08-02-2018
    License: MIT
    Notes:
        Provides matchmaking castles.
*/

#include "../Stdinclude.hpp"

namespace Backend
{
    namespace PvPCastle
    {
        std::unordered_map<uint64_t /* PlayerID */, MQEL_json::object_t> Castles;

        // Populate the castle-map.
        void Loadcastles()
        {
            auto Filelist = Package::Findfiles("Castle_");

            // Iterate over the castles.
            for (auto &Item : Filelist)
            {
                // Get the castle ID safely.
                uint64_t CastleID{0};

                if (Item.rfind("Castle_", 0) != 0)
                    continue;

                try
                {
                    CastleID = std::stoull(Item.substr(7));
                }
                catch (...)
                {
                    Debugprint(va(
                        "PvPCastle: invalid castle filename \"%s\"",
                        Item.c_str()));
                    continue;
                }

                // Invalid filename or already loaded.
                if (CastleID == 0)
                    continue;

                auto &Entry = Castles[CastleID];

                if (Entry.size() > 0)
                    continue;

                Infoprint(va(
                    "PvPCastle: loading %s for AccountId=%llu",
                    Item.c_str(),
                    (unsigned long long)CastleID));

                try
                {
                    std::string Data = Package::Read(Item);

                    Infoprint(va(
                        "PvPCastle: read %zu bytes from %s",
                        Data.size(),
                        Item.c_str()));

                    Entry = MQEL_json::parse(Data);

                    Infoprint(va(
                        "PvPCastle: loaded %s (AccountId=%llu, Rooms=%zu)",
                        Item.c_str(),
                        (unsigned long long)CastleID,
                        Entry["Rooms"].is_array() ? Entry["Rooms"].size() : 0));
                }
                catch (std::exception &e)
                {
                    Debugprint(va(
                        "%s (\"%s\"): %s",
                        __FUNCTION__,
                        Item.c_str(),
                        e.what()));

                    // Do not leave a partially-created entry behind.
                    Castles.erase(CastleID);
                }
            }
        }

        // Get a players castle.
        MQEL_json Getcastle(uint64_t AccountID)
        {
            static bool Initialized = false;

            if (!Initialized)
            {
                Loadcastles();
                Createrecurringtask(Loadcastles, 5000);
                Initialized = true;
            }

            return Castles[AccountID];
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
