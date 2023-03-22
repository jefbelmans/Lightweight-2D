#include "Input.h"
#include "Command.h"

void LW2D::Input::AddCommand(const ControllerKey& key, std::unique_ptr<Command> command)
{
	m_ControllerCommands.emplace(std::make_pair(key, std::move(command)));
}

void LW2D::Input::AddCommand(const SDL_KeyCode key, std::unique_ptr<Command> command)
{
	m_KeyboardCommands.emplace(std::make_pair(key, std::move(command)));
}
