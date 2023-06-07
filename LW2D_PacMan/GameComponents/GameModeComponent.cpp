#include "GameModeComponent.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"

#include "../GameComponents/MapComponent.h"
#include "../GameComponents/HealthComponent.h"

LW2D::GameModeComponent::GameModeComponent(std::weak_ptr<GameObject> go)
	: Component(go)
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
	std::cout << "You win!";
}

void LW2D::GameModeComponent::HandleGameOver()
{
	std::cout << "You lose!";
}