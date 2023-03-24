#include "gamepadmanager.h"

#include <QDebug>
#include <QThread>

GamepadManager::GamepadManager(QObject *parent) : QObject(parent), m_running(true)
{
    QThread* thread = new QThread();
    this->moveToThread(thread);

    connect(thread, &QThread::started, this, &GamepadManager::pollGamepads);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}

void GamepadManager::pollGamepads()
{
    int joystickIncrement = 0;
    while (m_running)
    {
        for (DWORD i = 0; i < XUSER_MAX_COUNT; ++i)
        {
            XINPUT_STATE state;
            ZeroMemory(&state, sizeof(XINPUT_STATE));

            // Get the state of the gamepad
            DWORD result = XInputGetState(i, &state);

            if (result == ERROR_SUCCESS)
            {
                processStateButton(state, i);
                if(joystickIncrement>15){
                    processStateJoystick(state, i);
                    joystickIncrement = 0;
                }
            }
            else if (result == ERROR_DEVICE_NOT_CONNECTED)
            {
                // Gamepad is not connected
            }
        }
        joystickIncrement++;
        // Poll gamepads at a 60Hz rate
        QThread::msleep(16);
    }
}

void GamepadManager::processStateButton(const XINPUT_STATE &state, int gamepadIndex)
{
    for (int i = 0; i < 16; i++){
        if(state.Gamepad.wButtons & (1 << i) && buttonPressed[i] == false){
            emit gamepadButtonPressed(i);
            buttonPressed[i] = true;
        }
        else if((state.Gamepad.wButtons & (1 << i)) == false && buttonPressed[i] == true){
            buttonPressed[i] = false;
        }
    }
}

void GamepadManager::processStateJoystick(const XINPUT_STATE &state, int gamepadIndex){
    short sThumbLX = state.Gamepad.sThumbLX;
    short sThumbLY = state.Gamepad.sThumbLY;
    short sThumbRX = state.Gamepad.sThumbRX;
    short sThumbRY = state.Gamepad.sThumbRY;
    short leftTrigger = state.Gamepad.bLeftTrigger;
    short rightTrigger = state.Gamepad.bRightTrigger;

    emit gamepadJoystickChanged(sThumbLX, sThumbLY, sThumbRX, sThumbRY, leftTrigger, rightTrigger);
    //QThread::msleep(250);
}
