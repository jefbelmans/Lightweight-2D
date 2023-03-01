#pragma once

#include <string>
#include "Component.h"
#include "Texture2D.h"

namespace LW2D
{
	class RenderComponent final : public Component
	{
	public:
		RenderComponent(std::weak_ptr<GameObject> go);
		~RenderComponent() = default;

		void Update() override;
		void Render() const override;

		void SetTexture(const std::shared_ptr<Texture2D> pTexture) { m_pTexture = pTexture; }
		void SetTexture(const std::string& filename);

	private:
		std::shared_ptr<Texture2D> m_pTexture;
	};
}