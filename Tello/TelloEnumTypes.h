#ifndef TELLOENUMTYPES_H
#define TELLOENUMTYPES_H

#define TELLO_DEBUG_OUTPUT false
#define TELLO_STATE_OUTPUT false

enum class TelloResponse {
    TELLO_OK,                 // Positive answear
    TELLO_ERROR,              // Negative Answear
    TELLO_VALUE,              // Positive answear with value return
    TELLO_ERROR_NOT_JOYSTICK, // Wait untill send next command
    TELLO_ERROR_NO_VALID_IMU, // Maybe had some changes on room light
    TELLO_OUT_OF_RANGE,       // If the range is like 20-500 and you give a number out of the range
    TELLO_UNDEFINED,          // Used while I don't discover all of error types
    TELLO_TIMEOUT             // When the drone doesn't respond
};

enum class TelloStatus {
    TELLO_CONNECTED,
    TELLO_DISCONNECTED,
};

#endif // TELLOENUMTYPES_H
