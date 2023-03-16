#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "CacheTrasher.h"

// IMGUI
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl2.h"
#include "imgui_plot.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void LW2D::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void LW2D::Renderer::Render()
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	if (m_showDemo)
	{
		ImGui::ShowDemoWindow();
	}

	ImGui::Begin("Exercise 1");
	ImGui::InputInt("# samples", &nrSamplesFloat, 1, 10);
	if (ImGui::Button("Trash the cache!"))
	{
		m_floatAvg = CacheTrasher::GetInstance().TakeMeasurement<float>(nrSamplesFloat);
	}

	// FLOAT GRAPH
	ImGui::PlotConfig conf;
	conf.values.xs = m_steps.data();
	conf.values.ys = m_floatAvg.data();
	conf.values.count = (int)m_steps.size();
	conf.values.color = ImColor(255, 165, 0);
	conf.grid_x.show = false;
	conf.grid_y.show = true;
	conf.grid_y.size = 500.f;
	conf.scale.min = 0.f;
	conf.scale.max = 6'000.f;
	conf.tooltip.show = true;
	conf.tooltip.format = "x=%.2f, y=%.2f";
	conf.frame_size = ImVec2(200, 100);
	conf.line_thickness = 2.f;

	ImGui::Plot("plot", conf);
	ImGui::End();
	
	ImGui::Begin("Exercise 2");
	ImGui::InputInt("# samples", &nrSamplesGO, 1, 10);

	if (ImGui::Button("Trash the cache with GameObject3D!"))
	{
		m_3DAvg = CacheTrasher::GetInstance().TakeMeasurement<GameObject3D>(nrSamplesGO);
	}

	// 3D GRAPH
	conf.values.ys = m_3DAvg.data();
	conf.values.color = ImColor(32, 255, 32);
	conf.scale.max = 45'000.f;
	conf.grid_y.size = 5'000.f;
	ImGui::Plot("plot_float", conf);

	if (ImGui::Button("Trash the cache with GameObject3DAlt!"))
	{
		m_3DAltAvg = CacheTrasher::GetInstance().TakeMeasurement<GameObject3DAlt>(nrSamplesGO);
	}

	// 3DALT GRAPH
	conf.values.ys = m_3DAltAvg.data();
	conf.values.color = ImColor(0, 255, 255);
	ImGui::Plot("plot_3D", conf);

	ImGui::Text("Combined:");

	// COMBINED GRAPH
	const float* data[] = { m_3DAvg.data(), m_3DAltAvg.data() };
	ImGui::PlotConfig confCombined;
	confCombined.values.xs = m_steps.data();
	confCombined.values.count = (int)m_steps.size();
	confCombined.values.ys_list = data; // use ys_list to draw several lines simultaneously
	confCombined.values.ys_count = 2;
	confCombined.values.colors = m_colors;
	confCombined.scale.min = 0.f;
	confCombined.scale.max = 45'000.f;
	confCombined.tooltip.show = true;
	confCombined.grid_x.show = false;
	confCombined.grid_y.show = true;
	confCombined.grid_y.size = 5'000.f;
	confCombined.tooltip.format = "x=%.2f, y=%.2f";
	confCombined.frame_size = ImVec2(200, 100);
	confCombined.line_thickness = 2.0f;

	ImGui::Plot("plot_3DAlt", confCombined);
	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_renderer);
}

void LW2D::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void LW2D::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void LW2D::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

inline SDL_Renderer* LW2D::Renderer::GetSDLRenderer() const { return m_renderer; }
