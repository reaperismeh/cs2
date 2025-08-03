#pragma once
#include <WinUser.h>
#include <Maths/Vector.hpp>

namespace Events
{
	void SimulateLMBClick()
	{
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(INPUT));

		ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	void SimulateMouseMoveRCS(int dx, int dy)
	{
		INPUT input = { 0 };

		// Move mouse horizontally
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		input.mi.dx = dx;
		input.mi.dy = dy;
		input.mi.dwExtraInfo = 0;
		SendInput(1, &input, sizeof(INPUT));
	}


	void MoveMouseToClosestEnemy(const Vector2& position) {
		if (position.x == 0.0f && position.y == 0.0f)
			return;

		Vector2 screenCenter{
			(float)GetSystemMetrics(SM_CXSCREEN) / 2,
			(float)GetSystemMetrics(SM_CYSCREEN) / 2
		};

		auto deltaX = position.x - screenCenter.x;
		auto deltaY = position.y - screenCenter.y;

		deltaX = std::clamp(deltaX, -100.0f, 100.0f);
		deltaY = std::clamp(deltaY, -100.0f, 100.0f);
		mouse_event(MOUSEEVENTF_MOVE, (DWORD)deltaX, (DWORD)deltaY, 0, 0);

		//mouse_event(MOUSEEVENTF_MOVE, static_cast<LONG>(deltaX), static_cast<LONG>(deltaY), 0, 0);
	}


	//void MoveMouseToClosestEnemy(const Vector2& position) {
	//	if (position.x == 0.0f && position.y == 0.0f)
	//		return;

	//	Vector2 screenCenter{
	//		(float)GetSystemMetrics(SM_CXSCREEN) / 2,
	//		(float)GetSystemMetrics(SM_CYSCREEN) / 2
	//	};

	//	auto deltaX = position.x - screenCenter.x;
	//	auto deltaY = position.y - screenCenter.y;

	//	INPUT input = { 0 };
	//	input.type = INPUT_MOUSE;
	//	input.mi.dx = static_cast<LONG>(deltaX);
	//	input.mi.dy = static_cast<LONG>(deltaY);
	//	input.mi.dwFlags = MOUSEEVENTF_MOVE;

	//	SendInput(1, &input, sizeof(INPUT));
	//}


	//void MoveMouseToClosestEnemy(Vector2 position)
	//{
	//	if (position.x == 0.0 && position.y == 0.0)
	//		return;

	//	Vector2 Center_Of_Screen{ (float)GetSystemMetrics(0) / 2, (float)GetSystemMetrics(1) / 2 };

	//	auto new_x = position.x - Center_Of_Screen.x;
	//	auto new_y = position.y - Center_Of_Screen.y;

	//	mouse_event(MOUSEEVENTF_MOVE, new_x, new_y, 0, 0);
	//}
}