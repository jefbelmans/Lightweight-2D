#pragma once
#include "Singleton.h"

namespace LW2D
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
	};

}
