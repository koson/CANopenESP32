/**
 * @file dunker.h
 * @author Alexander Miller (alexander.miller@fh-dortmund.de)
 * @brief Dunker Motoren CANOpen
 * @version 0.1
 * @date 2020-04-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef DUNKER_H_
#define DUNKER_H_

#include <stdint.h>
#include "CANopen.h"
#include "CO_OD.h"

class Dunker
{
private:
    /*Constants*/
    static constexpr uint32_t STAT_Enabled = (1 << 0);
    static constexpr uint32_t STAT_Error = (1 << 1);
    static constexpr uint32_t STAT_Warning = (1 << 2);
    static constexpr uint32_t STAT_Moving = (1 << 3);
    static constexpr uint32_t STAT_Reached = (1 << 4);
    static constexpr uint32_t STAT_Limit = (1 << 5);
    static constexpr uint32_t STAT_FollowingError = (1 << 6);
    static constexpr uint32_t STAT_HomingDone = (1 << 7);
    static constexpr uint32_t STAT_Toggle = (1 << 8);
    static constexpr uint32_t STAT_CommandToggle = (1 << 9);
    static constexpr uint32_t STAT_CommandError = (1 << 10);
    static constexpr uint32_t STAT_StopOrHalt = (1 << 11);
    static constexpr uint32_t STAT_LimitCurrent = (1 << 12);
    static constexpr uint32_t STAT_LimitVel = (1 << 13);
    static constexpr uint32_t STAT_LimitPos = (1 << 14);
    static constexpr uint32_t STAT_LimitPWM = (1 << 15);
    static constexpr uint32_t STAT_LimitSetpointVq = (1 << 16);
    static constexpr uint32_t STAT_LimitSetpointVd = (1 << 17);
    static constexpr uint32_t STAT_ComOperational = (1 << 18);
    static constexpr uint32_t STAT_ComStarted = (1 << 19);
    static constexpr uint32_t STAT_OverTemperature = (1 << 20);
    static constexpr uint32_t STAT_MotorOvervoltage = (1 << 21);
    static constexpr uint32_t STAT_MotorUndervoltage = (1 << 22);
    static constexpr uint32_t STAT_Blockage = (1 << 23);
    static constexpr uint32_t STAT_ParamCmdExec = (1 << 24);
    static constexpr uint32_t STAT_BallastCircuit = (1 << 25);
    static constexpr uint32_t STAT_Direction = (1 << 26);
    static constexpr uint32_t STAT_Overload = (1 << 27);

    static constexpr int16_t OPERATION_MODE = 0x03; //Operation Mode 2 "Special Profile Velocity"

    static constexpr uint8_t MAX_MOTORS = 1; //Maximum number of motors

    /**
     * @brief Struct for all nessesary registers in the object-dictionary
     * 
     */
    struct motorRegister
    {
        uint8_t *command;
        int16_t *error;
        uint32_t *status;
        uint8_t *mode;
        uint8_t *power;
        int32_t *velocity;
    };

    static motorRegister motor[MAX_MOTORS];

public:
    CO_t *_CO;

    /*Functions*/

    /**
     * @brief Construct a new Dunker object
     * 
     */
    Dunker();

    /**
     * @brief Initialize motor by setting the RPDO- and TPDO-Mapping
     * 
     * @param CO Pointer to CANopen object
     * @return int8_t 0 = No Error, -n = Errorcode of mapRPDO + mapTPDO
     */
    int8_t init(CO_t *CO);

    /**
     * @brief Change motor status to "Operation enabled" or "Switch on disabled"
     * 
     * @param value 1 = "Operation enabled" ; 0 = "Switch on disabled"
     * @return int8_t 0 = No Error, -1 = Motor in "FAULT"
     */
    int8_t setEnable(uint8_t value);

    /**
     * @brief Set the motor velocity
     * 
     * @param speed Motor velocity
     * @return int8_t 0 = No Error, -1 = Motor in "FAULT"
     */
    int8_t setSpeed(int32_t speed);

    /**
     * @brief Process pending SDO-Download
     * 
     * @return int8_t 0 = No Error, -n = Errorcode of "CO_SDOclientDownload()"
     */
    int8_t coProcessDownloadSDO();

    /**
     * @brief Process pending SDO-Upload
     * 
     * @return int8_t 0 = No Error, -n = Errorcode of "CO_SDOclientUpload()"
     */
    int8_t coProcessUploadSDO();

    /**
     * @brief Changes the PDO-Mapping of given RPDO
     * 
     * @param pdoNumber PDO to change (0-n)
     * @param nodeId Node ID
     * @param mappedObjects Array with objects to be mapped 
     * @param numMappedObjects Nummber of mapped objects
     * @return int8_t 0 = No Error, -n = Errorcode of "coProcessDownloadSDO()"
     */
    int8_t mapRPDO(uint8_t pdoNumber, uint8_t nodeId, uint32_t *mappedObjects, uint8_t numMappedObjects);

    /**
     * @brief Changes the PDO-Mapping of given TPDO
     * 
     * @param pdoNumber PDO to change (0-n)
     * @param nodeId Node ID
     * @param mappedObjects Array with objects to be mapped 
     * @param numMappedObjects Nummber of mapped objects
     * @param eventTime TPDO event time in 100us steps
     * @param inhibitTime TPDO inhibit time in 100us steps
     * @return int8_t 0 = No Error, -n = Errorcode of "coProcessDownloadSDO()"
     */
    int8_t mapTPDO(uint8_t pdoNumber, uint8_t nodeId, uint32_t *mappedObjects, uint8_t numMappedObjects, uint16_t eventTime, uint16_t inhibitTime);
};

#endif /* DUNKER_H_ */