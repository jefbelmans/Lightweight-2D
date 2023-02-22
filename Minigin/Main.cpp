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
#include "TextObject.h"
#include "Scene.h"
#include <iostream>

void load()
{
	auto& scene = LW2D::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<LW2D::GameObject>();
	auto textComp = go->AddComponent<LW2D::TextComponent>();
	if (textComp.get() != nullptr)
	{
		// textComp->Update();
		// go->GetComponent<LW2D::TextComponent>()->Update();
	}
	scene.Add(go);

	/*go = std::make_shared<LW2D::GameObject>();
	go->SetTexture("logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = LW2D::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<LW2D::TextObject>("Programming 4 Assignment", font);
	to->SetPosition(80, 20);
	scene.Add(to);*/
}

int main(int, char*[]) {
	LW2D::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}