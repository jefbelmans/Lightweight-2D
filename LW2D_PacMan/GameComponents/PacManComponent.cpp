#include "PacManComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameTime.h"
#include "MapComponent.h"
#include "Scene.h"
#include "HealthComponent.h"
#include "CharacterComponent.h"
#include "ServiceLocator.h"

LW2D::PacManComponent::PacManComponent(std::weak_ptr<GameObject> go)
	: Component{ go }
{
	// Get cached refs
	m_pMap = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Map")->GetComponent<MapComponent>();
	if (m_pMap.lock() == nullptr)
		throw std::exception("PacManComponent::PacManComponent() > Failed to find MapComponent");

	m_pCharacter = go.lock()->GetComponent<CharacterComponent>();
	if (m_pCharacter.lock() == nullptr)
		throw std::exception("PacManComponent::PacManComponent() > Failed to find CharacterComponent");

	// Subscribe to events
	go.lock()->GetComponent<HealthComponent>()->GetOnKillEvent()->AddListener(std::bind(&PacManComponent::OnKill, this));
	go.lock()->GetComponent<HealthComponent>()->GetOnDeathEvent()->AddListener(std::bind(&PacManComponent::OnDeath, this));
}

void LW2D::PacManComponent::Update()
{
	auto& pos{ GetTransform().GetWorldPosition() };

	// Check if we are on a pellet / power pellet
	Cell currentCell{ m_pMap.lock()->GetCellFromPos({ pos.x, pos.y }) };
	if (currentCell == Cell::Pellet || currentCell == Cell::PowerPellet)
	{
		m_pMap.lock()->CollectPellet({ pos.x, pos.y });
	}
}

void LW2D::PacManComponent::OnKill()
{
	if(!m_IsDead)
		LW2D::ServiceLocator::GetSoundSystem().PlaySound((unsigned short)Sounds::PacManDeath, 64.f);
}

void LW2D::PacManComponent::OnDeath()
{
	m_IsDead = true;
	m_pCharacter.lock()->SetDoMove(false);
}