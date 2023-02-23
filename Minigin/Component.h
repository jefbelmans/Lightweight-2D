#pragma once
#include <memory>

namespace LW2D
{
	class GameObject;
	class Component
	{
	public:
		Component(std::weak_ptr<GameObject> go, bool doUpdate = true, bool doRender = false);
		virtual void Update() = 0;
		virtual void Render() const = 0;
		
		virtual std::shared_ptr<GameObject> GetGameObject() const { return m_GameObject.lock(); }

		bool GetDoUpdate() const { return m_DoUpdate; }
		bool GetDoRender() const { return m_DoRender; }

	protected:
		bool m_DoUpdate;
		bool m_DoRender;

		std::weak_ptr<GameObject> m_GameObject;
	};
}