#include <mcp_can.h>
#include "vesc_can_bus_arduino.h"

MCP_CAN CAN0(10); // CS pin

bool print_realtime_data = true;
long last_print_data;

VescCAN driver1(CAN0, 10);    // ID 10
VescCAN driver2(CAN0, 11);    // ID 11
//VescCAN driver3(CAN0, 12);
//VescCAN driver4(CAN0, 13);
VescCAN* drivers[] = {&driver1, &driver2}; // &driver3, &driver4};
const int numdrivers = 2;

void setup()
{

    Serial.begin(115200); // Wired comms from USB port

    CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ); // Set the CAN bus speed and clock frequency
    CAN0.setMode(MCP_NORMAL);
}

void loop() {
    driver1.spin();
    driver2.spin();

    driver1.vesc_set_erpm(30);
    driver2.vesc_set_erpm(30);

    if (millis() - last_print_data > 1000) {
        for (int i = 0; i < numdrivers; i++) {

            Serial.print("driver ");
            Serial.print(i + 1);  // Print wheel number (1 or 2)
            Serial.println(" data:");
            
            Serial.print("rpm: ");
            Serial.println(drivers[i]->erpm);
            Serial.print("voltage: ");
            Serial.println(drivers[i]->inpVoltage);
            Serial.print("dutyCycleNow: ");
            Serial.println(drivers[i]->dutyCycleNow);
            Serial.print("avgInputCurrent: ");
            Serial.println(drivers[i]->avgInputCurrent);
            Serial.print("avgMotorCurrent: ");
            Serial.println(drivers[i]->avgMotorCurrent);
            Serial.print("tempFET: ");
            Serial.println(drivers[i]->tempFET);
            Serial.print("tempMotor: ");
            Serial.println(drivers[i]->tempMotor);
            Serial.print("WattHours: ");
            Serial.println(drivers[i]->WattHours);
            Serial.println("..............................");
        }
        last_print_data = millis();
        Serial.println();  // Extra newline for separation between cycles
    }
}
