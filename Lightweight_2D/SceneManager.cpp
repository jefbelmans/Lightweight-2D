#include "SceneManager.h"
#include "Scene.h"
#include "GameTime.h"

void LW2D::SceneManager::Initialize()
{
	for (auto& scene : m_scenes)
	{
		scene->Initialize();
	}
}

void LW2D::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void LW2D::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void LW2D::SceneManager::OnGUI()
{
	for (const auto& scene : m_scenes)
	{
		scene->OnGUI();
	}
}

LW2D::Scene& LW2D::SceneManager::CreateScene(const std::string& name, const std::function<void()>& onGUI)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name, onGUI));
	m_scenes.push_back(scene);
	m_pGameTime = std::make_shared<GameTime>();
	return *scene;
}
