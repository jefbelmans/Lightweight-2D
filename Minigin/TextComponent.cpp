#include "TextComponent.h"

#include <stdexcept>

#include "RenderComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "Font.h"
#include "Renderer.h"
#include "Texture2D.h"

LW2D::TextComponent::TextComponent(std::weak_ptr<GameObject> go)
	: Component(go, true, true)
	, m_Text("Text")
	, m_NeedsUpdate(false)
{
	m_pRenderComponent = go.lock()->GetComponent<RenderComponent>();
}

void LW2D::TextComponent::Update()
{
	if (!m_DoUpdate) return;

	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pTexture = std::make_shared<Texture2D>(texture);
		m_pRenderComponent.lock()->SetTexture(m_pTexture);
		m_NeedsUpdate = false;
	}
}

void LW2D::TextComponent::Render() const
{
	if (!m_DoRender || !m_pTexture.get()) return;
}
