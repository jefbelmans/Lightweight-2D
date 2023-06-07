#pragma once
#include <memory>

#include "EngineComponents/Component.h"

namespace LW2D
{
	class HealthComponent;
	class TextComponent;
	class GameModeComponent final : public Component
	{
	public:
		GameModeComponent(std::weak_ptr<GameObject> go);
		~GameModeComponent() = default;

		GameModeComponent(const GameModeComponent&) = delete;
		GameModeComponent(GameModeComponent&&) = delete;
		GameModeComponent& operator=(const GameModeComponent&) = delete;
		GameModeComponent& operator=(GameModeComponent&&) = delete;

		void Initialize() override;

		bool GetIsGameOver() const { return m_IsGameOver; }

	private:
		// Cached refs
		std::weak_ptr<TextComponent> m_pEndOfGameText{};
		std::weak_ptr<HealthComponent> m_pHealthP1{};
		std::weak_ptr<HealthComponent> m_pHealthP2{};

		bool m_IsGameOver{};

		int m_RemainingPellets{};

		void OnPelletCollected(int);
		void OnPlayerDied();

		void HandleGameWin();
		void HandleGameOver();
	};
}