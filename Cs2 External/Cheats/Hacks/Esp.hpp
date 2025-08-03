#pragma once
#include <Memory/Memory.hpp>
#include <Readers/Game.hpp>
#include <Readers/LocalPlayer.hpp>
#include <Utilities/Config.hpp>
#include <Utilities/DrawUtils.hpp>
#include <Maths/WorldToScreen.hpp>
#include <Utilities/Bomb.hpp>
#include <Utilities/World.hpp>



void DrawTextWithShadow(const char* text, Vector2 position, ImColor mainColor, ImColor shadowColor, float shadowOffset = 1.0f)
{
	// Convert ImColor to ImU32
	ImU32 mainColorU32 = ImGui::ColorConvertFloat4ToU32(mainColor.Value);
	ImU32 shadowColorU32 = ImGui::ColorConvertFloat4ToU32(shadowColor.Value);

	// Draw shadow
	Vector2 shadowPos = { position.x + shadowOffset, position.y + shadowOffset };
	DrawUtils::DrawTextW(text, shadowPos, shadowColorU32, false);

	// Draw main text
	DrawUtils::DrawTextW(text, position, mainColorU32, false);
}

void DrawFilledRect2(int x, int y, int w, int h, const ImColor color)
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
}

void DrawCornerBox2(float x, float y, float width, float height, float thickness, ImColor color)
{
	ImDrawList* drawList = ImGui::GetForegroundDrawList();

	// Top-left corner
	drawList->AddLine(ImVec2(x, y), ImVec2(x + width / 4, y), ImGui::ColorConvertFloat4ToU32(color.Value), thickness);
	drawList->AddLine(ImVec2(x, y), ImVec2(x, y + height / 4), ImGui::ColorConvertFloat4ToU32(color.Value), thickness);

	// Top-right corner
	drawList->AddLine(ImVec2(x + width, y), ImVec2(x + width - width / 4, y), ImGui::ColorConvertFloat4ToU32(color.Value), thickness);
	drawList->AddLine(ImVec2(x + width, y), ImVec2(x + width, y + height / 4), ImGui::ColorConvertFloat4ToU32(color.Value), thickness);

	// Bottom-left corner
	drawList->AddLine(ImVec2(x, y + height), ImVec2(x + width / 4, y + height), ImGui::ColorConvertFloat4ToU32(color.Value), thickness);
	drawList->AddLine(ImVec2(x, y + height), ImVec2(x, y + height - height / 4), ImGui::ColorConvertFloat4ToU32(color.Value), thickness);

	// Bottom-right corner
	drawList->AddLine(ImVec2(x + width, y + height), ImVec2(x + width - width / 4, y + height), ImGui::ColorConvertFloat4ToU32(color.Value), thickness);
	drawList->AddLine(ImVec2(x + width, y + height), ImVec2(x + width, y + height - height / 4), ImGui::ColorConvertFloat4ToU32(color.Value), thickness);
}




