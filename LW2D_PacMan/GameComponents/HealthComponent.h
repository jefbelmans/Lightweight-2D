#pragma once
#include "EngineComponents/Component.h"
#include "Event.h"

namespace LW2D
{
	class HealthComponent final : public Component
	{
	public:
		HealthComponent(std::weak_ptr<GameObject> go);
		~HealthComponent() = default;
		
		void Kill();

		// GETTERS
		Event<int>* GetOnKillEvent() const { return m_pOnKill.get(); }
		Event<>* GetOnDeathEvent() const { return m_pOnDeath.get(); }
		bool IsDead() const { return m_Lives <= 0; }

		// SETTERS
		void SetLives(int lives) { m_Lives = lives; }
		
	private:
		int m_Lives;
		std::unique_ptr<Event<int>> m_pOnKill;
		std::unique_ptr<Event<>> m_pOnDeath;
	};
}