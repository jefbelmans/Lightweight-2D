#pragma once
#include <memory>

namespace LW2D
{
	class GameObject;
	class Component
	{
	public:
		Component();
		virtual void Update() = 0;
		virtual void Render() const = 0;

	protected:
		bool m_DoUpdate;
		bool m_DoRender;

		std::shared_ptr<GameObject> m_GameObject;
	};
}