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
#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"

void load()
{
	auto& scene = LW2D::SceneManager::GetInstance().CreateScene("Demo");


	// BACKGROUND OBJECT
	auto go = std::make_shared<LW2D::GameObject>("Background");
	go->AddComponent<LW2D::RenderComponent>()->SetTexture("background.tga");
	scene.Add(go);

	// LOGO OBJECT
	go = std::make_shared<LW2D::GameObject>("Logo");
	go->AddComponent<LW2D::RenderComponent>()->SetTexture("logo.tga");
	go->AddComponent<LW2D::TextComponent>();
	go->RemoveComponent<LW2D::TextComponent>();
	go->GetTrasform().SetPosition(216.f, 180.f, 0.f);
	scene.Add(go);

	// FPS OBJECT
	go = std::make_shared<LW2D::GameObject>("FPS Counter");
	go->GetTrasform().SetPosition(10.f, 10.f, 0.f);

	go->AddComponent<LW2D::RenderComponent>();
	auto textComponent = go->AddComponent<LW2D::TextComponent>();

	auto font = LW2D::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	textComponent->SetFont(font);
	textComponent->SetColor(SDL_Color{ 255, 255, 0});

	go->AddComponent<LW2D::FPSComponent>();

	scene.Add(go);

	// TITLE OBJECT
	go = std::make_shared<LW2D::GameObject>("Title Text");
	go->GetTrasform().SetPosition(100.f, 20.f, 0.f);

	go->AddComponent<LW2D::RenderComponent>();
	textComponent = go->AddComponent<LW2D::TextComponent>();

	font = LW2D::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
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