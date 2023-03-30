#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"
#include "Command.h"
#include "Input.h"
#include "GenericController.h"

// COMPONENTS
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "HealthComponent.h"

void SetUI(int t)
{
	std::cout << "Got event: " << t << std::endl;
}

void load()
{
	auto& scene = LW2D::SceneManager::GetInstance().CreateScene("Demo");

	// BACKGROUND OBJECT
	auto go = std::make_shared<LW2D::GameObject>("Background");
	go->GetTransform().SetParent(go);
	go->AddComponent<LW2D::RenderComponent>()->SetTexture("background.tga");
	scene.Add(go);

	// LOGO OBJECT
	go = std::make_shared<LW2D::GameObject>("Logo");
	go->GetTransform().SetParent(go);
	go->AddComponent<LW2D::RenderComponent>()->SetTexture("logo.tga");
	go->GetTransform().SetLocalPosition(216.f, 180.f, 0.f);
	scene.Add(go);

	// TITLE OBJECT
	go = std::make_shared<LW2D::GameObject>("Title Text");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(100.f, 20.f, 0.f);

	go->AddComponent<LW2D::RenderComponent>();
	auto textComponent = go->AddComponent<LW2D::TextComponent>();

	auto font = LW2D::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	textComponent->SetText("Programing 4 Assignment");
	textComponent->SetFont(font);
	textComponent->SetColor(SDL_Color{ 255, 255, 255 });

	scene.Add(go);

	// FPS COMPONENT
	go = std::make_shared<LW2D::GameObject>("FPS Component");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(10.f, 10.f, 0.f);

	go->AddComponent<LW2D::RenderComponent>();
	textComponent = go->AddComponent<LW2D::TextComponent>();

	font = LW2D::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	textComponent->SetFont(font);
	textComponent->SetColor(SDL_Color{ 255, 255, 255 });

	go->AddComponent<LW2D::FPSComponent>();

	scene.Add(go);

	// PLAYER1
	auto p1 = std::make_shared<LW2D::GameObject>("Player 1");
	p1->GetTransform().SetParent(p1);
	p1->GetTransform().SetLocalPosition(100.f, 100.f, 0.f);
	p1->AddComponent<LW2D::RenderComponent>()->SetTexture("PacMan.png");
	auto healthComponent = p1->AddComponent<LW2D::HealthComponent>();
	healthComponent->SetLives(3);
	scene.Add(p1);

	// HEALTH DISPLAY P1
	go = std::make_shared<LW2D::GameObject>("Health Display P1");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(10.f, 200.f, 0.f);

	go->AddComponent<LW2D::RenderComponent>();
	textComponent = go->AddComponent<LW2D::TextComponent>();
	textComponent->SetFont(font);
	textComponent->SetColor(SDL_Color{ 64, 255, 64 });
	textComponent->SetText("Lives: 3");
	scene.Add(go);
	
	// PLAYER2
	auto p2 = std::make_shared<LW2D::GameObject>("Player 2");
	p2->GetTransform().SetParent(p2);
	p2->GetTransform().SetLocalPosition(200.f, 200.f, 0.f);
	p2->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");
	scene.Add(p2);

	const float moveSpeed{ 0.25f };

	// INPUT DEVICES
	LW2D::Input::GetInstance().AddController(std::make_shared<LW2D::GenericController>(0));

	// KEYBOARD
	// MOVE UP
	auto moveCommand = std::make_shared<LW2D::MoveCommand>(p1, glm::vec3{ 0.f, -1.f, 0.f }, moveSpeed);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_W, SDL_KEYMAPCHANGED), moveCommand);

	// MOVE DOWN
	moveCommand = std::make_shared<LW2D::MoveCommand>(p1, glm::vec3{ 0.f, 1.f, 0.f }, moveSpeed);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_S, SDL_KEYMAPCHANGED), moveCommand);

	// MOVE LEFT
	moveCommand = std::make_shared<LW2D::MoveCommand>(p1, glm::vec3{ -1.f, 0.f, 0.f }, moveSpeed);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_A, SDL_KEYMAPCHANGED), moveCommand);

	// MOVE RIGHT
	moveCommand = std::make_shared<LW2D::MoveCommand>(p1, glm::vec3{ 1.f, 0.f, 0.f }, moveSpeed);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_D, SDL_KEYMAPCHANGED), moveCommand);

	// KILL
	auto killCommand = std::make_shared<LW2D::KillCommand>(p1->GetComponent<LW2D::HealthComponent>());
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_K, SDL_KEYDOWN), killCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::ButtonSouth, LW2D::KeyState::Down), killCommand);

	// CONTROLLER
	moveCommand = std::make_shared<LW2D::MoveCommand>(p2, glm::vec3{ 0.f, -1.f, 0.f }, moveSpeed * 2.f);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadUp, LW2D::KeyState::Pressed), moveCommand);

	moveCommand = std::make_shared<LW2D::MoveCommand>(p2, glm::vec3{ 0.f, 1.f, 0.f }, moveSpeed * 2.f);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadDown, LW2D::KeyState::Pressed), moveCommand);

	moveCommand = std::make_shared<LW2D::MoveCommand>(p2, glm::vec3{ -1.f, 0.f, 0.f }, moveSpeed * 2.f);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadLeft, LW2D::KeyState::Pressed), moveCommand);

	moveCommand = std::make_shared<LW2D::MoveCommand>(p2, glm::vec3{ 1.f, 0.f, 0.f }, moveSpeed * 2.f);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadRight, LW2D::KeyState::Pressed), moveCommand);
}

int main(int, char*[]) {
	LW2D::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}