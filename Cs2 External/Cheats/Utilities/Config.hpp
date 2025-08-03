#pragma once
#include <Maths/Vector.hpp>
#include <Readers/LocalPlayer.hpp>

// Forward declaration
extern class LocalPlayer player;

namespace Config {
    namespace Aimbot {
        inline ImColor fovColor = ImColor(255, 255, 255, 255);
        inline bool showFov = false;
        inline float aimfov = 50.f;
        inline float Smoothing = 5.f;
        inline bool enableHotKey = true;
        inline const char* HotkeyOptions[] = {
            "X2_Mouse Button",
            "X1_Mouse Button",
            "Right Mouse Button",
            "Left Shift",
            "Left CTRL",
            "Left Alt"
        };
        inline int CurrentHotkeyIndex = 0;
        inline int currentKey = 0x06;
        inline const char* AimPositions[] = {
            "Head",
            "Penis",
            "Neck",
            "Chest",
            "Stomach"
        };
        inline int CurrentAimPosIndex = 0;
        inline bool TeamCheck = true;
        inline bool VisCheck = true;
        inline bool enablePrediction = true;
        inline float predictionTime = .03f;
    }

    namespace Game {
        inline ImColor TerroristColor = ImColor(234, 209, 139, 255);
        inline ImColor CounterTerroristColor = ImColor(182, 212, 238, 255);
        inline ImVec4 SpectatorColor = ImVec4(1.f, 1.f, 0.f, 1.f);
        inline bool DrawCrosshair = false;
        inline float CrosshairSize = 10.0f;
        inline ImColor CrosshairColor = ImColor(255, 255, 255, 255);
        inline float Sensitivity = player.Sensitivity;
    }

    namespace TriggerBot {
        inline bool enableHotKey = true;
        inline bool enableAlwaysActive = false;
        inline bool enableTeamCheck = true;
        inline const char* HotkeyOptions[] = {
            "Left Shift",
            "Left CTRL",
            "Left Alt",
            "X1_Mouse Button",
            "X2_Mouse Button"
        };
        inline int CurrentHotkeyIndex = 0;
        inline int currentKey = 0xA0;
    }

    namespace RCS {
        inline float oldPunch_x = 0.0f;
        inline float oldPunch_y = 0.0f;
    }

    namespace Esp {
        inline bool watermark = true;
        inline bool enableEspBoxes = true;
        inline bool enableEspLines = false;
        inline bool enableEspHealthBar = true;
        inline bool enableEspArmorBar = true;
        inline bool enableEspNames = false;
        inline bool enableDistanceEsp = false;
        inline bool enableWeaponNameOnPlayers = true;
        inline bool PlayerFlashed = true;
        inline bool PlayerDefusing = true;
        inline bool VisCheck = true;
        inline bool enableEspBones = true;
        inline bool enableEspHealthPointsText = false;
        inline bool enableEspArmorPointsText = true;
        inline bool enableTeamCheck = true;
        inline bool HeadFilled = false;
        inline bool ShowBoneJoints = false;
        inline bool enableC4Box = true;
        inline bool enableC4StatusEsp = true;
        inline bool enableC4Distance = true;
        inline bool enableChickensEsp = true;
        inline bool enableHostagesEsp = true;
        inline bool enableDroppedWeaponEsp = true;
        inline bool enableProjectilesEsp = true;

        inline ImColor BoxColor = ImColor(255, 255, 255, 255);
        inline ImColor VisibleBoxColor = ImColor(0, 183, 0, 255);
        inline ImColor HealthBarColor = ImColor(0, 255, 0, 255);
        inline ImColor ArmorBarColor = ImColor(182, 212, 238, 255);
        inline ImColor LinesColor = ImColor(255, 255, 255, 255);
        inline ImColor NameColor = ImColor(255, 255, 255, 255);
        inline ImColor BonesColor = ImColor(255, 255, 255, 255);
        inline ImColor HeadColor = ImColor(255, 255, 255, 255);
        inline ImColor DistanceColor = ImColor(255, 255, 255, 255);
        inline ImColor IsFlashColor = ImColor(255, 255, 0, 255);
        inline ImColor C4_Box_Color = ImColor(255, 255, 255, 255);
        inline ImColor C4_Status_Text_Color = ImColor(255, 255, 255, 255);
        inline ImColor C4_Distance_Text_Color = ImColor(255, 255, 255, 255);
        inline ImColor Weapon_Esp_Text_Color = ImColor(255, 255, 0, 255);
        inline ImColor Projectiles_Text_Color = ImColor(255, 0, 0, 255);
        inline ImColor Chicken_Text_Color = ImColor(255, 255, 255, 255);
        inline ImColor World_Entities_BoxColor = ImColor(255, 255, 255, 255);

        inline float BonesThickness = 0.5f;
        inline float C4BoxThickness = 2.5f;
        inline float LinesThickness = 1.5f;
        inline float BoxThickness = 1.0f;

        constexpr float JointDisplayDistance = 1000.0f;
        constexpr float SLEndPointDisplayDistance = 3000.0f;

        inline const char* availableSnapLinesPos[] = { "Top", "Center", "Bottom" };
        inline int currentlinePosIndex = 2;
        inline Vector2 currentlinePos;

        inline const char* availableBoxDimensions[] = { "2D Box", "3D Box", "Corner Box" };
        inline int currentBoxDimensionIndex = 0;
    }

    namespace Fov {
        inline int Default = 60;
    }

    namespace Bhop {
        inline bool enabled = false;
        inline int minJumpDelay = 15;
        inline int minReleaseDelay = 5;
    }
}
