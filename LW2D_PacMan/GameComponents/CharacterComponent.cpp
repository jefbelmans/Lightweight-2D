#include "CharacterComponent.h"
#include "GameObject.h"
#include "GameTime.h"
#include "SceneManager.h"
#include "Scene.h"

#include "MapComponent.h"
#include "HealthComponent.h"

LW2D::CharacterComponent::CharacterComponent(std::weak_ptr<GameObject> go, const glm::vec2& spawnPos, float respawnTime)
	: Component{ go }
	, m_SpawnPos{ spawnPos }
	, m_RespawnTime{ respawnTime }
{
	GetTransform().SetLocalPosition(m_SpawnPos);

	m_pMap = SceneManager::GetInstance().GetActiveScene()->FindObjectByName("Map").lock()->GetComponent<MapComponent>();

	go.lock()->GetComponent<HealthComponent>()->GetOnKillEvent()->AddListener(std::bind(&CharacterComponent::Respawn, this, std::placeholders::_1, false));
}

void LW2D::CharacterComponent::Update()
{
	HandleMovement();

	// Handle respawning
	if (m_IsRespawning)
	{
		m_RespawnTimer -= SceneManager::GetInstance().GetGameTime()->GetDeltaTime() / 1000;
		if (m_RespawnTimer <= 0.f)
		{
			m_RespawnTimer = 0.0f;
			GetTransform().SetLocalPosition(m_SpawnPos);
			m_IsRespawning = false;
			m_DoMove = true;
			m_CurrentDirection = LW2D::Direction::Right;
		}
	}
}

void LW2D::CharacterComponent::Respawn(int lives, bool instant)
{
	// Only if we have lives left, do we want to respawn
	if (lives > 0)
	{
		
		m_RespawnTimer = instant ? 0.f : m_RespawnTime;
		m_IsRespawning = true;
		m_DoMove = false;
	}
}

LW2D::Direction LW2D::CharacterComponent::GetOppositeDirection() const
{
	switch (m_CurrentDirection)
	{
	case LW2D::Direction::Up:
		return LW2D::Direction::Down;
	case LW2D::Direction::Down:
		return LW2D::Direction::Up;
	case LW2D::Direction::Left:
		return LW2D::Direction::Right;
	case LW2D::Direction::Right:
		return LW2D::Direction::Left;
	default:
		return LW2D::Direction::Up;
	}
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

void LW2D::CharacterComponent::CheckForIntersection(const glm::vec2& pos, const std::vector<bool>& walls)
{
	const int cellSize{ m_pMap.lock()->GetCellSize() };
	std::vector<LW2D::Direction> availableDirections{};

	for (int i = 0; i < static_cast<int>(m_DirectionTranslations.size()); i++)
	{
		if (!walls[i]) availableDirections.emplace_back(static_cast<LW2D::Direction>(i));
	}

	bool isVertical = (m_CurrentDirection == Direction::Up || m_CurrentDirection == Direction::Down);
	bool isHorizontal = (m_CurrentDirection == Direction::Left || m_CurrentDirection == Direction::Right);
	m_IsAtIntersection = ((isVertical && static_cast<int>(pos.y) % cellSize == 0 && (!walls[static_cast<int>(Direction::Left)] || !walls[static_cast<int>(Direction::Right)])) ||
		(isHorizontal && static_cast<int>(m_PreviousPos.x) % cellSize == 0 && (!walls[static_cast<int>(Direction::Up)] || !walls[static_cast<int>(Direction::Down)])) ||
		availableDirections.size() == 1);

	if (m_IsAtIntersection)
	{
		m_AvailableDirections = availableDirections;
	}
}

// I took some inspiration on handling movement from following source: https://github.com/Kofybrek/Pacman/tree/Main
void LW2D::CharacterComponent::HandleMovement()
{
	if(!m_DoMove) return;

	// Cache variables
	const float dt{ SceneManager::GetInstance().GetGameTime()->GetDeltaTime() / 1000.f };
	auto& transform{ m_pGameObject.lock()->GetTransform() };
	const auto& pos{ transform.GetWorldPosition() };
	const int cellSize{ m_pMap.lock()->GetCellSize() };

	// Get walls around character
	std::vector<bool> walls
	{
		m_pMap.lock()->GetCellFromPos({pos.x, pos.y - 1}) == Cell::Wall ? true : false,
		m_pMap.lock()->GetCellFromPos({pos.x, pos.y + cellSize }) == Cell::Wall ? true : false,
		m_pMap.lock()->GetCellFromPos({pos.x - 1, pos.y }) == Cell::Wall ? true : false,
		m_pMap.lock()->GetCellFromPos({pos.x + cellSize, pos.y }) == Cell::Wall ? true : false
	};

	// Wait for intersection before changing direction
	if (m_DoChangeDirection)
	{
		m_CoyoteTimer -= dt;
		m_DoChangeDirection = m_CoyoteTimer > 0.f;

		// If character is at an intersection
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

	CheckForIntersection(pos, walls);

	m_PreviousPos = pos;
}