#include "Command.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameTime.h"

// COMPONENTS
#include "HealthComponent.h"

void LW2D::MoveCommand::Execute()
{
	auto movement = m_GameObject->GetTransform().GetLocalPosition() + 
		(m_Direction * m_Speed * SceneManager::GetInstance().GetGameTime()->GetDeltaTime());
	m_GameObject->GetTransform().SetLocalPosition(movement.x, movement.y, 0.0f);
}

void LW2D::KillCommand::Execute()
{
	if(!m_pHealthComponent) throw std::runtime_error("HealthComponent is nullptr");
	m_pHealthComponent->Kill();
}
