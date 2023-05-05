#pragma once
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

		void SetMap(std::weak_ptr<MapComponent> map) { m_pMap = map; }
		void SetDirection(LW2D::Direction dir);

	private:
		std::weak_ptr<MapComponent> m_pMap;
		float m_Speed{ 64.f };

		Direction m_Direction{Direction::Down};

		const float m_CoyoteTime{ .5f };
		float m_CoyoteTimer{ 0.f };
		bool m_DoChangeDirection{ false };
		Direction m_PendingDirection{Direction::Down};
	};
}
