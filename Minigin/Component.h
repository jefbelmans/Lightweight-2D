#pragma once
#include <memory>

namespace LW2D
{
	class GameObject;
	class Component
	{
	public:
		Component(std::weak_ptr<GameObject> go);
		virtual void Update() = 0;
		virtual void Render() const = 0;
		
		virtual std::shared_ptr<GameObject> GetGameObject() const { return m_GameObject.lock(); }

	protected:
		bool m_DoUpdate;
		bool m_DoRender;

		std::weak_ptr<GameObject> m_GameObject;
	};
}