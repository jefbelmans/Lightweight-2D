#pragma once
#include <memory>
#include "SoundSystem.h"

namespace LW2D
{
	class ServiceLocator final
	{
		static std::unique_ptr<ISoundSystem> m_pSSInstance;
		static NullSoundSystem m_DefaultSS;

	public:
		static ISoundSystem& GetSoundSystem() { return *m_pSSInstance.get(); }
		static void RegisterSoundSystem(ISoundSystem* pSoundSystem)
		{
			m_pSSInstance.reset(pSoundSystem == nullptr ? &m_DefaultSS : pSoundSystem);
		}
	};

	std::unique_ptr<ISoundSystem> ServiceLocator::m_pSSInstance{};
	NullSoundSystem ServiceLocator::m_DefaultSS;
}