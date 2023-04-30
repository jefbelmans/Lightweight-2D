#include "RenderComponent.h"
#include "../Renderer.h"
#include "../ResourceManager.h"
#include "../GameObject.h"

LW2D::RenderComponent::RenderComponent(std::weak_ptr<GameObject> go)
	: Component(go)
{
}

void LW2D::RenderComponent::Render() const
{
	const auto& pos = m_pGameObject.lock()->GetTransform().GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}

void LW2D::RenderComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}
