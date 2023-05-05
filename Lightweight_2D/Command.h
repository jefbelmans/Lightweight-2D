#pragma once
#include <memory>

namespace LW2D
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};
}