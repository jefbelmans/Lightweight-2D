#pragma once
#include <map>
#include <vector>

#include "SDL_events.h"
#include "Singleton.h"
#include "GenericController.h"

namespace LW2D
{
	class Command;

	enum class KeyState : int
	{
		Down,
		Pressed,
		Up
	};

	class Input final : public Singleton<Input>
	{
	public:
		using ControllerKey = std::tuple<unsigned, GenericController::ControllerButton, KeyState>;
		using ControllerCommands = std::map<ControllerKey, std::shared_ptr<Command>>;

		using KeyboardKey = std::pair <SDL_Scancode, SDL_EventType>;
		using KeyboardCommands = std::map<KeyboardKey, std::shared_ptr<Command>>;

		// SETTERS
		void AddController(std::shared_ptr<GenericController> controller) { m_Controllers.push_back(controller); }

		void AddCommand(const ControllerKey& key, std::shared_ptr<Command> command);
		void AddCommand(const KeyboardKey& key, std::shared_ptr<Command> command);

		// GETTERS
		const ControllerCommands& GetControllerCommands() const { return m_ControllerCommands; }
		const KeyboardCommands& GetKeyboardCommands() const { return m_KeyboardCommands; }
		std::vector<std::shared_ptr<GenericController>> GetControllers() const { return m_Controllers; }

	private:
		ControllerCommands m_ControllerCommands;
		KeyboardCommands m_KeyboardCommands;
		std::vector<std::shared_ptr<GenericController>> m_Controllers;
	};
}