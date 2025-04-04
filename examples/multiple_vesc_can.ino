#include <mcp_can.h>
#include "vesc_can_bus_arduino.h"

MCP_CAN CAN0(10); // CS pin

bool print_realtime_data = true;
long last_print_data;

VescCAN llanta1(CAN0, 0x0A);    // ID 10
VescCAN llanta2(CAN0, 0x0B);    // ID 11
//VescCAN llanta3(CAN0, 0x0C);
//VescCAN llanta4(CAN0, 0x0D);
VescCAN* llantas[] = {&llanta1, &llanta2}; //&llanta3, &llanta4};
const int numLlantas = 2;

void setup()
{

    Serial.begin(115200); // Wired comms from USB port

    CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ); // Set the CAN bus speed and clock frequency
    CAN0.setMode(MCP_NORMAL);
}

void loop() {
    llanta1.spin();
    llanta2.spin();

    llanta1.vesc_set_erpm(30);
    llanta2.vesc_set_erpm(30);

    if (millis() - last_print_data > 1000) {
        for (int i = 0; i < numLlantas; i++) {

            Serial.print("Llanta ");
            Serial.print(i + 1);  // Print wheel number (1 or 2)
            Serial.println(" data:");
            
            Serial.print("rpm: ");
            Serial.println(llantas[i]->erpm);
            Serial.print("voltage: ");
            Serial.println(llantas[i]->inpVoltage);
            Serial.print("dutyCycleNow: ");
            Serial.println(llantas[i]->dutyCycleNow);
            Serial.print("avgInputCurrent: ");
            Serial.println(llantas[i]->avgInputCurrent);
            Serial.print("avgMotorCurrent: ");
            Serial.println(llantas[i]->avgMotorCurrent);
            Serial.print("tempFET: ");
            Serial.println(llantas[i]->tempFET);
            Serial.print("tempMotor: ");
            Serial.println(llantas[i]->tempMotor);
            Serial.print("WattHours: ");
            Serial.println(llantas[i]->WattHours);
            Serial.println("..............................");
        }
        last_print_data = millis();
        Serial.println();  // Extra newline for separation between cycles
    }
}
