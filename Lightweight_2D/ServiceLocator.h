#pragma once
#include <memory>
#include "SoundSystem.h"

namespace LW2D
{
	class ServiceLocator final
	{
	public:
		static SoundSystem& GetSoundSystem() { return *m_pSSInstance.get(); }
		static void RegisterSoundSystem(SoundSystem* pSoundSystem)
		{
			m_pSSInstance.reset(pSoundSystem == nullptr ? &m_DefaultSS : pSoundSystem);
		}

	private:
		static std::unique_ptr<SoundSystem> m_pSSInstance;
		static NullSoundSystem m_DefaultSS;
	};
}