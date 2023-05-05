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
		LW2D::Direction m_Direction{Direction::Down};
	};
}
