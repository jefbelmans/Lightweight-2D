#include "WanderState.h"
#include "GameObject.h"

#include "../GameComponents/CharacterComponent.h"
#include "../GameComponents/GhostComponent.h"
#include "../Game Files/ChaseState.h"
#include "../Game Files/FleeState.h"
#include "../Game Files/Blackboard.h"

LW2D::State* LW2D::WanderState::Update(std::shared_ptr<Blackboard> blackboard)
{
    const auto go = blackboard->Get<std::weak_ptr<GameObject>>("Agent");
    const auto character = go.lock()->GetComponent<CharacterComponent>();
    const auto availableDirections = character->GetAvailableDirections();
    const auto closestPlayer = blackboard->Get<std::weak_ptr<GameObject>>("ClosestPlayer");

    if (character->GetIsAtIntersection())
		character->SetDirection(availableDirections[rand() % availableDirections.size()]);

    if (character->GetIsVulnerable())
        return new FleeState();
    else if (glm::distance(closestPlayer.lock()->GetTransform().GetWorldPosition(), go.lock()->GetTransform().GetWorldPosition()) < m_ChangeToChaseDist)
        return new ChaseState();

    return nullptr;
}