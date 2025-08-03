#pragma once
#define SCREEN_WIDTH GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_HEIGHT GetSystemMetrics(SM_CYSCREEN)
#define MIN_MENU_WIDTH 800
#define MIN_MENU_HEIGHT 600
#define GAME_METRIC_UNITS 40.0f;

#include "..\Memory\Process.hpp"

//Fonts
ImFont* mainfont;
ImFont* childtitle;
ImFont* titlefont;
ImFont* icons;
ImFont* logo;
ImFont* arrow;
ImFont* brandfont;
ImFont* notiffont;

namespace Flags
{
	inline bool IsRunning = true;
	inline bool IsVisible = true;
    inline bool g_showWarningBox = true;
    inline bool g_showConsole= true;
    // cheats flags;
    inline bool enableTriggerbot = false;
    inline bool enableRCS = false;
    inline bool enableAntiflash = false;
    inline bool enableRadarHack = false;
    inline bool enableGlowHack = false;
    inline bool enableAimbot = false;
    inline bool enableJumpShot = true;
    inline bool enablePlayerEsp = true;
    inline bool enableC4Esp = true;
    inline bool enableWorldEsp = false;
    inline bool enableNoSky = false;
    inline bool enableNoSpread = false;
    inline bool enableMapView = false;
    inline bool enableFovChanger = false;
    inline bool enableSensiChanger = false;
}
namespace CS2
{
    inline DWORD ProcID = 0;
    inline DWORD SteamProcID = 0;
    inline uintptr_t ClientDll = 0;
    inline uintptr_t Engine2Dll = 0;
    inline uintptr_t InputsystemDll = 0;
    inline uintptr_t MatchMakingDll = 0;
    inline uintptr_t SoundsystemDll = 0;
    inline uintptr_t ServerDll = 0;

    inline void Initialize()
    {
        ProcID = PM.GetProcessID(L"cs2.exe");
        SteamProcID = PM.GetProcessID(L"steam.exe");

        if (ProcID == 0)
            return; // 

        ClientDll = PM.GetModuleBaseAddress(ProcID, L"client.dll");
        Engine2Dll = PM.GetModuleBaseAddress(ProcID, L"engine2.dll");
        InputsystemDll = PM.GetModuleBaseAddress(ProcID, L"inputsystem.dll");
        MatchMakingDll = PM.GetModuleBaseAddress(ProcID, L"matchmaking.dll");
        SoundsystemDll = PM.GetModuleBaseAddress(ProcID, L"soundsystem.dll");
        ServerDll = PM.GetModuleBaseAddress(ProcID, L"server.dll");
    }
}


namespace Offsets
{
  
    inline int dwForceAttack = 0x1AED5B0; // uintptr_t
    inline int dwForceAttack2 = 0x1AED640; // uintptr_t
    inline int dwForceJump = 0x1AEDAC0; // uintptr_t

    
    inline int dwEntityList = 0x1CBE620; // uintptr_t
    inline int dwGameRules = 0x1D1D4E0; // uintptr_t
    inline int dwGameTypes = 0x1B3F10; // uintptr_t
    inline int dwGlobalVars = 0x1AE9458; // uintptr_t
    inline int dwLocalPlayerController = 0x1D10240; // uintptr_t
    inline int dwLocalPlayerPawn = 0x1AF4B80; // uintptr_t
    inline int dwPlantedC4 = 0x1D27180; // uintptr_t
    inline int dwSensitivity = 0x1D1E018; // uintptr_t
    inline int dwSensitivity_sensitivity = 0x48; // uintptr_t
    inline int dwViewAngles = 0x1D2C7C0;
    inline int dwViewMatrix = 0x1D21A00; // uintptr_t
    inline int dwWeaponC4 = 0x1CC1888; // uintptr_t
    inline int dwCSGOInput = 30588944;

    inline int m_ArmorValue = 0x28E4; // int32
    inline int m_AttributeManager = 0x11D0; // C_AttributeContainer econ
    inline int m_Glow = 0xCC0; // CGlowProperty
    inline int m_Item = 0x50; // C_EconItemView
    inline int m_aimPunchAngle = 0x185C; // QAngle
    inline int m_bBeingDefused = 0x11AC; // bool
    inline int m_bBombDropped = 0x9A4; // bool
    inline int m_bBombPlanted = 0x9A5; // bool
    inline int m_bCTTimeOutActive = 0x4D; // bool
    inline int m_bGlowing = 0x51; // bool
    inline int m_bHasExploded = 0x11A5; // bool
    inline int m_bIsScoped = 0x28B0; // bool
    inline int m_bSpotted = 0x8; // bool
    inline int m_bTerroristTimeOutActive = 0x4C; // bool
    inline int m_bWarmupPeriod = 0x41; // bool
    inline int m_entitySpottedState = 0x1180; // EntitySpottedState_t MULTI OFFSETS
    inline int m_fFlags = 0x63; // uint32
    inline int m_flDefuseLength = 0x11BC; // float32
    inline int m_flFlashBangTime = 0x1668; // float32
    inline int m_flFlashOverlayAlpha = 0x1670; // float32
    inline int m_glowColorOverride = 0x40; // Color
    inline int m_hActiveWeapon = 0x58; // CHandle<C_BasePlayerWeapon>
    inline int m_hOwnerEntity = 0x520; // CHandle<C_BaseEntity>
    inline int m_hPlayerPawn = 0x8FC; // CHandle<C_CSPlayerPawn>
    inline int m_iFOV = 0x288; // uint32
    inline int m_iHealth = 0x34C; // int32
    inline int m_iIDEntIndex = 0x1734; // CEntityIndex
    inline int m_iItemDefinitionIndex = 0x1BA; // uint16
    inline int m_iPing = 0x818; // uint32
    inline int m_iShotsFired = 0x28C4; // int32
    inline int m_iTeamNum = 0x3EB; // uint8
    inline int m_nBombSite = 0x1174; // int32
    inline int m_pCameraServices = 0x1438; // (CPlayer_CameraServices*)
    inline int m_pClippingWeapon = 0x1620; // C_CSWeaponBase*
    inline int m_pGameSceneNode = 0x330; // CGameSceneNode*
    inline int m_sSanitizedPlayerName = 0x850; // CUtlString
    inline int m_totalRoundsPlayed = 0x84; // int32
    inline int m_vOldOrigin = 0x15B0; // Vector
    inline int m_vecAbsOrigin = 0xD0; // Vector
    inline int m_vecAbsVelocity = 0x3FC; // Vector
    inline int m_vecViewOffset = 0xD98; // CNetworkViewOffsetVector
}
