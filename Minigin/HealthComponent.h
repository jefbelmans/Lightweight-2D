#pragma once
#include "Component.h"
#include "Event.h"

namespace LW2D
{
	class HealthComponent final : public Component
	{
	public:
		HealthComponent(std::weak_ptr<GameObject> go);
		~HealthComponent() = default;
		
		// GETTERS
		Event<int>* GetOnDeathEvent() const { return m_pOnDeath.get(); }

		// SETTERS
		void SetLives(int lives) { m_Lives = lives; }

		void Kill();
		
	private:
		int m_Lives;
		std::unique_ptr<Event<int>> m_pOnDeath;
	};
}