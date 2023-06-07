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
#include "ServiceLocator.h"

// COMPONENTS
#include "EngineComponents/RenderComponent.h"
#include "EngineComponents/TextComponent.h"
#include "EngineComponents/FPSComponent.h"

#include "../GameComponents/HealthComponent.h"
#include "../GameComponents/ScoreComponent.h"
#include "../GameComponents/MapComponent.h"
#include "../GameComponents/PacManComponent.h"
#include "../GameComponents/GhostComponent.h"
#include "../GameComponents/CharacterComponent.h"

using namespace std::placeholders;

void OnGUI()
{

}

void load(SDL_Window* pWindow)
{
#pragma region Audio
	// Initialize sound system in service locator
#if _DEBUG
	LW2D::ServiceLocator::RegisterSoundSystem(new LW2D::Logging_SoundSystem(new LW2D::SDL_SoundSystem()));
#else
	LW2D::ServiceLocator::RegisterSoundSystem(new LW2D::SDL_SoundSystem());
#endif // _DEBUG

	LW2D::ServiceLocator::GetSoundSystem().AddSound("pacman_chomp.wav", (unsigned short)LW2D::Sounds::PacManEat, true);
	LW2D::ServiceLocator::GetSoundSystem().AddSound("pacman_death.wav", (unsigned short)LW2D::Sounds::PacManDeath, false);
	LW2D::ServiceLocator::GetSoundSystem().AddSound("pacman_eatghost.wav", (unsigned short)LW2D::Sounds::PacManEatGhost, false);
#pragma endregion

#pragma region Menu Scene

	auto& sceneMenu = LW2D::SceneManager::GetInstance().CreateScene("Menu", OnGUI);

#pragma region Scene
	// BACKGROUND OBJECT
	auto go = std::make_shared<LW2D::GameObject>("Background");
	go->GetTransform().SetParent(go);
	go->AddComponent<LW2D::RenderComponent>()->SetTexture("background.tga");
	sceneMenu.Add(go);

	// MAP OBJECT
	go = std::make_shared<LW2D::GameObject>("Map");
	go->GetTransform().SetParent(go);

	auto map = go->AddComponent<LW2D::MapComponent>();

	// RESIZE WINDOW TO MAP
	SDL_SetWindowSize(pWindow, map->GetCellSize() * map->GetCols(), map->GetCellSize() * map->GetRows() + 144);

	// FPS COMPONENT
	go = std::make_shared<LW2D::GameObject>("FPS Component");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(18.f, 1.f);
	go->AddComponent<LW2D::FPSComponent>();

	sceneMenu.Add(go);

	sceneMenu.GetOnSceneActived()->AddListener([]() {
		LW2D::ServiceLocator::GetSoundSystem().StopSound((unsigned short)LW2D::Sounds::PacManEat);
		});

	sceneMenu.GetOnSceneDeactived()->AddListener([]() {
		LW2D::ServiceLocator::GetSoundSystem().PlaySound((unsigned short)LW2D::Sounds::PacManEat, 64.f);
		});
#pragma endregion

#pragma endregion

#pragma region Solo Scene
	auto& sceneSolo = LW2D::SceneManager::GetInstance().CreateScene("Solo", OnGUI);

#pragma region Scene
	// BACKGROUND OBJECT
	go = std::make_shared<LW2D::GameObject>("Background");
	go->GetTransform().SetParent(go);
	go->AddComponent<LW2D::RenderComponent>()->SetTexture("background.tga");
	sceneSolo.Add(go);

	// MAP OBJECT
	go = std::make_shared<LW2D::GameObject>("Map");
	go->GetTransform().SetParent(go);

	map = go->AddComponent<LW2D::MapComponent>();
	sceneSolo.Add(go);

	// RESIZE WINDOW TO MAP
	SDL_SetWindowSize(pWindow, map->GetCellSize() * map->GetCols(), map->GetCellSize() * map->GetRows() + 144);

	// FPS COMPONENT
	go = std::make_shared<LW2D::GameObject>("FPS Component");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(18.f, 1.f);

	go->AddComponent<LW2D::FPSComponent>();

	sceneSolo.Add(go);
#pragma endregion

#pragma region Player1
	// PLAYER1
	auto p1= std::make_shared<LW2D::GameObject>("Player 1");
	p1->GetTransform().SetParent(p1);

	p1->AddComponent<LW2D::RenderComponent>()->SetTexture("PacMan.png");

	// Health Component
	auto healthComponent = p1->AddComponent<LW2D::HealthComponent>();
	healthComponent->SetLives(3);

	// Score Component
	auto scoreComponent = p1->AddComponent<LW2D::ScoreComponent>();
	sceneSolo.Add(p1);

	auto pacMan = p1->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 160.f, 240.f });
	p1->AddComponent<LW2D::PacManComponent>();

	// HEALTH DISPLAY P1
	go = std::make_shared<LW2D::GameObject>("Health Display P1");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(10.f, 350.f);

	auto font = LW2D::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	auto textComponent = go->AddComponent<LW2D::TextComponent>(font, "Lives: 3", SDL_Color{64, 255, 64});
	sceneSolo.Add(go);

	// BIND EVENTS
	std::function<void(int)> DecrementHealth = [textComponent](int lives)
	{
		textComponent->SetText("Lives: " + std::to_string(lives));
	};
	healthComponent->GetOnKillEvent()->AddListener(DecrementHealth);

	// SCORE DISPLAY P1
	go = std::make_shared<LW2D::GameObject>("Score Display P1");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(10.f, 370.f);

	textComponent = go->AddComponent<LW2D::TextComponent>(font, "Score: 0", SDL_Color{ 64, 255, 64 });
	sceneSolo.Add(go);

	// BIND EVENTS
	std::function<void(int)> IncrementScoreP1 = [scoreComponent, textComponent](int score)
	{
		scoreComponent->AddScore(score);
		textComponent->SetText("Score: " + std::to_string(scoreComponent->GetScore()));
	};
	map->GetOnPelletCollected()->AddListener(IncrementScoreP1);
	
	// ADD INPUT DEVICES
	LW2D::Input::GetInstance().AddController(std::make_shared<LW2D::GenericController>(0));

	// MOVE UP
	auto moveCommand = std::make_shared<LW2D::MoveCommand>(pacMan, LW2D::Direction::Up);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_W, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadUp, LW2D::KeyState::Down), moveCommand);

	// MOVE DOWN
	moveCommand = std::make_shared<LW2D::MoveCommand>(pacMan, LW2D::Direction::Down);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_S, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadDown, LW2D::KeyState::Down), moveCommand);

	// MOVE LEFT
	moveCommand = std::make_shared<LW2D::MoveCommand>(pacMan, LW2D::Direction::Left);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_A, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadLeft, LW2D::KeyState::Down), moveCommand);

	// MOVE RIGHT
	moveCommand = std::make_shared<LW2D::MoveCommand>(pacMan, LW2D::Direction::Right);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_D, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadRight, LW2D::KeyState::Down), moveCommand);
