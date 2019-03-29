/**
 * Print CAN data from Supermileage cars in a more readable format
 * 
 * \author Sam Ellicott
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <unistd.h>
#include <cstdint>

#include "CanNode/CanNode.h"
#include "CanNode/can.h"

using namespace std::chrono;
using std::int64_t;

void nodeHandler(CanMessage *msg);
void timeHandler(CanMessage *msg);
void countHandler(CanMessage *msg);

int main(int nargs, char **args) {

  CanNode node(MEGASQUIRT, nodeHandler);
  node.addFilter(WHEEL_TIME, timeHandler);
  node.addFilter(WHEEL_TACH, countHandler);
  int time_diff;
  CanMessage msg;

  char buff[256];

   int64_t prev = duration_cast<milliseconds>(
           system_clock::now().time_since_epoch()
           ).count();
  //CanNode::requestName(PITOT, buff, 255, 500);
  // printf("%s\n", buff);
  while (1) {
    //for (int i = 0; i < 5; i++) {
    //CanNode::checkForMessages();
    //usleep(500);
    //check for messages
    if(CanNode::is_can_msg_pending()){
        CanNode::can_rx(&msg, 2);

       if(msg.id == 1512){
           int64_t now = duration_cast<milliseconds>(
                   system_clock::now().time_since_epoch()
                   ).count();

           time_diff = now - prev;
           
           prev = now;

           std::cout << time_diff << std::endl;
       }
    }
    //}
    //CanNode_requestInfo(PITOT, buff, 255, 500);
    //printf("%s\n", buff);
  }

  return 0;
}

void nodeHandler(CanMessage *msg) {}

void timeHandler(CanMessage *msg) {
  uint16_t data;
  CanState ret = CanNode::getData(msg, &data);
  printf("id: %x, data: %4d ret: %d\n", msg->id, data, (int) ret);
}

void countHandler(CanMessage *msg) {
  uint8_t data;
  CanNode::getData(msg, &data);
  printf("id: %x, data: %d\n", msg->id, data);
}
