/**
 * Print CAN data from Supermileage cars in a more readable format
 * 
 * \author Sam Ellicott
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include "CanNode/CanNode.h"

void nodeHandler(CanMessage *msg);

int main(int nargs, char **args) {

  CanNode motor_dir((CanNodeType) 1006, nodeHandler);
  CanNode cmode((CanNodeType) 1007, nodeHandler);
  CanNode max_current((CanNodeType) 1008, nodeHandler);
  CanNode max_velocity((CanNodeType) 1009, nodeHandler);
  CanNode max_acceleration((CanNodeType) 1010, nodeHandler);
  CanNode motor_en((CanNodeType) 1011, nodeHandler);

  CanNode throttle((CanNodeType) 900, nodeHandler);
  CanMessage msg;

  uint32_t min_throttle = 100;
  uint32_t max_throttle = 1500;
  int64_t throttle_val = 00000;
  int add_sub = 20;

  max_current.sendData((uint32_t) 60000);

  while(1){
    // motor_dir.sendData((uint8_t) 1);
    // cmode.sendData((uint8_t) 0);

    // max_current.sendData((uint16_t) 3000);
    // max_velocity.sendData((uint32_t) 8000);
    // max_acceleration.sendData((uint32_t) 5000);


    // usleep(1000000);
    // motor_dir.sendData((uint8_t) 0);
    // cmode.sendData((uint8_t) 0);

    // max_current.sendData((uint16_t) 6000);
    // max_velocity.sendData((uint32_t) 10000);
    // max_acceleration.sendData((uint32_t) 3000);
    // usleep(1000000);

    //uint32_t real_throttle = (uint32_t) throttle_val;
    throttle.sendData((uint32_t) throttle_val);
    motor_en.sendData((uint8_t) 1);
    //printf("%ld\n", real_throttle);
    //add_sub = (throttle_val >= max_throttle || throttle_val <= min_throttle) ? -add_sub : add_sub;
    //throttle_val += add_sub;
    usleep(100000);

  }

  return 0;
}

void nodeHandler(CanMessage *msg) {}
