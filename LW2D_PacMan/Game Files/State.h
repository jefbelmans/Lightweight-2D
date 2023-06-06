#pragma once
#include <memory>

namespace LW2D
{
	class GameObject;
	class State
	{
	public:
		virtual ~State() = default;

		virtual State* Update(std::shared_ptr<GameObject> pGo) { return nullptr; };
		virtual void OnEnter(std::shared_ptr<GameObject> pGo) {};
		virtual void OnExit(std::shared_ptr<GameObject> pGo) {};
	};
}