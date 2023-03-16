#pragma once
#include <SDL.h>
#include "Singleton.h"
#include <vector>
#include "imgui.h"


namespace LW2D
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
		bool m_showDemo{ false };

		// GRAPH DATA
		std::vector<float> m_steps = { 1, 2, 4, 6, 8, 16, 32, 64, 128, 256, 512, 1024 };
		std::vector<float> m_floatAvg = std::vector<float>(11);
		std::vector<float> m_3DAvg = std::vector<float>(11);
		std::vector<float> m_3DAltAvg = std::vector<float>(11);

		int nrSamplesFloat{ 10 };
		int nrSamplesGO{ 10 };

		// GRAPH PROPERTIES
		const ImU32 m_colors[2] = { ImColor(32, 255, 32), ImColor(0, 255, 255) };

	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