#pragma endregion

#pragma region Ghosts
	// GHOST1
	auto ghost = std::make_shared<LW2D::GameObject>("Ghost 1");
	ghost->GetTransform().SetParent(ghost);

	ghost->AddComponent<LW2D::HealthComponent>()->SetLives(2);

	auto charComp = ghost->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 145.f, 144.f }, 0.f);
	charComp->SetSpeed(56.f);

	auto ghostComp = ghost->AddComponent<LW2D::GhostComponent>();
	ghostComp->GetOnGhostKilled()->AddListener(IncrementScoreP1);
	
	ghost->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");
	sceneSolo.Add(ghost);

	// GHOST2
	ghost = std::make_shared<LW2D::GameObject>("Ghost 2");
	ghost->GetTransform().SetParent(ghost);

	ghost->AddComponent<LW2D::HealthComponent>()->SetLives(2);

	charComp = ghost->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 160.f, 144.f }, 0.f);
	charComp->SetSpeed(56.f);

	ghostComp = ghost->AddComponent<LW2D::GhostComponent>();
	ghostComp->GetOnGhostKilled()->AddListener(IncrementScoreP1);

	ghost->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");
	sceneSolo.Add(ghost);

	// GHOST3
	ghost = std::make_shared<LW2D::GameObject>("Ghost 3");
	ghost->GetTransform().SetParent(ghost);

	ghost->AddComponent<LW2D::HealthComponent>()->SetLives(2);

	charComp = ghost->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 176.f, 144.f }, 0.f);
	charComp->SetSpeed(56.f);

	ghostComp = ghost->AddComponent<LW2D::GhostComponent>();
	ghostComp->GetOnGhostKilled()->AddListener(IncrementScoreP1);

	ghost->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");
	sceneSolo.Add(ghost);

	// GHOST4
	ghost = std::make_shared<LW2D::GameObject>("Ghost 4");
	ghost->GetTransform().SetParent(ghost);

	ghost->AddComponent<LW2D::HealthComponent>()->SetLives(2);

	charComp = ghost->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 161.f, 128.f }, 0.f);
	charComp->SetSpeed(56.f);

	ghostComp = ghost->AddComponent<LW2D::GhostComponent>();
	ghostComp->GetOnGhostKilled()->AddListener(IncrementScoreP1);

	ghost->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");
	sceneSolo.Add(ghost);
