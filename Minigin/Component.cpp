#include "Component.h"

LW2D::Component::Component(std::weak_ptr<GameObject> go, bool doUpdate, bool doRender)
	: m_GameObject(go)
	, m_DoUpdate(doUpdate)
	, m_DoRender(doRender)
	, m_MarkedForDeletion(false)
{
}
