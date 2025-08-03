#pragma once
#include <Hacks/Aimbot.hpp>
#include <Hacks/Esp.hpp>
#include "Antiflash.hpp"
#include "Rcs.hpp"
#include "Bhop.hpp"
#include "FovChanger.hpp"
#include <Hacks/Triggerbot.hpp>

namespace CHEATS
{
	void ProcessCheatInputs();
	void InitializeBasicCheats();
}

void CHEATS::ProcessCheatInputs()
{
	if (GetAsyncKeyState(VK_F1) & 1) {
		Flags::enableAimbot = !Flags::enableAimbot;
	}

	if (GetAsyncKeyState(VK_F2) & 1) {
		Flags::enableTriggerbot = !Flags::enableTriggerbot;
	}

	if (GetAsyncKeyState(VK_F3) & 1) {
		Flags::enablePlayerEsp = !Flags::enablePlayerEsp;
		Flags::enableC4Esp = !Flags::enableC4Esp;
	}
}

void CHEATS::InitializeBasicCheats()
{
	// Update player information
	player.UpdateLocalPlayer();

	// RCS
	if (Flags::enableRCS)
	{
		STANDALONE_RCS();
	}

	// Antiflash
	if (Flags::enableAntiflash)
	{
	ANTIFLASH();
	}

	// Bunny-Hop
	if (Config::Bhop::enabled)
	{
		BHOP();
	}


	// Player-Esp
	if (Flags::enablePlayerEsp)
	{
		PLAYER_ESP();
	}

	// Bomb-Esp
	if (Flags::enableC4Esp)
	{
		C4_ESP();
	}

	// World_Esp
	if (Flags::enableWorldEsp)
	{
		ITEM_ESP();
	}

	// Fov-Changer
	if (Flags::enableFovChanger)
	{
		ChangeFoV();
	}

	if (Config::TriggerBot::enableHotKey && (GetAsyncKeyState(hotkeys::triggerbotkey) & 0x8000))
	{
		TRIGGER_BOT(); // Run Triggerbot logic
	}

	// Always active triggerbot mode
	if (Config::TriggerBot::enableAlwaysActive && Flags::enableTriggerbot)
	{
		TRIGGER_BOT_ALWAYS_ACTIVE(); // Run Always Active Triggerbot logic
	}

	if (Flags::enableAimbot && (GetAsyncKeyState(hotkeys::aimkey) & 0x8000))
	{
		aimbot.Run();
	}

	

	mem.WriteMemory<bool>(CS2::ServerDll + 0x58C, true);

	//// NoSky
	//if (Flags::enableNoSky)
	//{
	//	ENABLE_NO_SKY();
	//}
	//else
	//{
	//	DISABLE_NO_SKY();
	//}

	//// NoSpread
	//if (Flags::enableNoSpread)
	//{
	//	ENABLE_SV_CHEATS();
	//	ENABLE_NO_SPREAD();
	//}
	//else
	//{
	//	DISABLE_NO_SPREAD();
	//}

	//// Map-View
	//if (Flags::enableMapView)
	//{
	//	ENABLE_MAP_VIEW();
	//}
	//else
	//{
	//	DISABLE_MAP_VIEW();
	//}
}