#pragma endregion

#pragma endregion

#pragma region Versus Scene
	auto& sceneVersus = LW2D::SceneManager::GetInstance().CreateScene("Versus", OnGUI);

#pragma region Scene
	// BACKGROUND OBJECT
	go = std::make_shared<LW2D::GameObject>("Background");
	go->GetTransform().SetParent(go);
	go->AddComponent<LW2D::RenderComponent>()->SetTexture("background.tga");
	sceneVersus.Add(go);

	// MAP OBJECT
	go = std::make_shared<LW2D::GameObject>("Map");
	go->GetTransform().SetParent(go);

	map = go->AddComponent<LW2D::MapComponent>();
	sceneVersus.Add(go);

	// RESIZE WINDOW TO MAP
	SDL_SetWindowSize(pWindow, map->GetCellSize()* map->GetCols(), map->GetCellSize()* map->GetRows() + 144);

	// FPS COMPONENT
	go = std::make_shared<LW2D::GameObject>("FPS Component");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(18.f, 1.f);

	go->AddComponent<LW2D::FPSComponent>();

	sceneVersus.Add(go);
#pragma endregion

#pragma region Player1
	// PLAYER1
	p1 = std::make_shared<LW2D::GameObject>("Player 1");
	p1->GetTransform().SetParent(p1);

	p1->AddComponent<LW2D::RenderComponent>()->SetTexture("PacMan.png");

	// Health Component
	healthComponent = p1->AddComponent<LW2D::HealthComponent>();
	healthComponent->SetLives(3);

	// Score Component
	scoreComponent = p1->AddComponent<LW2D::ScoreComponent>();
	sceneVersus.Add(p1);

	pacMan = p1->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 160.f, 240.f });
	p1->AddComponent<LW2D::PacManComponent>();

	// HEALTH DISPLAY P1
	go = std::make_shared<LW2D::GameObject>("Health Display P1");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(10.f, 350.f);

	font = LW2D::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	textComponent = go->AddComponent<LW2D::TextComponent>(font, "Lives: 3", SDL_Color{ 64, 255, 64 });
	sceneVersus.Add(go);

	// BIND EVENTS
	DecrementHealth = [textComponent](int lives)
	{
		textComponent->SetText("Lives: " + std::to_string(lives));
	};
	healthComponent->GetOnKillEvent()->AddListener(DecrementHealth);

	// SCORE DISPLAY P1
	go = std::make_shared<LW2D::GameObject>("Score Display P1");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(10.f, 370.f);

	textComponent = go->AddComponent<LW2D::TextComponent>(font, "Score: 0", SDL_Color{ 64, 255, 64 });
	sceneVersus.Add(go);

	// BIND EVENTS
	IncrementScoreP1 = [scoreComponent, textComponent](int score)
	{
		scoreComponent->AddScore(score);
		textComponent->SetText("Score: " + std::to_string(scoreComponent->GetScore()));
	};
	map->GetOnPelletCollected()->AddListener(IncrementScoreP1);

	// MOVE UP
	moveCommand = std::make_shared<LW2D::MoveCommand>(pacMan, LW2D::Direction::Up);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_W, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadUp, LW2D::KeyState::Down), moveCommand);

	// MOVE DOWN
	moveCommand = std::make_shared<LW2D::MoveCommand>(pacMan, LW2D::Direction::Down);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_S, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadDown, LW2D::KeyState::Down), moveCommand);

	// MOVE LEFT
	moveCommand = std::make_shared<LW2D::MoveCommand>(pacMan, LW2D::Direction::Left);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_A, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadLeft, LW2D::KeyState::Down), moveCommand);

	// MOVE RIGHT
	moveCommand = std::make_shared<LW2D::MoveCommand>(pacMan, LW2D::Direction::Right);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_D, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadRight, LW2D::KeyState::Down), moveCommand);
