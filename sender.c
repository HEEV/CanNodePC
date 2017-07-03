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

int main(int nargs, char **args) {

  CanNode *pitot = CanNode_init(PITOT, nodeHandler);
  CanNode *throt = CanNode_init(THROTTLE, nodeHandler);
  CanMessage msg;

  const char *pitot_info = "Pitot Tube for Sting (dummy)";
  const char *pitot_name = "Pitot";
  CanNode_setName(pitot, pitot_name);
  CanNode_setInfo(pitot, pitot_info);

  while(1){
    CanNode_checkForMessages();
    for(int i = 0; i < 2048; i += 16) {
      CanNode_sendData_uint32(throt, i);
      HAL_Delay(10);
    }
    //CanNode_sendData_uint16(throt, 5);
    //usleep(250000);
  }

  return 0;
}

void nodeHandler(CanMessage *msg) {}
