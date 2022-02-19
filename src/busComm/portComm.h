#pragma once


#include "../common/common.h"


class portComm{
    private:
        uint16_t portNumber;
        int speedType;

    public:

        portComm(uint16_t portNumber, int speedType) : portNumber(portNumber), speedType(speedType){};

        uint8_t highSpeedRead();

        uint8_t highSpeedWrite(uint8_t data);

        //uint8_t lowSpeedRead();

        //uint8_t lowSpeedWrite(uint8_t data);      
};