#pragma once
#include "Component.h"
#include "glm/glm.hpp"

namespace LW2D
{
	class GameTime;
	class OrbitComponent final : public Component
	{
	public:
		OrbitComponent(std::weak_ptr<GameObject> go);
		~OrbitComponent() = default;

		void Update() override;

		void SetDistance(float distance) { m_Distance = distance; }
		void SetSpeed(float speed) { m_Speed = speed; }
		
	private:
		float m_Distance{ 24.0f };
		float m_Speed{ 5.f };
		float m_CurrAngle{ 0.f };
		const glm::vec3 m_OrbitPos{};

		std::weak_ptr<GameTime> m_pGameTime;
	};
}
