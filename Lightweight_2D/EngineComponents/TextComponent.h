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
		TextComponent(std::weak_ptr<GameObject> go,
			std::shared_ptr<Font> pFont,
			const std::string& text = "Text Component",
			const SDL_Color& color = { 255, 255, 255 });

		~TextComponent() = default;

		void Initialize() override;
		void Update() override;

		void SetFont(std::shared_ptr<Font> pFont) { m_pFont = pFont; m_NeedsUpdate = true; }
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

		void UpdateTexture();
	};
}