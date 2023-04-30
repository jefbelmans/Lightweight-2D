#pragma once
#include "EngineComponents/Component.h"
#include "Event.h"

namespace LW2D
{
	class ScoreComponent final : public Component
	{
	public:
		ScoreComponent(std::weak_ptr<GameObject> go);
		~ScoreComponent() = default;
		
		// GETTERS
		int GetScore() const { return m_Score; }
		Event<int>* GetOnScoreChangedEvent() const { return m_pOnScoreChanged.get(); }
		
		// SETTERS
		void SetScore(int score) { m_Score = score;  m_pOnScoreChanged->Invoke(m_Score); }
		void AddScore(int score) { m_Score += score; m_pOnScoreChanged->Invoke(m_Score); }

	private:
		int m_Score{ 0 };
		std::unique_ptr<Event<int>> m_pOnScoreChanged;
	};
}