#include <limits.h>
#include "arm.h"
#include "gtest/gtest.h"
namespace {

// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.

// Tests factorial of negative numbers.

TEST(ArmTest, steps) {
    Arm arm;

    // int destination[] = {1, 2, 3, 4, 5, 6};
    // arm.destinationValues[0] = 1;
    // arm.destinationValues[1] = 2;
    // arm.destinationValues[2] = 3;
    // arm.destinationValues[3] = 4;
    // arm.destinationValues[4] = 5;
    // arm.destinationValues[5] = 6;
    // // arm.destinationValues = destination;
    // // arm.destinationValues = {1, 2, 3, 4, 5, 6};
    // arm.setDestinationValues();
    // // arm.setDestinationValues(destination);

    arm.step(1000);
    // EXPECT_EQ(arm.joints[0].position, 1);
    // arm.step(2000);
    // EXPECT_EQ(arm.joints[0].position, 1);
    // EXPECT_EQ(arm.joints[1].position, 1);
    // arm.step(3000);
    // EXPECT_EQ(arm.joints[1].position, 2);
    // arm.step(4000);
    // EXPECT_EQ(arm.joints[2].position, 1);
    // arm.step(5000);
    // EXPECT_EQ(arm.joints[0].position, 1);
    // EXPECT_EQ(arm.joints[1].position, 2);
    // EXPECT_EQ(arm.joints[2].position, 2);
}

struct ATTACH_SERVO {
    uint16_t min;
    uint16_t max;
    uint8_t pin;
} attachServo;

// Tests positive input.
TEST(BufferToStructTest, SERVO) {
    const int min = 500;
    const int max = 2200;
    uint8_t buffer[5] = {
        (uint8_t) (min & 0XFF), (uint8_t) (min >> 8),
        (uint8_t) (max & 0XFF), (uint8_t) (max >> 8),
        3
    };

    memcpy(&attachServo, buffer, sizeof(ATTACH_SERVO));

    // memcpy(&attachServo.min, buffer, 2);
    // memcpy(&attachServo.max, buffer + 2, 2);
    // memcpy(&attachServo.pin, buffer + 4, 1);

    EXPECT_EQ(attachServo.min, 500);
    EXPECT_EQ(attachServo.max, 2200);
    EXPECT_EQ(attachServo.pin, 3);
}

}  // namespace
