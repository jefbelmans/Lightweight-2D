#pragma once
#include <map>
#include <glm/vec2.hpp>

#include "EngineComponents/Component.h"
#include "../Game Files/Utils.h"

namespace LW2D
{
	class MapComponent;
	class CharacterComponent;
	class PacManComponent final : public Component
	{
	public:
		PacManComponent(std::weak_ptr<GameObject> go);
		~PacManComponent() = default;

		void Update() override;

		void Reset() { m_IsDead = false; }

	private:
		// Cached refs
		std::weak_ptr<MapComponent> m_pMap;
		std::weak_ptr<CharacterComponent> m_pCharacter;

		bool m_IsDead{ false };

		void OnKill();
		void OnDeath();
	};
}
