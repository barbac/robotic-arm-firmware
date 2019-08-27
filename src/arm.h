#pragma once

#include <stdint.h>

#ifndef NO_ARDUINO
#include <Arduino.h>
#include  <Servo.h>
#endif

struct Joint {
    double position = 90;
    bool invert = false;

    int timeStep = 5;
    int positionIncrement = 1;
    int currentPosition = 90;
    int targetPosition = 90;
    int lastStep = 0;

#ifndef NO_ARDUINO
    Servo actuator;
#endif
};

static const int JOINT_COUNT = 6;
static const int STEP_DELTA = 1;

class Arm {
public:
    Arm();
    void step(double currentTime);

    void setTargetPosition(int joint, int position);

    int position(Joint &joint, int currentTime);

    int jointCount = JOINT_COUNT;
    Joint joints[JOINT_COUNT];

    void setDestinationValues(/*const int *values*/);
    bool shouldStep(double lastTimestamp, double timeStep);

    uint8_t destinationValues[JOINT_COUNT];

    bool simulationFinished = true;
    unsigned int valueIndex = 0;
    int destination = 0;
    int delta = 0;
    unsigned int stepsCount = 0;
    int stepSign = 0;
    unsigned int stepIndex = 0;
    unsigned long lastTimestamp = 0;
};
