#pragma once
#include <Memory/Memory.hpp>
#include <Maths/Vector.hpp>
#include <Globals/Globals.hpp>
#include <ImGui/imgui.h>
#include <Utilities/Bones.hpp>

class Entities
{
public:
    int Team;
    int Health;
    int Armor;
    char Names[128] = { 0 };
    Vector3 Position;
    uintptr_t BoneMatrix;
    bool IsFiring;
    uint32_t Ping;
    Vector3 VecViewOffset;
    bool IsSpotted;
    bool IsScoped;
    float FlashAlpha = 0.0f;
    bool IsBombBeingDefused;


public:
    void UpdateEntities(uintptr_t currentPawn, uintptr_t currentController)
    {
        uintptr_t NameAddress = mem.ReadMemory<uintptr_t>(currentController + Offsets::m_sSanitizedPlayerName);
        mem.ReadArray<char>(NameAddress, Names, sizeof(Names));

        Team = mem.ReadMemory<int>(currentPawn + Offsets::m_iTeamNum);
        Health = mem.ReadMemory<int>(currentPawn + Offsets::m_iHealth);
        Armor = mem.ReadMemory<int>(currentPawn + Offsets::m_ArmorValue);
        Position = mem.ReadMemory<Vector3>(currentPawn + Offsets::m_vOldOrigin);
        BoneMatrix = GetBoneMatrix(currentPawn);
        IsFiring = mem.ReadMemory<bool>(currentPawn + Offsets::m_iShotsFired);
        Ping = mem.ReadMemory<uint32_t>(currentController + Offsets::m_iPing);
        VecViewOffset = mem.ReadMemory<Vector3>(currentPawn + Offsets::m_vecViewOffset);
        IsSpotted = mem.ReadMemory<bool>(currentPawn + Offsets::m_entitySpottedState + Offsets::m_bSpotted);
        IsScoped = mem.ReadMemory<bool>(currentPawn + Offsets::m_bIsScoped);
        IsBombBeingDefused = mem.ReadMemory<bool>(currentPawn + Offsets::m_bBeingDefused);

        FlashAlpha = mem.ReadMemory<float>(currentPawn + Offsets::m_flFlashOverlayAlpha);
    }

    float GetHeadHeight(Vector2 screenPos, Vector2 headScreenPos)
    {
        return (screenPos.y - headScreenPos.y) / 8;
    }


    ImColor GetColorHealth()
    {
        float clampedHealth = std::clamp(static_cast<float>(Health), 0.0f, 100.0f);
        float r, g, b;

        if (clampedHealth >= 60) {
            float t = (clampedHealth - 60) / 40.0f; // 60 to 100
            r = 255 * (1.0f - t);
            g = 255;
            b = 0;
        }
        else if (clampedHealth >= 30) {
            float t = (clampedHealth - 30) / 30.0f; // 30 to 60
            r = 255;
            g = 165 + t * (255 - 165);
            b = 0;
        }
        else {
            float t = clampedHealth / 30.0f; // 0 to 30
            r = 255;
            g = t * 165;
            b = 0;
        }

        return ImColor(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b), 255);
    }

}; inline Entities entities[64];