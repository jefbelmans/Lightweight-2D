#include "FleeState.h"
#include "GameObject.h"

#include "../GameComponents/CharacterComponent.h"
#include "../GameComponents/GhostComponent.h"
#include "../GameComponents/MapComponent.h"
#include "../GameComponents/HealthComponent.h"
#include "../Game Files/WanderState.h"
#include "../Game Files/Blackboard.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameTime.h"

LW2D::State* LW2D::FleeState::Update(std::shared_ptr<Blackboard> blackboard)
{
	HandleMovement(blackboard);

	m_FleeTimer -= SceneManager::GetInstance().GetGameTime()->GetDeltaTime() / 1000.f;
	if (m_FleeTimer < 0.f)
		return new WanderState();

    return nullptr;
}

void LW2D::FleeState::OnEnter(std::shared_ptr<Blackboard> blackboard)
{
	m_FleeTimer = m_FleeTime;
	std::cout << "run!!!\n";
	// TODO: set sprite to flee
}

void LW2D::FleeState::OnExit(std::shared_ptr<Blackboard> blackboard)
{
	std::cout << "pfew!!!\n";
	blackboard->Get<std::weak_ptr<GameObject>>("Agent").lock()->GetComponent<CharacterComponent>()->SetIsVulnerable(true);
}

void LW2D::FleeState::HandleMovement(std::shared_ptr<Blackboard> blackboard)
{
	const auto gameObject = blackboard->Get<std::weak_ptr<GameObject>>("Agent");
	const auto character = gameObject.lock()->GetComponent<CharacterComponent>();

	if (!character->GetIsAtIntersection()) return;

	const auto& pos = gameObject.lock()->GetTransform().GetWorldPosition();
	const auto closestPlayer = blackboard->Get<std::weak_ptr<GameObject>>("ClosestPlayer");

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

void LW2D::FleeState::HandlePlayerCollision(std::shared_ptr<Blackboard> blackboard)
{
	auto go = blackboard->Get<std::weak_ptr<GameObject>>("Agent");
	const auto map = blackboard->Get<std::weak_ptr<MapComponent>>("Map");
	const auto player1 = blackboard->Get<std::weak_ptr<GameObject>>("Player1");
	const auto player2 = blackboard->Get<std::weak_ptr<GameObject>>("Player2");

	// Kill player if they are on the same tile
	const auto& rowCol = map.lock()->GetIndicesFromPos(go.lock()->GetTransform().GetWorldPosition());
	if (!player1.expired())
	{
		const auto& player1RowCol = map.lock()->GetIndicesFromPos(player1.lock()->GetTransform().GetWorldPosition());
		if (rowCol == player1RowCol && go.lock()->GetComponent<CharacterComponent>()->GetIsVulnerable())
			go.lock()->GetComponent<HealthComponent>()->Kill();
	}

	if (!player2.expired())
	{
		const auto& player2RowCol = map.lock()->GetIndicesFromPos(player2.lock()->GetTransform().GetWorldPosition());
		if (rowCol == player2RowCol && go.lock()->GetComponent<CharacterComponent>()->GetIsVulnerable())
			go.lock()->GetComponent<HealthComponent>()->Kill();
	}
}
