#pragma once
#include <memory>

#include "EngineComponents/Component.h"

namespace LW2D
{
	class HealthComponent;
	class GameModeComponent final : public Component
	{
	public:
		GameModeComponent(std::weak_ptr<GameObject> go);
		~GameModeComponent() = default;

		GameModeComponent(const GameModeComponent&) = delete;
		GameModeComponent(GameModeComponent&&) = delete;
		GameModeComponent& operator=(const GameModeComponent&) = delete;
		GameModeComponent& operator=(GameModeComponent&&) = delete;

	private:
		std::weak_ptr<HealthComponent> m_pHealthP1{};
		std::weak_ptr<HealthComponent> m_pHealthP2{};

		int m_RemainingPellets{};

		void OnPelletCollected(int);
		void OnPlayerDied();

		void HandleGameWin();
		void HandleGameOver();
	};
}