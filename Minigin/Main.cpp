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

void load()
{
	auto& scene = LW2D::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<LW2D::GameObject>("FPS Counter");
	auto textComp = go->AddComponent<LW2D::TextComponent>();
	if (textComp != nullptr)
	{
		textComp->Update();
		std::cout << go->GetComponent<LW2D::TextComponent>()->GetGameObject()->GetName() << std::endl;
		if (go->HasComponent<LW2D::TextComponent>())
		{
			std::cout << "TextComponent on GO!\n";
		}
	}
	go->RemoveComponent<LW2D::TextComponent>();
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