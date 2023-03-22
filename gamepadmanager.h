#ifndef GAMEPADMANAGER_H
#define GAMEPADMANAGER_H

#include <QObject>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

class GamepadManager : public QObject
{
    Q_OBJECT
public:
    explicit GamepadManager(QObject *parent = nullptr);

signals:
    void gamepadButtonPressed(int buttonId);
    void gamepadJoystickChanged(short sThumbLX, short sThumbLY, short sThumbRX, short sThumbRY, short leftTrigger, short rightTrigger);

private slots:
    void pollGamepads();

private:
    void processStateButton(const XINPUT_STATE &state, int gamepadIndex);
    void processStateJoystick(const XINPUT_STATE &state, int gamepadIndex);

    bool m_running = true;
    bool buttonPressed[16] = {0};
};

#endif // GAMEPADMANAGER_H
