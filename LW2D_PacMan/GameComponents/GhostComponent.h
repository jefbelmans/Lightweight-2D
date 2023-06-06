#pragma once
#include <memory>
#include <vector>

#include "EngineComponents/Component.h"

namespace LW2D
{
	class MapComponent;
	class CharacterComponent;
	class State;
	class GhostComponent final : public Component
	{
	public:
		GhostComponent(std::weak_ptr<GameObject> go);
		~GhostComponent() = default;

		void Update() override;

		// Getters
		auto GetClosestPlayer() const { return m_pClosestPlayer; }

	private:
		// Cached refs
		std::weak_ptr<MapComponent> m_pMap;
		std::weak_ptr<CharacterComponent> m_pCharacter;

		std::weak_ptr<GameObject> m_pPlayer1;
		std::weak_ptr<GameObject> m_pPlayer2;
		std::weak_ptr<GameObject> m_pClosestPlayer;

		// States
		std::shared_ptr<State> m_pCurrentState;

		void CalculateClosestPlayer();
	};
}