#include "OrbitComponent.h"
#include "SceneManager.h"
#include "GameTime.h"
#include "GameObject.h"

LW2D::OrbitComponent::OrbitComponent(std::weak_ptr<GameObject> go)
	: Component(go)
	, m_OrbitPos(go.lock()->GetTransform().GetWorldPosition())
{
	m_pGameTime = SceneManager::GetInstance().GetGameTime();
}

void LW2D::OrbitComponent::Update()
{
	m_CurrAngle += m_Speed * (m_pGameTime.lock()->GetDeltaTime() / 1000.f);

	if (m_pGameObject.lock()->HasParent())
	{
		GetTransform().SetLocalPosition(std::cosf(m_CurrAngle) * m_Distance,
			std::sinf(m_CurrAngle) * m_Distance, 0.f);
	}
	else
	{
		GetTransform().SetLocalPosition(std::cosf(m_CurrAngle) * m_Distance + m_OrbitPos.x,
			std::sinf(m_CurrAngle) * m_Distance + m_OrbitPos.y, 0.f);
	}

}
