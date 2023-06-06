#include "WanderState.h"
#include "GameObject.h"

#include "../GameComponents/CharacterComponent.h"
#include "../GameComponents/GhostComponent.h"
#include "../Game Files/ChaseState.h"
#include "../Game Files/FleeState.h"

LW2D::State* LW2D::WanderState::Update(std::shared_ptr<GameObject> pGo)
{
    const auto& character = pGo->GetComponent<CharacterComponent>();
    const auto& availableDirections = character->GetAvailableDirections();
    const auto& closestPlayer = pGo->GetComponent<GhostComponent>()->GetClosestPlayer();

    if (character->GetIsAtIntersection())
		character->SetDirection(availableDirections[rand() % availableDirections.size()]);

    if (pGo->GetComponent<CharacterComponent>()->GetIsVulnerable())
        return new FleeState();
    else if (glm::distance(closestPlayer.lock()->GetTransform().GetWorldPosition(), pGo->GetTransform().GetWorldPosition()) < m_ChangeToChaseDist)
        return new ChaseState();

    return nullptr;
}