#include <mcp_can.h>
#include "vesc_can_bus_arduino.h"

CAN can;             // get torque sensor data, throttle for now

bool print_realtime_data;
long last_print_data;


void setup()
{

    Serial.begin(115200); // Wired comms from USB port

    can.initialize();

}

void loop()
{
    can.spin();

    can.vesc_set_current(2); //2 amps of current

    if (print_realtime_data == true)
    {
        if (millis() - last_print_data > 200)
        {
            Serial.print(can.erpm); 
            Serial.print(can.inpVoltage);
            Serial.print(can.dutyCycleNow);
            Serial.print(can.avgInputCurrent);
            Serial.print(can.avgMotorCurrent);
            Serial.print(can.tempFET);
            Serial.print(can.tempMotor);

            last_print_data = millis();
        }
    }
}
