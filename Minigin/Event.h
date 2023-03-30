#pragma once
#include <functional>
#include <vector>

template<typename... Args>
class Event final
{
public:
	using Callback = std::function<void(Args...)>;

	Event() = default;
	~Event() = default;

	void AddListener(const Callback& callback)
	{
		m_Listeners.push_back(callback);
	}

	void RemoveListener(const Callback& callback)
	{
		for (auto it = m_Listeners.begin(); it != m_Listeners.end(); ++it)
		{
			if (*it == callback)
			{
				m_Listeners.erase(it);
				return;
			}
		}
	}

	void Invoke(Args... args)
	{
		for (auto& callback : m_Listeners)
			callback(args...);
	}

private:
	std::vector<Callback> m_Listeners;
};

