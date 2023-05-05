#include "Commands.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameTime.h"

// COMPONENTS
#include "GameComponents/HealthComponent.h"
#include "GameComponents/ScoreComponent.h"
#include "GameComponents/PacManComponent.h"

void LW2D::MoveCommand::Execute()
{
	if (!m_PacMan) throw std::runtime_error("PacManComponent is nullptr");
	m_PacMan->SetDirection(m_Direction);
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
