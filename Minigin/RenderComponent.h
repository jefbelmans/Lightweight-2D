#pragma once
#include "Component.h"
#include "Texture2D.h"
#include <vector>

namespace LW2D
{
	class RenderComponent final : public Component
	{
	public:
		RenderComponent(std::weak_ptr<GameObject> go);
		~RenderComponent() = default;

		void Update() override;
		void Render() const override;

		void RenderTexture(std::shared_ptr<Texture2D> texture, float posX, float posY) const;

	};
}


