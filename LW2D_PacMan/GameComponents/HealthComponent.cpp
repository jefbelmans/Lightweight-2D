#include "HealthComponent.h"
#include "Event.h"

LW2D::HealthComponent::HealthComponent(std::weak_ptr<GameObject> go)
	: Component(go)
{
	m_pOnKill = std::make_unique<Event<int>>();
	m_pOnDeath = std::make_unique<Event<>>();
}

void LW2D::HealthComponent::Kill()
{
	--m_Lives;
	if (m_Lives <= 0)
	{
		m_Lives = 0;
		m_pOnDeath->Invoke();
	}

	m_pOnKill->Invoke(m_Lives);
}
