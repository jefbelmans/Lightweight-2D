#pragma once
#include <memory>

namespace LW2D
{
	class Blackboard;
	class State
	{
	public:
		virtual ~State() = default;

		virtual State* Update(std::shared_ptr<Blackboard> blackboard) { return nullptr; };
		virtual void OnEnter(std::shared_ptr<Blackboard> blackboard) {};
		virtual void OnExit(std::shared_ptr<Blackboard> blackboard) {};
	};
}