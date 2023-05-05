#include "PacManComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameTime.h"
#include "MapComponent.h"

LW2D::PacManComponent::PacManComponent(std::weak_ptr<GameObject> go)
	: Component{ go }
{}

void LW2D::PacManComponent::Update()
{
	const float dt{ SceneManager::GetInstance().GetGameTime()->GetDeltaTime() / 1000.f };
	const auto pos{ m_pGameObject.lock()->GetTransform().GetWorldPosition() };

	std::vector<bool> walls
	{
		m_pMap.lock()->IsWall({pos.x, pos.y - 16.f }),
		m_pMap.lock()->IsWall({pos.x, pos.y + 16.f }),
		m_pMap.lock()->IsWall({pos.x - 16.f, pos.y }),
		m_pMap.lock()->IsWall({pos.x + 16.f, pos.y })
	};

	switch (m_Direction)
	{
	case LW2D::Direction::Up:
		if(!walls[0])
			m_pGameObject.lock()->GetTransform().Translate(0.f, -m_Speed * dt);
		break;
	case LW2D::Direction::Down:
		if (!walls[1])
			m_pGameObject.lock()->GetTransform().Translate(0.f, m_Speed * dt);
		break;
	case LW2D::Direction::Left:
		if (!walls[2])
			m_pGameObject.lock()->GetTransform().Translate(-m_Speed * dt, 0.f);
		break;
	case LW2D::Direction::Right:
		if (!walls[3])
			m_pGameObject.lock()->GetTransform().Translate(m_Speed * dt, 0.f);
		break;
	}
}

void LW2D::PacManComponent::SetDirection(LW2D::Direction dir)
{
	const auto pos{ m_pGameObject.lock()->GetTransform().GetWorldPosition() };

	std::vector<bool> walls
	{
		m_pMap.lock()->IsWall({pos.x, pos.y - 16.f }),
		m_pMap.lock()->IsWall({pos.x, pos.y + 16.f }),
		m_pMap.lock()->IsWall({pos.x - 16.f, pos.y }),
		m_pMap.lock()->IsWall({pos.x + 16.f, pos.y })
	};

	if(!walls[static_cast<int>(dir)])
		m_Direction = dir;
}
