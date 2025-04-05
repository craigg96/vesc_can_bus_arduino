#include <mcp_can.h>
#include <SPI.h>
#include "vesc_can_bus_arduino.h"

VescCAN::VescCAN(MCP_CAN& can, uint8_t devId) : CAN0(can), deviceId(devId) {
    dutyCycleNow = 0;
    avgMotorCurrent = 0;
    erpm = 0;
    WattHours = 0;
    tempFET = 0;
    tempMotor = 0;
    avgInputCurrent = 0;
    inpVoltage = 0;
    len = 0;
}

void VescCAN::spin() {
    get_frame();

    uint32_t dutyCycleId = 0x80000900 | deviceId;
    uint32_t wattHoursId = 0x80000F00 | deviceId;
    uint32_t tempId = 0x80001000 | deviceId;
    uint32_t voltageId = 0x80001B00 | deviceId;

    if (rxId == dutyCycleId) {
        dutyCycleNow = process_data_frame_vesc('D', rxBuf[6], rxBuf[7]);
        avgMotorCurrent = process_data_frame_vesc('C', rxBuf[4], rxBuf[5]);
        erpm = ((rxBuf[0] << 24) | (rxBuf[1] << 16) | (rxBuf[2] << 8) | rxBuf[3]) / 30;
    }
    if (rxId == wattHoursId) {
        WattHours = ((rxBuf[0] << 24) | (rxBuf[1] << 16) | (rxBuf[2] << 8) | rxBuf[3]) * 0.01;
    }
    if (rxId == tempId) {
        tempFET = process_data_frame_vesc('F', rxBuf[0], rxBuf[1]);
        tempMotor = process_data_frame_vesc('T', rxBuf[2], rxBuf[3]);
        avgInputCurrent = process_data_frame_vesc('I', rxBuf[4], rxBuf[5]);
        if (tempMotor > 100.0) {
            Serial.println("Advertencia: Temperatura del motor demasiado alta o sensor no conectado.");
        }
    }
    if (rxId == voltageId) {
        char receivedByte[4];
        sprintf(receivedByte, "%02X%02X", rxBuf[4], rxBuf[5]);
        inpVoltage = hex2int(receivedByte) * 0.1;
    }
}

float VescCAN::process_data_frame_vesc(char datatype, unsigned char byte1, unsigned char byte2) {
    char receivedByte[4];
    sprintf(receivedByte, "%02X%02X", byte1, byte2);
    float output = hex2int(receivedByte);

    switch (datatype) {
        case 'D': output *= 0.001; break; // dutyCycleNow
        case 'C': output *= 0.1; break;   // avgMotorCurrent
        case 'F': output *= 0.1; break;   // tempFET
        case 'T': output *= 0.1; break;   // tempMotor
        case 'I': output *= 0.1; break;   // avgInputCurrent
        case 'V': output *= 0.1; break;   // inpVoltage
    }
    return output;
}

int VescCAN::hex2int(char buf[]) {
    return (short)strtol(buf, NULL, 16);
}

void VescCAN::vesc_set_duty(float duty) {
    uint32_t set_value = duty * 100000;
    uint8_t buffer[4];
    buffer[0] = (set_value >> 24) & 0xFF;
    buffer[1] = (set_value >> 16) & 0xFF;
    buffer[2] = (set_value >> 8) & 0xFF;
    buffer[3] = set_value & 0xFF;
    uint32_t canId = 0x00000000 | deviceId;
    CAN0.sendMsgBuf(canId, 1, 4, buffer);
}

void VescCAN::vesc_set_current(float current) {
    uint32_t set_value = current * 1000;
    uint8_t buffer[4];
    buffer[0] = (set_value >> 24) & 0xFF;
    buffer[1] = (set_value >> 16) & 0xFF;
    buffer[2] = (set_value >> 8) & 0xFF;
    buffer[3] = set_value & 0xFF;
    uint32_t canId = 0x00000100 | deviceId;
    CAN0.sendMsgBuf(canId, 1, 4, buffer);
}

void VescCAN::vesc_set_erpm(float eerpm) {
    uint32_t set_value = eerpm * 30;
    uint8_t buffer[4];
    buffer[0] = (set_value >> 24) & 0xFF;
    buffer[1] = (set_value >> 16) & 0xFF;
    buffer[2] = (set_value >> 8) & 0xFF;
    buffer[3] = set_value & 0xFF;
    uint32_t canId = 0x00000300 | deviceId;
    CAN0.sendMsgBuf(canId, 1, 4, buffer);
}

void VescCAN::get_frame() {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);
}
