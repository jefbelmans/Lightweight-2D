#pragma once

#include <string>
#include "../Texture2D.h"
#include "Component.h"

namespace LW2D
{
	class RenderComponent final : public Component
	{
	public:
		RenderComponent(std::weak_ptr<GameObject> go);
		~RenderComponent() = default;

		void Render() const override;

		void SetTexture(const std::shared_ptr<Texture2D> pTexture) { m_pTexture = pTexture; }
		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_pTexture;
	};
}