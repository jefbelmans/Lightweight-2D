#pragma once
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "EngineComponents/Component.h"
#include "Transform.h"

namespace LW2D
{
	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject() = default;
		GameObject(std::string name);
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Initialize();
		void Update();
		void Render() const;

		// COMPONENTS

		template <typename T, typename... Args> std::shared_ptr<T> AddComponent(Args&&... args);
		template <typename T> std::shared_ptr<T> GetComponent() const;
		template <typename T> bool RemoveComponent();
		template <typename T> bool HasComponent() const;

		// NAME
		std::string GetName() const { return m_Name; }
		void SetName(std::string& name) { m_Name = name; }

		// TRANSFORM
		Transform& GetTransform() { return m_Transform; }

		// SCENEGRAPH
		std::shared_ptr<GameObject> GetParent() const { return m_pParent.lock(); }
		void SetParent(std::shared_ptr<GameObject> pParent, bool keepWorldPosition = true);
		bool HasParent() const { return m_pParent.lock() != nullptr; }

		size_t GetChildCount() const { return m_pChildren.size(); }
		std::vector<std::weak_ptr<GameObject>> GetChildren() const { return m_pChildren; }
		std::weak_ptr<GameObject> GetChildAt(size_t index) const { return m_pChildren.at(index); }

		bool AddChild(std::shared_ptr<GameObject> pChild);
		bool RemoveChild(std::shared_ptr<GameObject> pChild);

	private:
		// GAMEOBJECT PROPERTIES
		std::string m_Name;
		Transform m_Transform{};

		// SCENEGRAPH
		std::weak_ptr<GameObject> m_pParent;
		std::vector<std::weak_ptr<GameObject>> m_pChildren;

		// COMPONENTS
		std::vector<std::shared_ptr<Component>> m_pComponents;
	};

	template<typename T, typename... Args>
	std::shared_ptr<T> GameObject::AddComponent(Args&&... args)
	{
		if (std::is_base_of<Component, T>())
		{
			m_pComponents.emplace_back(std::make_shared<T>(shared_from_this(), std::forward<Args>(args)...));
			return std::dynamic_pointer_cast<T>(m_pComponents.back());
		}

		return nullptr;
	}

	template<typename T>
	inline std::shared_ptr<T> GameObject::GetComponent() const
	{
		for (std::shared_ptr<Component> c : m_pComponents)
		{
			auto comp = std::dynamic_pointer_cast<T, Component>(c);
			if (comp != nullptr)
				return comp;
		}
		return nullptr;
	}

	template<typename T>
	inline bool GameObject::RemoveComponent()
	{
		if (m_pComponents.empty()) return false;

		auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(), [&](std::shared_ptr<Component> component)
			{
				return std::dynamic_pointer_cast<T, Component>(component) != nullptr;
			});

		if (it != m_pComponents.end())
		{
			it->get()->MarkForDeletion();
			return true;
		}

		return false;
	}

	template<typename T>
	inline bool GameObject::HasComponent() const
	{
		if (m_pComponents.empty()) return false;

		auto it = std::find_if(m_pComponents.begin(), m_pComponents.end(), [&](std::shared_ptr<Component> component)
			{
				return std::dynamic_pointer_cast<T, Component>(component) != nullptr;
			});

		return it != m_pComponents.end();
	}
}
