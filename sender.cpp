/**
 * Print CAN data from Supermileage cars in a more readable format
 * 
 * \author Sam Ellicott
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "CanNode/CanNode.h"

void nodeHandler(CanMessage *msg);

int main(int nargs, char **args) {

  CanNode starter_switch_node((CanNodeType) 916, nodeHandler);
  CanNode stop_switch_node((CanNodeType) 912, nodeHandler);
  CanMessage msg;

  while(1){
      /*
    for(uint16_t i = 0; i < 2048; i += 16) {
      CanNode::checkForMessages();
      throt.sendData(i);
      usleep(10000);
      CanNode::checkForMessages();
      wheel.sendData(i);
      usleep(10000);
    }
    */
    starter_switch_node.sendData((uint8_t) 1);
    usleep(250000);
    stop_switch_node.sendData((uint8_t) 1);
    usleep(250000);
  }

  return 0;
}

void nodeHandler(CanMessage *msg) {}
