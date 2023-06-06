#pragma once
#include <map>
#include <glm/vec2.hpp>
#include "EngineComponents/Component.h"
#include "Event.h"
#include "../Game Files/Utils.h"

namespace LW2D
{
	class MapComponent;
	class CharacterComponent final : public Component
	{
	public:
		CharacterComponent(std::weak_ptr<GameObject> go);
		~CharacterComponent() = default;

		void Update() override;

		// GETTERS
		LW2D::Direction GetDirection() const { return m_CurrentDirection; }
		LW2D::Direction GetOppositeDirection() const;
		auto GetDirectionTranslations() const { return m_DirectionTranslations; }
		bool GetIsSnappedToGrid() const { return m_IsSnappedToGrid; }
		Event<std::vector<LW2D::Direction>>* GetIsAtIntersection() const { return m_pIsAtIntersection.get(); }

		// SETTERS
		void SetMap(std::weak_ptr<MapComponent> map) { m_pMap = map; }
		void SetDirection(LW2D::Direction dir);
		void SetSpeed(float speed) { m_Speed = speed; }

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
		const float m_CoyoteTime{ .3f };
		float m_CoyoteTimer{ 0.f };

		void SnapToGrid();
		void CheckForIntersection(const glm::vec2& pos, const std::vector<bool>& walls);

		std::unique_ptr<Event<std::vector<LW2D::Direction>>> m_pIsAtIntersection;
	};
}


