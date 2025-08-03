#pragma once
#include <unordered_map>
#include <string>

static const std::unordered_map<std::string, std::string> EntityType = {
    {"chicken", "Chicken"},
    {"hostage_entity", "Hostage"}
};

static const std::unordered_map<std::string, std::string> ProjectilesType = {
    {"smokegrenade_projectile", "Smoke Grenade"},
    {"flashbang_projectile", "Flashbang"},
    {"hegrenade_projectile", "HE Grenade"},
    {"molotov_projectile", "Molotov"},
    {"incendiarygrenade_projectile", "Incendiary Grenade"},
    {"decoy_projectile", "Decoy Grenade"}
};

std::string GetWeaponNameByIndex(short index)
{
    switch (index)
    {
    case 1:   return "Desert Eagle";
    case 2:   return "Dual Berettas";
    case 3:   return "Five-Seven";
    case 4:   return "Glock-18";
    case 7:   return "AK-47";
    case 8:   return "AUG";
    case 9:   return "AWP";
    case 10:  return "FAMAS";
    case 11:  return "G3SG1";
    case 13:  return "Galil AR";
    case 14:  return "M249";
    case 16:  return "M4A4";
    case 17:  return "MAC-10";
    case 19:  return "P90";
    case 23:  return "MP5-SD";
    case 24:  return "UMP-45";
    case 25:  return "XM1014";
    case 26:  return "PP-Bizon";
    case 27:  return "MAG-7";
    case 28:  return "Negev";
    case 29:  return "Sawed-Off";
    case 30:  return "Tec-9";
    case 31:  return "Zeus x27";
    case 32:  return "P2000";
    case 33:  return "MP7";
    case 34:  return "MP9";
    case 35:  return "Nova";
    case 36:  return "P250";
    case 38:  return "SCAR-20";
    case 39:  return "SG 553";
    case 40:  return "SSG 08";
    case 42:  return "Knife";
    case 43:  return "Flashbang";
    case 44:  return "HE Grenade";
    case 45:  return "Smoke Grenade";
    case 46:  return "Molotov";
    case 47:  return "Decoy Grenade";
    case 48:  return "Incendiary Grenade";
    case 49:  return "C4";
    case 59:  return "T Knife";
    case 60:  return "M4A1-S";
    case 61:  return "USP-S";
    case 63:  return "CZ75-Auto";
    case 64:  return "R8 Revolver";

    case 500: return "Bayonet";
    case 505: return "Flip Knife";
    case 506: return "Gut Knife";
    case 507: return "Karambit";
    case 508: return "M9 Bayonet";
    case 509: return "Huntsman Knife";
    case 512: return "Falchion Knife";
    case 514: return "Bowie Knife";
    case 515: return "Butterfly Knife";
    case 516: return "Shadow Daggers";
    case 517: return "Paracord Knife";
    case 518: return "Survival Knife";
    case 519: return "Ursus Knife";
    case 520: return "Navaja Knife";
    case 521: return "Nomad Knife";
    case 522: return "Stiletto Knife";
    case 523: return "Talon Knife";
    case 525: return "Classic Knife";
    case 526: return "Skeleton Knife";

    default: return "Unknown";
    }
}



static const std::unordered_map<std::string, std::string> WeaponsType = {
    {"weapon_ak47", "AK-47"},
    {"weapon_m4a1", "M4A1"},
    {"weapon_awp", "AWP"},
    {"weapon_elite", "Elite"},
    {"weapon_famas", "Famas"},
    {"weapon_flashbang", "Flashbang"},
    {"weapon_g3sg1", "G3SG1"},
    {"weapon_galilar", "Galil AR"},
    {"weapon_healthshot", "Health Shot"},
    {"weapon_hegrenade", "HE Grenade"},
    {"weapon_incgrenade", "Incendiary Grenade"},
    {"weapon_m249", "M249"},
    {"weapon_m4a1_silencer", "M4A1-S"},
    {"weapon_mac10", "MAC-10"},
    {"weapon_mag7", "MAG-7"},
    {"weapon_molotov", "Molotov"},
    {"weapon_mp5sd", "MP5-SD"},
    {"weapon_mp7", "MP7"},
    {"weapon_mp9", "MP9"},
    {"weapon_negev", "Negev"},
    {"weapon_nova", "Nova"},
    {"weapon_p90", "P90"},
    {"weapon_sawedoff", "Sawed-Off"},
    {"weapon_scar20", "SCAR-20"},
    {"weapon_sg556", "SG 553"},
    {"weapon_smokegrenade", "Smoke Grenade"},
    {"weapon_ssg08", "SSG 08"},
    {"weapon_tagrenade", "TA Grenade"},
    {"weapon_taser", "Taser"},
    {"weapon_ump45", "UMP-45"},
    {"weapon_xm1014", "XM1014"},
    {"weapon_aug", "AUG"},
    {"weapon_bizon", "PP-Bizon"},
    {"weapon_decoy", "Decoy Grenade"},
    {"weapon_fiveseven", "Five-Seven"},
    {"weapon_hkp2000", "P2000"},
    {"weapon_usp_silencer", "USP-S"},
    {"weapon_p250", "P250"},
    {"weapon_tec9", "Tec-9"},
    {"weapon_cz75a", "CZ75-Auto"},
    {"weapon_deagle", "Desert Eagle"},
    {"weapon_revolver", "R8 Revolver"},
    {"weapon_glock", "Glock-18"}
};


// Function to retrieve item names
std::string GetWeaponType(const std::string& itemIdentifier)
{
    auto it = WeaponsType.find(itemIdentifier);
    if (it != WeaponsType.end())
        return it->second;
    return "<unknown>";
}

std::string GetProjectileType(const std::string& itemIdentifier)
{
    auto it = ProjectilesType.find(itemIdentifier);
    if (it != ProjectilesType.end())
        return it->second;
    return "<unknown>";
}

std::string GetEntityType(const std::string& itemIdentifier)
{
    auto it = EntityType.find(itemIdentifier);
    if (it != EntityType.end())
        return it->second;
    return "<unknown>";
}