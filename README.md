**VESC library for CAN bus control using MCP2515 SPI Driver**

![image](https://github.com/craigg96/vesc_can_bus_arduino/blob/main/images/header.png?raw=true "Header")

Check out my [open source vesc hardware implementation](https://github.com/craigg96/VESC6_OPEN_HARDWARE)

This repo contains the CAN bus protocol for VESC that can be used with arduino and the MCP2515 driver library by Cory Fowler (see credits).
The MCP2515 is the most common way to integrate can bus onto the arduino platform. The VESC platform is the gold standard for brushless motor control for skateboards and ebikes. This library allows you to use the can bus instead of UART (see solidgeek vescuart library) to communicate with the VESC which is more reliable and allows the single UART port on most arduinos to be free for debugging. The documentation from vesc-project about the CAN protocol can be found [here](https://vesc-project.com/sites/default/files/imce/u15301/VESC6_CAN_CommandsTelemetry.pdf) 

Checkout the [multiple vescs](https://github.com/craigg96/vesc_can_bus_arduino/tree/Multiple-VESCs) branch by [Sesgaro](https://github.com/Sesgaro) to control two vescs independently (UNTESTED).

## Realtime Data Messages
|     Parameter     |
|:-----------------:|
|        rpm        |
|     inpVoltage    |
|    dutyCycleNow   |
|  avgInputCurrent  |
|  avgMotorCurrent  |
|      tempFET      |
|     tempMotor     |
|     WattHours     |

## Command Messages
|     Parameter     |
|:-----------------:|
|        erpm       |
|      dutycycle    |
|       current     |

To use the library out of the box, the vesc tool must be configured as shown in images/vesc_tool_app_settings.png.
VESC ID = 10
CAN STATUS_MESSAGE_MODE = CAN_STATUS_1_2_3_4_5
CAN Baud Rate = CAN_BAUD_250K

Place the .h and .cpp file in the same folder as the .ino file to use this library with no changes.

# Credits

* Craig Gault - https://github.com/craigg96
* Sesgaro - https://github.com/Sesgaro

DEPENDENCIES

* https://github.com/coryjfowler/MCP_CAN_lib

LICENSE

MIT - https://opensource.org/license/mit/
