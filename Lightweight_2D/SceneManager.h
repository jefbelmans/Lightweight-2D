#pragma once
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "Singleton.h"

namespace LW2D
{
	class Scene;
	class GameTime;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name, const std::function<void()>& onGUI);
		std::shared_ptr<GameTime> GetGameTime() const { return m_pGameTime; }

		void Initialize();
		void Update();
		void Render();
		void OnGUI();

		void LoadSceneByIndex(uint8_t index);
		void LoadSceneByName(const std::string& name);
		void LoadNextScene();

		// Getters
		std::shared_ptr<Scene> GetActiveScene() const { return m_scenes[m_ActiveSceneIndex]; }

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		uint8_t m_ActiveSceneIndex{0};
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<GameTime> m_pGameTime;
	};
}
