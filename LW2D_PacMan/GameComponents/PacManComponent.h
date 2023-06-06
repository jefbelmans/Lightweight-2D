#pragma once
#include <map>
#include <glm/vec2.hpp>

#include "EngineComponents/Component.h"
#include "../Game Files/Utils.h"

namespace LW2D
{
	class MapComponent;
	class CharacterComponent;
	class PacManComponent : public Component
	{
	public:
		PacManComponent(std::weak_ptr<GameObject> go);
		~PacManComponent() = default;

		void Update() override;

	private:
		// Cached refs
		std::weak_ptr<MapComponent> m_pMap;

		void OnRespawn();
	};
}
