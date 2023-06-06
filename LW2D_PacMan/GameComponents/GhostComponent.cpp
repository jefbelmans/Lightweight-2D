#include "GhostComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "../GameComponents/CharacterComponent.h"
#include "../GameComponents/MapComponent.h"
#include "../GameComponents/HealthComponent.h"

LW2D::GhostComponent::GhostComponent(std::weak_ptr<GameObject> go)
	: Component(go)
{
	m_pCharacter = go.lock()->GetComponent<CharacterComponent>();

	m_pPlayer1 = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 1");
	m_pPlayer2 = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 2");

	m_pMap = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Map")->GetComponent<MapComponent>();
}

void LW2D::GhostComponent::Update()
{
	const auto& rowCol = m_pMap.lock()->GetIndicesFromPos(GetTransform().GetWorldPosition());
	if (!m_pPlayer1.expired())
	{
		const auto& player1RowCol = m_pMap.lock()->GetIndicesFromPos(m_pPlayer1.lock()->GetTransform().GetWorldPosition());
		if (rowCol == player1RowCol && m_pPlayer1.lock()->GetComponent<CharacterComponent>()->GetIsVulnerable())
			m_pPlayer1.lock()->GetComponent<HealthComponent>()->Kill();
	}
		
	if (!m_pPlayer2.expired())
	{
		const auto& player2RowCol = m_pMap.lock()->GetIndicesFromPos(m_pPlayer2.lock()->GetTransform().GetWorldPosition());
		if (rowCol == player2RowCol && m_pPlayer2.lock()->GetComponent<CharacterComponent>()->GetIsVulnerable())
			m_pPlayer2.lock()->GetComponent<HealthComponent>()->Kill();
	}
	
}

void LW2D::GhostComponent::ChangeDirection(std::vector<LW2D::Direction> availableDirections)
{
	const auto& pos = GetTransform().GetWorldPosition();

	// Closest player
	std::weak_ptr<GameObject> closestPlayer{};
	if (!m_pPlayer1.expired() && m_pPlayer2.expired())
	{
		closestPlayer = m_pPlayer1;
	}
	else if(m_pPlayer1.expired() && !m_pPlayer2.expired())
	{
		closestPlayer = m_pPlayer2;
	}
	else if (!m_pPlayer1.expired() && !m_pPlayer2.expired())
	{
		if (glm::distance(m_pPlayer1.lock()->GetTransform().GetWorldPosition(), pos) < glm::distance(m_pPlayer2.lock()->GetTransform().GetWorldPosition(), pos))
			closestPlayer = m_pPlayer1;
		else
			closestPlayer = m_pPlayer2;
	}

	const glm::vec2& closestPlayerPos = closestPlayer.lock()->GetTransform().GetWorldPosition();

	// Get the direction to the closest player
	glm::vec2 closestDirectionToPlayer{ m_pCharacter.lock()->GetDirectionTranslations()[availableDirections[0]] };
	LW2D::Direction closestDirection{availableDirections[0]};
	for (const auto& dir : availableDirections)
	{
		const glm::vec2 translation = m_pCharacter.lock()->GetDirectionTranslations()[dir];
		if (glm::distance(pos + translation, closestPlayerPos) < glm::distance(pos + closestDirectionToPlayer, closestPlayerPos))
		{
			closestDirectionToPlayer = translation;
			closestDirection = dir;
		}
	}

	m_pCharacter.lock()->SetDirection(closestDirection);
}
