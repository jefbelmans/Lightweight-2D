#include "GameModeComponent.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

#include "../GameComponents/MapComponent.h"
#include "../GameComponents/HealthComponent.h"
#include "../GameComponents/CharacterComponent.h"
#include "../GameComponents/ScoreComponent.h"
#include "../GameComponents/PacManComponent.h"
#include "EngineComponents/TextComponent.h"

LW2D::GameModeComponent::GameModeComponent(std::weak_ptr<GameObject> go)
	: Component(go)
{}

void LW2D::GameModeComponent::Initialize()
{
	auto goP1 = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 1");
	if (!goP1.expired())
	{
		m_pHealthP1 = goP1.lock()->GetComponent<HealthComponent>();
		m_pHealthP1.lock()->GetOnDeathEvent()->AddListener(std::bind(&GameModeComponent::OnPlayerDied, this));
	}

	auto goP2 = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 2");
	if (!goP2.expired())
	{
		m_pHealthP2 = goP2.lock()->GetComponent<HealthComponent>();
		m_pHealthP2.lock()->GetOnDeathEvent()->AddListener(std::bind(&GameModeComponent::OnPlayerDied, this));
	}

	auto mapComp = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Map").lock()->GetComponent<MapComponent>();
	m_RemainingPellets = mapComp->GetNumPellets();
	mapComp->GetOnPelletCollected()->AddListener(std::bind(&GameModeComponent::OnPelletCollected, this, std::placeholders::_1));
	mapComp->GetOnPowerPelletCollected()->AddListener(std::bind(&GameModeComponent::OnPelletCollected, this, std::placeholders::_1));

	m_pEndOfGameText = m_pGameObject.lock()->GetComponent<TextComponent>();
	m_pGameObject.lock()->SetActive(false);
}

void LW2D::GameModeComponent::ResetGame()
{
	auto activeScene = SceneManager::GetInstance().GetActiveScene();

	// Reset game over
	m_IsGameOver = false;
	m_pGameObject.lock()->SetActive(false);
	m_RemainingPellets = activeScene->FindObjectByName("Map").lock()->GetComponent<MapComponent>()->GetNumPellets();

	// Reload map
	activeScene->FindObjectByName("Map").lock()->GetComponent<LW2D::MapComponent>()->ReloadMap();

	// Reset players
	auto p1 = activeScene->FindObjectByName("Player 1");
	if (!p1.expired())
	{
		p1.lock()->GetComponent<LW2D::HealthComponent>()->SetLives(4);
		p1.lock()->GetComponent<LW2D::CharacterComponent>()->Respawn(4, true);
		p1.lock()->GetComponent<LW2D::ScoreComponent>()->SetScore(0);
		p1.lock()->GetComponent<LW2D::PacManComponent>()->Reset();
		auto healthDisplay = activeScene->FindObjectByName("Health Display P1");
		if (!healthDisplay.expired())
			healthDisplay.lock()->GetComponent<LW2D::TextComponent>()->SetText("Lives P1: 4");
	}

	auto p2 = activeScene->FindObjectByName("Player 2");
	if (!p2.expired())
	{
		p2.lock()->GetComponent<LW2D::HealthComponent>()->SetLives(4);
		p2.lock()->GetComponent<LW2D::CharacterComponent>()->Respawn(4, true);
		p2.lock()->GetComponent<LW2D::PacManComponent>()->Reset();
		auto healthDisplay = activeScene->FindObjectByName("Health Display P2");
		if (!healthDisplay.expired())
			healthDisplay.lock()->GetComponent<LW2D::TextComponent>()->SetText("Lives P2: 4");
	}

	// Reset ghosts
	auto ghost = activeScene->FindObjectByName("Ghost 1");
	if (!ghost.expired())
	{
		ghost.lock()->GetComponent<LW2D::CharacterComponent>()->Respawn(4);
	}

	ghost = activeScene->FindObjectByName("Ghost 2");
	if (!ghost.expired())
	{
		ghost.lock()->GetComponent<LW2D::CharacterComponent>()->Respawn(4);
	}

	ghost = activeScene->FindObjectByName("Ghost 3");
	if (!ghost.expired())
	{
		ghost.lock()->GetComponent<LW2D::CharacterComponent>()->Respawn(4);
	}

	ghost = activeScene->FindObjectByName("Ghost 4");
	if (!ghost.expired())
	{
		ghost.lock()->GetComponent<LW2D::CharacterComponent>()->Respawn(4);
	}
}

void LW2D::GameModeComponent::OnPelletCollected(int)
{
	--m_RemainingPellets;
	if (m_RemainingPellets <= 0)
	{
		HandleGameWin();
	}
}

void LW2D::GameModeComponent::OnPlayerDied()
{
	if (!m_pHealthP1.expired() && m_pHealthP2.expired())
	{
		if (m_pHealthP1.lock()->IsDead())
		{
				HandleGameOver();
		}
	}
	else if(m_pHealthP1.expired() && !m_pHealthP2.expired())
	{
		if (m_pHealthP2.lock()->IsDead())
		{
			HandleGameOver();
		}
	}
	else if (!m_pHealthP1.expired() && !m_pHealthP2.expired())
	{
		if (m_pHealthP1.lock()->IsDead() && m_pHealthP2.lock()->IsDead())
		{
			HandleGameOver();
		}
	}
}

void LW2D::GameModeComponent::HandleGameWin()
{
	m_pGameObject.lock()->SetActive(true);
	m_pEndOfGameText.lock()->SetText("Game Over - You win! :)");
	m_IsGameOver = true;
}

void LW2D::GameModeComponent::HandleGameOver()
{
	m_pGameObject.lock()->SetActive(true);
	m_pEndOfGameText.lock()->SetText("Game Over - You lose! :(");
	m_IsGameOver = true;
}