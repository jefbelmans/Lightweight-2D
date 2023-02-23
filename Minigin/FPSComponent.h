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

		void Update() override;
		void Render() const override;

	private:
		std::weak_ptr<GameTime> m_pGameTime;
		std::weak_ptr<TextComponent> m_pTextComponent;
	};
}