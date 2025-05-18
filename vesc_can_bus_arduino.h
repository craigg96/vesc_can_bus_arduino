#ifndef VESC_CAN_BUS_ARDUINO_H
#define VESC_CAN_BUS_ARDUINO_H

#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>

class VescCAN {
private:
    MCP_CAN& CAN0;
    uint8_t deviceId;
    long unsigned int rxId;
    unsigned char len;
    unsigned char rxBuf[8];

public:
    float dutyCycleNow;
    float avgMotorCurrent;
    float erpm;
    float WattHours;
    float tempFET;
    float tempMotor;
    float avgInputCurrent;
    float inpVoltage;
    void processMessage(unsigned long id, unsigned char* data);

    VescCAN(MCP_CAN& can, uint8_t devId);

    void spin();
    void vesc_set_duty(float duty);
    void vesc_set_current(float current);
    void vesc_set_erpm(float eerpm);

private:
    void get_frame();
    float process_data_frame_vesc(char datatype, unsigned char byte1, unsigned char byte2);
    int hex2int(char buf[]);
};

#endif
