#include "WanderState.h"
#include "GameObject.h"

#include "../GameComponents/CharacterComponent.h"
#include "../GameComponents/GhostComponent.h"
#include "../GameComponents/HealthComponent.h"
#include "../Game Files/ChaseState.h"
#include "../Game Files/FleeState.h"
#include "../Game Files/Blackboard.h"

LW2D::State* LW2D::WanderState::Update(std::shared_ptr<Blackboard> blackboard)
{
    const auto go = blackboard->Get<GameObject*>("Agent");
    const auto closestPlayer = blackboard->Get<GameObject*>("ClosestPlayer");

    // Transitions
    if (glm::distance(closestPlayer->GetTransform().GetWorldPosition(), go->GetTransform().GetWorldPosition()) < m_ChangeToChaseDist
        && !closestPlayer->GetComponent<HealthComponent>()->IsDead())
        return new ChaseState();

    if (!blackboard->Get<bool>("IsCPU")) return nullptr;

    // Update movement
    const auto character = go->GetComponent<CharacterComponent>();
    const auto availableDirections = character->GetAvailableDirections();
    if (character->GetIsAtIntersection())
        character->SetDirection(availableDirections[rand() % availableDirections.size()]);

    return nullptr;
}