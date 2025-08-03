#pragma once
#include <Maths/Vector.hpp>
#include <Maths/WorldToScreen.hpp>
#include <Memory/Memory.hpp>
#include <Readers/Game.hpp>
#include <Readers/LocalPlayer.hpp>
#include <Inputs/Events.hpp>
#include <Windows.h>

class AIMBOT {
public:
    void Run() {
        Vector2 closest = FindClosestEnemy();
      
        if (closest.x != 0 && closest.y != 0) {
            // Original mouse-based aimbot
            Vector2 screenCenter{
                (float)GetSystemMetrics(SM_CXSCREEN) / 2,
                (float)GetSystemMetrics(SM_CYSCREEN) / 2
            };

            Vector2 delta = { closest.x - screenCenter.x, closest.y - screenCenter.y };
            float smoothing = Config::Aimbot::Smoothing;
            Vector2 smoothedDelta = { delta.x / smoothing, delta.y / smoothing };

            Vector2 newMousePos = { screenCenter.x + smoothedDelta.x, screenCenter.y + smoothedDelta.y };
            Events::MoveMouseToClosestEnemy(newMousePos);
        }
    }

private:
    Vector2 FindClosestEnemy() {
        Vector2 closestPos = { 0, 0 };
        Vector2 screenCenter{
            (float)GetSystemMetrics(SM_CXSCREEN) / 2,
            (float)GetSystemMetrics(SM_CYSCREEN) / 2
        };
        float lowestDistance = 10000.0f;

        for (int i = 1; i < 64; i++) {
            uintptr_t listEntry = mem.ReadMemory<uintptr_t>(game.EntityList + (8 * (i & 0x7FFF) >> 9) + 16);
            if (!listEntry) continue;

            uintptr_t controller = mem.ReadMemory<uintptr_t>(listEntry + 120 * (i & 0x1FF));
            if (!controller) continue;

            uint32_t pawnHandle = mem.ReadMemory<uint32_t>(controller + Offsets::m_hPlayerPawn);
            if (!pawnHandle) continue;

            uintptr_t listEntry2 = mem.ReadMemory<uintptr_t>(game.EntityList + 8 * ((pawnHandle & 0x7FFF) >> 9) + 16);
            if (!listEntry2) continue;

            uintptr_t pawn = mem.ReadMemory<uintptr_t>(listEntry2 + 120 * (pawnHandle & 0x1FF));
            if (!pawn) continue;

            entities->UpdateEntities(pawn, controller);

            if (pawn == player.localPlayerPawn) continue;
            if (Config::Aimbot::TeamCheck && entities->Team == player.Team) continue;
            if (entities->Health <= 0) continue;

            uintptr_t boneMatrix = GetBoneMatrix(pawn);
            if (Config::Aimbot::VisCheck) {
                if (!(entities->IsSpotted && (1u << player.localplayerindex)))
                    continue;
            }
            
            Vector3 targetVelocity = mem.ReadMemory<Vector3>(pawn + Offsets::m_vecAbsVelocity);
            Vector3 aimPos3D;

            switch (Config::Aimbot::CurrentAimPosIndex) {
                case 0: aimPos3D = SetBoneCoordinate(boneMatrix, BoneIndex::HEAD); break;
                case 1: aimPos3D = SetPenisCoordinate(entities->Position, pawn); break;
                case 2: aimPos3D = SetBoneCoordinate(boneMatrix, BoneIndex::NECK); break;
                case 3: aimPos3D = SetBoneCoordinate(boneMatrix, BoneIndex::CHEST); break;
                case 4: aimPos3D = SetBoneCoordinate(boneMatrix, BoneIndex::STOMACH); break;
                default: aimPos3D = SetBoneCoordinate(boneMatrix, BoneIndex::HEAD); break;
            }

            if (Config::Aimbot::enablePrediction) {
                float predictionTime = Config::Aimbot::predictionTime > 0 ? Config::Aimbot::predictionTime : 0.2f;
                aimPos3D = PredictPlayer(aimPos3D, targetVelocity, player.Position, predictionTime);
            }

            Vector2 screenPos;
            if (WorldToScreen(aimPos3D, screenPos, game.ViewMatrix)) {
                float dist = std::sqrt(std::pow(screenPos.x - screenCenter.x, 2) + std::pow(screenPos.y - screenCenter.y, 2));

                if (dist > Config::Aimbot::aimfov) continue;

                if (dist < lowestDistance) {
                    lowestDistance = dist;
                    closestPos = screenPos;
                }
            }
        }

        return closestPos;
    }

    Vector3 PredictPlayer(Vector3 targetPos, Vector3 targetVelocity, Vector3 localPos, float predictionTime) {
        return targetPos + targetVelocity * predictionTime;
    }

    Vector3 SetBoneCoordinate(uintptr_t boneMatrix, int boneIndex) {
        return mem.ReadMemory<Vector3>(boneMatrix + (boneIndex * 0x20));
    }

    Vector3 SetPenisCoordinate(Vector3 entityPosition, uintptr_t entityPawn) {
        return entityPosition + mem.ReadMemory<Vector3>(entityPawn + Offsets::m_vecViewOffset) * 0.5f;
    }
};

inline AIMBOT aimbot;