#pragma endregion

#pragma region Ghosts
	// GHOST1
	ghost = std::make_shared<LW2D::GameObject>("Ghost 1");
	ghost->GetTransform().SetParent(ghost);

	ghost->AddComponent<LW2D::HealthComponent>()->SetLives(2);

	charComp = ghost->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 145.f, 144.f }, 0.f);
	charComp->SetSpeed(56.f);

	ghostComp = ghost->AddComponent<LW2D::GhostComponent>();
	ghostComp->GetOnGhostKilled()->AddListener(IncrementScoreP1);

	ghost->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");
	sceneVersus.Add(ghost);

	// GHOST2
	ghost = std::make_shared<LW2D::GameObject>("Ghost 2");
	ghost->GetTransform().SetParent(ghost);

	ghost->AddComponent<LW2D::HealthComponent>()->SetLives(2);

	charComp = ghost->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 160.f, 144.f }, 0.f);
	charComp->SetSpeed(56.f);

	ghostComp = ghost->AddComponent<LW2D::GhostComponent>();
	ghostComp->GetOnGhostKilled()->AddListener(IncrementScoreP1);

	ghost->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");
	sceneVersus.Add(ghost);

	// GHOST3
	ghost = std::make_shared<LW2D::GameObject>("Ghost 3");
	ghost->GetTransform().SetParent(ghost);

	ghost->AddComponent<LW2D::HealthComponent>()->SetLives(2);

	charComp = ghost->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 176.f, 144.f }, 0.f);
	charComp->SetSpeed(56.f);

	ghostComp = ghost->AddComponent<LW2D::GhostComponent>();
	ghostComp->GetOnGhostKilled()->AddListener(IncrementScoreP1);

	ghost->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");
	sceneVersus.Add(ghost);
#pragma endregion

#pragma region Player2 (ghost)
	// Player2
	ghost = std::make_shared<LW2D::GameObject>("Ghost 4");
	ghost->GetTransform().SetParent(ghost);

	ghost->AddComponent<LW2D::HealthComponent>()->SetLives(2);

	charComp = ghost->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 161.f, 112.f }, 0.f);
	charComp->SetSpeed(56.f);

	ghostComp = ghost->AddComponent<LW2D::GhostComponent>(false);
	ghostComp->GetOnGhostKilled()->AddListener(IncrementScoreP1);

	ghost->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");
	sceneVersus.Add(ghost);

	// ADD INPUT DEVICES
	LW2D::Input::GetInstance().AddController(std::make_shared<LW2D::GenericController>(1));

	// MOVE UP
	moveCommand = std::make_shared<LW2D::MoveCommand>(charComp, LW2D::Direction::Up);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_UP, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(1, LW2D::GenericController::ControllerButton::DPadUp, LW2D::KeyState::Down), moveCommand);

	// MOVE DOWN
	moveCommand = std::make_shared<LW2D::MoveCommand>(charComp, LW2D::Direction::Down);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_DOWN, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(1, LW2D::GenericController::ControllerButton::DPadDown, LW2D::KeyState::Down), moveCommand);

	// MOVE LEFT
	moveCommand = std::make_shared<LW2D::MoveCommand>(charComp, LW2D::Direction::Left);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_LEFT, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(1, LW2D::GenericController::ControllerButton::DPadLeft, LW2D::KeyState::Down), moveCommand);

	// MOVE RIGHT
	moveCommand = std::make_shared<LW2D::MoveCommand>(charComp, LW2D::Direction::Right);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_RIGHT, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(1, LW2D::GenericController::ControllerButton::DPadRight, LW2D::KeyState::Down), moveCommand);
#pragma endregion
#pragma endregion

#pragma region Coop Scene
	auto& sceneCoop = LW2D::SceneManager::GetInstance().CreateScene("Coop", OnGUI);

#pragma region Scene
	// BACKGROUND OBJECT
	go = std::make_shared<LW2D::GameObject>("Background");
	go->GetTransform().SetParent(go);
	go->AddComponent<LW2D::RenderComponent>()->SetTexture("background.tga");
	sceneCoop.Add(go);

	// MAP OBJECT
	go = std::make_shared<LW2D::GameObject>("Map");
	go->GetTransform().SetParent(go);

	map = go->AddComponent<LW2D::MapComponent>();
	sceneCoop.Add(go);

	// RESIZE WINDOW TO MAP
	SDL_SetWindowSize(pWindow, map->GetCellSize()* map->GetCols(), map->GetCellSize()* map->GetRows() + 144);

	// FPS COMPONENT
	go = std::make_shared<LW2D::GameObject>("FPS Component");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(18.f, 1.f);

	go->AddComponent<LW2D::FPSComponent>();

	sceneCoop.Add(go);
