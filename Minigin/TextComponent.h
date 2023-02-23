#pragma once
#include <memory>
#include "Component.h"

namespace LW2D
{
	class TextComponent final : public Component
	{
	public:
		TextComponent(std::weak_ptr<GameObject> go);
		~TextComponent() = default;
		void Update() override;
		void Render() const override;
	};
}