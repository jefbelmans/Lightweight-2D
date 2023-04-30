#pragma once

namespace LW2D
{
	class Keyboard final
	{
		class KeyboardImpl;
		KeyboardImpl* m_pImpl;
	public:
		explicit Keyboard();
		~Keyboard();

		void Update();

		bool IsDown(int button) const;
		bool IsUp(int button) const;
		bool IsPressed(int button) const;
	};
}