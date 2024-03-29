
/** 
 * \file CanNode.h
 * \brief Interface for using CanNode devices.
 * 
 * CanNode.h provides high-level functions for protocol implementation.
 * Driver level functions are in can.h. This compilation unit has a goal to be 
 * easily portable to a PC application. 
 *
 * \author Samuel Ellicott
 * \date 6-20-16
 */

#ifndef _CAN_NODE_H_
#define _CAN_NODE_H_

#include "CanTypes.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


/**
 * \typedef filterHandler
 * \brief Function pointer to a function that accepts a CanMessage pointer
 *
 * A function of this type should look like
 *
 * <code> void foo(CanMessage* msg) </code>
 *
 * Functions of this type can be used to handle filter matches. These functions
 * are added set to handle filters with the CanNode_addFilter() function. They
 * are
 * called by the CanNode_checkForMessages() function
 *
 * \see CanNode_addFilter
 * \see CanNode_checkForMessages
 */
typedef void (*filterHandler)(CanMessage *data);

/** \addtogroup CanNode_Module CanNode
 * \brief Library to provide a higher level protocol for CAN communication.
 * Specifically for stm32 microcontrollers
 *
 * A CanNode should be initilized with CanNode_init() this will return a pointer
 *to
 * an initilized node to be used with the other functions. Callbacks to occur on
 * messages from a particular id can be added to a node with the
 *CanNode_addFilter()
 * function. To translate [CanMessages](\ref CanMessage) to useful data, use the
 * getData functions. Data can be sent from a node with the sendData functions.
 *
 * ### Initilizing a node ###
 *
 * In order to send data and use filter callbacks you need to initilize a node.
 * to do this call CanNode_init(), passing in a CanNodeType (acts like an id), a
 * filterHandler for handling RTR (Retrun Transmission Request) callbacks for
 * that node, this returns a CanNode pointer to
 * a newly initilized CanNode struct. You can initilize up to \ref MAX_NODES
 * nodes in this way.
 *
 * Example code
 * ~~~~~~~~~~~~ {.c}
 *
 * CanNode* newNode;
 * void pitotRTR(CanMessage* msg);
 *
 * void main(){
 *	newNode = CanNode_init(PITOT, pitotRTR);
 *	//other stuff here
 *	//...
 * }
 *
 * void pitotRTR(CanMessage* msg){
 *	//continue to do what needs to be done.
 *	uint16_t data = getSensorData();
 *	//call one of the \ref sendData functions to return the data
 *	CanNode_sendData_uint16(newNode, data);
 * }
 * ~~~~~~~~~~~~
 *
 * Another often useful thing to do is add filters
 *
 * ### Adding filters ###
 *
 * This is how to add a filter for a single id
 *
 * Example code
 *
 * ~~~~~~~~~~~~ {.c}
 * CanNode* node;
 * const uint16_t filterId = 1200;
 * //initilize node
 * //...
 * CanNode_addFilter(node, filterId, handler);
 * ~~~~~~~~~~~~
 *
 * This is how a filter mask is added (data from multiple ids).
 *
 * Example code
 *
 * ~~~~~~~~~~~~ {.c}
 * CanNode* node;
 * //initilize node
 * //...
 * uint16_t id = can_add_filter_mask(id_to_filter, id_mask);
 * CanNode_addFilter(node, id, handler);
 * ~~~~~~~~~~~~
 *@{
 */

/**
 * \defgroup CanNode_Functions CanNode Public Functions
 *
 * \brief Functions for interfacing with the CanNode struct, reciving and sending
 * CAN messages.
 *
 *@{
 */

class CanNode {


private:
  static bool newMessage;
  static CanMessage tmpMsg;
  static const unsigned int UNUSED_FILTER = 0xFFFF;
  static CanNode *nodes[MAX_NODES];

  uint16_t id;                   ///< id of the node
  uint8_t status;                ///< status of the node (not currently used)
  uint16_t filters[NUM_FILTERS]; ///< array of id's to handle
  filterHandler rtrHandle;       ///< function to handle rtr requests for
                                 /// the node

  filterHandler handle[NUM_FILTERS]; ///< array of function pointers to call
                                     ///< when a id in filters is found
  CanNodeType sensorType;            ///< Type of sensor
  const char *nameStr;               ///< points to the name of the node
  const char *infoStr;               ///< points to the info string for the node

public:
  /// \brief Initilize a CanNode from given parameters.
  CanNode(CanNodeType id, filterHandler rtrHandle);
  /// \brief Add a filter and handler to a given CanNode.
  bool addFilter(uint16_t filter, filterHandler handle);
  /// \brief Check all initilized CanNodes for messages and call callbacks.
  static void checkForMessages();

