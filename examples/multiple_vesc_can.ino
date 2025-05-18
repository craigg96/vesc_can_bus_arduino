#include <mcp_can.h>
#include "vesc_can_bus_arduino.h"

MCP_CAN CAN0(10); // CS pin

bool print_realtime_data = true;
long last_print_data;

VescCAN driver1(CAN0, 10);    // ID 10
VescCAN driver2(CAN0, 11);    // ID 11
VescCAN* drivers[] = {&driver1, &driver2};
const int numdrivers = 2;

// Buffer for serial commands
String commandBuffer = "";

void setup()
{
    Serial.begin(115200);

    // Initialize CAN bus
    CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ); // Set the CAN bus speed and clock frequency
    CAN0.setMode(MCP_NORMAL);

    Serial.println("Starting VESC control via CAN bus...");
}

void loop() {
    // Process CAN messages for both drivers
    driver1.spin();
    driver2.spin();

    // Process commands from Serial, example: D1:40;D2:40;
    while (Serial.available()) {
        char c = Serial.read();
        commandBuffer += c;
        if (c == '\n') {
            commandBuffer.trim();
            // Process setpoint commands
            if (commandBuffer.startsWith("D1:") && commandBuffer.endsWith(";")) {
                float erpm1 = 0.0, erpm2 = 0.0;

                // Parse the commands
                int pos1 = commandBuffer.indexOf("D1:");
                int pos2 = commandBuffer.indexOf("D2:");
                int endPos = commandBuffer.length();

                if (pos1 != -1 && pos2 != -1) {
                    erpm1 = commandBuffer.substring(pos1 + 3, commandBuffer.indexOf(';', pos1)).toFloat();
                    erpm2 = commandBuffer.substring(pos2 + 3, commandBuffer.indexOf(';', pos2)).toFloat();

                    // Update erpm of the drivers
                    Serial.print("Received setpoints - Driver 1: ");
                    Serial.print(erpm1);
                    Serial.print(", Driver 2: ");
                    Serial.println(erpm2);
                    driver1.vesc_set_erpm(erpm1);
                    driver2.vesc_set_erpm(erpm2);
                } else {
                    Serial.println("Invalid command format.");
                }
            }
            commandBuffer = "";
        }
    }

    // Print real-time data every second
    if (millis() - last_print_data > 1000) {
        for (int i = 0; i < numdrivers; i++) {
            Serial.print("driver ");
            Serial.print(i + 1);  // Print driver number (1 or 2)
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
