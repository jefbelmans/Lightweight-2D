#include "CharacterComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameTime.h"
#include "MapComponent.h"
#include "Scene.h"

LW2D::CharacterComponent::CharacterComponent(std::weak_ptr<GameObject> go)
	: Component{ go }
{
	m_pMap = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Map")->GetComponent<MapComponent>();
	if (m_pMap.lock() == nullptr)
	{
		throw std::exception("PacManComponent::PacManComponent() > Failed to find MapComponent");
	}
}

void LW2D::CharacterComponent::Update()
{
	// Cache variables
	const float dt{ SceneManager::GetInstance().GetGameTime()->GetDeltaTime() / 1000.f };
	auto& transform{ m_pGameObject.lock()->GetTransform() };
	const auto& pos{ transform.GetWorldPosition() };
	const int cellSize{ m_pMap.lock()->GetCellSize() };

	// Get walls around character
	std::vector<bool> walls
	{
		m_pMap.lock()->IsWall({pos.x, pos.y - 1}),
		m_pMap.lock()->IsWall({pos.x, pos.y + cellSize }),
		m_pMap.lock()->IsWall({pos.x - 1, pos.y }),
		m_pMap.lock()->IsWall({pos.x + cellSize, pos.y })
	};

	// Wait for intersection before changing direction
	if (m_DoChangeDirection)
	{
		// If Pac Man is at an intersection
		if (((m_PendingDirection == Direction::Up || m_PendingDirection == Direction::Down) && static_cast<int>(pos.x) % cellSize == 0 ||
			(m_PendingDirection == Direction::Left || m_PendingDirection == Direction::Right) && static_cast<int>(pos.y) % cellSize == 0))
		{
			// If there is no wall in the direction we want to move in
			if (!walls[int(m_PendingDirection)])
			{
				m_CurrentDirection = m_PendingDirection;
				m_DoChangeDirection = false;
			}
		}

		m_CoyoteTimer -= dt;
		m_DoChangeDirection = m_CoyoteTimer > 0.f;
	}

	// Update character's position based on the current direction
	if (!walls[static_cast<int>(m_CurrentDirection)])
	{
		// Get the translation vector based on the current direction and move character
		const glm::vec2 translation = m_DirectionTranslations[m_CurrentDirection];
		transform.Translate(m_Speed * dt * translation.x, m_Speed * dt * translation.y);

		// Wrap character around the map
		if (m_CurrentDirection == Direction::Left && pos.x + cellSize <= 0.f)
		{
			transform.SetLocalPosition(static_cast<float>(m_pMap.lock()->GetCols() * cellSize), pos.y);
		}
		else if (m_CurrentDirection == Direction::Right && pos.x > m_pMap.lock()->GetCols() * cellSize)
		{
			transform.SetLocalPosition(static_cast<float>(-cellSize), pos.y);
		}

		m_IsSnappedToGrid = false;
	}
	else
		SnapToGrid();

	// Check if we are on a pellet
	if (m_pMap.lock()->IsPellet({ pos.x, pos.y }))
	{
		m_pMap.lock()->CollectPellet({ pos.x, pos.y });
	}

	m_PreviousPos = pos;
}

void LW2D::CharacterComponent::SetDirection(LW2D::Direction dir)
{
	m_DoChangeDirection = true;
	m_PendingDirection = dir;
	m_CoyoteTimer = m_CoyoteTime;
}

void LW2D::CharacterComponent::SnapToGrid()
{
	if (m_IsSnappedToGrid) return;

	const int cellSize{ m_pMap.lock()->GetCellSize() };
	const auto& pos{ GetTransform().GetWorldPosition() };

	// character's pivot point is at the top left corner, so if we are moving left or up, we need to snap to the previous cell
	int cellX = m_CurrentDirection == Direction::Left ? static_cast<int>(m_PreviousPos.x / cellSize) : static_cast<int>(pos.x / cellSize);
	int cellY = m_CurrentDirection == Direction::Up ? static_cast<int>(m_PreviousPos.y / cellSize) : static_cast<int>(pos.y / cellSize);
	float snappedPosX = static_cast<float>(cellX * cellSize);
	float snappedPosY = static_cast<float>(cellY * cellSize);

	GetTransform().SetLocalPosition(snappedPosX, snappedPosY);
	m_IsSnappedToGrid = true;
}