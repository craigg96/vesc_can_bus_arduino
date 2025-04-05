**VESC library for CAN bus control using MCP2515 SPI Driver**

![image](https://github.com/craigg96/vesc_can_bus_arduino/blob/main/images/header.png?raw=true "Header")

This repo contains the CAN bus protocol for VESC that can be used with arduino and the MCP2515 driver library by Cory Fowler (see credits).
The MCP2515 is the most common way to integrate can bus onto the arduino platform. The VESC platform is the gold standard for brushless motor control for skateboards and ebikes. This library allows you to use the can bus instead of UART (see solidgeek vescuart library) to communicate with the VESC which is more reliable and allows the single UART port on most arduinos to be free for debugging.  

Added support for different VESC drivers through the same CAN, this has been tested by reading data from 2 drivers with an ESP32-S3.

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

To use the library out of the box, the vesc tool must be configured as shown in images/vesc_tool_app_settings.png, (you can set the ID of your choice, as long as the other driver has a different one).

VESC ID = (the one you want)

CAN STATUS_MESSAGE_MODE = CAN_STATUS_1_2_3_4_5

CAN Baud Rate = CAN_BAUD_250K

This library has been tested with an ESP32-S3, in case your board has the CS pin on another pin modify the `MCP_CAN CAN0({required pin});` line, no need to modify the library.

Add this library in Arduino

# Credits

* Craig Gault - https://github.com/craigg96
* Sesgaro     - https://github.com/Sesgaro

DEPENDENCIES

* https://github.com/coryjfowler/MCP_CAN_lib

LICENSE

MIT - https://opensource.org/license/mit/
