#pragma once
#include <queue>
#include <memory>

namespace LW2D
{
	using SoundId = unsigned short;

	struct PlayMessage
	{
		SoundId id;
		float volume;
	};

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void PlaySound(const SoundId id, const float volume) = 0;

	protected:
		virtual void Update() = 0;
	};

	class NullSoundSystem final : public SoundSystem
	{
	public:
		void PlaySound(const SoundId, const float) override {};

	private:
		void Update() override {};
	};

	class SDL_SoundSystem final : public SoundSystem
	{
	public:
		void PlaySound(const SoundId id, const float volume) override;

	private:
		std::queue<PlayMessage> m_Pending;
		void Update() override;
	};

	class Logging_SoundSystem final : public SoundSystem
	{
	public:
		Logging_SoundSystem(SoundSystem* ss) : m_pSS(ss) {};

		void PlaySound(const SoundId id, const float volume) override;
	private:
		std::unique_ptr<SoundSystem> m_pSS;

		void Update() override;
	};
}