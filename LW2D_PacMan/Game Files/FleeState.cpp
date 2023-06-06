#include "FleeState.h"
#include "GameObject.h"

#include "../GameComponents/CharacterComponent.h"
#include "../GameComponents/GhostComponent.h"
#include "../GameComponents/MapComponent.h"
#include "../GameComponents/HealthComponent.h"
#include "../Game Files/WanderState.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameTime.h"

LW2D::FleeState::FleeState()
{
	m_pMap = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Map")->GetComponent<MapComponent>();
	m_pPlayer1 = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 1");
	m_pPlayer2 = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Player 2");
}

LW2D::State* LW2D::FleeState::Update(std::shared_ptr<GameObject> pGo)
{
	HandleMovement(pGo);

	m_FleeTimer -= SceneManager::GetInstance().GetGameTime()->GetDeltaTime() / 1000.f;
	if (m_FleeTimer < 0.f)
		return new WanderState();

    return nullptr;
}

void LW2D::FleeState::OnEnter(std::shared_ptr<GameObject> pGo)
{
	m_FleeTimer = m_FleeTime;
	std::cout << "run!!!\n";
	// TODO: set sprite to flee
}

void LW2D::FleeState::OnExit(std::shared_ptr<GameObject> pGo)
{
	std::cout << "pfew!!!\n";
	pGo->GetComponent<CharacterComponent>()->SetIsVulnerable(false);
}

void LW2D::FleeState::HandleMovement(std::shared_ptr<GameObject> pGo)
{
	const auto& character = pGo->GetComponent<CharacterComponent>();
	if (!character->GetIsAtIntersection()) return;

	const auto& pos = pGo->GetTransform().GetWorldPosition();
	const auto& closestPlayer = pGo->GetComponent<GhostComponent>()->GetClosestPlayer();

	const glm::vec2& closestPlayerPos = closestPlayer.lock()->GetTransform().GetWorldPosition();

	// Get the direction to the closest player
	const auto& availableDirections = character->GetAvailableDirections();

	glm::vec2 furthestDirectionToPlayer{ character->GetDirectionTranslations()[availableDirections[0]] };
	LW2D::Direction furthestDirection{ availableDirections[0] };
	for (const auto& dir : availableDirections)
	{
		const glm::vec2 translation = character->GetDirectionTranslations()[dir];
		if (glm::distance(pos + translation, closestPlayerPos) > glm::distance(pos + furthestDirectionToPlayer, closestPlayerPos))
		{
			furthestDirectionToPlayer = translation;
			furthestDirection = dir;
		}
	}

	character->SetDirection(furthestDirection);
}

void LW2D::FleeState::HandlePlayerCollision(std::shared_ptr<GameObject> pGo)
{
	// Kill player if they are on the same tile
	const auto& rowCol = m_pMap.lock()->GetIndicesFromPos(pGo->GetTransform().GetWorldPosition());
	if (!m_pPlayer1.expired())
	{
		const auto& player1RowCol = m_pMap.lock()->GetIndicesFromPos(m_pPlayer1.lock()->GetTransform().GetWorldPosition());
		if (rowCol == player1RowCol && pGo->GetComponent<CharacterComponent>()->GetIsVulnerable())
			pGo->GetComponent<HealthComponent>()->Kill();
	}

	if (!m_pPlayer2.expired())
	{
		const auto& player2RowCol = m_pMap.lock()->GetIndicesFromPos(m_pPlayer2.lock()->GetTransform().GetWorldPosition());
		if (rowCol == player2RowCol && pGo->GetComponent<CharacterComponent>()->GetIsVulnerable())
			pGo->GetComponent<HealthComponent>()->Kill();
	}
}
