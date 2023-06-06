#pragma once
#include "State.h"

namespace LW2D
{
	class MapComponent;
	class GameObject;
	class FleeState final : public State
	{
	public:
		FleeState();
		~FleeState() override = default;

		State* Update(std::shared_ptr<GameObject> pGo) override;
		void OnEnter(std::shared_ptr<GameObject> pGo) override;
		void OnExit(std::shared_ptr<GameObject> pGo) override;

	private:
		// Cached refs
		std::weak_ptr<MapComponent> m_pMap;
		std::weak_ptr<GameObject> m_pPlayer1;
		std::weak_ptr<GameObject> m_pPlayer2;

		const float m_FleeTime{ 4.f };
		float m_FleeTimer{};

		void HandleMovement(std::shared_ptr<GameObject> pGo);
		void HandlePlayerCollision(std::shared_ptr<GameObject> pGo);
	};
}