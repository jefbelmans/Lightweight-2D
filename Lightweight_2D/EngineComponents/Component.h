#pragma once
#include <memory>

namespace LW2D
{
	class GameObject;
	class Transform;
	class Component
	{
	public:
		virtual void Update() {};
		virtual void Render() const {};
		
		virtual std::shared_ptr<GameObject> GetGameObject() const { return m_pGameObject.lock(); }

		bool IsMarkedForDeletion() const { return m_MarkedForDeletion; }
		void MarkForDeletion() { m_MarkedForDeletion = true; }

	protected:
		Component(std::weak_ptr<GameObject> go);

		bool m_MarkedForDeletion;
		std::weak_ptr<GameObject> m_pGameObject;

		Transform& GetTransform();
	};
}