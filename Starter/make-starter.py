#!/usr/bin/env python3
"""Regenerates Starter/starter.save: the save new players get on first run.

It mirrors what HeroService.cpp::ChooseFirstHero builds for a brand-new Archer, so a new
player starts at level 1 with the starter kit equipped (Pea shooter, not a leftover dev
save). Castle_3 and Quests.BB (tutorial already completed, so the stalling Goldus intro is
skipped) are carried over from the existing starter.save.

    python Starter/make-starter.py
"""
import json, os, zipfile

here = os.path.dirname(os.path.abspath(__file__))
out = os.path.join(here, "starter.save")

# Keep the castle and the completed-tutorial quest list from the current starter.
with zipfile.ZipFile(out) as z:
    castle = z.read("Castle_3")
    quests = z.read("Quests.BB")

def gear(template_id, archetype):
    return {"Type": "HeroEquipmentItem", "ItemLevel": 1, "ArchetypeId": archetype,
            "PrimaryStatsModifiers": [0.4, 0.4, 0.4], "TemplateId": template_id,
            "DyeTemplateId": 0}

heroes = {
    "Mage": None, "Knight": None,
    "Archer": {
        "HeroSpecContainerId": 3,          # eHerotype::Archer
        "Level": 1,                        # XP omitted = 0, exactly like a new hero
        "Equipment": {
            "Head":      gear(125, 8),     # Pure Ginger
            "Shoulders": gear(133, 8),     # Smirking Shrug
            "Body":      gear(126, 8),     # Trusty Underpants
            "Hands":     gear(127, 8),     # Trigger Finger
            "MainHand":  gear(124, 2),     # Pea shooter (weapons are archetype 2)
        },
        "EquippedConsumables": [{"TemplateId": 1, "SlotIndex": 0}],
        "Stats": {},
        "AttackRegions": [{"AttackRegionId": 1, "Status": 2}],
    },
    "Runaway": None, "Defaulthero": 3,
}
wallets = {str(i): {"Amount": 0, "Capacity": 0} for i in range(5)}
wallets["2"]["Amount"] = 1000      # gold
wallets["4"]["Amount"] = 10000     # life force
inventory = {"Inventory": [], "Inbox": []}

def dump(o): return json.dumps(o, indent=4).encode()
tmp = out + ".tmp"
with zipfile.ZipFile(tmp, "w", zipfile.ZIP_STORED) as z:
    z.writestr("Wallets.json", dump(wallets))
    z.writestr("Heroes.json", dump(heroes))
    z.writestr("Inventory.json", dump(inventory))
    z.writestr("Castle_3", castle)
    z.writestr("Quests.BB", quests)
os.replace(tmp, out)
print("wrote", out)
