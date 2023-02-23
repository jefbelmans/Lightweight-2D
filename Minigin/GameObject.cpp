#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Transform.h"

LW2D::GameObject::GameObject(std::string name)
	: m_Name(name)
{
}

void LW2D::GameObject::Update()
{
	for (auto& component : m_pComponents)
	{
		if(component->GetDoUpdate())
			component->Update();
	}
}

void LW2D::GameObject::Render() const
{
	for (auto& component : m_pComponents)
	{
		if(component->GetDoRender())
			component->Render();
	}
}
