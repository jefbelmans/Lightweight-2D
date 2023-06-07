#pragma once
#include <memory>
#include <vector>

#include "EngineComponents/Component.h"

namespace LW2D
{
	class MapComponent;
	class CharacterComponent;
	class State;
	class Blackboard;
	class GhostComponent final : public Component, public std::enable_shared_from_this<GhostComponent>
	{
	public:
		GhostComponent(std::weak_ptr<GameObject> go);
		~GhostComponent() = default;

		void Update() override;

	private:
		// Cached refs
		std::shared_ptr<Blackboard> m_pBlackboard;

		// States
		std::shared_ptr<State> m_pCurrentState;

		void CalculateClosestPlayer();
	};
}