#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include "Input.h"
#include "Command.h"

bool LW2D::InputManager::ProcessInput()
{
	SDL_Event e;
	// auto keyBoardCommands = Input::GetInstance().GetKeyboardCommands();
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT) return false;

		if (e.type == SDL_KEYDOWN)
		{
			/*for (const auto& kv : keyBoardCommands)
			{
				if (kv.first == e.key.keysym.sym)
					kv.second->Execute();
			}*/
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			
		}
		
		// PROCESS IMGUI
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}
