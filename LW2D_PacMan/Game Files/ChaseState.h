#pragma once
#include <memory>

#include "State.h"

namespace LW2D
{
	class GameObject;
	class MapComponent;
	class ChaseState final : public State
	{
	public:
		ChaseState();
		~ChaseState() override = default;

		State* Update(std::shared_ptr<GameObject> pGo) override;
		void OnEnter(std::shared_ptr<GameObject> pGo) override {};
		void OnExit(std::shared_ptr<GameObject> pGo) override {};

	private:
		// Cached refs
		std::weak_ptr<GameObject> m_pPlayer1;
		std::weak_ptr<GameObject> m_pPlayer2;

		std::weak_ptr<MapComponent> m_pMap;

		const float m_ChangeToWanderDistance{ 64.f };

		void HanlePlayerCollision(std::shared_ptr<GameObject> pGo);
		void HandleMovement(std::shared_ptr<GameObject> pGo);
	};
}