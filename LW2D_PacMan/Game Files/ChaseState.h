#pragma once
#include <memory>

#include "State.h"

namespace LW2D
{
	class GameObject;
	class MapComponent;
	class ChaseState final : public State
	{
	public:
		ChaseState() = default;
		~ChaseState() override = default;

		State* Update(std::shared_ptr<Blackboard> blackboard) override;
		void OnEnter(std::shared_ptr<Blackboard> blackboard) override {};
		void OnExit(std::shared_ptr<Blackboard> blackboard) override {};

	private:
		const float m_ChangeToWanderDistance{ 64.f };

		void HanlePlayerCollision(std::shared_ptr<Blackboard> blackboard);
		void HandleMovement(std::shared_ptr<Blackboard> blackboard);
	};
}