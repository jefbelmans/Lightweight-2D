#pragma once
#include <map>
#include <vector>

#include "SDL_keycode.h"
#include "Singleton.h"
#include "GenericController.h"

namespace LW2D
{
	class Command;
	class Input final : public Singleton<Input>
	{
	public:
		using ControllerKey = std::pair<unsigned, GenericController::ControllerButton>;
		using ControllerCommands = std::map<ControllerKey, std::unique_ptr<Command>>;
		using KeyboardCommands = std::map<SDL_KeyCode, std::unique_ptr<Command>>;

		void AddCommand(const ControllerKey& key, std::unique_ptr<Command> command);
		void AddCommand(const SDL_KeyCode key, std::unique_ptr<Command> command);

		const ControllerCommands& GetControllerCommands() const { return m_ControllerCommands; }
		const KeyboardCommands& GetKeyboardCommands() const { return m_KeyboardCommands; }

	private:
		ControllerCommands m_ControllerCommands;
		KeyboardCommands m_KeyboardCommands;
		std::vector<std::unique_ptr<GenericController>> m_Controllers;
	};
}