#pragma once
#include "Command.h"
#include "Utils.h"

namespace LW2D
{
	class CharacterComponent;
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(std::shared_ptr<CharacterComponent> character, const LW2D::Direction& dir)
			: m_pCharacter(character), m_MovementDirection(dir){};
		virtual void Execute() override;

	private:
		std::shared_ptr<CharacterComponent> m_pCharacter;
		LW2D::Direction m_MovementDirection;
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

	class LoadNextSceneCommand final : public Command
	{
	public:
		LoadNextSceneCommand() = default;
		virtual void Execute() override;
	};
}