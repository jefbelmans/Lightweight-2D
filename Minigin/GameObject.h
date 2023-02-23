#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Transform.h"
#include "Component.h"
#include <iostream>

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

		void Update();
		void Render() const;

		template <typename T> std::shared_ptr<T> AddComponent();
		template <typename T> std::shared_ptr<T> GetComponent() const;
		template <typename T> bool RemoveComponent();
		template <typename T> bool HasComponent() const;
		

		std::string GetName() const { return m_Name; }
		void SetName(std::string& name) { m_Name = name; }

	private:
		std::string m_Name;
		Transform m_Transform{};

		std::vector<std::shared_ptr<Component>> m_pComponents;
	};

	template<typename T>
	inline std::shared_ptr<T> GameObject::AddComponent()
	{
		if (std::is_base_of<Component, T>())
		{
			m_pComponents.emplace_back(std::make_shared<T>(shared_from_this()));
			std::cout << "Successfuly added component!" << std::endl;

			return std::dynamic_pointer_cast<T, Component>(m_pComponents.back());
		}
		
		std::cout << "Failed to add component!" << std::endl;
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
			m_pComponents.erase(it);
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
