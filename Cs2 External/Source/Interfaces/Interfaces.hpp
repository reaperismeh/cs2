#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <Globals/Globals.hpp>
#include <Interfaces/Elements.hpp>
#include <Utilities/Config.hpp>
#include "Loader.hpp"
#include "LSConfigSystem.hpp"
#include <Readers/LocalPlayer.hpp>
#include <Readers/Game.hpp>
#include <Utilities/Bomb.hpp>
#include <Utilities/Updater.hpp>
#include <Overlay/ExeConfig.hpp>

#define ALPHA    ( ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar )

//Helpers
ImDrawList* drawlist;
ImVec2 pos;
ImVec4 menuColor = ImColor(46, 108, 250, 255);
int tabs = 0;
int page = 0;
int changelog = 0;
static int animtype = 0;

namespace hotkeys
{
    int aimkey = 0; // Default to no key
    int triggerbotkey = 0;
}

std::string GetKeyName(int key)
{
    char buffer[128] = "None"; // Default name

    // Handle mouse buttons explicitly
    if (key == VK_LBUTTON) return "Left Click";
    if (key == VK_RBUTTON) return "Right Click";
    if (key == VK_MBUTTON) return "Middle Click";
    if (key == VK_XBUTTON1) return "Side Button 1";
    if (key == VK_XBUTTON2) return "Side Button 2";

    // Handle keyboard keys
    if (key > 0)
    {
        LONG scancode = MapVirtualKeyA(key, MAPVK_VK_TO_VSC) << 16;

        // Extended keys
        if (key == VK_LEFT || key == VK_UP || key == VK_RIGHT || key == VK_DOWN ||
            key == VK_PRIOR || key == VK_NEXT || key == VK_END || key == VK_HOME ||
            key == VK_INSERT || key == VK_DELETE || key == VK_DIVIDE)
        {
            scancode |= 0x01000000; // Extended flag
        }

        // Retrieve key name
        if (GetKeyNameTextA(scancode, buffer, sizeof(buffer)) > 0)
        {
            return std::string(buffer);
        }
    }
    return std::string(buffer); // Fallback to "None"
}
bool IsKeyConflict(int key, int otherKey)
{
    return key != 0 && key == otherKey; // Return true if keys are the same and not "None"
}
void ChangeKey(int& key, const int& otherKey)
{
    for (int i = 0; i < 0x87; i++) // Iterate over virtual key codes
    {
        if (GetAsyncKeyState(i) & 0x8000) // Check if key is pressed
        {
            if (IsKeyConflict(i, otherKey))
            {
                std::cout << "Key conflict detected! Choose another key." << std::endl;
                return; // Reject the key assignment
            }

            key = i; // Assign the pressed key
            return;
        }
    }

    // Check for mouse buttons
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) { key = VK_LBUTTON; return; }
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) { key = VK_RBUTTON; return; }
    if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) { key = VK_MBUTTON; return; }
    if (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) { key = VK_XBUTTON1; return; }
    if (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) { key = VK_XBUTTON2; return; }
}

