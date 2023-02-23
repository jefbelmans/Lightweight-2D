#include "RenderComponent.h"
#include "Renderer.h"

LW2D::RenderComponent::RenderComponent(std::weak_ptr<GameObject> go)
	: Component(go, false, false)
{
}

void LW2D::RenderComponent::Update()
{
	if (!m_DoUpdate) return;
}

void LW2D::RenderComponent::Render() const
{
	if (!m_DoRender) return;
}

void LW2D::RenderComponent::RenderTexture(std::shared_ptr<Texture2D> texture, float posX, float posY) const
{
	Renderer::GetInstance().RenderTexture(*texture.get(), posX, posY);
}
