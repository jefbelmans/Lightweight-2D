#include <iostream>
#include "SteamAchievements.h"

LW2D::SteamAchievements::SteamAchievements(Achievement_t* Achievements, int NumAchievements):
m_AppID(0),
m_Initialized(false),
m_CallbackUserStatsReceived(this, &SteamAchievements::OnUserStatsReceived),
m_CallbackUserStatsStored(this, &SteamAchievements::OnUserStatsStored),
m_CallbackAchievementStored(this, &SteamAchievements::OnAchievementStored)
{
    m_AppID = SteamUtils()->GetAppID();
    m_pAchievements = Achievements;
    m_NumAchievements = NumAchievements;
    RequestStats();
}

bool LW2D::SteamAchievements::RequestStats()
{
	// Is Steam loaded? If not we can't get stats.
	if (NULL == SteamUserStats() || NULL == SteamUser())
	{
		return false;
	}
	// Is the user logged on?  If not we can't get stats.
	if (!SteamUser()->BLoggedOn())
	{
		return false;
	}
	// Request user stats.
	return SteamUserStats()->RequestCurrentStats();
}

bool LW2D::SteamAchievements::SetAchievement(const char* ID)
{
	// Have we received a call back from Steam yet?
	if (m_Initialized)
	{
		SteamUserStats()->SetAchievement(ID);
		return SteamUserStats()->StoreStats();
	}
	// If not then we can't set achievements yet
	return false;
}

void LW2D::SteamAchievements::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_AppID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			std::cout << "Received stats and achievements from Steam\n";
			m_Initialized = true;

			// load achievements
			for (int iAch = 0; iAch < m_NumAchievements; ++iAch)
			{
				Achievement_t& ach = m_pAchievements[iAch];

				SteamUserStats()->GetAchievement(ach.m_pchAchievementID, &ach.m_bAchieved);
				_snprintf(ach.m_rgchName, sizeof(ach.m_rgchName), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
						"name"));
				_snprintf(ach.m_rgchDescription, sizeof(ach.m_rgchDescription), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
						"desc"));
			}
		}
		else
		{
			char buffer[128];
			_snprintf(buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult);
			std::cout << buffer;
		}
	}
}

void LW2D::SteamAchievements::OnUserStatsStored(UserStatsStored_t * pCallback)
{
		// we may get callbacks for other games' stats arriving, ignore them
		if (m_AppID == pCallback->m_nGameID)
		{
			if (k_EResultOK == pCallback->m_eResult)
			{
				std::cout << "Stored stats for Steam\n";
			}
			else
			{
				char buffer[128];
				_snprintf(buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult);
				std::cout << buffer;
			}
		}
	}

void LW2D::SteamAchievements::OnAchievementStored(UserAchievementStored_t* pCallback)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_AppID == pCallback->m_nGameID)
	{
		std::cout << "Stored Achievement for Steam\n";
	}
}