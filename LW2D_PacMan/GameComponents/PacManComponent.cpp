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
	const auto& pos{ transform.GetWorldPosition() };
	const int cellSize{ m_pMap.lock()->GetCellSize() };

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
		// If Pac Man is at an intersection
		if (((m_PendingDirection == Direction::Up || m_PendingDirection == Direction::Down) && int(pos.x) % cellSize == 0) ||
			(m_PendingDirection == Direction::Left || m_PendingDirection == Direction::Right) && int(pos.y) % cellSize == 0)
		{
			// If there is no wall in the direction we want to move in
			if (!walls[int(m_PendingDirection)])
			{
				m_CurrentDirection = m_PendingDirection;
				m_IsAgainstWall = m_DoChangeDirection = false;
			}
		}
			
		m_CoyoteTimer -= dt;
		m_DoChangeDirection = m_CoyoteTimer > 0.f;
	}

	// Update Pac Man's position based on the current direction
	m_IsAgainstWall = walls[static_cast<int>(m_CurrentDirection)] && (static_cast<int>(pos.x) % cellSize == 0) && (static_cast<int>(pos.y) % cellSize == 0);
	if(!m_IsAgainstWall)
	{
		// Get the translation vector based on the current direction and move Pac Man
		const glm::vec2 translation = m_DirectionTranslations[m_CurrentDirection];
		transform.Translate(m_Speed * dt * translation.x, m_Speed * dt * translation.y);

		// Wrap Pac Man around the map
		if (m_CurrentDirection == LW2D::Direction::Left && pos.x + cellSize <= 0.f)
		{
			transform.SetLocalPosition(static_cast<float>(m_pMap.lock()->GetCols() * cellSize), pos.y);
		}
		else if (m_CurrentDirection == LW2D::Direction::Right && pos.x > m_pMap.lock()->GetCols() * cellSize)
		{
			transform.SetLocalPosition(static_cast<float>(-cellSize), pos.y);
		}
		
		m_IsSnappedToGrid = false;
	}
	else if(!m_IsSnappedToGrid)
	{
		transform.SetLocalPosition(floorf(pos.x), floorf(pos.y));
		m_IsSnappedToGrid = true;
	}
	
	// Check if we are on a pellet
	if (m_pMap.lock()->IsPellet({pos.x, pos.y}))
	{
		m_pMap.lock()->CollectPellet({ pos.x, pos.y });
	}
}

void LW2D::PacManComponent::SetDirection(LW2D::Direction dir)
{
	m_DoChangeDirection = true;
	m_PendingDirection = dir;
	m_CoyoteTimer = m_CoyoteTime;
}
