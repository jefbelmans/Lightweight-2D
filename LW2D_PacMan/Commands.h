#pragma once
#include "Command.h"
#include "Utils.h"

namespace LW2D
{
	class PacManComponent;
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(std::shared_ptr<PacManComponent> pacman, const LW2D::Direction& dir, float speed)
			: m_PacMan(pacman), m_Direction(dir), m_Speed(speed) {};
		virtual void Execute() override;

	private:
		std::shared_ptr<PacManComponent> m_PacMan;
		LW2D::Direction m_Direction;
		float m_Speed;
	};

	class HealthComponent;
	class KillCommand final : public Command
	{
	public:
		KillCommand(std::shared_ptr<HealthComponent> healthComponent)
			: m_pHealthComponent(healthComponent) {};
		virtual void Execute() override;

	private:
		std::shared_ptr<HealthComponent> m_pHealthComponent;
	};

	class ScoreComponent;
	class AddScoreCommand final : public Command
	{
	public:
		AddScoreCommand(std::shared_ptr<ScoreComponent> scoreComponent)
			: m_pScoreComponent(scoreComponent) {};
		virtual void Execute() override;

	private:
		std::shared_ptr<ScoreComponent> m_pScoreComponent;
	};
}