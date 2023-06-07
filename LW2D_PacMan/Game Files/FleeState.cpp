#include "FleeState.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameTime.h"


#include "EngineComponents/RenderComponent.h"
#include "../GameComponents/CharacterComponent.h"
#include "../GameComponents/GhostComponent.h"
#include "../GameComponents/MapComponent.h"
#include "../GameComponents/HealthComponent.h"

#include "../Game Files/WanderState.h"
#include "../Game Files/Blackboard.h"

LW2D::State* LW2D::FleeState::Update(std::shared_ptr<Blackboard> blackboard)
{
	HandleMovement(blackboard);

	m_FleeTimer -= SceneManager::GetInstance().GetGameTime()->GetDeltaTime() / 1000.f;
	if (m_FleeTimer < 0.f)
		return new WanderState();

    return HandlePlayerCollision(blackboard);
}

void LW2D::FleeState::OnEnter(std::shared_ptr<Blackboard> blackboard)
{
	m_FleeTimer = m_FleeTime;
	GameObject* go = blackboard->Get<GameObject*>("Agent");
	go->GetComponent<CharacterComponent>()->SetSpeed(32.f);
	go->GetComponent<RenderComponent>()->SetTexture("ScaredGhost.png");
}

void LW2D::FleeState::OnExit(std::shared_ptr<Blackboard> blackboard)
{
	GameObject* go = blackboard->Get<GameObject*>("Agent");
	go->GetComponent<CharacterComponent>()->SetSpeed(56.f);
	go->GetComponent<RenderComponent>()->SetTexture("Ghost.png");
}

void LW2D::FleeState::HandleMovement(std::shared_ptr<Blackboard> blackboard)
{
	if (!blackboard->Get<bool>("IsCPU")) return;

	const auto gameObject = blackboard->Get<GameObject*>("Agent");
	const auto character = gameObject->GetComponent<CharacterComponent>();

	if (!character->GetIsAtIntersection()) return;

	const auto& pos = gameObject->GetTransform().GetWorldPosition();
	const auto closestPlayer = blackboard->Get<GameObject*>("ClosestPlayer");

	const glm::vec2& closestPlayerPos = closestPlayer->GetTransform().GetWorldPosition();

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

LW2D::State* LW2D::FleeState::HandlePlayerCollision(std::shared_ptr<Blackboard> blackboard)
{
	auto go = blackboard->Get<GameObject*>("Agent");
	const auto map = blackboard->Get<MapComponent*>("Map");
	const auto player1 = blackboard->Get<GameObject*>("Player1");
	const auto player2 = blackboard->Get<GameObject*>("Player2");

	// Kill player if they are on the same tile
	const auto& rowCol = map->GetIndicesFromPos(go->GetTransform().GetWorldPosition());
	if (player1)
	{
		const auto& player1RowCol = map->GetIndicesFromPos(player1->GetTransform().GetWorldPosition());
		if (rowCol == player1RowCol && !go->GetComponent<CharacterComponent>()->GetIsRespawning())
		{
			go->GetComponent<HealthComponent>()->Kill();
			return new WanderState();
		}	
	}

	if (player2)
	{
		const auto& player2RowCol = map->GetIndicesFromPos(player2->GetTransform().GetWorldPosition());
		if (rowCol == player2RowCol && !go->GetComponent<CharacterComponent>()->GetIsRespawning())
		{
			go->GetComponent<HealthComponent>()->Kill();
			return new WanderState();
		}
	}

	return nullptr;
}
