#pragma once
#include <glm/glm.hpp>
#include <memory>

namespace LW2D
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

	class GameObject;
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(std::shared_ptr<GameObject> go, float speed)
			: m_GameObject(go), m_Speed(speed) {};
		virtual void Execute() override;

	private:
		std::shared_ptr<GameObject> m_GameObject;
		glm::vec3 m_Direction;
		float m_Speed;
	};
}