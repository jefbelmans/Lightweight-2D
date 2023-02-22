#pragma once
#include "Component.h"

namespace LW2D
{
	class TextComponent final : public Component
	{
	public:
		~TextComponent() = default;
		void Update() override;
		void Render() const override;
	};
}