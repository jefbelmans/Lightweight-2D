#pragma once
#include "SceneManager.h"

namespace LW2D
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name, const std::function<void()>& onGUI);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Initialize();
		void Update();
		void Render() const;
		void OnGUI() const;

		std::weak_ptr<GameObject> FindObjectByName(const std::string& name) const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name, const std::function<void()>& onGUI);

		std::string m_name;
		std::vector <std::shared_ptr<GameObject>> m_objects{};
		const std::function<void()> m_GUI;

		static unsigned int m_idCounter; 
	};

}
