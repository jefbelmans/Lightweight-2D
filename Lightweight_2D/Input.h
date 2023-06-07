#pragma once
#include <map>
#include <vector>

#include "SDL_events.h"
#include "Singleton.h"
#include "GenericController.h"
#include "Keyboard.h"

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
		using ControllerCommands = std::multimap<ControllerKey, std::shared_ptr<Command>>;

		using KeyboardKey = std::pair <SDL_Scancode, SDL_EventType>;
		using KeyboardCommands = std::multimap<KeyboardKey, std::shared_ptr<Command>>;

		Input()
		{
			m_pKeyboard = std::make_shared<Keyboard>();
		}

		~Input() = default;

		// SETTERS
		void AddController(std::shared_ptr<GenericController> controller) { m_Controllers.push_back(controller); }
		void SetKeyboard(std::shared_ptr<Keyboard> keyboard) { m_pKeyboard = keyboard; }

		void AddCommand(const ControllerKey& key, std::shared_ptr<Command> command);
		void AddCommand(const KeyboardKey& key, std::shared_ptr<Command> command);

		// GETTERS
		const ControllerCommands& GetControllerCommands() const { return m_ControllerCommands; }
		const KeyboardCommands& GetKeyboardCommands() const { return m_KeyboardCommands; }
		
		std::vector<std::shared_ptr<GenericController>> GetControllers() const { return m_Controllers; }
		std::shared_ptr<Keyboard> GetKeyboard() const { return m_pKeyboard; }

	private:
		ControllerCommands m_ControllerCommands;
		KeyboardCommands m_KeyboardCommands;

		std::vector<std::shared_ptr<GenericController>> m_Controllers;
		std::shared_ptr<Keyboard> m_pKeyboard;
	};
}