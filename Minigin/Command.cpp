#include "Command.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameTime.h"

void LW2D::MoveCommand::Execute()
{
	auto movement = m_GameObject->GetTransform().GetLocalPosition() + 
		(m_Direction * m_Speed * SceneManager::GetInstance().GetGameTime()->GetDeltaTime());
	m_GameObject->GetTransform().SetLocalPosition(movement.x, movement.y, 0.0f);
}
