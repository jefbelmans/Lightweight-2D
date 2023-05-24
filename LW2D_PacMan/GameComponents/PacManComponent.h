#pragma once
#include <map>
#include <glm/vec2.hpp>

#include "EngineComponents/Component.h"
#include "../Utils.h"

namespace LW2D
{
	class MapComponent;
	class PacManComponent : public Component
	{
	public:
		PacManComponent(std::weak_ptr<GameObject> go);
		~PacManComponent() = default;

		void Update() override;

		// GETTERS
		bool GetIsSnappedToGrid() const { return m_IsSnappedToGrid; }

		// SETTERS
		void SetMap(std::weak_ptr<MapComponent> map) { m_pMap = map; }
		void SetDirection(LW2D::Direction dir);

	private:
		// Cached refs
		std::weak_ptr<MapComponent> m_pMap;

		// Movement
		bool m_IsSnappedToGrid{ false };
		float m_Speed{ 64.f };
		Direction m_CurrentDirection{ Direction::Left };
		
		// Double buffer
		glm::vec2 m_PreviousPos{};

		std::map<LW2D::Direction, glm::vec2> m_DirectionTranslations
		{
			{LW2D::Direction::Up, {0.f, -1.f}},
			{LW2D::Direction::Down, {0.f, 1.f}},
			{LW2D::Direction::Left, {-1.f, 0.f}},
			{LW2D::Direction::Right, {1.f, 0.f}}
		};

		// Direction change
		bool m_DoChangeDirection{ false };
		Direction m_PendingDirection{ Direction::Left };
		const float m_CoyoteTime{ .6f };
		float m_CoyoteTimer{ 0.f };

		void SnapToGrid();
	};
}
