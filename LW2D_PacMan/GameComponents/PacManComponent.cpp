#include "PacManComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameTime.h"
#include "MapComponent.h"
#include "Scene.h"

LW2D::PacManComponent::PacManComponent(std::weak_ptr<GameObject> go)
	: Component{ go }
{
	m_pMap = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Map")->GetComponent<MapComponent>();
	if (m_pMap.lock() == nullptr)
	{
		throw std::exception("PacManComponent::PacManComponent() > Failed to find MapComponent");
	}
}

void LW2D::PacManComponent::Update()
{
	auto& pos{ GetTransform().GetWorldPosition() };

	// Check if we are on a pellet
	if (m_pMap.lock()->IsPellet({ pos.x, pos.y }))
	{
		m_pMap.lock()->CollectPellet({ pos.x, pos.y });
	}
}