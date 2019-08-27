#ifdef NO_ARDUINO
#include <iostream>
using namespace std;
#endif

#include "arm.h"

#define BASE_PIN 3;
#define SHOULDER_PIN 5;
#define ELBOW_PIN 6;
#define WRIST_PIN 9;
#define HAND_PIN 10;
#define GRIPPER_PIN 11;

template<typename T>
void print(T value)
{
#ifdef NO_ARDUINO
    // cout << value;
#else
    Serial.print(value);
#endif
}

Arm::Arm()
{
    /*
#ifndef NO_ARDUINO
    this->joints[0].actuator.attach(BASE_PIN);
    this->joints[1].actuator.attach(SHOULDER_PIN);
    this->joints[2].actuator.attach(ELBOW_PIN);
    this->joints[3].actuator.attach(WRIST_PIN);
    this->joints[4].actuator.attach(HAND_PIN);
    this->joints[5].actuator.attach(GRIPPER_PIN);
#endif
    */
}

void Arm::setDestinationValues(/*const int *values*/)
{
    // for (int i = 0; i < JOINT_COUNT; ++i) {
    //     this->destinationValues[i] = values[i];
    // }
    this->simulationFinished = false;

    this->valueIndex = 0;
    this->destination = 0;
    this->stepsCount = 0;
    this->stepSign = 0;
    this->stepIndex = 0;
}

bool Arm::shouldStep(double lastTimestamp, double timestamp)
{
    ///
    const int stepsPerSecond = 30;
    const int rounded = 1000 % stepsPerSecond ? 1 : 0;
    const int stepTimeDelta = (1000 / stepsPerSecond) + rounded;
    ///

    const auto timeDelta = timestamp - lastTimestamp;
    return timeDelta >= stepTimeDelta;
}

void Arm::step(double currentTime)
{
    if (!this->shouldStep(this->lastTimestamp, currentTime)) {
        return;
    } else {
        this->lastTimestamp = currentTime;
    }

    while (this->valueIndex < JOINT_COUNT) {

        Joint *joint = &this->joints[this->valueIndex];

        print("val index: ");
        print(this->valueIndex);
        print(" pos: ");
        print(joint->position);

        if (!this->stepSign) {
            this->destination = this->destinationValues[this->valueIndex];
            const auto initialPosition = joint->position;
            this->delta = destination - initialPosition;
            if (!this->delta) {
                this->valueIndex++;
                print(" skipped\n");
                continue; //don't waste 1 step if no movement is needed.
            }

            const int steps = this->delta / STEP_DELTA;
            this->stepsCount = abs(steps);
            this->stepSign = steps > 0 ? 1 : -1;

            print("\nnew step sign steps: ");
            print(this->stepsCount);
            print("\n");
        }

        print(" Step: ");
        print(this->stepIndex);
        print(" dest: ");
        print(this->destination);
        print("\n");

        if (this->stepIndex < this->stepsCount) {
            this->stepIndex++;
            joint->position += STEP_DELTA * this->stepSign;
#ifdef ARDUINO
            joint->actuator.write(joint->invert ? 180 - joint->position :
                    joint->position);
#endif

            print(" adding.. total: ");
            print(joint->position);
            print("\n");

            return;
        }
        if (this->delta % STEP_DELTA) {
            //the missing part.
            joint->position = this->destination;
#ifdef ARDUINO
            joint->actuator.write(joint->invert ? 180 - joint->position :
                    joint->position);
#endif

            print(" modu ");
            print(joint->position);
            print("\n");
        }

        print("step done index ");
        print(this->valueIndex);
        print(" post: ");
        print(joint->position);
        print("\n");

        //reset for next step.
        this->stepSign = 0;
        this->stepIndex = 0;
        this->valueIndex++;

        // std::cout << currentTime << ' ' << this->valueIndex << ' ';
        // std::cout << joint->position <<  ' ' << this->simulationFinished << std::endl;
    }
    this->simulationFinished = true;

    print("waaaa\n");



    for (int i = 0; i <= this->jointCount; ++i) {
#ifndef NO_ARDUINO
        // auto &servo = this->joints[i].actuator;
        // servo.write(this->position(joints[i], currentTime));
#endif
        // std::cout << currentTime;
    }
}

int Arm::position(Joint &joint, int currentTime)
{
    if (joint.targetPosition == joint.currentPosition)
        return joint.currentPosition;

    const int timeDelta = currentTime - joint.lastStep;

    if (timeDelta >= joint.timeStep) {
        joint.currentPosition += joint.positionIncrement;
        joint.lastStep = currentTime;
    }

    return joint.currentPosition;
}

void Arm::setTargetPosition(int joint, int position)
{
    if (joint < 0 || joint >= this->jointCount)
        return;

    this->joints[0].targetPosition = position;
}
