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

		void Update();
		void Render();
		void OnGUI();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<GameTime> m_pGameTime;
	};
}
