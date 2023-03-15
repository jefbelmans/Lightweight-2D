#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

LW2D::Component::Component(std::weak_ptr<GameObject> go)
	: m_pGameObject(go)
	, m_MarkedForDeletion(false)
{
}

LW2D::Transform& LW2D::Component::GetTransform()
{
	return m_pGameObject.lock()->GetTransform();
}
