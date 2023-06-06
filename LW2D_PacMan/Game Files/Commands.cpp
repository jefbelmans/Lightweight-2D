#include "Commands.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameTime.h"

// COMPONENTS
#include "../GameComponents/HealthComponent.h"
#include "../GameComponents/ScoreComponent.h"
#include "../GameComponents/CharacterComponent.h"

void LW2D::MoveCommand::Execute()
{
	if (!m_pCharacter) throw std::runtime_error("CharacterComponent is nullptr");
	m_pCharacter->SetDirection(m_MovementDirection);
}

void LW2D::KillCommand::Execute()
{
	if (!m_pHealthComponent) throw std::runtime_error("HealthComponent is nullptr");
	m_pHealthComponent->Kill();
}

void LW2D::AddScoreCommand::Execute()
{
	if (!m_pScoreComponent) throw std::runtime_error("ScoreComponent is nullptr");
	m_pScoreComponent->AddScore(50);
}
