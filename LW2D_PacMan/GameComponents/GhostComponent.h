#pragma once
#include <memory>
#include <vector>

#include "EngineComponents/Component.h"
#include "Event.h"

namespace LW2D
{
	class MapComponent;
	class CharacterComponent;
	class State;
	class Blackboard;
	class GhostComponent final : public Component, public std::enable_shared_from_this<GhostComponent>
	{
	public:
		GhostComponent(std::weak_ptr<GameObject> go, bool isCPU = true);
		~GhostComponent() = default;

		void Update() override;

		// Getters
		Event<int>* GetOnGhostKilled() const { return m_pOnGhostKilled.get(); }

	private:
		// Cached refs
		std::shared_ptr<Blackboard> m_pBlackboard;

		// States
		std::shared_ptr<State> m_pCurrentState;

		// Events
		const int m_GhostKilledScore{ 100 };
		std::unique_ptr<Event<int>> m_pOnGhostKilled;

		void CalculateClosestPlayer();
		void ChangeState(State* newState);
	};
}