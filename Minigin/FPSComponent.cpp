#include "FPSComponent.h"
#include "SceneManager.h"
#include "GameTime.h"
#include "TextComponent.h"
#include "GameObject.h"

LW2D::FPSComponent::FPSComponent(std::weak_ptr<GameObject> go)
	: Component(go, true, false)
{
	m_pGameTime = SceneManager::GetInstance().GetGameTime();
	m_pTextComponent = go.lock()->GetComponent<TextComponent>();
}

void LW2D::FPSComponent::Update()
{
	// Divide 1000 ms (1 second) by the frametime in ms
	const int fps = static_cast<int>(1000.f / m_pGameTime.lock()->GetDeltaTime());
	m_pTextComponent.lock()->SetText("FPS: " + std::to_string(fps));
}

void LW2D::FPSComponent::Render() const
{
}
