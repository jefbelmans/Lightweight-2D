#pragma once
#include <glm/glm.hpp>
#include <memory>

namespace LW2D
{
	class GameObject;
	class Transform final
	{
	public:
		const glm::vec2& GetWorldPosition();
		const glm::vec2& GetLocalPosition() const { return m_LocalPosition; }

		void SetPositionDirty();

		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec2& pos) { m_LocalPosition = pos; SetPositionDirty(); }

		void Translate(float x, float y);

		void SetParent(std::shared_ptr<GameObject> pParent) { m_pGameObject = pParent; }

	private:
		bool m_IsWorldPositionDirty{ false };
		glm::vec2 m_LocalPosition;
		glm::vec2 m_WorldPosition;

		std::weak_ptr<GameObject> m_pGameObject;

		void UpdateWorldPosition();
	};
}
