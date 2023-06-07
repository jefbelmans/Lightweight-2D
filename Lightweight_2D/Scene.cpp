#include "Scene.h"
#include "GameObject.h"
#include "EngineComponents/TextComponent.h"

using namespace LW2D;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name, const std::function<void()>& onGUI) : m_name(name), m_GUI(onGUI) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void LW2D::Scene::Initialize()
{
	for (auto& object : m_objects)
	{
		object->Initialize();
	}
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void LW2D::Scene::OnGUI() const
{
	m_GUI();
}

std::weak_ptr<GameObject> LW2D::Scene::FindObjectByName(const std::string& name) const
{
	auto it = std::find_if(begin(m_objects), end(m_objects), [name](const std::shared_ptr<GameObject>& go) {
		return go->GetName() == name;
		});

	if (it != end(m_objects))
		return *it;
	else
		return std::weak_ptr<GameObject>();
}
