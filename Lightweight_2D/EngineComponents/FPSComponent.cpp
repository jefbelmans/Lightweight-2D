#include "FPSComponent.h"

#include "../ResourceManager.h"
#include "../SceneManager.h"
#include "../GameTime.h"
#include "../GameObject.h"
#include "TextComponent.h"


LW2D::FPSComponent::FPSComponent(std::weak_ptr<GameObject> go)
	: Component(go)
	, m_SteadyDisplay(true)
	, m_TimeSinceLastDisplay(1000.f)
	, m_FPS(0)
{}

void LW2D::FPSComponent::Initialize()
{
	m_pGameTime = SceneManager::GetInstance().GetGameTime();
	if(m_pGameObject.lock()->HasComponent<TextComponent>())
		m_pTextComponent = m_pGameObject.lock()->GetComponent<TextComponent>();
	else
	{
		auto font = LW2D::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
		m_pTextComponent = m_pGameObject.lock()->AddComponent<TextComponent>(font);
	}
		
}

void LW2D::FPSComponent::Update()
{
	m_TimeSinceLastDisplay += m_pGameTime.lock()->GetDeltaTime();
	if (m_SteadyDisplay && m_TimeSinceLastDisplay >= 1000.f)
	{
		m_FPS = static_cast<int>(1000.f / m_pGameTime.lock()->GetDeltaTime());
		m_TimeSinceLastDisplay = 0.f;
	}
	else if(!m_SteadyDisplay)
		m_FPS = static_cast<int>(1000.f / m_pGameTime.lock()->GetDeltaTime());

	m_pTextComponent.lock()->SetText("FPS: " + std::to_string(m_FPS));
}