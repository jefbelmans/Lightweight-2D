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
	m_scenes[m_ActiveSceneIndex]->Update();
}

void LW2D::SceneManager::Render()
{
	m_scenes[m_ActiveSceneIndex]->Render();
}

void LW2D::SceneManager::OnGUI()
{
	m_scenes[m_ActiveSceneIndex]->OnGUI();
}

LW2D::Scene& LW2D::SceneManager::CreateScene(const std::string& name, const std::function<void()>& onGUI)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name, onGUI));
	m_scenes.push_back(scene);
	m_pGameTime = std::make_shared<GameTime>();
	m_ActiveSceneIndex = static_cast<uint8_t>(m_scenes.size()) - 1;
	return *scene;
}

void LW2D::SceneManager::LoadSceneByIndex(uint8_t index)
{
	m_scenes[m_ActiveSceneIndex]->OnSceneDeactived();
	m_ActiveSceneIndex = index;
	m_scenes[m_ActiveSceneIndex]->OnSceneActived();
}

void LW2D::SceneManager::LoadSceneByName(const std::string& name)
{
	for (uint8_t i = 0; i < m_scenes.size(); ++i)
	{
		if (m_scenes[i]->GetName() == name)
		{
			m_scenes[m_ActiveSceneIndex]->OnSceneDeactived();
			m_ActiveSceneIndex = i;
			m_scenes[m_ActiveSceneIndex]->OnSceneActived();
			return;
		}
	}
}

void LW2D::SceneManager::LoadNextScene()
{
	m_scenes[m_ActiveSceneIndex]->OnSceneDeactived();
	m_ActiveSceneIndex = (m_ActiveSceneIndex + 1) % m_scenes.size();
	m_scenes[m_ActiveSceneIndex]->OnSceneActived();
}