#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

#include "GenericController.h"

using namespace LW2D;

class GenericController::GenericControllerImpl
{
    XINPUT_STATE previousState{};
    XINPUT_STATE currentState{};

    WORD buttonsPresssedThisFrame;
    WORD buttonsReleasedThisFrame;

    int _controllerIndex;
public:
    GenericControllerImpl(int controllerIndex)
    {
        ZeroMemory(&previousState, sizeof(XINPUT_STATE));
        ZeroMemory(&currentState, sizeof(XINPUT_STATE));
        _controllerIndex = controllerIndex;
    }
    ~GenericControllerImpl() = default;

    void Update()
    {
        CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
        ZeroMemory(&currentState, sizeof(XINPUT_STATE));
        XInputGetState(_controllerIndex, &currentState);

        auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
        buttonsPresssedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
        buttonsPresssedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
    }

    bool IsDownThisFrame(unsigned int button) const { return buttonsPresssedThisFrame & button; }
    bool IsUpThisFrame(unsigned int button) const { return buttonsReleasedThisFrame & button; }
    bool IsPressed(unsigned int button) const { return currentState.Gamepad.wButtons & button; }
};

GenericController::GenericController(int controllerIndex)
{
    m_pImpl = std::make_unique<GenericControllerImpl>(controllerIndex);
}

void GenericController::Update()
{
    m_pImpl->Update();
}

bool GenericController::IsDown(ControllerButton button) const
{
    return m_pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool GenericController::IsUp(ControllerButton button) const
{
    return m_pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
}

bool GenericController::IsPressed(ControllerButton button) const
{
    return m_pImpl->IsPressed(static_cast<unsigned int>(button));
}