#pragma endregion

#pragma region Player1
	// PLAYER1
	p1 = std::make_shared<LW2D::GameObject>("Player 1");
	p1->GetTransform().SetParent(p1);
	sceneCoop.Add(p1);

	p1->AddComponent<LW2D::RenderComponent>()->SetTexture("PacMan.png");

	// Health Component
	healthComponent = p1->AddComponent<LW2D::HealthComponent>();
	healthComponent->SetLives(3);

	// Score Component
	scoreComponent = p1->AddComponent<LW2D::ScoreComponent>();
	
	pacMan = p1->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 176.f, 240.f });
	p1->AddComponent<LW2D::PacManComponent>();

	// HEALTH DISPLAY P1
	go = std::make_shared<LW2D::GameObject>("Health Display P1");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(10.f, 350.f);

	font = LW2D::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	textComponent = go->AddComponent<LW2D::TextComponent>(font, "Lives P1: 3", SDL_Color{ 64, 255, 64 });
	sceneCoop.Add(go);

	// BIND EVENTS
	DecrementHealth = [textComponent](int lives)
	{
		textComponent->SetText("Lives P1: " + std::to_string(lives));
	};
	healthComponent->GetOnKillEvent()->AddListener(DecrementHealth);

	// SCORE DISPLAY P1
	go = std::make_shared<LW2D::GameObject>("Score Display P1");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(10.f, 370.f);

	textComponent = go->AddComponent<LW2D::TextComponent>(font, "Score: 0", SDL_Color{ 64, 255, 64 });
	sceneCoop.Add(go);

	// BIND EVENTS
	IncrementScoreP1 = [scoreComponent, textComponent](int score)
	{
		scoreComponent->AddScore(score);
		textComponent->SetText("Score: " + std::to_string(scoreComponent->GetScore()));
	};
	map->GetOnPelletCollected()->AddListener(IncrementScoreP1);

	// MOVE UP
	moveCommand = std::make_shared<LW2D::MoveCommand>(pacMan, LW2D::Direction::Up);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_W, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadUp, LW2D::KeyState::Down), moveCommand);

	// MOVE DOWN
	moveCommand = std::make_shared<LW2D::MoveCommand>(pacMan, LW2D::Direction::Down);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_S, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadDown, LW2D::KeyState::Down), moveCommand);

	// MOVE LEFT
	moveCommand = std::make_shared<LW2D::MoveCommand>(pacMan, LW2D::Direction::Left);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_A, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadLeft, LW2D::KeyState::Down), moveCommand);

	// MOVE RIGHT
	moveCommand = std::make_shared<LW2D::MoveCommand>(pacMan, LW2D::Direction::Right);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_D, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(0, LW2D::GenericController::ControllerButton::DPadRight, LW2D::KeyState::Down), moveCommand);
#pragma endregion

