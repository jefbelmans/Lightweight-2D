#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include "Input.h"
#include "Command.h"
#include <iostream>

bool LW2D::InputManager::ProcessInput()
{
	SDL_Event e;
	auto controllers = Input::GetInstance().GetControllers();
	auto keyBoardCommands = Input::GetInstance().GetKeyboardCommands();
	auto controllerCommands = Input::GetInstance().GetControllerCommands();

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
		default:
			break;
		}
	}

	// POLL KEYBOARD
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) return false;

		if (e.type == SDL_KEYDOWN)
		{
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			
		}

		// PROCESS IMGUI
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
	for (const auto& kv : keyBoardCommands)
	{
		if (currentKeyStates[kv.first.first])
		{
			kv.second->Execute();
		}
	}

	return true;
}
