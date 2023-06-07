#include "GhostComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

#include "ServiceLocator.h"

#include "../GameComponents/CharacterComponent.h"
#include "../GameComponents/MapComponent.h"
#include "../GameComponents/HealthComponent.h"
#include "../GameComponents/GameModeComponent.h"

#include "../Game Files/State.h"
#include "../Game Files/WanderState.h"
#include "../Game Files/FleeState.h"
#include "../Game Files/Blackboard.h"

LW2D::GhostComponent::GhostComponent(std::weak_ptr<GameObject> go, bool isCPU)
	: Component(go)
	, m_IsCPU{ isCPU }
{
	m_pOnGhostKilled = std::make_unique<Event<int>>();
	m_pBlackboard = std::make_shared<Blackboard>();
	m_pCurrentState = std::make_shared<WanderState>();

	// Flee when power pellet is collected
	MapComponent* map = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Map").lock()->GetComponent<MapComponent>().get();
	map->GetOnPowerPelletCollected()->AddListener([&](int) {
		ChangeState(new FleeState());
		});

	// Kill ghost when player has power pellet
	m_pGameObject.lock()->GetComponent<HealthComponent>()->GetOnKillEvent()->AddListener([&](int)
		{
			LW2D::ServiceLocator::GetSoundSystem().PlaySound((unsigned short)LW2D::Sounds::PacManEatGhost, 64.f);
			m_pGameObject.lock()->GetComponent<HealthComponent>()->SetLives(2);
			m_pOnGhostKilled->Invoke(m_GhostKilledScore);
		});
}

void LW2D::GhostComponent::Initialize()
{
	// Initialize blackboard
	m_pBlackboard->Set("IsCPU", m_IsCPU);
	m_pBlackboard->Set("Player1", SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 1").lock().get());
	m_pBlackboard->Set("Player2", SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 2").lock().get());
	m_pBlackboard->Set("Agent", GetGameObject().get());
	m_pBlackboard->Set("Map", SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Map").lock()->GetComponent<MapComponent>().get());
	m_pBlackboard->Set("GameMode", SceneManager::GetInstance().GetActiveScene()->FindObjectByName("GameMode").lock()->GetComponent<GameModeComponent>().get());
}

void LW2D::GhostComponent::Update()
{
	CalculateClosestPlayer();
;
	LW2D::State* state = m_pCurrentState->Update(m_pBlackboard);
	ChangeState(state);
}

void LW2D::GhostComponent::CalculateClosestPlayer()
{
	const auto& pos = GetTransform().GetWorldPosition();
	const auto player1 = m_pBlackboard->Get<GameObject*>("Player1");
	const auto player2 = m_pBlackboard->Get<GameObject*>("Player2");
	GameObject* closestPlayer = nullptr;

	// Get closest player
	if (player1 && !player2)
	{
		closestPlayer = player1;
	}
	else if (!player1 && player2)
	{
		closestPlayer = player2;
	}
	else if (player1 && player2)
	{
		if (glm::distance(player1->GetTransform().GetWorldPosition(), pos) < glm::distance(player2->GetTransform().GetWorldPosition(), pos))
			closestPlayer = player1;
		else
			closestPlayer = player2;
	}

	m_pBlackboard->Set("ClosestPlayer", closestPlayer);
}

void LW2D::GhostComponent::ChangeState(State* newState)
{
	if (newState != nullptr)
	{
		m_pCurrentState->OnExit(m_pBlackboard);
		m_pCurrentState.reset(newState);
		m_pCurrentState->OnEnter(m_pBlackboard);
	}
}