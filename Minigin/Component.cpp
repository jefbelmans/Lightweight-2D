#include "Component.h"

LW2D::Component::Component(std::weak_ptr<GameObject> go)
	: m_GameObject(go)
	, m_MarkedForDeletion(false)
{
}
