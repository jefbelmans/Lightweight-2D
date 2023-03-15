#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "OrbitComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"

void load()
{
	auto& scene = LW2D::SceneManager::GetInstance().CreateScene("Demo");

	// TITLE OBJECT
	auto go = std::make_shared<LW2D::GameObject>("Title Text");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(100.f, 20.f, 0.f);

	go->AddComponent<LW2D::RenderComponent>();
	auto textComponent = go->AddComponent<LW2D::TextComponent>();

	auto font = LW2D::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	textComponent->SetText("Programing 4 Assignment");
	textComponent->SetFont(font);
	textComponent->SetColor(SDL_Color{ 255, 255, 255 });

	scene.Add(go);
}

int main(int, char*[]) {
	LW2D::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}