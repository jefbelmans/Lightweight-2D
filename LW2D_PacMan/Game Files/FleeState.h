#pragma once
#include "State.h"

namespace LW2D
{
	class MapComponent;
	class GameObject;
	class FleeState final : public State
	{
	public:
		FleeState() = default;
		~FleeState() override = default;

		State* Update(std::shared_ptr<Blackboard> blackboard) override;
		void OnEnter(std::shared_ptr<Blackboard> blackboard) override;
		void OnExit(std::shared_ptr<Blackboard> blackboard) override;

	private:
		const float m_FleeTime{ 4.f };
		float m_FleeTimer{};

		void HandleMovement(std::shared_ptr<Blackboard> blackboard);
		void HandlePlayerCollision(std::shared_ptr<Blackboard> blackboard);
	};
}