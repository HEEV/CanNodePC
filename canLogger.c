/**
 * Print CAN data from Supermileage cars in a more readable format
 * 
 * \author Sam Ellicott
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "can.h"
#include "CanNode.h"

void nodeHandler(CanMessage *msg);
void timeHandler(CanMessage *msg);
void countHandler(CanMessage *msg);

int main(int nargs, char **args) {

  CanNode *node = CanNode_init(MEGASQUIRT, nodeHandler);
  CanNode_addFilter(node, WHEEL_TIME, timeHandler);
  CanNode_addFilter(node, WHEEL_TACH, countHandler);
  CanMessage msg;

  char buff[256];

  while (1) {
    //for (int i = 0; i < 5; i++) {
    CanNode_checkForMessages();
    usleep(500);
    //}
    //CanNode_requestName(PITOT, buff, 255, 500);
    //printf("%s\n", buff);
    //usleep(50000);
    //CanNode_requestInfo(PITOT, buff, 255, 500);
    //printf("%s\n", buff);
  }

  return 0;
}

void nodeHandler(CanMessage *msg) {}

void timeHandler(CanMessage *msg) {
  uint32_t data;
  CanNode_getData_uint32(msg, &data);
  printf("id: %x, data: %d\n", msg->id, data);
}

void countHandler(CanMessage *msg) {
  uint8_t data;
  CanNode_getData_uint8(msg, &data);
  printf("id: %x, data: %d\n", msg->id, data);
}
