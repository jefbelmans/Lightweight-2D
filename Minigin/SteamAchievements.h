#pragma once
#include <steam_api.h>

namespace LW2D
{
	#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
	struct Achievement_t
	{
		int m_eAchievementID;
		const char* m_pchAchievementID;
		char m_rgchName[128];
		char m_rgchDescription[256];
		bool m_bAchieved;
		int m_iIconImage;
	};

	class SteamAchievements
	{
	public:
		SteamAchievements(Achievement_t* Achievements, int NumAchievements);
		~SteamAchievements() = default;

		bool RequestStats();
		bool SetAchievement(const char* ID);

		STEAM_CALLBACK(SteamAchievements, OnUserStatsReceived, UserStatsReceived_t,
			m_CallbackUserStatsReceived);
		STEAM_CALLBACK(SteamAchievements, OnUserStatsStored, UserStatsStored_t,
			m_CallbackUserStatsStored);
		STEAM_CALLBACK(SteamAchievements, OnAchievementStored,
			UserAchievementStored_t, m_CallbackAchievementStored);

	private:
		uint64_t m_AppID; // Our current AppID
		Achievement_t* m_pAchievements; // Achievements data
		int m_NumAchievements; // The number of Achievements
		bool m_Initialized; // Have we called Request stats and received the callback?
	};
}