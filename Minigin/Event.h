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
		auto it = std::find_if(m_Listeners.begin(), m_Listeners.end(), [&](const Callback& cb) {
			return cb.target_type() == callback.target_type() && cb.target<void(*)(Args...)>() == callback.target<void(*)(Args...)>();
			});
		if (it != m_Listeners.end())
		{
			m_Listeners.erase(it);
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

