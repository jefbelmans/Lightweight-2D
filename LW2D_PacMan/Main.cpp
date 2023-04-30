#include <SDL.h>
#include <imgui.h>

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Lightweight_2D.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Scene.h"
#include "Commands.h"
#include "Input.h"
#include "GenericController.h"

// COMPONENTS
#include "EngineComponents/RenderComponent.h"
#include "EngineComponents/TextComponent.h"
#include "EngineComponents/FPSComponent.h"
#include "GameComponents/HealthComponent.h"
#include "GameComponents/ScoreComponent.h"


void OnGUI()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

	if(ImGui::Begin("Game Scene", nullptr, window_flags))
	{
		ImGui::Dummy(ImVec2{ 100.f,50.f });
		ImGui::End();
	}
}

void load()
{
	auto& scene = LW2D::SceneManager::GetInstance().CreateScene("Demo", OnGUI);

#pragma region Scene
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
#pragma endregion

#pragma region Player1
	// PLAYER1
	auto p1 = std::make_shared<LW2D::GameObject>("Player 1");
	p1->GetTransform().SetParent(p1);
	p1->GetTransform().SetLocalPosition(180.f, 200.f, 0.f);

	p1->AddComponent<LW2D::RenderComponent>()->SetTexture("PacMan.png");

	auto healthComponent = p1->AddComponent<LW2D::HealthComponent>();
	healthComponent->SetLives(3);

	auto scoreComponent = p1->AddComponent<LW2D::ScoreComponent>();

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

	// BIND ONKILL EVENT
	auto UpdateLivesDisplayP1 = [textComponent](int lives)
	{
		textComponent->SetText("Lives: " + std::to_string(lives));
	};
	healthComponent->GetOnDeathEvent()->AddListener(UpdateLivesDisplayP1);

	// SCORE DISPLAY P1
	go = std::make_shared<LW2D::GameObject>("Score Display P1");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(10.f, 220.f, 0.f);

	go->AddComponent<LW2D::RenderComponent>();
	textComponent = go->AddComponent<LW2D::TextComponent>();
	textComponent->SetFont(font);
	textComponent->SetColor(SDL_Color{ 64, 255, 64 });
	textComponent->SetText("Score: 0");
	scene.Add(go);

	// BIND ONDSCORE EVENT
	auto UpdateScoreDisplayP1 = [textComponent](int score)
	{
		textComponent->SetText("Score: " + std::to_string(score));
	};
	scoreComponent->GetOnScoreChangedEvent()->AddListener(UpdateScoreDisplayP1);

	const float moveSpeed{ 0.25f };
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

	// KILL P1
	auto killCommand = std::make_shared<LW2D::KillCommand>(p1->GetComponent<LW2D::HealthComponent>());
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_K, SDL_KEYDOWN), killCommand);

	// ADD SCORE P1
	auto addScoreCommand = std::make_shared<LW2D::AddScoreCommand>(p1->GetComponent<LW2D::ScoreComponent>());
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_L, SDL_KEYDOWN), addScoreCommand);

#pragma endregion

#pragma region Player2
	// PLAYER2
	auto p2 = std::make_shared<LW2D::GameObject>("Player 2");
	p2->GetTransform().SetParent(p2);
	p2->GetTransform().SetLocalPosition(200.f, 200.f, 0.f);

	p2->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");

	healthComponent = p2->AddComponent<LW2D::HealthComponent>();
	healthComponent->SetLives(3);

	scoreComponent = p2->AddComponent<LW2D::ScoreComponent>();

	scene.Add(p2);

	// HEALTH DISPLAY P2
	go = std::make_shared<LW2D::GameObject>("Health Display P2");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(10.f, 250.f, 0.f);

	go->AddComponent<LW2D::RenderComponent>();
	textComponent = go->AddComponent<LW2D::TextComponent>();
	textComponent->SetFont(font);
	textComponent->SetColor(SDL_Color{ 255, 64, 64 });
	textComponent->SetText("Lives: 3");
	scene.Add(go);

	// BIND ONKILL EVENT
	auto UpdateLivesDisplayP2 = [textComponent](int lives)
	{
		textComponent->SetText("Lives: " + std::to_string(lives));
	};
	healthComponent->GetOnDeathEvent()->AddListener(UpdateLivesDisplayP2);

	// SCORE DISPLAY P2
	go = std::make_shared<LW2D::GameObject>("Score Display P2");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(10.f, 270.f, 0.f);

	go->AddComponent<LW2D::RenderComponent>();
	textComponent = go->AddComponent<LW2D::TextComponent>();
	textComponent->SetFont(font);
	textComponent->SetColor(SDL_Color{ 255, 64, 64 });
	textComponent->SetText("Score: 0");
	scene.Add(go);

	// BIND ONSCORE EVENT
	auto UpdateScoreDisplayP2 = [textComponent](int score)
	{
		textComponent->SetText("Score: " + std::to_string(score));
	};
	scoreComponent->GetOnScoreChangedEvent()->AddListener(UpdateScoreDisplayP2);

	// ADD INPUT DEVICES
	LW2D::Input::GetInstance().AddController(std::make_shared<LW2D::GenericController>(0));

	// CONTROLLER
	moveCommand = std::make_shared<LW2D::MoveCommand>(p2, glm::vec3{ 0.f, -1.f, 0.f }, moveSpeed * 2.f);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadUp, LW2D::KeyState::Pressed), moveCommand);

	moveCommand = std::make_shared<LW2D::MoveCommand>(p2, glm::vec3{ 0.f, 1.f, 0.f }, moveSpeed * 2.f);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadDown, LW2D::KeyState::Pressed), moveCommand);

	moveCommand = std::make_shared<LW2D::MoveCommand>(p2, glm::vec3{ -1.f, 0.f, 0.f }, moveSpeed * 2.f);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadLeft, LW2D::KeyState::Pressed), moveCommand);

	moveCommand = std::make_shared<LW2D::MoveCommand>(p2, glm::vec3{ 1.f, 0.f, 0.f }, moveSpeed * 2.f);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadRight, LW2D::KeyState::Pressed), moveCommand);

	// KILL P2
	killCommand = std::make_shared<LW2D::KillCommand>(p2->GetComponent<LW2D::HealthComponent>());
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::ButtonSouth, LW2D::KeyState::Down), killCommand);

	// ADD SCORE P2
	addScoreCommand = std::make_shared<LW2D::AddScoreCommand>(p2->GetComponent<LW2D::ScoreComponent>());
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::ButtonWest, LW2D::KeyState::Down), addScoreCommand);
#pragma endregion

}

int main(int, char* [])
{
	std::cout << "\n[CONTROLS CONTROLLER]\n[X/Square]: Add points\n[A/Cross]: Subtract lives\n\n";
	std::cout << "[CONTROLS KEYBOARDD]\n[L]: Add points\n[K]: Subtract lives\n\n";

	LW2D::Lightweight_2D engine("../Data/");
	engine.Run(load);

	return 0;
}