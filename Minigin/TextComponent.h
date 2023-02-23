#pragma once
#include <memory>
#include <string>
#include <SDL_ttf.h>
#include "Component.h"

namespace LW2D
{
	class Font;
	class RenderComponent;
	class Texture2D;
	class TextComponent final : public Component
	{
	public:
		TextComponent(std::weak_ptr<GameObject> go);
		~TextComponent() = default;
		void Update() override;
		void Render() const override;

		void SetFont(std::shared_ptr<Font> font) { m_pFont = font; m_NeedsUpdate = true; }
		void SetText(const std::string& text) { m_Text = text; m_NeedsUpdate = true; }
		void SetColor(const SDL_Color& color) { m_Color = color; m_NeedsUpdate = true; }

	private:
		// Component references
		std::weak_ptr<RenderComponent> m_pRenderComponent;

		// Data members
		bool m_NeedsUpdate;
		std::string m_Text;
		SDL_Color m_Color;
		std::shared_ptr<Font> m_pFont;
		std::shared_ptr<Texture2D> m_pTexture;
	};
}