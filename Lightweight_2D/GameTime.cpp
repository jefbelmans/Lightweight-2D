#include "GameTime.h"

void LW2D::GameTime::Update()
{
	m_CurrTime = std::chrono::steady_clock::now();
	m_DeltaTime = std::chrono::duration<float, std::milli>(m_CurrTime - m_PrevTime).count();

	if (m_DeltaTime >= m_MaxDeltaTime)
		m_DeltaTime = m_MaxDeltaTime;

	m_Time += m_DeltaTime;
	m_PrevTime = m_CurrTime;
}
