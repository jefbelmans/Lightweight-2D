#include "Input.h"
#include "Command.h"
#include <utility>

void LW2D::Input::AddCommand(const ControllerKey& key, std::shared_ptr<Command> command)
{
	m_ControllerCommands.emplace(std::make_pair(key, command));
}

void LW2D::Input::AddCommand(const KeyboardKey& key, std::shared_ptr<Command> command)
{
	m_KeyboardCommands.emplace(std::make_pair(key, command));
}