  /**
   * \anchor sendData
   * \name sendData Functions
   * These functions that send data over the CANBus and support various integer
   * types of data. They are non-blocking.
   * @{
   */
  /// \brief Send a signed 8-bit integer.
  void sendData(int8_t data) const;
  /// \brief Send an unsigned 8-bit integer.
  void sendData(uint8_t data) const;
  /// \brief Send a signed 16-bit integer.
  void sendData(int16_t data) const;
  /// \brief Send an unsigned 16-bit integer.
  void sendData(uint16_t data) const;
  /// \brief Send a signed 32-bit integer.
  void sendData(int32_t data) const;
  /// \brief Send an unsigned 32-bit integer.
  void sendData(uint32_t data) const;

  /// \brief Send an array of uinsigned 8-bit integers.
  CanState sendData(int8_t *data, uint8_t len) const;
  /// \brief Send an array of signed 8-bit integers.
  CanState sendData(uint8_t *data, uint8_t len) const;
  /// \brief Send an array of uinsigned 16-bit integers.
  CanState sendData(int16_t *data, uint8_t len) const;
  /// \brief Send an array of signed 16-bit integers.
  CanState sendData(uint16_t *data, uint8_t len) const;
  //@}

  /**
   * \anchor getData
   * \name getData Functions
   * These functions get data of various integer types from a CanMessage. They
   * are
   * non-blocking. If the data is not of the same type as the called function
   * \ref INVALID_TYPE is returned.
   *
   * These functions are useful for data parsing in a handler function of the
   * type
   * passed to CanNode_addFilter() where a CanMessage pointer is passed to the
   * handler as input.
   * @{
   */
  /// \brief Get a signed 8-bit integer from a CanMessage.
  static CanState getData(const CanMessage *msg, int8_t *data);
  /// \brief Get an unsigned 8-bit integer from a CanMessage.
  static CanState getData(const CanMessage *msg, uint8_t *data);
  /// \brief Get a signed 16-bit integer from a CanMessage.
  static CanState getData(const CanMessage *msg, int16_t *data);
  /// \brief Get an unsigned 16-bit integer from a CanMessage.
  static CanState getData(const CanMessage *msg, uint16_t *data);
  /// \brief Get a signed 32-bit integer from a CanMessage.
  static CanState getData(const CanMessage *msg, int32_t *data);
  /// \brief Get an unsigned 32-bit integer from a CanMessage.
  static CanState getData(const CanMessage *msg, uint32_t *data);

  /// \brief Get an array of signed 8-bit integers from a CanMessage.
  static CanState getData(const CanMessage *msg, int8_t data[7], uint8_t *len);
  /// \brief Get an array of unsigned 8-bit integers from a CanMessage.
  static CanState getData(const CanMessage *msg, uint8_t data[7], uint8_t *len);
  /// \brief Get an array of signed 16-bit integers from a CanMessage.
  static CanState getData(const CanMessage *msg, int16_t data[2], uint8_t *len);
  /// \brief Get an array of unsigned 16-bit integers from a CanMessage.
  static CanState getData(const CanMessage *msg, uint16_t data[2], uint8_t *len);
  //@}

  /**
   * \anchor infoFunctions
   * \name Info Functions
   * These functions handle names for the \ref CanNode_Module library. They
   * allow
   * for providing a name and descriptive text for a node and requesting the
   * same
   * information from another node.
   * @{
   */
  /// \brief Set the name string
  void setName(const char *name);
  /// \brief Set the info string
  void setInfo(const char *info);
  /// \brief request the name string from another CanNode
  static void requestName(CanNodeType id, char *buff, uint8_t len,
                          uint16_t timeout);
  /// \brief request the info string from another CanNode
  static void requestInfo(CanNodeType id, char *buff, uint8_t len,
                          uint16_t timeout);

  //@}

  /*@}*/

  /*@}*/

  /// \brief Add a filter to the can hardware with an id
  static uint16_t can_add_filter_id(uint16_t id);
  /// \brief Add a filter to the can hardware with a mask
  static uint16_t can_add_filter_mask(uint16_t id, uint16_t mask);

  /// \brief Send a CanMessage over the bus.
  static CanState can_tx(CanMessage *tx_msg, uint32_t timeout);
  /// \brief Get a CanMessage from the hardware if it is availible.
  static CanState can_rx(CanMessage *rx_msg, uint32_t timeout);
  /// \brief Check if a new message is avalible.
  static bool is_can_msg_pending();

private:
  // private functions to handle CanNode name functions

  /**
   * private function to send a node's name string on the CAN bus
   *
   * \param[in] node CanNode whose information should be sent
   */
  void sendName();

  /**
   * private function to send a node's info string on the CAN bus
   *
   * \param[in] node CanNode whose information should be sent
   */
  void sendInfo();

  /// \brief Get a string
  static void getString(uint16_t id, char *buff, uint8_t len, uint8_t timeout);
  /// \brief Send a string
  static void sendString(uint16_t id, const char *str);

  /// \brief Initilize CAN hardware.
  static void can_init(void);
  /// \brief Enable CAN hardware.
  static void can_enable(void);
  /// \brief Put CAN hardware to sleep.
  static void can_sleep(void);
  /// \brief Set the speed of the CANBus.
  static void can_set_bitrate(canBitrate bitrate);
};
#endif //_CAN_NODE_H_
