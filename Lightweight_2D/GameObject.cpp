#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"

LW2D::GameObject::GameObject(std::string name)
	: m_Name(name)
{
}

void LW2D::GameObject::Initialize()
{
	for (int i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i]->Initialize();
	}
}

void LW2D::GameObject::Update()
{
	for (auto& component : m_pComponents)
	{
		if(!component->IsMarkedForDeletion())
			component->Update();
	}

	std::erase_if(m_pComponents, [](std::shared_ptr<Component> c)
		{
			return c->IsMarkedForDeletion();
		});
		
}

void LW2D::GameObject::Render() const
{
	for (auto& component : m_pComponents)
	{
			component->Render();
	}
}


void LW2D::GameObject::SetParent(std::shared_ptr<GameObject> pParent, bool keepWorldPosition)
{
	if (pParent == nullptr)
	{
		m_Transform.SetLocalPosition(m_Transform.GetWorldPosition());
	}
	else if(keepWorldPosition && m_pParent.lock())
	{
		m_Transform.SetLocalPosition(m_Transform.GetLocalPosition() - m_pParent.lock()->GetTransform().GetWorldPosition());
	}

	// Remove us as child from current parent
	if (m_pParent.lock())
	{
		m_pParent.lock()->RemoveChild(shared_from_this());
	}

	// Set new parent
	m_pParent = pParent;

	// Add us as child of new parent
	if (m_pParent.lock())
	{
		m_pParent.lock()->AddChild(shared_from_this());
	}
}

bool LW2D::GameObject::AddChild(std::shared_ptr<GameObject> pChild)
{
	m_pChildren.push_back(pChild);
	return true;
}

bool LW2D::GameObject::RemoveChild(std::shared_ptr<GameObject> pChild)
{
	auto it = std::find_if(begin(m_pChildren), end(m_pChildren), [&](std::weak_ptr<GameObject> child)
		{
			return child.lock() == pChild;
		});

	// Child is not parented to this GameObject
	if (it == end(m_pChildren)) return false;

	// Child is parented to this GameObject, remove it
	m_pChildren.erase(it);
	return true;
}
