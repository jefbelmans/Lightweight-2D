#include "TextComponent.h"

#include <stdexcept>

#include "RenderComponent.h"
#include "../Transform.h"
#include "../GameObject.h"
#include "../Font.h"
#include "../Renderer.h"
#include "../Texture2D.h"

LW2D::TextComponent::TextComponent(std::weak_ptr<GameObject> go)
	: Component(go)
	, m_Text("Text")
	, m_NeedsUpdate(true)
{}

LW2D::TextComponent::TextComponent(std::weak_ptr<GameObject> go, std::shared_ptr<Font> pFont, const std::string& text, const SDL_Color& color)
	: Component(go)
	, m_pFont(pFont)
	, m_Text(text)
	, m_Color(color)
	, m_NeedsUpdate(true)
{}

void LW2D::TextComponent::Initialize()
{
	if (m_pGameObject.lock()->HasComponent<RenderComponent>())
		m_pRenderComponent = m_pGameObject.lock()->GetComponent<RenderComponent>(); // Get RenderComponent and cache it if present on object
	else
		m_pRenderComponent = m_pGameObject.lock()->AddComponent<RenderComponent>(); // Add RenderComponent and cache it if not present on object
}

void LW2D::TextComponent::Update()
{
	UpdateTexture();
}

void LW2D::TextComponent::UpdateTexture()
{
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
