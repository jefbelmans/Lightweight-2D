#pragma once
#include <memory>
#include <string>

namespace LW2D
{
	enum class Sounds : unsigned short
	{
		PacManDeath = 0,
		PacManEat
	};

	using SoundId = unsigned short;

	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual void PlaySound(const SoundId id, const float volume) = 0;
		virtual void StopSound(const SoundId id) = 0;
		virtual void AddSound(const std::string& path, const SoundId id, bool doLoop = false) = 0;

		virtual void StartUp() = 0;
		virtual void Shutdown() = 0;
		virtual bool IsShutdown() = 0;
	};

	class NullSoundSystem final : public ISoundSystem
	{
	public:
		void PlaySound(const SoundId, const float) override {};
		void StopSound(const SoundId) override {};
		void AddSound(const std::string&, const SoundId, bool = false) override {};

		void StartUp() override {};
		void Shutdown() override {};
		bool IsShutdown() override { return false; };
	};

	class SDL_SoundSystem final : public ISoundSystem
	{
	public:
		explicit SDL_SoundSystem();
		~SDL_SoundSystem() = default;

		void PlaySound(const SoundId id, const float volume) override;
		void StopSound(const SoundId id) override;
		void AddSound(const std::string& path, const SoundId id, bool doLoop = false) override;

		void StartUp() override;
		void Shutdown() override;
		bool IsShutdown() override;

	private:
		class SDL_SoundSystemImpl;
		std::unique_ptr<SDL_SoundSystemImpl> m_pImpl;
	};

	class Logging_SoundSystem final : public ISoundSystem
	{
	public:
		Logging_SoundSystem(ISoundSystem* ss) : m_pSS(ss) {};

		void PlaySound(const SoundId id, const float volume) override;
		void StopSound(const SoundId id) override;
		void AddSound(const std::string& path, const SoundId id, bool doLoop = false) override;

		void StartUp() override;
		void Shutdown() override;
		bool IsShutdown() override;

	private:
		std::unique_ptr<ISoundSystem> m_pSS;
	};
}