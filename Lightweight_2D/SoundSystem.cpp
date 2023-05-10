#include "SoundSystem.h"
#include <SDL_mixer.h>
#include <iostream>

#pragma region SDL_SoundSystem
void LW2D::SDL_SoundSystem::PlaySound(const SoundId id, const float volume)
{
	m_Pending.push({ id, volume });
}

void LW2D::SDL_SoundSystem::Update()
{
}
#pragma endregion

#pragma region Logging_SoundSystem
void LW2D::Logging_SoundSystem::PlaySound(const SoundId id, const float volume)
{
	m_pSS->PlaySound(id, volume);
	std::cout << "Queueing sound with ID [" << id << "] at [" << volume << "] volume\n";
}
void LW2D::Logging_SoundSystem::Update()
{
	
}
#pragma endergion