#pragma region Player2 (MsPacMan)
	// PLAYER2
	auto p2 = std::make_shared<LW2D::GameObject>("Player 2");
	p2->GetTransform().SetParent(p2);
	sceneCoop.Add(p2);

	p2->AddComponent<LW2D::RenderComponent>()->SetTexture("MsPacMan.png");

	// Health Component
	healthComponent = p2->AddComponent<LW2D::HealthComponent>();
	healthComponent->SetLives(3);

	auto msPacMan = p2->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 144.f, 240.f });
	p2->AddComponent<LW2D::PacManComponent>();

	// HEALTH DISPLAY P1
	go = std::make_shared<LW2D::GameObject>("Health Display P2");
	go->GetTransform().SetParent(go);
	go->GetTransform().SetLocalPosition(128.f, 350.f);

	font = LW2D::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	textComponent = go->AddComponent<LW2D::TextComponent>(font, "Lives P2: 3", SDL_Color{ 64, 255, 64 });
	sceneCoop.Add(go);

	// BIND EVENTS
	DecrementHealth = [textComponent](int lives)
	{
		textComponent->SetText("Lives P2: " + std::to_string(lives));
	};
	healthComponent->GetOnKillEvent()->AddListener(DecrementHealth);

	// MOVE UP
	moveCommand = std::make_shared<LW2D::MoveCommand>(msPacMan, LW2D::Direction::Up);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_UP, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(1, LW2D::GenericController::ControllerButton::DPadUp, LW2D::KeyState::Down), moveCommand);

	// MOVE DOWN
	moveCommand = std::make_shared<LW2D::MoveCommand>(msPacMan, LW2D::Direction::Down);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_DOWN, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(1, LW2D::GenericController::ControllerButton::DPadDown, LW2D::KeyState::Down), moveCommand);

	// MOVE LEFT
	moveCommand = std::make_shared<LW2D::MoveCommand>(msPacMan, LW2D::Direction::Left);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_LEFT, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(1, LW2D::GenericController::ControllerButton::DPadLeft, LW2D::KeyState::Down), moveCommand);

	// MOVE RIGHT
	moveCommand = std::make_shared<LW2D::MoveCommand>(msPacMan, LW2D::Direction::Right);
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_RIGHT, SDL_KEYMAPCHANGED), moveCommand);
	LW2D::Input::GetInstance().AddCommand(std::make_tuple(1, LW2D::GenericController::ControllerButton::DPadRight, LW2D::KeyState::Down), moveCommand);
#pragma endregion

#pragma region Ghosts
	// GHOST1
	ghost = std::make_shared<LW2D::GameObject>("Ghost 1");
	ghost->GetTransform().SetParent(ghost);

	ghost->AddComponent<LW2D::HealthComponent>()->SetLives(2);

	charComp = ghost->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 145.f, 144.f }, 0.f);
	charComp->SetSpeed(56.f);

	ghostComp = ghost->AddComponent<LW2D::GhostComponent>();
	ghostComp->GetOnGhostKilled()->AddListener(IncrementScoreP1);

	ghost->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");
	sceneCoop.Add(ghost);

	// GHOST2
	ghost = std::make_shared<LW2D::GameObject>("Ghost 2");
	ghost->GetTransform().SetParent(ghost);

	ghost->AddComponent<LW2D::HealthComponent>()->SetLives(2);

	charComp = ghost->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 160.f, 144.f }, 0.f);
	charComp->SetSpeed(56.f);

	ghostComp = ghost->AddComponent<LW2D::GhostComponent>();
	ghostComp->GetOnGhostKilled()->AddListener(IncrementScoreP1);

	ghost->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");
	sceneCoop.Add(ghost);

	// GHOST3
	ghost = std::make_shared<LW2D::GameObject>("Ghost 3");
	ghost->GetTransform().SetParent(ghost);

	ghost->AddComponent<LW2D::HealthComponent>()->SetLives(2);

	charComp = ghost->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 176.f, 144.f }, 0.f);
	charComp->SetSpeed(56.f);

	ghostComp = ghost->AddComponent<LW2D::GhostComponent>();
	ghostComp->GetOnGhostKilled()->AddListener(IncrementScoreP1);

	ghost->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");
	sceneCoop.Add(ghost);

	// GHOST4
	ghost = std::make_shared<LW2D::GameObject>("Ghost 4");
	ghost->GetTransform().SetParent(ghost);

	ghost->AddComponent<LW2D::HealthComponent>()->SetLives(2);

	charComp = ghost->AddComponent<LW2D::CharacterComponent>(glm::vec2{ 161.f, 128.f }, 0.f);
	charComp->SetSpeed(56.f);

	ghostComp = ghost->AddComponent<LW2D::GhostComponent>();
	ghostComp->GetOnGhostKilled()->AddListener(IncrementScoreP1);

	ghost->AddComponent<LW2D::RenderComponent>()->SetTexture("Ghost.png");
	sceneCoop.Add(ghost);
#pragma endregion
#pragma endregion

	auto loadNextSceneCommand = std::make_shared<LW2D::LoadNextSceneCommand>();
	LW2D::Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_F1, SDL_KEYDOWN), loadNextSceneCommand);

	LW2D::SceneManager::GetInstance().LoadSceneByIndex(0);
}

int main(int, char* [])
{
	std::cout << "\n[CONTROLS CONTROLLER]\n[D-Pad]: Change direction\n\n";
	std::cout << "[CONTROLS KEYBOARDD]\n[WASD]: Change direction\n\n";

	LW2D::Lightweight_2D engine("../Data/");
	engine.Run(load);

	return 0;
}