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
#include "../Game Files/Blackboard.h"

LW2D::State* LW2D::ChaseState::Update(std::shared_ptr<Blackboard> blackboard)
{
	HanlePlayerCollision(blackboard);
	HandleMovement(blackboard);

	const auto go = blackboard->Get<std::shared_ptr<GameObject>>("Agent");
	const auto closestPlayer = blackboard->Get<std::shared_ptr<GameObject>>("ClosestPlayer");

	if (go->GetComponent<CharacterComponent>()->GetIsVulnerable())
		return new FleeState();
	else if (glm::distance(closestPlayer->GetTransform().GetWorldPosition(), go->GetTransform().GetWorldPosition()) > m_ChangeToWanderDistance)
		return new WanderState();

	return nullptr;
}

void LW2D::ChaseState::HanlePlayerCollision(std::shared_ptr<Blackboard> blackboard)
{
	const auto go = blackboard->Get<std::shared_ptr<GameObject>>("Agent");
	const auto map = blackboard->Get<std::shared_ptr<MapComponent>>("Map");
	const auto player1 = blackboard->Get<std::shared_ptr<GameObject>>("Player1");
	const auto player2 = blackboard->Get<std::shared_ptr<GameObject>>("Player2");

	// Kill player if they are on the same tile
	const auto& rowCol = map->GetIndicesFromPos(go->GetTransform().GetWorldPosition());
	if (player1)
	{
		const auto& player1RowCol = map->GetIndicesFromPos(player1->GetTransform().GetWorldPosition());
		if (rowCol == player1RowCol && player1->GetComponent<CharacterComponent>()->GetIsVulnerable())
			player1->GetComponent<HealthComponent>()->Kill();
	}

	if (player2)
	{
		const auto& player2RowCol = map->GetIndicesFromPos(player2->GetTransform().GetWorldPosition());
		if (rowCol == player2RowCol && player2->GetComponent<CharacterComponent>()->GetIsVulnerable())
			player2->GetComponent<HealthComponent>()->Kill();
	}
}

void LW2D::ChaseState::HandleMovement(std::shared_ptr<Blackboard> blackboard)
{
	const auto go = blackboard->Get<std::shared_ptr<GameObject>>("Agent");
	const auto character = go->GetComponent<CharacterComponent>();

	if (!character->GetIsAtIntersection()) return;

	const auto& pos = go->GetTransform().GetWorldPosition();
	const auto closestPlayer = blackboard->Get<std::shared_ptr<GameObject>>("ClosestPlayer");

	const glm::vec2& closestPlayerPos = closestPlayer->GetTransform().GetWorldPosition();

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