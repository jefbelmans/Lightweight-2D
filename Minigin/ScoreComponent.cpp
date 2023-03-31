#include "ScoreComponent.h"

LW2D::ScoreComponent::ScoreComponent(std::weak_ptr<GameObject> go)
	: Component(go)
{
	m_pOnScoreChanged = std::make_unique<Event<int>>();
}
