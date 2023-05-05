#include "Transform.h"
#include "GameObject.h"

const glm::vec3& LW2D::Transform::GetWorldPosition()
{
	if (m_IsWorldPositionDirty)
		UpdateWorldPosition();

	return m_WorldPosition;
}

void LW2D::Transform::SetPositionDirty()
{
	m_IsWorldPositionDirty = true;

	for (std::weak_ptr<GameObject> child : m_pGameObject.lock()->GetChildren())
	{
		if (child.expired()) continue;

		child.lock()->GetTransform().SetPositionDirty();
	}
}

void LW2D::Transform::SetLocalPosition(const float x, const float y, const float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;

	SetPositionDirty();
}

void LW2D::Transform::Translate(float x, float y)
{
	m_LocalPosition.x += x;
	m_LocalPosition.y += y;
	SetPositionDirty();
}

void LW2D::Transform::UpdateWorldPosition()
{
	if (m_IsWorldPositionDirty)
	{
		// The GO this transform is attached to has no parent
		if (!m_pGameObject.lock()->GetParent())
			m_WorldPosition = m_LocalPosition;
		else
			m_WorldPosition = m_pGameObject.lock()->GetParent()->GetTransform().GetWorldPosition() + m_LocalPosition;
	}

	m_IsWorldPositionDirty = false;
}