VOID PLAYER_ESP() // Main Loop
{
	for (int i = 1; i < 64; i++)
	{
		uintptr_t listEntry = mem.ReadMemory<uintptr_t>(game.EntityList + (8 * (i & 0x7FFF) >> 9) + 16);
		if (!listEntry)
			continue;

		uintptr_t currentController = mem.ReadMemory<uintptr_t>(listEntry + 120 * (i & 0x1FF));
		if (!currentController)
			continue;

		uint32_t pawnHandle = mem.ReadMemory<uint32_t>(currentController + Offsets::m_hPlayerPawn);
		if (!pawnHandle)
			continue;

		uintptr_t listEntry2 = mem.ReadMemory<uintptr_t>(game.EntityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 16);
		if (!listEntry2)
			continue;

		uintptr_t currentPawn = mem.ReadMemory<uintptr_t>(listEntry2 + 120 * (pawnHandle & 0x1FF));
		if (!currentPawn)
			continue;

		if (currentPawn == player.localPlayerPawn)
			continue;

		entities->UpdateEntities(currentPawn, currentController);
		game.UpdateGameVars();

		if (entities->Health <= 0 || entities->Health > 100)
			continue;

		if (Config::Esp::enableTeamCheck)
		{
			if (player.Team == entities->Team)
				continue;
		}
		if (Config::Aimbot::showFov)
		{
			DWORD ScreenCenterX = GetSystemMetrics(SM_CXSCREEN);
			DWORD ScreenCenterY = GetSystemMetrics(SM_CYSCREEN);
			ImGui::GetForegroundDrawList()->AddCircle(ImVec2(ScreenCenterX / 2, ScreenCenterY / 2), Config::Aimbot::aimfov, Config::Aimbot::fovColor, 10000);
		}
		Vector3 HeadPosition = entities->Position + Vector3{ 0.0f,0.0f,72.0f }; // Box Height 72 Units;

		Vector2 screenPos, headScreenPos;
		Vector2 screenTop(SCREEN_WIDTH / 2.0f, 0.0f);
		Vector2 screenCenter(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
		Vector2 screenBottom(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT);

		if (WorldToScreen(entities->Position, screenPos, game.ViewMatrix) &&  // Entities W2S Position;
			WorldToScreen(HeadPosition, headScreenPos, game.ViewMatrix)) // Entities W2S Head Position;
		{
			float boxHeight = std::fabs(headScreenPos.y - screenPos.y);
			float boxWidth = boxHeight * 0.6f;

			Vector2 topLeft(screenPos.x - boxWidth / 2, headScreenPos.y);
			Vector2 topRight(screenPos.x + boxWidth / 2 + 12.0f, headScreenPos.y);
			Vector2 bottomRight(screenPos.x + boxWidth / 2, screenPos.y);

			// Draw the Box
			if (Config::Esp::enableEspBoxes)
			{
				Vector3 corners[8];
				float boxHeight = 72.0f;
				float boxWidth = boxHeight * 0.6f;
				float boxDepth = boxWidth;
				corners[0] = entities->Position + Vector3(-boxWidth / 2, -boxDepth / 2, 0);
				corners[1] = entities->Position + Vector3(boxWidth / 2, -boxDepth / 2, 0);
				corners[2] = entities->Position + Vector3(boxWidth / 2, boxDepth / 2, 0);
				corners[3] = entities->Position + Vector3(-boxWidth / 2, boxDepth / 2, 0);

				corners[4] = corners[0] + Vector3(0, 0, (float)boxHeight);
				corners[5] = corners[1] + Vector3(0, 0, (float)boxHeight);
				corners[6] = corners[2] + Vector3(0, 0, (float)boxHeight);
				corners[7] = corners[3] + Vector3(0, 0, (float)boxHeight);

				if (Config::Esp::currentBoxDimensionIndex == 0)
				{
					// Draw a 2D Box
					DrawUtils::DrawBox(topLeft, bottomRight, Config::Esp::BoxColor, Config::Esp::BoxThickness);
				}
				else if (Config::Esp::currentBoxDimensionIndex == 1)
				{
					// Draw a 3D Box
					DrawUtils::Draw3DBox(corners, Config::Esp::BoxColor, Config::Esp::BoxThickness);
				}
				else if (Config::Esp::currentBoxDimensionIndex == 2)
				{
					// Draw Corner Box
					float width = boxWidth;
					float height = boxHeight;
					ImColor boxColor = Config::Esp::BoxColor;

					DrawCornerBox2(screenPos.x - width / 2, headScreenPos.y, width, height, 1.0f, boxColor);
				}
			}

			// Draw the Health Bar
			if (Config::Esp::enableEspHealthBar)
			{
				Config::Esp::HealthBarColor = entities->GetColorHealth();

				DrawUtils::DrawHealthBar
				(
					topLeft,
					bottomRight,
					entities->Health,
					Config::Esp::HealthBarColor,
					2.0f,
					6.0f
				);
			}

			// Draw the Health Text
			if (Config::Esp::enableEspHealthPointsText)
			{
				char healthText[16];
				snprintf(healthText, sizeof(healthText), "[ %d ]", entities->Health);

				// Calculate the position below the box
				Vector2 healthTextPos((topLeft.x + bottomRight.x) / 2, bottomRight.y + 10.0f);

				// Get text size for proper centering
				const ImVec2 textSize = ImGui::CalcTextSize(healthText);

				// Center horizontally
				healthTextPos.x -= textSize.x / 2;

				// Draw the health text with a shadow
				DrawTextWithShadow(
					healthText,
					healthTextPos,
					Config::Esp::HealthBarColor,
					ImColor(0, 0, 0, 255),
					1.0f
				);
			}





			// Draw the Armor Bar
			if (Config::Esp::enableEspArmorBar)
			{
				if (entities->Team == 2)
				{
					Config::Esp::ArmorBarColor = Config::Game::TerroristColor;
				}
				else if (entities->Team == 3)
				{
					Config::Esp::ArmorBarColor = Config::Game::CounterTerroristColor;
				}

				DrawUtils::DrawHealthBar
				(
					topRight,
					bottomRight,
					entities->Armor,
					Config::Esp::ArmorBarColor,
					2.0f,
					6.0f
				);
			}

			// Draw the Snap Lines
			if (Config::Esp::enableEspLines)
			{
				switch (Config::Esp::currentlinePosIndex) {
				case 0: Config::Esp::currentlinePos = screenTop; break; // Top
				case 1: Config::Esp::currentlinePos = screenCenter; break; // Center
				case 2: Config::Esp::currentlinePos = screenBottom; break; // Bottom
				default: Config::Esp::currentlinePos = screenBottom; break; // Default
				}

				if (game.Dist_ETLP < Config::Esp::SLEndPointDisplayDistance)
				{
					DrawUtils::DrawLine
					(
						Config::Esp::currentlinePos,
						screenPos,
						Config::Esp::LinesColor,
						Config::Esp::LinesThickness,
						true,
						3.5f,
						Config::Esp::LinesColor
					);
				}
				else
				{
					DrawUtils::DrawLine
					(
						Config::Esp::currentlinePos,
						screenPos,
						Config::Esp::LinesColor,
						Config::Esp::LinesThickness
					);
				}


			}

			// Draw the Entity Name Text
			if (Config::Esp::enableEspNames)
			{
				// Calculate the position to center the name above the box
				Vector2 namePos(screenPos.x, headScreenPos.y - 15.0f);

				// Get text size for proper centering
				const ImVec2 textSize = ImGui::CalcTextSize(entities->Names);

				// Center horizontally
				namePos.x -= textSize.x / 2;

				// Draw the name with a shadow
				DrawTextWithShadow(
					entities->Names,
					namePos,
					Config::Esp::NameColor,
					ImColor(0, 0, 0, 255),
					1.0f
				);
			}
			if (Config::Esp::enableDistanceEsp)
			{
				char distText[32];
				float distanceMeters = game.Dist_ETLP / GAME_METRIC_UNITS;
				snprintf(distText, sizeof(distText), "[%.1f m]", distanceMeters);

				Vector2 distTextPos((topLeft.x + bottomRight.x) / 2, bottomRight.y + 10.0f);
				const ImVec2 distTextSize = ImGui::CalcTextSize(distText);
				distTextPos.x -= distTextSize.x / 2;

				DrawTextWithShadow(
					distText,
					distTextPos,
					Config::Esp::DistanceColor,
					ImColor(0, 0, 0, 255),
					1.0f
				);

				if (Config::Esp::PlayerFlashed && entities->FlashAlpha > 0.0f)
				{
					Vector2 flashedTextPos(distTextPos.x, distTextPos.y + distTextSize.y + 5.0f);
					DrawTextWithShadow(
						"Player Flashed",
						flashedTextPos,
						Config::Esp::IsFlashColor,
						ImColor(0, 0, 0, 255),
						1.0f
					);
				}
			}

			// Draw the Bones
			if (Config::Esp::enableEspBones)
			{
				DrawUtils::DrawBones(currentPawn);
			}

			Vector2 distTextPos;
			ImVec2 distTextSize;

			if (Config::Esp::enableDistanceEsp)
			{
				char distText[32];
				float distanceMeters = game.Dist_ETLP / GAME_METRIC_UNITS;
				snprintf(distText, sizeof(distText), "[%.1f m]", distanceMeters);
				distTextPos = Vector2((topLeft.x + bottomRight.x) / 2, bottomRight.y + 10.0f);
				distTextSize = ImGui::CalcTextSize(distText);
				distTextPos.x -= distTextSize.x / 2;
				DrawTextWithShadow(
					distText,
					distTextPos,
					Config::Esp::DistanceColor,
					ImColor(0, 0, 0, 255),
					1.0f
				);
			}
			else
			{
				distTextPos = Vector2((topLeft.x + bottomRight.x) / 2, bottomRight.y + 10.0f);
				distTextSize = ImVec2(0, 0);
			}


			if (Config::Esp::enableWeaponNameOnPlayers)
			{
				uintptr_t currentweapon = mem.ReadMemory<uintptr_t>(currentPawn + Offsets::m_pClippingWeapon);

				short weaponindex = mem.ReadMemory<short>(
					currentweapon + Offsets::m_AttributeManager + Offsets::m_Item + Offsets::m_iItemDefinitionIndex
				);

				// Print the weapon index
				printf("Weapon Index: %d\n", weaponindex);

				// Get weapon name from index
				std::string weaponName = GetWeaponNameByIndex(weaponindex);


				Vector2 weaponTextPos = distTextPos;
				weaponTextPos.y += distTextSize.y + 5.0f;

				ImVec2 weaponTextSize = ImGui::CalcTextSize(weaponName.c_str());
				weaponTextPos.x -= weaponTextSize.x / 2;

				DrawTextWithShadow(
					weaponName.c_str(),
					weaponTextPos,
					IM_COL32(255, 255, 255, 255),
					IM_COL32(0, 0, 0, 255),
					1.0f
				);

				distTextPos = weaponTextPos;
				distTextSize = weaponTextSize;
			}


			if (Config::Esp::PlayerDefusing && entities->IsBombBeingDefused)
			{
				Vector2 defusingTextPos = distTextPos;
				defusingTextPos.y += distTextSize.y + 5.0f;
				const char* defusingText = "Player is defusing";
				ImVec2 defusingTextSize = ImGui::CalcTextSize(defusingText);
				defusingTextPos.x -= defusingTextSize.x / 2;

				DrawTextWithShadow(
					defusingText,
					defusingTextPos,
					Config::Esp::IsFlashColor,
					ImColor(0, 0, 0, 255),
					1.0f
				);
			}


		}
	}
}


VOID C4_ESP()
{
	// Bomb Esp
	Vector3 PlantedC4Pos3D = c4.GetPlantedC4Position();
	Vector3 DroppedC4Pos3D = c4.GetDroppedC4Position();
	float C4Width = 30.0f;
	float C4Height = 20.0f;

	if (game.BombPlanted)
	{
		Vector2 Planted_C4_ScreenPos, PlantedC4Height;
		Vector3 PlantedC4HeightPos = PlantedC4Pos3D + Vector3{ 0.0f, 0.0f, C4Height }; // Box height based on bomb size

		if (WorldToScreen(PlantedC4Pos3D, Planted_C4_ScreenPos, game.ViewMatrix) &&
			WorldToScreen(PlantedC4HeightPos, PlantedC4Height, game.ViewMatrix))
		{
			float BoxHeight = std::fabs(PlantedC4Height.y - Planted_C4_ScreenPos.y);
			float boxWidth = BoxHeight * (C4Width / C4Height); // Adjust box width based on actual C4 size

			Vector2 topLeftPBP(Planted_C4_ScreenPos.x - boxWidth / 2.0f, Planted_C4_ScreenPos.y - BoxHeight / 2.0f);
			Vector2 bottomRightPBP(Planted_C4_ScreenPos.x + boxWidth / 2.0f, Planted_C4_ScreenPos.y + BoxHeight / 2.0f);

			if (Config::Esp::enableC4Box)
			{
				DrawUtils::DrawBox(topLeftPBP, bottomRightPBP, Config::Esp::C4_Box_Color, Config::Esp::C4BoxThickness);
			}

			if (Config::Esp::enableC4StatusEsp)
			{
				DrawUtils::DrawTextW("C4_Planted", topLeftPBP, Config::Esp::C4_Status_Text_Color);
			}

			if (Config::Esp::enableC4Distance)
			{
				char DistTextArray[128];
				float PlantedC4_DistanceInMeters = c4.Get_Distance_From_PlantedC4_To_LocalPlayer() / GAME_METRIC_UNITS;
				snprintf(DistTextArray, sizeof(DistTextArray), "[%.1f m]", PlantedC4_DistanceInMeters);

				DrawUtils::DrawTextW(DistTextArray, bottomRightPBP, Config::Esp::C4_Distance_Text_Color);
			}
		}
	}

	if (game.BombDropped)
	{
		Vector2 Dropped_C4_ScreenPos, Dropped_C4_Height;
		Vector3 DroppedC4HeightPos = DroppedC4Pos3D + Vector3{ 0.0f, 0.0f, C4Height };

		if (WorldToScreen(DroppedC4Pos3D, Dropped_C4_ScreenPos, game.ViewMatrix) &&
			WorldToScreen(DroppedC4HeightPos, Dropped_C4_Height, game.ViewMatrix))
		{
			float BoxHeight = std::fabs(Dropped_C4_Height.y - Dropped_C4_ScreenPos.y);
			float boxWidth = BoxHeight * (C4Width / C4Height);

			Vector2 topLeftDBP(Dropped_C4_ScreenPos.x - boxWidth / 2.0f, Dropped_C4_ScreenPos.y - BoxHeight / 2.0f);
			Vector2 bottomRightDBP(Dropped_C4_ScreenPos.x + boxWidth / 2.0f, Dropped_C4_ScreenPos.y + BoxHeight / 2.0f);

			if (Config::Esp::enableC4Box)
			{
				DrawUtils::DrawBox(topLeftDBP, bottomRightDBP, Config::Esp::C4_Box_Color, Config::Esp::C4BoxThickness);
			}

			if (Config::Esp::enableC4StatusEsp)
			{
				DrawUtils::DrawTextW("C4_Dropped", topLeftDBP, Config::Esp::C4_Status_Text_Color);
			}

			if (Config::Esp::enableC4Distance)
			{
				char DistTextArray[128];
				float DroppedC4_DistanceInMeters = c4.Get_Distance_From_DroppedC4_To_LocalPlayer() / GAME_METRIC_UNITS;
				snprintf(DistTextArray, sizeof(DistTextArray), "[%.1f m]", DroppedC4_DistanceInMeters);

				DrawUtils::DrawTextW(DistTextArray, bottomRightDBP, Config::Esp::C4_Distance_Text_Color);
			}
		}
	}
}

VOID ITEM_ESP()
{	
	for (int i = 64; i < 1024; i++)
	{
		DWORD64 itemEntityListEntry = mem.ReadMemory<DWORD64>(game.EntityList + 8LL * ((i & 0x7FFF) >> 9) + 16);
		if (!itemEntityListEntry)
			continue;

		DWORD64 itemEntity = mem.ReadMemory<DWORD64>(itemEntityListEntry + 120LL * (i & 0x1FF)); // Item Entity
		if (!itemEntity)
			continue;

		DWORD64 itemEntityNode = mem.ReadMemory<DWORD64>(itemEntity + Offsets::m_pGameSceneNode); // Item Entity Node
		Vector3 itemEntityOrigin = mem.ReadMemory<Vector3>(itemEntityNode + Offsets::m_vecAbsOrigin); // Item Entity Origin

		uintptr_t weaponOwner = mem.ReadMemory<uintptr_t>(itemEntity + 0x440);
		if (weaponOwner != 0)
			continue; // Don't draw if the item has an owner


		Vector2 ScreenPos;
		if (!WorldToScreen(itemEntityOrigin, ScreenPos, game.ViewMatrix))
			continue;

		DWORD64 itemInfo = mem.ReadMemory<DWORD64>(itemEntity + 0x10);
		DWORD64 itemTypePtr = mem.ReadMemory<DWORD64>(itemInfo + 0x20);


		if (itemEntityOrigin.x)
		{
			char type[128]{};
			mem.ReadArray<char>(itemTypePtr, type, sizeof(type));
			string weaponName = GetWeaponType(type);
			string Projectiles = GetProjectileType(type);
			string Entity = GetEntityType(type);

			if (weaponName != "<unknown>") // Dropped Weapon Esp
			{	
				if (Config::Esp::enableDroppedWeaponEsp)
					DrawUtils::DrawTextW(weaponName.c_str(), { ScreenPos.x, ScreenPos.y }, Config::Esp::Weapon_Esp_Text_Color);
			}

			if (Projectiles != "<unknown>") // Projectiles Weapon Esp
			{
				if (Config::Esp::enableProjectilesEsp)
					DrawUtils::DrawTextW(Projectiles.c_str(), { ScreenPos.x, ScreenPos.y }, Config::Esp::Projectiles_Text_Color);
			}


			if (Entity != "<unknown>") // Chicken Esp
			{
				if (Config::Esp::enableChickensEsp)
				{
					if (strstr(type, "chicken"))
					{
						Vector3 ChickenHeight = itemEntityOrigin + Vector3{ 0.0f, 0.0f, 20.0f }; // Box Height 72 Units;
						Vector2 ChickenScreenPos;

						if (!WorldToScreen(ChickenHeight, ChickenScreenPos, game.ViewMatrix))
							continue;

						float boxHeight = std::fabs(ChickenScreenPos.y - ScreenPos.y);
						float boxWidth = boxHeight * 1.6f;

						Vector2 topLeft(ScreenPos.x - boxWidth / 2, ChickenScreenPos.y);
						Vector2 topRight(ScreenPos.x + boxWidth / 2 + 12.0f, ChickenScreenPos.y);
						Vector2 bottomRight(ScreenPos.x + boxWidth / 2, ScreenPos.y);

						//ESP::DrawBox(topLeft, bottomRight);
						DrawUtils::DrawTextW("Chicken", { ScreenPos.x, ScreenPos.y }, Config::Esp::Chicken_Text_Color);
					}
				}

			}

			if (Entity != "<unknown>") // Hostages Esp
			{
				if (Config::Esp::enableHostagesEsp)
				{
					if (strstr(type, "hostage_entity"))
					{
						Vector3 HostagesHeight = itemEntityOrigin + Vector3{ 0.0f, 0.0f, 72.0f }; // Box Height 72 Units;
						Vector2 HostageScreenPos;

						if (!WorldToScreen(HostagesHeight, HostageScreenPos, game.ViewMatrix))
							continue;

						float boxHeight = std::fabs(HostageScreenPos.y - ScreenPos.y);
						float boxWidth = boxHeight * 0.6f;

						Vector2 topLeft(ScreenPos.x - boxWidth / 2, HostageScreenPos.y);
						Vector2 topRight(ScreenPos.x + boxWidth / 2 + 12.0f, HostageScreenPos.y);
						Vector2 bottomRight(ScreenPos.x + boxWidth / 2, ScreenPos.y);

						DrawUtils::DrawBox(topLeft, bottomRight, Config::Esp::World_Entities_BoxColor);
						DrawUtils::DrawTextW("Hostage", topRight, ImColor(0, 183, 0, 255));
					}
				}
			}
		}
	}
}