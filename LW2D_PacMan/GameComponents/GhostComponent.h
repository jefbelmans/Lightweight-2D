#pragma once
#include <vector>

#include "EngineComponents/Component.h"
#include "../Game Files/Utils.h"

namespace LW2D
{
	class MapComponent;
	class CharacterComponent;
	class GhostComponent final : public Component
	{
	public:
		GhostComponent(std::weak_ptr<GameObject> go);
		~GhostComponent() = default;

		void Update() override;

		void ChangeDirection(std::vector<LW2D::Direction> availableDirections);

	private:
		// Cached refs
		std::weak_ptr<MapComponent> m_pMap;
		std::weak_ptr<CharacterComponent> m_pCharacter;
		std::weak_ptr<GameObject> m_pPlayer1;
		std::weak_ptr<GameObject> m_pPlayer2;
	};
}