#include "Component.h"

LW2D::Component::Component(std::weak_ptr<GameObject> go)
	: m_GameObject(go)
	, m_DoUpdate(false)
	, m_DoRender(false)
{
}
