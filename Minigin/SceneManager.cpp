#include "SceneManager.h"
#include "Scene.h"
#include "GameTime.h"

void LW2D::SceneManager::Update()
{
	// Updates the time and delta time
	m_pGameTime->Update();
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

LW2D::Scene& LW2D::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	m_pGameTime = std::make_shared<GameTime>();
	return *scene;
}
