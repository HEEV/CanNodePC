/**
 * \file CanTypes.h
 * \brief Type definitions for CanNode functions.
 *
 * Defines various structs and enums for storing states of CanNodes.
 * \author Samuel Ellicott
 * \date 7-12-16
 */
#ifndef _CAN_TYPES_H_
#define _CAN_TYPES_H_

#include <stdbool.h>
#include <stdint.h>

#define HAL_Delay(ms_delay) (usleep(ms_delay * 1000))
/**
 * \addtogroup CanNode_Module CanNode
 *@{
 */

#ifndef MAX_NODES
/// Maximum number of nodes stored internally. Can be overwriten by redefinition
#define MAX_NODES 50
#endif

#ifndef NUM_FILTERS
/// Number of filters each node can have. Can be overwriten by redefinition
#define NUM_FILTERS 10
#endif

/// Maximum length of a name string for the CanNode_getName()
#define MAX_NAME_LEN 30
/// Maximum length of a info string for the CanNode_getInfo()
#define MAX_INFO_LEN 90

/**
 * \defgroup CanNode_Data_Types CanNode Public Data Types
 *@{
 */

/**
 * \enum CanNodeType
 * \brief Defines sensor type and base id
 *
 * These are the base types that Ryan Gordon and myself thought of for the
 * supermileage team. These constants should be modified if new sensor types are
 * added. Also, we have only defined the base addresses for each sensor type,
 * Individual sensor nodes must be added separately. Each node takes up four
 * addresses. The description gives the format for each message from the node.
 *
 */
typedef enum {
  MEGASQUIRT = 800,   ///< its own format

  RELAY = 850,        ///< uint8

  SWITCH = 900,       ///< uint8
  THROTTLE = 900,     ///< throttle position (uint16)
  THROT_BODY = 904,   ///< throttle body servo
  TACT = 908,

  PRESSURE = 950,     ///< uint16
  PITOT = 950,        ///< Pitot Tube

  TEMPURATURE = 1000, ///< uint16
  ENGINE_TEMP = 1000, ///< Engine Case Tempurature
  COOL_TEMP = 1004,   ///< Engine Coolent Tempurature

  VOLTAGE = 1050,     ///< uint16
  SYS_V = 1050,       ///< System Voltage

  CURRENT = 1100,    ///< uint16
  SYS_I = 1100,      ///< System Current

  TACHOMETER = 1150, ///< uint8 - revolutions per second
  WHEEL_TACH = 1150, ///< Drive Wheel Tachometer
                     ///< Number of revolutions in the last second
  WHEEL_TIME = 1154, ///< Give the time taken for the last wheel revolution
                     ///< time is in mili-seconds.

  LED = 1100,        ///< array of 4 uint8 RGBA
} CanNodeType;

/**
 * \enum canBitrate
 * \brief Defines various avalible bitrates for the CANBus
 *
 */
typedef enum {
  CAN_BITRATE_10K,   ///< 10k baud
  CAN_BITRATE_20K,   ///< 20k baud
  CAN_BITRATE_50K,   ///< 50k baud
  CAN_BITRATE_100K,  ///< 100k baud
  CAN_BITRATE_125K,  ///< 125k baud
  CAN_BITRATE_250K,  ///< 250k baud
  CAN_BITRATE_500K,  ///< 500k baud
  CAN_BITRATE_750K,  ///< 750k baud
  CAN_BITRATE_1000K, ///< 1M baud
} canBitrate;

/**
 * \enum CanState
 * \brief Defines various errors and states of the CANBus
 *
 */
typedef enum {
  BUS_OK = 0,    ///< Good state
  DATA_OK = 0,   ///< Good state
  DATA_ERROR,    ///< Catch all data error
  NO_DATA,       ///< No data availible on the bus
  INVALID_TYPE,  ///< The bus has data, but the wrong \ref getData function was
                 ///used
  DATA_OVERFLOW, ///< Too much data was was put in the message
  BUS_BUSY,      ///< The bus is working with someone else right now
  BUS_OFF        ///< The bus is off - call can_init() and can_enable()
} CanState;

/**
 * \struct CanMessage
 * \brief Stucture for holding a CANBus message.
 *
 */
typedef struct {                                                                                    
  uint16_t id;     ///< ID of the sender                                                            
  uint8_t len;     ///< Length of the message                                                       
  uint8_t fmi;     ///< Filter mask index (what filter triggered message)                           
  bool rtr;        ///< Asking for data (true) or sending data (false)                              
  uint8_t data[8]; ///< Data                                                                        
} CanMessage;

/**
 * \enum CanNodeDataType
 * \brief CanNode Data Type Enum.
 *
 * Enum that contains various types of data that can be stored in a CanNode
 * message.
 * Since the actual values for this enum are stored in a 3-bit value, there
 * can only be 8 individual types in this enum.
 */
typedef enum {
  CAN_UINT8,     ///< Unsigned 8-bit integer
  CAN_INT8,      ///< Signed 8-bit integer
  CAN_UINT16,    ///< Unsigned 16-bit integer
  CAN_INT16,     ///< Signed 16-bit integer
  CAN_UINT32,    ///< Unsigned 32-bit integer
  CAN_INT32,     ///< Signed 32-bit integer
  CAN_BIT_FIELD, ///< Each bit defines the state of value
  CAN_CUSTOM     ///< Catch all
} CanNodeDataType;

/**
 * \enum CanNodeMsgType
 * \brief CanNode Message Type Enum.
 *
 * Enum for command that can be sent and recieved from a CanNode.
 * Since the actual values for this enum are sent in a 5-bit value, there can
 * only be 32 individual types in this enum.
 */
typedef enum {
  CAN_DATA,         ///< Normal operation, device is sending data to other nodes
  CAN_DATA_MODE,    ///< Sent by master to enter data mode (default mode)
  CAN_CONFIG_MODE,  ///< Sent by master to enter config mode
  CAN_SET_ID,       ///< Sent by master to change the id of the node
  CAN_SET_NAME,     ///< Sent by master to change the name of the node
  CAN_SET_INFO,     ///< Sent by master to change the info string of the node
  CAN_ID_SET_ERROR, ///< Error sent by node if the new id is not availible
  CAN_CONFIG_ERROR, ///< General configuration error
  CAN_GET_NAME,     ///< Ask a node for its name (use CanNode_getName())
  CAN_GET_INFO,     ///< Ask a node for its info (use CanNode_getInfo())
  CAN_NAME_INFO     ///< Message is part of a name/info message
} CanNodeMsgType;

//@}

//@}
#endif //_CAN_TYPES_H_
