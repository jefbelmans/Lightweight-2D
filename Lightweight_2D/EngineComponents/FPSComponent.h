#pragma once
#include "Component.h"

namespace LW2D
{
	class GameTime;
	class TextComponent;
	class FPSComponent final : public Component
	{
	public:
		FPSComponent(std::weak_ptr<GameObject> go);
		~FPSComponent() = default;

		void Initialize() override;
		void Update() override;

		void SetSteadyDisplay(bool steadDisplay) { m_SteadyDisplay = steadDisplay; }
	private:
		// STEADY DISPLAY
		bool m_SteadyDisplay;
		float m_TimeSinceLastDisplay;
		
		int m_FPS;

		std::weak_ptr<GameTime> m_pGameTime;
		std::weak_ptr<TextComponent> m_pTextComponent;
	};
}