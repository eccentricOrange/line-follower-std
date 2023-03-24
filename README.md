# A basic line follower

This is the code for a prototypical line follower.

This robot has the following features:
* It would use 3 infrared sensors to detect the line. Each sensor outputs a Boolean value.
* It would follow [differential drive](https://en.wikipedia.org/wiki/Differential_wheeled_robot) principles for movement.
* The expected control hardware is an Arduino-compatible microcontroller.
* The expected motor driver is a something like an L293D or L298N motor driver.

## Logic
The logic in this program is very simple. It can be worked by using a model or drawing of the robot, and a line on a piece of paper.

The robot has 3 sensors, and 2 motors (or another even number). The sensors are placed on the left, middle, and right of the robot. The sensors are placed in a line, and the robot is placed on the line.

For every possible sensor configuration, the robot will move in a certain way, and if you can determine which movement will bring teh centre sensor to the line, you can determine the logic.

## Motor control
This robot uses my own [Differential Drive library, DDBot](https://github.com/eccentricOrange/DDBot).

There is an upgraded version of this, which is expected to be smoother because it uses a closed-loop feedback system to actuate the motors. It is available in the [line-follower-smooth repository](https://github.com/eccentricOrange/line-follower-smooth).