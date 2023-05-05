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
	// Cache variables
	const float dt{ SceneManager::GetInstance().GetGameTime()->GetDeltaTime() / 1000.f };
	auto& transform{ m_pGameObject.lock()->GetTransform() };
	const auto pos{ transform.GetWorldPosition() };
	const auto cellSize{ m_pMap.lock()->GetCellSize() };

	// Get walls around PacMan
	std::vector<bool> walls
	{
		m_pMap.lock()->IsWall({pos.x, pos.y - cellSize}),
		m_pMap.lock()->IsWall({pos.x, pos.y + cellSize }),
		m_pMap.lock()->IsWall({pos.x - cellSize, pos.y }),
		m_pMap.lock()->IsWall({pos.x + cellSize, pos.y })
	};

	// Wait for intersection before changing direction
	if (m_DoChangeDirection)
	{
		if (((m_PendingDirection == Direction::Up || m_PendingDirection == Direction::Down) && int(pos.x) % cellSize == 0) ||
			(m_PendingDirection == Direction::Left || m_PendingDirection == Direction::Right) && int(pos.y) % cellSize == 0)
		{
			if (!walls[int(m_PendingDirection)])
			{
				m_Direction = m_PendingDirection;
				m_DoChangeDirection = false;
			}
		}
			
		m_CoyoteTimer -= dt;

		if (m_CoyoteTimer <= 0.f)
			m_DoChangeDirection = false;
	}

	// Move in current direction if not blocked by wall
	switch (m_Direction)
	{
	case LW2D::Direction::Up:
		if (!walls[0] || int(pos.y) % cellSize != 0)
		{
			transform.Translate(0.f, -int(m_Speed * dt));
		}
		break;
	case LW2D::Direction::Down:
		if (!walls[1] || int(pos.y) % cellSize != 0)
		{
			transform.Translate(0.f, int(m_Speed * dt));
		}
		break;
	case LW2D::Direction::Left:
		if (!walls[2] || int(pos.x) % cellSize != 0)
		{
			if (pos.x + cellSize < 0.f)
			{
				transform.SetLocalPosition(float(m_pMap.lock()->GetCols() * cellSize), pos.y, 0.f);
			}
			transform.Translate(-int(m_Speed * dt), 0.f);
		}
		break;
	case LW2D::Direction::Right:
		if (!walls[3] || int(pos.x) % cellSize != 0)
		{
			if (pos.x > m_pMap.lock()->GetCols() * cellSize)
			{
				transform.SetLocalPosition(-cellSize, pos.y, 0.f);
			}
			transform.Translate(int(m_Speed * dt), 0.f);
		}
		break;
	}
}

void LW2D::PacManComponent::SetDirection(LW2D::Direction dir)
{
	m_DoChangeDirection = true;
	m_PendingDirection = dir;
	m_CoyoteTimer = m_CoyoteTime;
}
