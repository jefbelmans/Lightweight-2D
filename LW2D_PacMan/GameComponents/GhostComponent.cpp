#include "GhostComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

#include "../GameComponents/CharacterComponent.h"
#include "../GameComponents/MapComponent.h"
#include "../Game Files/State.h"
#include "../Game Files/WanderState.h"

LW2D::GhostComponent::GhostComponent(std::weak_ptr<GameObject> go)
	: Component(go)
{
	m_pCharacter = go.lock()->GetComponent<CharacterComponent>();

	m_pPlayer1 = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 1");
	m_pPlayer2 = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 2");

	m_pMap = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Map")->GetComponent<MapComponent>();
	
	const auto& SetIsVulnerable = [&](int) {
		m_pCharacter.lock()->SetIsVulnerable(true);
	};
	m_pMap.lock()->GetOnPowerPelletCollected()->AddListener(SetIsVulnerable);

	m_pCurrentState = std::make_shared<WanderState>();
}

void LW2D::GhostComponent::Update()
{
	CalculateClosestPlayer();

	LW2D::State* state = m_pCurrentState->Update(m_pGameObject.lock());
	if (state != nullptr)
	{
		m_pCurrentState->OnExit(m_pGameObject.lock());
		m_pCurrentState.reset(state);
		m_pCurrentState->OnEnter(m_pGameObject.lock());
	}
}

void LW2D::GhostComponent::CalculateClosestPlayer()
{
	const auto& pos = GetTransform().GetWorldPosition();

	// Closest player
	if (!m_pPlayer1.expired() && m_pPlayer2.expired())
	{
		m_pClosestPlayer = m_pPlayer1;
	}
	else if (m_pPlayer1.expired() && !m_pPlayer2.expired())
	{
		m_pClosestPlayer = m_pPlayer2;
	}
	else if (!m_pPlayer1.expired() && !m_pPlayer2.expired())
	{
		if (glm::distance(m_pPlayer1.lock()->GetTransform().GetWorldPosition(), pos) < glm::distance(m_pPlayer2.lock()->GetTransform().GetWorldPosition(), pos))
			m_pClosestPlayer = m_pPlayer1;
		else
			m_pClosestPlayer = m_pPlayer2;
	}
}
