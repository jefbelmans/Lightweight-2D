#pragma once
#include "State.h"

namespace LW2D
{
	class WanderState final : public State
	{
	public:
		WanderState() = default;
		~WanderState() override = default;

		State* Update(std::shared_ptr<Blackboard> blackboard) override;
		void OnEnter(std::shared_ptr<Blackboard> blackboard) override {};
		void OnExit(std::shared_ptr<Blackboard> blackboard) override {};

	private:
		const float m_ChangeToChaseDist{ 96.f };
	};
}