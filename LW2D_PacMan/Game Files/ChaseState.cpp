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

	const auto go = blackboard->Get<std::weak_ptr<GameObject>>("Agent");
	const auto closestPlayer = blackboard->Get<std::weak_ptr<GameObject>>("ClosestPlayer");

	if (go.lock()->GetComponent<CharacterComponent>()->GetIsVulnerable())
		return new FleeState();
	else if (glm::distance(closestPlayer.lock()->GetTransform().GetWorldPosition(), go.lock()->GetTransform().GetWorldPosition()) > m_ChangeToWanderDistance)
		return new WanderState();

	return nullptr;
}

void LW2D::ChaseState::HanlePlayerCollision(std::shared_ptr<Blackboard> blackboard)
{
	const auto go = blackboard->Get<std::weak_ptr<GameObject>>("Agent");
	const auto map = blackboard->Get<std::weak_ptr<MapComponent>>("Map");
	const auto player1 = blackboard->Get<std::weak_ptr<GameObject>>("Player1");
	const auto player2 = blackboard->Get<std::weak_ptr<GameObject>>("Player2");

	// Kill player if they are on the same tile
	const auto& rowCol = map.lock()->GetIndicesFromPos(go.lock()->GetTransform().GetWorldPosition());
	if (!player1.expired())
	{
		const auto& player1RowCol = map.lock()->GetIndicesFromPos(player1.lock()->GetTransform().GetWorldPosition());
		if (rowCol == player1RowCol && player1.lock()->GetComponent<CharacterComponent>()->GetIsVulnerable())
			player1.lock()->GetComponent<HealthComponent>()->Kill();
	}

	if (!player2.expired())
	{
		const auto& player2RowCol = map.lock()->GetIndicesFromPos(player2.lock()->GetTransform().GetWorldPosition());
		if (rowCol == player2RowCol && player2.lock()->GetComponent<CharacterComponent>()->GetIsVulnerable())
			player2.lock()->GetComponent<HealthComponent>()->Kill();
	}
}

void LW2D::ChaseState::HandleMovement(std::shared_ptr<Blackboard> blackboard)
{
	const auto go = blackboard->Get<std::weak_ptr<GameObject>>("Agent");
	const auto character = go.lock()->GetComponent<CharacterComponent>();

	if (!character->GetIsAtIntersection()) return;

	const auto& pos = go.lock()->GetTransform().GetWorldPosition();
	const auto closestPlayer = blackboard->Get<std::weak_ptr<GameObject>>("ClosestPlayer");

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