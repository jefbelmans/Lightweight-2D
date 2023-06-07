#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include "Input.h"
#include "Command.h"
#include <iostream>

bool LW2D::InputManager::ProcessInput()
{
	auto controllers = Input::GetInstance().GetControllers();
	auto keyboard = Input::GetInstance().GetKeyboard();

	auto& controllerCommands = Input::GetInstance().GetControllerCommands();
	auto& keyboardCommands = Input::GetInstance().GetKeyboardCommands();

	// UPDATE CONTROLLERS
	for (const auto& controller : controllers)
	{
		controller->Update();
	}

	// POLL CONTROLLERS
	for (const auto& kv : controllerCommands)
	{
		const auto& controller = controllers[std::get<0>(kv.first)];
		
		auto button = std::get<1>(kv.first);
		switch (std::get<2>(kv.first))
		{
		case KeyState::Down:
			if (controller->IsDown(button))
			{
				kv.second->Execute();
			}
			break;
		case KeyState::Pressed:
			if (controller->IsPressed(button))
			{
				kv.second->Execute();
			}
			break;
		case KeyState::Up:
			if (controller->IsUp(button))
			{
				kv.second->Execute();
			}
			break;
		}
	}

	// UPDATE KEYBOARD
	keyboard->Update();

	// POLL KEYBOARD
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) return false;

		// PROCESS IMGUI
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	for (const auto& kv : keyboardCommands)
	{
		auto button = kv.first.first;
		switch (kv.first.second)
		{
		case SDL_KEYDOWN:
			if (keyboard->IsDown(button))
			{
				kv.second->Execute();
			}
			break;
		case SDL_KEYUP:
			if (keyboard->IsUp(button))
			{
				kv.second->Execute();
			}
			break;
		case SDL_KEYMAPCHANGED:
			if (keyboard->IsPressed(button))
			{
				kv.second->Execute();
			}
			break;
		}
	}

	return true;
}
