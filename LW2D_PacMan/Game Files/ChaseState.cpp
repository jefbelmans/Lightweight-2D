#include "ChaseState.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

#include "../GameComponents/MapComponent.h"
#include "../GameComponents/CharacterComponent.h"
#include "../GameComponents/HealthComponent.h"
#include "../GameComponents/GhostComponent.h"
#include "../Game Files/Utils.h"
#include "../Game Files/WanderState.h"
#include "../Game Files/FleeState.h"

LW2D::ChaseState::ChaseState()
{
	m_pMap = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Map")->GetComponent<MapComponent>();
	m_pPlayer1 = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 1");
	m_pPlayer2 = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 2");
}

LW2D::State* LW2D::ChaseState::Update(std::shared_ptr<GameObject> pGo)
{
	HanlePlayerCollision(pGo);
	HandleMovement(pGo);

	const auto& closestPlayer = pGo->GetComponent<GhostComponent>()->GetClosestPlayer();

	if (pGo->GetComponent<CharacterComponent>()->GetIsVulnerable())
		return new FleeState();
	else if (glm::distance(closestPlayer.lock()->GetTransform().GetWorldPosition(), pGo->GetTransform().GetWorldPosition()) > m_ChangeToWanderDistance)
		return new WanderState();

	return nullptr;
}

void LW2D::ChaseState::HanlePlayerCollision(std::shared_ptr<GameObject> pGo)
{
	// Kill player if they are on the same tile
	const auto& rowCol = m_pMap.lock()->GetIndicesFromPos(pGo->GetTransform().GetWorldPosition());
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

void LW2D::ChaseState::HandleMovement(std::shared_ptr<GameObject> pGo)
{
	const auto& character = pGo->GetComponent<CharacterComponent>();
	if (!character->GetIsAtIntersection()) return;

	const auto& pos = pGo->GetTransform().GetWorldPosition();
	const auto& closestPlayer = pGo->GetComponent<GhostComponent>()->GetClosestPlayer();

	const glm::vec2& closestPlayerPos = closestPlayer.lock()->GetTransform().GetWorldPosition();

	// Get the direction to the closest player
	const auto& availableDirections = character->GetAvailableDirections();

	glm::vec2 closestDirectionToPlayer{ character->GetDirectionTranslations()[availableDirections[0]]};
	LW2D::Direction closestDirection{ availableDirections[0] };
	for (const auto& dir : availableDirections)
	{
		const glm::vec2 translation = character->GetDirectionTranslations()[dir];
		if (glm::distance(pos + translation, closestPlayerPos) < glm::distance(pos + closestDirectionToPlayer, closestPlayerPos))
		{
			closestDirectionToPlayer = translation;
			closestDirection = dir;
		}
	}

	character->SetDirection(closestDirection);
}