void hotkey(const char* label, int& key, const int& otherKey, float samelineOffset, const ImVec2& size)
{
    static bool waitingForKey = false; // Tracks key input state

    const auto id = ImGui::GetID(label); // Unique ID for hotkey widget
    ImGui::PushID(label);

    ImGui::TextUnformatted(label); // Display label
    ImGui::SameLine(samelineOffset);

    // Retrieve the key name for display
    std::string keyName = GetKeyName(key);

    if (waitingForKey && ImGui::Button("Press a key...", size))
    {
        waitingForKey = false; // Stop waiting for key
    }
    else if (!waitingForKey && ImGui::Button(keyName.c_str(), size))
    {
        waitingForKey = true; // Start waiting for input
    }

    if (waitingForKey)
    {
        ChangeKey(key, otherKey); // Pass the conflicting key for validation
        if (key != 0) // Stop waiting after valid key press
        {
            waitingForKey = false;
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset", size))
    {
        key = 0;
    }

    ImGui::PopID();
}
void hotkey2(const char* label, int& key, const int& otherKey, float samelineOffset, const ImVec2& size)
{
    static bool waitingForKey = false; // Tracks key input state

    const auto id = ImGui::GetID(label); // Unique ID for hotkey widget
    ImGui::PushID(label);

    ImGui::TextUnformatted(label); // Display label
    ImGui::SameLine(samelineOffset);

    // Retrieve the key name for display
    std::string keyName = GetKeyName(key);

    if (waitingForKey && ImGui::Button("Press a key...", size))
    {
        waitingForKey = false; // Stop waiting for key
    }
    else if (!waitingForKey && ImGui::Button(keyName.c_str(), size))
    {
        waitingForKey = true; // Start waiting for input
    }

    if (waitingForKey)
    {
        ChangeKey(key, otherKey); // Pass the conflicting key for validation
        if (key != 0) // Stop waiting after valid key press
        {
            waitingForKey = false;
        }
    }

    ImGui::SameLine();

    // Add Reset Button
    if (ImGui::Button("Reset", size))
    {
        key = 0; // Reset to default (None)
    }

    ImGui::PopID();
}



//class Interfaces
//{
//private:
//    class Aim
//    {
//    public:
//    //    VOID GetAimbotWidgets()
//    //    {
//    //        ImGui::Text("Aimbot");                   
//    //        ImGui::BeginChild("Aimbot Container", ImVec2(0, 170), true);
//
//    //    //    Checkbox("Enable", &Flags::enableAimbot);
//    //    //    Checkbox("Team Check", &Config::Aimbot::TeamCheck);
//    //    //    Checkbox("Enable Hotkey", &Config::Aimbot::enableHotKey);
//    //    //    ComboBox(
//    //    //        "Aimbot Key",
//    //    //        Config::Aimbot::HotkeyOptions[Config::Aimbot::CurrentHotkeyIndex],
//    //    //        &Config::Aimbot::CurrentHotkeyIndex,
//    //    //        Config::Aimbot::HotkeyOptions,
//    //    //        IM_ARRAYSIZE(Config::Aimbot::HotkeyOptions),
//    //    //        2.0f,
//    //    //        150.0f
//    //    //    );
//
//    //    //    switch (Config::Aimbot::CurrentHotkeyIndex)
//    //    //    {
//    //    //    case 0: Config::Aimbot::currentKey = 0x06; break;
//    //    //    case 1: Config::Aimbot::currentKey = 0x05; break;
//    //    //    case 2: Config::Aimbot::currentKey = 0x02; break;
//    //    //    case 3: Config::Aimbot::currentKey = 0xA0; break;
//    //    //    case 4: Config::Aimbot::currentKey = 0xA2; break;
//    //    //    case 5: Config::Aimbot::currentKey = 0xA4; break;
//    //    //    }
//
//    //    //    ComboBox(
//    //    //        "Aim Position",
//    //    //        Config::Aimbot::AimPositions[Config::Aimbot::CurrentAimPosIndex],
//    //    //        &Config::Aimbot::CurrentAimPosIndex,
//    //    //        Config::Aimbot::AimPositions,
//    //    //        IM_ARRAYSIZE(Config::Aimbot::AimPositions),
//    //    //        2.0f,
//    //    //        150.0f
//    //    //    );
//
//    //    //    ImGui::EndChild();
//    //    //}
//
//    //    //VOID GetTriggerBotWidgets()
//    //    //{
//    //    //    ImGui::Text("Triggerbot");
//    //    //    ImGui::BeginChild("Triggerbot Container", ImVec2(0, 155), true);
//    //    //    Checkbox("Enable", &Flags::enableTriggerbot);
//    //    //    Checkbox("Team Check", &Config::TriggerBot::enableTeamCheck);
//    //    //    Checkbox("Enable Hotkey", &Config::TriggerBot::enableHotKey);
//
//    //    //    ComboBox(
//    //    //        "Triggerbot Key",
//    //    //        Config::TriggerBot::HotkeyOptions[Config::TriggerBot::CurrentHotkeyIndex],
//    //    //        &Config::TriggerBot::CurrentHotkeyIndex,
//    //    //        Config::TriggerBot::HotkeyOptions,
//    //    //        IM_ARRAYSIZE(Config::TriggerBot::HotkeyOptions),
//    //    //        2.0f,
//    //    //        150.0f
//    //    //    );
//
//    //    //    switch (Config::TriggerBot::CurrentHotkeyIndex)
//    //    //    {
//    //    //    case 0: Config::TriggerBot::currentKey = 0xA0; break;
//    //    //    case 1: Config::TriggerBot::currentKey = 0xA2; break;
//    //    //    case 2: Config::TriggerBot::currentKey = 0xA4; break;
//    //    //    case 3: Config::TriggerBot::currentKey = 0x06; break;
//    //    //    case 4: Config::TriggerBot::currentKey = 0x05; break;
//    //    //    default:Config::TriggerBot::currentKey = 0xA0; break;
//    //    //    }
//    //    //    ImGui::EndChild();
//    //    //}
//
//    //    //VOID GetRCSWidgets()
//    //    //{
//    //    //    ImGui::Text("Standalone Recoil Control System");
//    //    //    ImGui::BeginChild("RCS Container", ImVec2(0, 140), true);
//    //    //    ImGui::Text("Current Sensitivity : %.2f", player.Sensitivity);
//    //    //    Checkbox("Enable", &Flags::enableRCS);
//    //    //    ImGui::EndChild();
//    //    //}       
//    //};
//
//    class Visuals
//    {
//    public:
//        //VOID GetPlayerEspWidgets()
//        //{
//        //    ImGui::Text("Player Esp");
//        //    ImGui::BeginChild("VisualsContainer", ImVec2(0, 520), true);
//        //    Checkbox("Enable Player Esp", &Flags::enablePlayerEsp);
//        //    Checkbox("Team Check", &Config::Esp::enableTeamCheck);
//        //    Checkbox("Box", &Config::Esp::enableEspBoxes);
//        //    ComboBox("Box Dimension",
//        //        Config::Esp::availableBoxDimensions[Config::Esp::currentBoxDimensionIndex],
//        //        &Config::Esp::currentBoxDimensionIndex, Config::Esp::availableBoxDimensions,
//        //        IM_ARRAYSIZE(Config::Esp::availableBoxDimensions)
//        //    );
//        //    LeaveLine(1);
//        //    ColorEditor("Box Color", &Config::Esp::BoxColor);
//        //    Checkbox("Snap Lines", &Config::Esp::enableEspLines);
//        //    ColorEditor("Lines Color", &Config::Esp::LinesColor);
//        //    ComboBox("Line Position",
//        //        Config::Esp::availableSnapLinesPos[Config::Esp::currentlinePosIndex],
//        //        &Config::Esp::currentlinePosIndex, Config::Esp::availableSnapLinesPos,
//        //        IM_ARRAYSIZE(Config::Esp::availableSnapLinesPos)
//        //    );
//        //    LeaveLine(1);
//
//        //    Checkbox("Entities Name", &Config::Esp::enableEspNames);
//        //    Checkbox("Distance", &Config::Esp::enableDistanceEsp);
//        //    ColorEditor("Name Color", &Config::Esp::NameColor);
//        //    ColorEditor("Distance Text Color", &Config::Esp::DistanceColor);
//        //    Checkbox("Bones", &Config::Esp::enableEspBones);
//        //    Checkbox("Head Fill", &Config::Esp::HeadFilled);
//        //    Checkbox("Show Bone Joints", &Config::Esp::ShowBoneJoints);
//        //    ColorEditor("Bones Color", &Config::Esp::BonesColor);
//        //    ColorEditor("Head Color", &Config::Esp::HeadColor);
//        //    Checkbox("Health Bar", &Config::Esp::enableEspHealthBar);
//        //    Checkbox("Health Text", &Config::Esp::enableEspHealthPointsText);
//        //    Checkbox("Armor Bar", &Config::Esp::enableEspArmorBar);
//        //    SliderFloat("Box Thickness", &Config::Esp::BoxThickness, 0.5f, 5.0f, "%.1f");
//        //    SliderFloat("Lines Thickness", &Config::Esp::LinesThickness, 0.5f, 5.0f, "%.1f");
//        //    SliderFloat("Bones Thickness", &Config::Esp::BonesThickness, 0.5f, 5.0f, "%.1f");
//
//        //    ImGui::EndChild();
//        //}
//
//        //VOID GetC4EspWidgets()
//        //{
//        //    ImGui::Text("Bomb Esp");
//        //    ImGui::BeginChild("Bomb Esp Container", ImVec2(0, 520), true);
//        //    Checkbox("Enable Bomb Esp", &Flags::enableC4Esp);
//        //    Checkbox("Bomb Box Esp", &Config::Esp::enableC4Box);
//        //    ColorEditor("C4 Box Color", &Config::Esp::C4_Box_Color);
//        //    Checkbox("Bomb Distance", &Config::Esp::enableC4Distance);
//        //    ColorEditor("Distance Color", &Config::Esp::C4_Distance_Text_Color);
//        //    Checkbox("Bomb Status", &Config::Esp::enableC4StatusEsp);
//        //    ColorEditor("Status Color", &Config::Esp::C4_Status_Text_Color);
//        //    SliderFloat("C4 Box Thickness", &Config::Esp::C4BoxThickness, 0.5f, 5.0f, "%.1f");               
//        //    LeaveLine(1);
//        //    ImGui::EndChild();
//        //}
//
//        //VOID GetWorldEspWidgets()
//        //{
//        //    ImGui::Text("World Esp");
//        //    ImGui::BeginChild("World Esp Container", ImVec2(0, 520), true);
//        //    Checkbox("Enable World Esp", &Flags::enableWorldEsp);
//        //    Checkbox("Chicken Esp", &Config::Esp::enableChickensEsp);
//        //    Checkbox("Hostage Esp", &Config::Esp::enableHostagesEsp);
//        //    Checkbox("Weapon Esp", &Config::Esp::enableDroppedWeaponEsp);
//        //    Checkbox("Projectiles Esp", &Config::Esp::enableProjectilesEsp);
//        //    ColorEditor("Chicken Text Color", &Config::Esp::Chicken_Text_Color);
//        //    ColorEditor("Hostage Esp Box Color", &Config::Esp::World_Entities_BoxColor);
//        //    ColorEditor("Weapon Esp Text Color", &Config::Esp::Weapon_Esp_Text_Color);
//        //    ColorEditor("Projectiles Text Color", &Config::Esp::Projectiles_Text_Color);
//        //    ImGui::EndChild();
//        //}
//
//
//    };
//
//    class Memory
//    {
//    public:
//        //VOID GetMovementsWidget()
//        //{
//        //    ImGui::Text("Movement");
//        //    ImGui::BeginChild("Movement Container", ImVec2(0, 520), true);
//        //    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Not Safe to Use! Memory Written!");
//        //    Checkbox("Enable Bunny Hop", &Flags::enableBhop);
//        //    ImGui::EndChild();
//        //}
//
//        //VOID GetMemoryWidgets()
//        //{
//        //    ImGui::Text("Memory Cheats");
//        //    ImGui::BeginChild("Memory Cheats Container", ImVec2(0, 520), true);
//        //    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Not Safe to Use! Memory Written!");
//        //    Checkbox("Antiflash", &Flags::enableAntiflash);
//        //    //Checkbox("No Sky", &Flags::enableNoSky);
//        //    //Checkbox("No Spread", &Flags::enableNoSpread);
//        //    //Checkbox("Enable Third-Person-Perspective", &Flags::enableTPV);
//        //    //Checkbox("Enable Map-View", &Flags::enableMapView);
//        //    Checkbox("Fov Changer", &Flags::enableFovChanger);
//        //    SliderInt("Field-of-View", &Config::Fov::Default, 58, 140);
//        //    Checkbox("Sensitivity Changer", &Flags::enableSensiChanger);
//        //    SliderFloat("Sensitivity", &Config::Game::Sensitivity, 0.10f, 2000.0f, "%.2f");
//        //    ImGui::EndChild();
//        //}
//    };
//
//    class Misc
//    {
//    public:
//        //VOID GetKeyInfoWidgets()
//        //{
//        //    ImGui::Text("Keybind Info");
//        //    ImGui::BeginChild("Game Info Container", ImVec2(0, 235), true);
//        //    ImGui::TextColored(ImVec4(1, 1, 0, 1), " Cheat Shortcuts");
//        //    LeaveLine(1);
//
//        //    if (Flags::enableAimbot)
//        //    {
//        //        ImGui::TextColored(ImVec4(0, 183, 0, 255), " F1 : Aimbot Enable/Disable");
//        //    }
//        //    else
//        //    {
//        //        ImGui::Text(" F1 : Aimbot Enable/Disable");
//        //    }
//
//        //    if (Flags::enableTriggerbot)
//        //    {
//        //        ImGui::TextColored(ImVec4(0, 183, 0, 255), " F2 : Triggerbot Enable/Disable");
//        //    }
//        //    else
//        //    {
//        //        ImGui::Text(" F2 : Triggerbot Enable/Disable");
//        //    }
//
//        //    if (Flags::enablePlayerEsp)
//        //    {
//        //        ImGui::TextColored(ImVec4(0, 183, 0, 255), " F3 : Esp Enable/Disable");
//        //    }
//        //    else
//        //    {
//        //        ImGui::Text(" F3 : Esp Enable/Disable");
//        //    }
//
//        //    //if (Flags::enableTPV)
//        //    //{
//        //    //    ImGui::TextColored(ImVec4(0, 183, 0, 255), " V : Tpp Enable/Disable");
//        //    //}
//        //    //else
//        //    //{
//        //    //    ImGui::Text(" V : Tpp Enable/Disable");
//        //    //}
//
//        //    LeaveLine(1);
//        //    ImGui::TextColored(ImVec4(1, 1, 0, 1), " Menu Shortcuts");
//        //    LeaveLine(1);
//        //    ImGui::Text(" Insert : HIDE/UNHIDE Menu");
//        //    ImGui::Text(" End : Close Program");
//        //    ImGui::EndChild();
//        //}
//
//        //VOID GetOffsetsDownloaderWidgets()
//        //{
//        //    ImGui::Text("Offsets Downloader");
//        //    ImGui::BeginChild("Offsets Downloader Container", ImVec2(0, 200), true);
//
//        //    if (CustomButton("Download Latest Offsets", ImVec2(200.0, 40.0)))
//        //    {
//        //        updater.CheckAndDownload();
//        //    }
//
//        //    PlainText("If you're unable to download offsets using the button above, try downloading them");
//        //    PlainText("manually from the provided links.");
//        //    const char* Offsets_Json = "https://github.com/a2x/cs2-dumper/raw/main/output/offsets.json";
//        //    const char* Client_Dll_Json = "https://github.com/NeoXa7/Cpp-Updater-Class-for-CS2-Offsets/raw/main/Client_Dll.json";
//        //    const char* Buttons_Json = "https://github.com/NeoXa7/Cpp-Updater-Class-for-CS2-Offsets/raw/main/Client_Dll.json";
//        //    ImGui::TextLinkOpenURL("Offsets.json", Offsets_Json);
//        //    ImGui::TextLinkOpenURL("Client_Dll.json", Client_Dll_Json);
//        //    ImGui::TextLinkOpenURL("Buttons.json", Buttons_Json);
//
//        //    ImGui::EndChild();
//        //}
//
//        //VOID GetOtherSettingsWidget()
//        //{
//        //    PlainText("Other Settings");
//        //    ImGui::BeginChild("Other Settings Container", ImVec2(0, 120), true);
//
//        //    Checkbox("Show Console", &Flags::g_showConsole);
//        //    Checkbox("Show Warning", &Flags::g_showWarningBox);
//
//        //    if(ImGui::Button("Save Settings"))
//        //    {
//        //        SaveSettings("settings.json");
//        //    }
//
//        //    ImGui::EndChild();
//        //}
//
//        //VOID GetGameInfoWidgets()
//        //{
//        //    ImGui::Text("Game Info");
//        //    ImGui::BeginChild("Game Info Container", ImVec2(0, 520), true);
//
//        //    ImGui::Text("Local Player : %s", player.Name);
//        //    ImGui::Text("Ping : %d", player.Ping);
//        //    ImGui::Text("Sensitivity : %.2f", player.Sensitivity);
//        //    ImGui::Text("Mapname : %s", game.MapName);
//        //   // ImGui::Text("Gamemode: %s", game.GetGameModeName().c_str());
//
//        //    // local team;
//        //    switch (player.Team)
//        //    {
//        //    case 1: ImGui::TextColored(Config::Game::SpectatorColor, "Team : Spectator "); break;
//        //    case 2: ImGui::TextColored(Config::Game::TerroristColor, "Team : Terrorist "); break;
//        //    case 3: ImGui::TextColored(Config::Game::CounterTerroristColor, "Team : Counter-Terrorist "); break;
//        //    default: ImGui::Text("Team : ~Unknown"); break;
//        //    }
//
//        //    // local player health;
//        //    ImGui::Text("Health : %d", player.Health);
//
//        //    if (player.Health > 0)
//        //    {
//        //        ImGui::Text("Status : Alive");
//        //    }
//        //    else if (player.Health <= 0)
//        //    {
//        //        ImGui::Text("Status : Dead");
//        //    }
//        //    else
//        //    {
//        //        ImGui::Text("Status : ~Unknown");
//        //    }
//
//        //    // bomb info;
//        //    if (game.BombPlanted)
//        //    {
//        //        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Bomb Status : Planted");
//        //        if (c4.GetBombSite() == 0)
//        //        {
//        //            ImGui::Text("Bomb Site : A");
//        //        }
//        //        else if (c4.GetBombSite() == 1)
//
//        //        {
//        //            ImGui::Text("Bomb Site : B");
//        //        }
//
//        //        if (game.IsBombBeingDefused)
//        //        {
//        //            ImGui::TextColored(ImVec4(0, 183, 0, 255), "Bomb Is Being Defused");
//        //        }
//        //    }
//        //    else
//        //    {
//        //        if (game.BombDropped)
//        //        {
//        //            ImGui::TextColored(ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f), "Bomb Status : Dropped");
//        //        }
//        //        else
//        //        {
//        //            ImGui::TextColored(ImVec4(0, 183, 0, 255), "Bomb Status : Not-Planted");
//        //        }
//        //    }
//
//        //    ImGui::EndChild();
//        //}
//
//        //VOID GetEntityListWidgets()
//        //{
//        //    ImGui::Text("Entity List");
//        //    ImGui::BeginChild("Entity List Container", ImVec2(0, 520), true);
//
//        //    for (int i = 1; i < 64; i++)
//        //    {
//        //        uintptr_t listEntry = mem.ReadMemory<uintptr_t>(game.EntityList + (8 * (i & 0x7FFF) >> 9) + 16);
//        //        if (!listEntry)
//        //            continue;
//
//        //        uintptr_t currentController = mem.ReadMemory<uintptr_t>(listEntry + 120 * (i & 0x1FF));
//        //        if (!currentController)
//        //            continue;
//
//        //        uint32_t pawnHandle = mem.ReadMemory<uint32_t>(currentController + Offsets::m_hPlayerPawn);
//        //        if (!pawnHandle)
//        //            continue;
//
//        //        uintptr_t listEntry2 = mem.ReadMemory<uintptr_t>(game.EntityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 16);
//        //        if (!listEntry2)
//        //            continue;
//
//        //        uintptr_t currentPawn = mem.ReadMemory<uintptr_t>(listEntry2 + 120 * (pawnHandle & 0x1FF));
//        //        if (!currentPawn)
//        //            continue;
//
//        //        entities->UpdateEntities(currentPawn, currentController);
//
//        //        /*if (player.Team == entities->Team)
//        //            continue;*/
//
//        //        if (ImGui::TreeNode(entities->Names))
//        //        {
//        //            // Team Info
//        //            switch (entities->Team)
//        //            {
//        //            case 1: ImGui::TextColored(Config::Game::SpectatorColor, "Team : Spectator "); break;
//        //            case 2: ImGui::TextColored(Config::Game::TerroristColor, "Team : Terrorist "); break;
//        //            case 3: ImGui::TextColored(Config::Game::CounterTerroristColor, "Team : Counter-Terrorist "); break;
//        //            default: ImGui::Text("Team : ~Unknown"); break;
//        //            }
//
//        //            // Health and Armor Info
//        //            ImGui::Text("Health: %d", entities->Health);
//        //            ImGui::Text("Armor: %d", entities->Armor);
//        //            if (entities->Health > 0)
//        //            {
//        //                ImGui::Text("Status : Alive");
//        //            }
//        //            else if (entities->Health <= 0)
//        //            {
//        //                ImGui::Text("Status : Dead");
//        //            }
//        //            else
//        //            {
//        //                ImGui::Text("Status : ~Unknown");
//        //            }
//
//        //            // Fire State and isScoped Info
//        //            if (entities->IsFiring)
//        //            {
//        //                ImGui::Text("IsFiring : True");
//        //            }
//        //            else
//        //            {
//        //                ImGui::Text("IsFiring : False");
//        //            }
//
//        //            if (entities->IsScoped)
//        //            {
//        //                ImGui::Text("IsScoped : True");
//        //            }
//        //            else
//        //            {
//        //                ImGui::Text("IsScoped : False");
//        //            }
//
//        //            // Ping and Position Info;
//        //            ImGui::Text("Ping : %d", entities->Ping);
//        //            ImGui::Text("Position (x, y, z) : %.2f, %.2f, %.2f", entities->Position.x, entities->Position.y, entities->Position.z);
//
//        //            ImGui::TreePop();
//        //        }
//        //    }
//
//        //    ImGui::EndChild();
//        //}
//
//
//    };
//
//
//
//    friend VOID RenderMenu();
//};
VOID ProcessInputs()
{
    if (GetAsyncKeyState(VK_INSERT) & 1) 
    {
        Flags::IsVisible = !Flags::IsVisible; // Hide/Unhide Menu;
    }

    if (GetAsyncKeyState(VK_END) & 1) {
        Flags::IsRunning = false; // Close Program;
    }
}

VOID RenderMenu()
{
    if (Config::Esp::watermark)
    {
        ImGui::PushFont(brandfont); 
        ImColor Blue = ImColor(0, 122, 255, 255);
        std::string Watermark = "CounterStrike 2 External";
        ImVec2 textSize = ImGui::CalcTextSize(Watermark.c_str());
        ImVec2 watermarkPos(SCREEN_WIDTH - textSize.x - 20, 20);
        ImGui::GetForegroundDrawList()->AddText(watermarkPos, Blue, Watermark.c_str());
        ImGui::PopFont();
    }

    ProcessInputs();
    SetCustomStyle();
    if (Flags::IsVisible)
    {
        // Static variables for the config tab
        static char newConfigNameBuffer[128] = "";
        static int selectedConfig = -1;
        static bool initialScan = false;
        if (!initialScan) {
            ScanDirectoryForJsonFiles("."); // Initial scan of current directory
            initialScan = true;
        }

        ImGui::Begin("Cs2 External Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus);
        ImGui::SetWindowSize(ImVec2(570, 610));

        drawlist = ImGui::GetWindowDrawList();
        pos = ImGui::GetWindowPos();

        // Drawing code for menu background and border...
        drawlist->AddRectFilled(pos, ImVec2(pos.x + 560, pos.y + 600), ImColor(17, 17, 17, 255), 8.f);
        drawlist->AddRectFilled(pos, ImVec2(pos.x + 150, pos.y + 600), ImColor(23, 23, 23, 255), 8.f, ImDrawFlags_RoundCornersTop);
        drawlist->AddLine(ImVec2(pos.x + 150, pos.y), ImVec2(pos.x + 150, pos.y + 600), ImColor(38, 38, 38, 255), 1.5f);
        ImGui::GetForegroundDrawList()->AddRect(pos, ImVec2(pos.x + 560, pos.y + 600), ImColor(menuColor), 8.f, 0, 3.5f);
        ImGui::GetForegroundDrawList()->AddRect(pos, ImVec2(pos.x + 560, pos.y + 600), ImColor(menuColor), 8.f, 0, 1.5f);

        // Branding text
        ImGui::PushFont(brandfont);
        ImGui::SetCursorPos(ImVec2(20, 440));
        ImGui::Text("Counter");
        ImGui::SetCursorPos(ImVec2(25, 465));
        ImGui::TextColored(menuColor, "Strike II");
        ImGui::PopFont();
        ImGui::PushFont(brandfont);
        ImGui::SetCursorPos(ImVec2(15, 490));
        ImGui::Text("External");
        ImGui::PopFont();

        // Tabs
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 8));
        ImGui::SetCursorPos(ImVec2(15, 25));
        ImGui::BeginGroup();
        {
            if (ImGui::Tabs("Aimbot", "A", ImVec2(130, 50), tabs == 1)) tabs = 1;
            if (ImGui::Tabs("Visuals", "B", ImVec2(130, 50), tabs == 0)) tabs = 0;
            if (ImGui::Tabs("rage", "D", ImVec2(130, 50), tabs == 2)) tabs = 2;
            if (ImGui::Tabs("Configs", "E", ImVec2(130, 50), tabs == 3)) tabs = 3;
            if (ImGui::Tabs("Customize", "C", ImVec2(130, 50), tabs == 4)) tabs = 4;
        }
        ImGui::EndGroup();
        ImGui::PopStyleVar();

        // --- Aimbot Tab (tabs == 1) ---
        if (tabs == 1)
        {
            ImGui::SetCursorPos(ImVec2(175, 25));
            ImGui::BeginChild("ChildAimbot", ImVec2(370, 560), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            {
                ImGui::PushFont(brandfont);
                ImGui::SetCursorPos(ImVec2(15, 13));
                ImGui::Text("Aimbot");
                ImGui::PopFont();
                drawlist = ImGui::GetWindowDrawList();
                pos = ImGui::GetWindowPos();
                drawlist->AddLine(ImVec2(pos.x, pos.y + 40), ImVec2(pos.x + 370, pos.y + 40), ImColor(38, 38, 38, 255), 1.5f);

                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::SetCursorPos(ImVec2(0, 45));
                ImGui::BeginChild("ChildAimbotFunctions", ImVec2(370, 515));
                {
                    ImGui::SetCursorPos(ImVec2(15, 10));
                    ImGui::BeginGroup();
                    {
                        ImGui::Checkbox("Enable Aimbot", &Flags::enableAimbot);
                        ImGui::Checkbox("Aimbot Team Check", &Config::Aimbot::TeamCheck);
                        ImGui::Checkbox("vis check", &Config::Aimbot::VisCheck);
                        hotkey2("Aim Key", hotkeys::aimkey, hotkeys::triggerbotkey, 80.0f, ImVec2(100, 25));
                        ImGui::SliderFloat("Aimbot Smooth", &Config::Aimbot::Smoothing, 1.0f, 30.0f, "%.1f");
                        ImGui::SliderFloat("Aimbot FOV", &Config::Aimbot::aimfov, 10.f, 1000.0f, "%.f");
                        ImGui::Checkbox("Show FOV Circle", &Config::Aimbot::showFov);
                        ImGui::Combo("Aim Position", &Config::Aimbot::CurrentAimPosIndex, Config::Aimbot::AimPositions, IM_ARRAYSIZE(Config::Aimbot::AimPositions));
                        ImGui::Separator();
                        ImGui::Text("Movement Prediction Settings:");
                        ImGui::Checkbox("Enable Prediction", &Config::Aimbot::enablePrediction);
                        ImGui::Separator();
                        ImGui::Checkbox("Enable TriggerBot", &Flags::enableTriggerbot);
                        if (Flags::enableTriggerbot)
                        {
                            if (!Config::TriggerBot::enableAlwaysActive)
                            {
                                hotkey("Triggerbot Key", hotkeys::triggerbotkey, hotkeys::aimkey, 130.0f, ImVec2(90, 25));
                            }
                            ImGui::Checkbox("Always Active Mode", &Config::TriggerBot::enableAlwaysActive);
                            ImGui::Checkbox("Team Check", &Config::TriggerBot::enableTeamCheck);
                        }
                    }
                    ImGui::EndGroup();
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
            }
            ImGui::EndChild();
        }

        // --- Visuals Tab (tabs == 0) ---
        if (tabs == 0)
        {
            ImGui::SetCursorPos(ImVec2(175, 25));
            ImGui::BeginChild("ChildVisuals", ImVec2(370, 560), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            {
                ImGui::PushFont(brandfont);
                ImGui::SetCursorPos(ImVec2(15, 13));
                ImGui::Text("Visuals");
                ImGui::PopFont();
                drawlist = ImGui::GetWindowDrawList();
                pos = ImGui::GetWindowPos();
                drawlist->AddLine(ImVec2(pos.x, pos.y + 40), ImVec2(pos.x + 370, pos.y + 40), ImColor(38, 38, 38, 255), 1.5f);
                
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::SetCursorPos(ImVec2(0, 45));
                ImGui::BeginChild("ChildVisualsFunctions", ImVec2(370, 515));
                {
                    ImGui::SetCursorPos(ImVec2(15, 10));
                    ImGui::BeginGroup();
                    {
                        ImGui::Checkbox("Watermark", &Config::Esp::watermark);
                        ImGui::Checkbox("Enable Player Esp", &Flags::enablePlayerEsp);
                        ImGui::Checkbox("Team Check", &Config::Esp::enableTeamCheck);
                        ImGui::Checkbox("Box", &Config::Esp::enableEspBoxes);
                        if (Config::Esp::enableEspBoxes){ ImGui::Combo("Box Dimension", &Config::Esp::currentBoxDimensionIndex, Config::Esp::availableBoxDimensions, IM_ARRAYSIZE(Config::Esp::availableBoxDimensions)); }
                        if (ImGui::Checkbox("Snap Lines", &Config::Esp::enableEspLines)){ ImGui::Combo("Line Position", &Config::Esp::currentlinePosIndex, Config::Esp::availableSnapLinesPos, IM_ARRAYSIZE(Config::Esp::availableSnapLinesPos)); }
                        ImGui::Checkbox("Show Weapon Name", &Config::Esp::enableWeaponNameOnPlayers);
                        ImGui::Checkbox("Show Name", &Config::Esp::enableEspNames);
                        ImGui::Checkbox("Show Distance", &Config::Esp::enableDistanceEsp);
                        ImGui::Checkbox("Show Bones", &Config::Esp::enableEspBones);
                        ImGui::Checkbox("Show Head Fill", &Config::Esp::HeadFilled);
                        ImGui::Checkbox("Show Bone Joints", &Config::Esp::ShowBoneJoints);
                        ImGui::Checkbox("Show Health Bar", &Config::Esp::enableEspHealthBar);
                        ImGui::Checkbox("Show Armor Bar", &Config::Esp::enableEspArmorBar);
                        ImGui::SliderFloat("Box Thickness", &Config::Esp::BoxThickness, 0.5f, 5.0f, "%.1f");
                        ImGui::SliderFloat("Lines Thickness", &Config::Esp::LinesThickness, 0.5f, 5.0f, "%.1f");
                        ImGui::SliderFloat("Bones Thickness", &Config::Esp::BonesThickness, 0.5f, 5.0f, "%.1f");
                        ImGui::Separator();
                        ImGui::Checkbox("Player Flashed", &Config::Esp::PlayerFlashed);
                        ImGui::Checkbox("Player Defusing", &Config::Esp::PlayerDefusing);
                        ImGui::Separator();
                        ImGui::Checkbox("Enable Bomb Esp", &Flags::enableC4Esp);
                        ImGui::Checkbox("Bomb Box Esp", &Config::Esp::enableC4Box);
                        ImGui::Checkbox("Bomb Distance", &Config::Esp::enableC4Distance);
                        ImGui::Checkbox("Bomb Status", &Config::Esp::enableC4StatusEsp);
                        ImGui::SliderFloat("C4 Box Thickness", &Config::Esp::C4BoxThickness, 0.5f, 5.0f, "%.1f");
                        ImGui::Separator();
                        ImGui::Checkbox("Enable World Esp", &Flags::enableWorldEsp);
                        ImGui::Checkbox("Chicken Esp", &Config::Esp::enableChickensEsp);
                        ImGui::Checkbox("Hostage Esp", &Config::Esp::enableHostagesEsp);
                        ImGui::Checkbox("Weapon Esp", &Config::Esp::enableDroppedWeaponEsp);
                        ImGui::Checkbox("Projectiles Esp", &Config::Esp::enableProjectilesEsp);
                    }
                    ImGui::EndGroup();
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
            }
            ImGui::EndChild();
        }

        // --- Exploits Tab (tabs == 2) ---
        if (tabs == 2)
        {
            ImGui::SetCursorPos(ImVec2(175, 25));
            ImGui::BeginChild("ChildExploits", ImVec2(370, 560), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            {
                ImGui::PushFont(brandfont);
                ImGui::SetCursorPos(ImVec2(15, 13));
                ImGui::Text("Exploits");
                ImGui::PopFont();
                drawlist = ImGui::GetWindowDrawList();
                pos = ImGui::GetWindowPos();
                drawlist->AddLine(ImVec2(pos.x, pos.y + 40), ImVec2(pos.x + 370, pos.y + 40), ImColor(38, 38, 38, 255), 1.5f);

                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::SetCursorPos(ImVec2(0, 45));
                ImGui::BeginChild("ChildExploitsFunctions", ImVec2(370, 515));
                {
                    ImGui::SetCursorPos(ImVec2(15, 10));
                    ImGui::BeginGroup();
                    {
                        ImGui::TextColored(ImColor(252, 3, 3, 255), "All Of These Write To Memory\nDeemed Not Safe For Main Accounts");
                        ImGui::Separator();
                        ImGui::Columns(2, "exploits_columns", false);
                        ImGui::Checkbox("Bunny Hop", &Config::Bhop::enabled);
                        ImGui::Checkbox("Fov Changer", &Flags::enableFovChanger);
                        ImGui::NextColumn();
                        ImGui::Checkbox("Antiflash", &Flags::enableAntiflash);
                        ImGui::Checkbox("No Recoil", &Flags::enableRCS);
                        ImGui::Columns(1);
                        ImGui::Separator();
                        if (Flags::enableFovChanger) {
                            ImGui::SliderInt("Field-of-View", &Config::Fov::Default, 60, 140);
                        }
                    }
                    ImGui::EndGroup();
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
            }
            ImGui::EndChild();
        }

        // --- Configs Tab (tabs == 3) | Compact ---
        if (tabs == 3)
        {
            ImGui::SetCursorPos(ImVec2(175, 25));
            ImGui::BeginChild("ChildConfigs", ImVec2(370, 560), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            {
                ImGui::PushFont(brandfont);
                ImGui::SetCursorPos(ImVec2(15, 13));
                ImGui::Text("Configs");
                ImGui::PopFont();
                drawlist = ImGui::GetWindowDrawList();
                pos = ImGui::GetWindowPos();
                drawlist->AddLine(ImVec2(pos.x, pos.y + 40), ImVec2(pos.x + 370, pos.y + 40), ImColor(38, 38, 38, 255), 1.5f);

                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::SetCursorPos(ImVec2(0, 45));
                ImGui::BeginChild("ChildConfigsFunctions", ImVec2(370, 515));
                {
                    ImGui::SetCursorPos(ImVec2(15, 10));
                    ImGui::BeginGroup();
                    {
                        if (ImGui::Button("Refresh List", ImVec2(-1, 25))) {
                            ScanDirectoryForJsonFiles(".");
                            selectedConfig = -1;
                        }

                        ImGui::Text("Available Configs");
                        ImGui::BeginChild("ConfigList", ImVec2(0, 200), true);
                        {
                            for (int i = 0; i < jsonFiles.size(); ++i) {
                                if (ImGui::Selectable(jsonFiles[i].c_str(), selectedConfig == i)) {
                                    selectedConfig = i;
                                }
                            }
                        }
                        ImGui::EndChild();

                        float buttonWidth = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) / 2.0f;
                        if (ImGui::Button("Load Selected", ImVec2(buttonWidth, 25))) {
                            if (selectedConfig != -1) LoadConfig(jsonFiles[selectedConfig]);
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Save to Selected", ImVec2(buttonWidth, 25))) {
                            if (selectedConfig != -1) {
                                std::string name = jsonFiles[selectedConfig];
                                if (name.length() > 5 && name.substr(name.length() - 5) == ".json") {
                                    name = name.substr(0, name.length() - 5);
                                }
                                SaveConfig(name);
                            }
                        }
                        ImGui::Separator();
                        
                        // FIX for text input not working
                        ImGui::PushItemWidth(-100); // Leave 100px for the button
                        ImGui::InputTextWithHint("##NewConfigName", "New config name...", newConfigNameBuffer, sizeof(newConfigNameBuffer));
                        ImGui::PopItemWidth();
                        ImGui::SameLine();
                        if (ImGui::Button("Save New", ImVec2(95, 0))) {
                            if (strlen(newConfigNameBuffer) > 0) {
                                std::string newName = newConfigNameBuffer;
                                if (newName.length() > 5 && newName.substr(newName.length() - 5) == ".json") {
                                    newName = newName.substr(0, newName.length() - 5);
                                }
                                if (!newName.empty()) {
                                    SaveConfig(newName);
                                    ScanDirectoryForJsonFiles(".");
                                    newConfigNameBuffer[0] = '\0';
                                }
                            }
                        }
                    }
                    ImGui::EndGroup();
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
            }
            ImGui::EndChild();
        }

        // --- Customize Tab (tabs == 4) | NEW Full-Width Layout ---
        if (tabs == 4)
        {
            ImGui::SetCursorPos(ImVec2(175, 25));
            ImGui::BeginChild("ChildCustomize", ImVec2(370, 560), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            {
                ImGui::PushFont(brandfont);
                ImGui::SetCursorPos(ImVec2(15, 13));
                ImGui::Text("Customize");
                ImGui::PopFont();
                drawlist = ImGui::GetWindowDrawList();
                pos = ImGui::GetWindowPos();
                drawlist->AddLine(ImVec2(pos.x, pos.y + 40), ImVec2(pos.x + 370, pos.y + 40), ImColor(38, 38, 38, 255), 1.5f);

                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::SetCursorPos(ImVec2(0, 45));
                ImGui::BeginChild("ChildCustomizeFunctions", ImVec2(370, 515));
                {
                    ImGui::SetCursorPos(ImVec2(15, 10));
                    ImGui::BeginGroup();
                    {
                        if (ImGui::Button("Panic Key", ImVec2(120, 25))) { exit(0); }
                        ImGui::SameLine();
                        ImGui::ColorEdit4("Menu Color", (float*)&menuColor, ALPHA);

                        ImGui::Separator();
                        ImGui::Text("ESP COLORS");
                        float esp_width = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 2) / 3.0f;
                        ImGui::PushItemWidth(esp_width);
                        ImGui::ColorEdit4("Box", (float*)&Config::Esp::BoxColor, ALPHA); ImGui::SameLine();
                        ImGui::ColorEdit4("Lines", (float*)&Config::Esp::LinesColor, ALPHA); ImGui::SameLine();
                        ImGui::ColorEdit4("Name", (float*)&Config::Esp::NameColor, ALPHA);
                        ImGui::ColorEdit4("Distance", (float*)&Config::Esp::DistanceColor, ALPHA); ImGui::SameLine();
                        ImGui::ColorEdit4("Bones", (float*)&Config::Esp::BonesColor, ALPHA); ImGui::SameLine();
                        ImGui::ColorEdit4("Head", (float*)&Config::Esp::HeadColor, ALPHA);
                        ImGui::PopItemWidth();
                        
                        ImGui::Separator();
                        ImGui::Text("BOMB COLORS");
                        float bomb_width = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 2) / 3.0f;
                        ImGui::PushItemWidth(bomb_width);
                        ImGui::ColorEdit4("Status##C4", (float*)&Config::Esp::C4_Status_Text_Color, ALPHA); ImGui::SameLine();
                        ImGui::ColorEdit4("Distance##C4", (float*)&Config::Esp::C4_Distance_Text_Color, ALPHA); ImGui::SameLine();
                        ImGui::ColorEdit4("Box##C4", (float*)&Config::Esp::C4_Box_Color, ALPHA);
                        ImGui::PopItemWidth();
                        
                        ImGui::Separator();
                        ImGui::Text("WORLD COLORS");
                        float world_width = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 3) / 4.0f;
                        ImGui::PushItemWidth(world_width);
                        ImGui::ColorEdit4("Chicken", (float*)&Config::Esp::Chicken_Text_Color, ALPHA); ImGui::SameLine();
                        ImGui::ColorEdit4("Hostage", (float*)&Config::Esp::World_Entities_BoxColor, ALPHA); ImGui::SameLine();
                        ImGui::ColorEdit4("Weapon", (float*)&Config::Esp::Weapon_Esp_Text_Color, ALPHA); ImGui::SameLine();
                        ImGui::ColorEdit4("Projectile", (float*)&Config::Esp::Projectiles_Text_Color, ALPHA);
                        ImGui::PopItemWidth();
                    }
                    ImGui::EndGroup();
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
            }
            ImGui::EndChild();
        }

        ImGui::End(); // End the window
    } 
}