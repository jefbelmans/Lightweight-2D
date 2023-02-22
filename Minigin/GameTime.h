#pragma once
#include <chrono>

namespace LW2D
{
	class GameTime final
	{
	public:
		GameTime() = default;
		~GameTime() = default;

		GameTime(const GameTime& other) = delete;
		GameTime(GameTime&& other) noexcept = delete;
		GameTime& operator=(const GameTime& other) = delete;
		GameTime& operator=(GameTime&& other) noexcept = delete;

		void Update();

		float GetDeltaTime() const { return  m_DeltaTime; }
		float GetTime() const { return m_Time; }

	private:
		float m_Time{};
		float m_DeltaTime{};

		float m_MaxDeltaTime{ 0.03f };

		std::chrono::high_resolution_clock::time_point m_PrevTime{};
		std::chrono::high_resolution_clock::time_point m_CurrTime{};
	};
}


