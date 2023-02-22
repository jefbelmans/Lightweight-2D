#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Transform.h"
#include "Component.h"
#include <iostream>

namespace LW2D
{
	class GameObject final
	{
	public:
		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update();
		void Render() const;

		template <typename T> std::shared_ptr<T> AddComponent();
		template <typename T> std::shared_ptr<T> GetComponent() const;
		template <typename T> void RemoveComponent();


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
			auto component = std::make_shared<T>();
			m_pComponents.push_back(component);
			std::cout << "Successfuly added component!" << std::endl;
			return component;
		}
		
		std::cout << "Failed to add component!" << std::endl;
		return std::shared_ptr<T>();
	}

	template<typename T>
	inline std::shared_ptr<T> GameObject::GetComponent() const
	{
		return *std::find(m_pComponents.begin(), m_pComponents.end(), [](auto component)
			{
				return std::is_base_of<component, T>();
			});
	}

	template<typename T>
	inline void GameObject::RemoveComponent()
	{

	}
}
