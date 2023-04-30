#include "Keyboard.h"
#include "SDL.h"
#include <memory>

using namespace LW2D;

class Keyboard::KeyboardImpl
{
	Uint8* m_pPreviousKeyboardState = new Uint8[SDL_NUM_SCANCODES];
	Uint8* m_pCurrentKeyboardState = new Uint8[SDL_NUM_SCANCODES];

public:
	KeyboardImpl() = default;
	~KeyboardImpl()
	{
		delete[] m_pPreviousKeyboardState;
		delete[] m_pCurrentKeyboardState;
	};

	void Update()
	{
		memcpy(m_pPreviousKeyboardState, m_pCurrentKeyboardState, SDL_NUM_SCANCODES);
		memcpy(m_pCurrentKeyboardState, SDL_GetKeyboardState(nullptr), SDL_NUM_SCANCODES);
	}

	bool IsDown(int button) const {	return m_pCurrentKeyboardState[button] && !m_pPreviousKeyboardState[button]; }
	bool IsUp(int button) const { return !m_pCurrentKeyboardState[button] && m_pPreviousKeyboardState[button]; }
	bool IsPressed(int button) const { return m_pCurrentKeyboardState[button]; }
};;

LW2D::Keyboard::Keyboard()
{
	m_pImpl = new KeyboardImpl();
}

LW2D::Keyboard::~Keyboard()
{
	delete m_pImpl;
}

void Keyboard::Update()
{
	m_pImpl->Update();
}

bool Keyboard::IsDown(int button) const
{
    return m_pImpl->IsDown(button);
}

bool Keyboard::IsUp(int button) const
{
    return m_pImpl->IsUp(button);
}

bool Keyboard::IsPressed(int button) const
{
    return m_pImpl->IsPressed(button);
}
