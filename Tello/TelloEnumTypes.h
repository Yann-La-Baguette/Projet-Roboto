#ifndef TELLOENUMTYPES_H
#define TELLOENUMTYPES_H

enum class TelloResponse {
    TELLO_OK,                 // Positive answear
    TELLO_ERROR,              // Negative Answear
    TELLO_VALUE,              // Positive answear with value return
    TELLO_ERROR_NOT_JOYSTICK, // Wait untill send next command
    TELLO_ERROR_NO_VALID_IMU, // Maybe had some changes on room light
    TELLO_OUT_OF_RANGE,       // If the range is like 20-500 and you give a number out of the range
    TELLO_UNDEFINED,          // Used while I don't discover all of error types
    TELLO_TIMEOUT            // When the drone doesn't respond
};

enum class TelloAlerts {
    TELLO_CONNECTION_ESTABLISHED,
    TELLO_CONNECTION_WAITING,
    TELLO_CONNECTION_NO_RESPONSE,
    TELLO_CONNECTION_TIMEOUT,
    TELLO_CONNECTION_FAILED,
    SOCKET_CONNECTION_FAILED
};

#endif // TELLOENUMTYPES_H
