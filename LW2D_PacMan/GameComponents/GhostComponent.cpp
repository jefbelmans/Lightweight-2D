#include "GhostComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"


#include "../GameComponents/CharacterComponent.h"
#include "../GameComponents/MapComponent.h"
#include "../Game Files/State.h"
#include "../Game Files/WanderState.h"
#include "../Game Files/Blackboard.h"

LW2D::GhostComponent::GhostComponent(std::weak_ptr<GameObject> go)
	: Component(go)
{
	m_pBlackboard = std::make_shared<Blackboard>();
	m_pCurrentState = std::make_shared<WanderState>();

	m_pBlackboard->Set("Player1", SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 1"));
	m_pBlackboard->Set("Player2", SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 2"));
	m_pBlackboard->Set("Map", SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Map")->GetComponent<MapComponent>());
	m_pBlackboard->Set("ClosestPlayer", std::shared_ptr<GameObject>());
	m_pBlackboard->Set("Agent", GetGameObject());
}

void LW2D::GhostComponent::Update()
{
	CalculateClosestPlayer();

	LW2D::State* state = m_pCurrentState->Update(m_pBlackboard);
	if (state != nullptr)
	{
		m_pCurrentState->OnExit(m_pBlackboard);
		m_pCurrentState.reset(state);
		m_pCurrentState->OnEnter(m_pBlackboard);
	}
}

void LW2D::GhostComponent::CalculateClosestPlayer()
{
	const auto& pos = GetTransform().GetWorldPosition();
	auto closestPlayer = m_pBlackboard->Get<std::shared_ptr<GameObject>>("ClosestPlayer");
	const auto player1 = m_pBlackboard->Get<std::shared_ptr<GameObject>>("Player1");
	const auto player2 = m_pBlackboard->Get<std::shared_ptr<GameObject>>("Player2");

	// Closest player
	if (player1 && !player2)
	{
		closestPlayer = player1;
	}
	else if (!player1 && player2)
	{
		closestPlayer = player2;
	}
	else if (!player1 && !player2)
	{
		if (glm::distance(player1->GetTransform().GetWorldPosition(), pos) < glm::distance(player2->GetTransform().GetWorldPosition(), pos))
			closestPlayer = player1;
		else
			closestPlayer = player2;
	}

	m_pBlackboard->Set("ClosestPlayer", closestPlayer);
}
