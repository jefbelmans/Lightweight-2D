#pragma once
#include <string>
#include <functional>

namespace LW2D
{
	class Lightweight_2D
	{
	public:
		explicit Lightweight_2D(const std::string& dataPath);
		~Lightweight_2D();
		void Run(const std::function<void()>& load);

		Lightweight_2D(const Lightweight_2D& other) = delete;
		Lightweight_2D(Lightweight_2D&& other) = delete;
		Lightweight_2D& operator=(const Lightweight_2D& other) = delete;
		Lightweight_2D& operator=(Lightweight_2D&& other) = delete;
	};
}