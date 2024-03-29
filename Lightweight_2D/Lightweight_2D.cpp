#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Lightweight_2D.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "GameTime.h"
#include "ResourceManager.h"
#include <thread>

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_MIXER_VERSION(&version);
	printf("We compiled against SDL_mixer version %u.%u.%u ...\n",
				version.major, version.minor, version.patch);

	version = *Mix_Linked_Version();
	printf("We are linking against SDL_mixer version %u.%u.%u.\n\n",
						version.major, version.minor, version.patch);
}

LW2D::Lightweight_2D::Lightweight_2D(const std::string &dataPath)
{
#ifdef _DEBUG
	PrintSDLVersion();
#endif
	
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Jef Belmans - 2DAE15 - Pac Man - Programming 4 Exam Assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

LW2D::Lightweight_2D::~Lightweight_2D()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void LW2D::Lightweight_2D::Run(const std::function<void(SDL_Window* pWindow)>& load)
{
	load(g_window);

	// Enable VSync
	SDL_GL_SetSwapInterval(1);

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	sceneManager.Initialize();

	bool doContinue = true;
	while (doContinue)
	{
		// Update Time before all else
		sceneManager.GetGameTime()->Update();
		doContinue = input.ProcessInput();

		sceneManager.Update();
		renderer.Render();
	}
}
