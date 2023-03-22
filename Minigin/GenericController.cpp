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
    }

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

void GenericController::Update()
{
}

bool GenericController::IsDown(ControllerButton button) const
{
    return false;
}

bool GenericController::IsUp(ControllerButton button) const
{
    return false;
}

bool GenericController::IsPressed(ControllerButton button) const
{
    return false;
}

GenericController::GenericController(int controllerIndex)
{
}

GenericController::~GenericController()
{
}
