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

void rpmHandler(CanMessage *msg);
void currentHandler(CanMessage *msg);

int main(int nargs, char **args) {

  CanNode node(MEGASQUIRT, nodeHandler);
  node.addFilter(WHEEL_TIME, timeHandler);
  node.addFilter(WHEEL_TACH, rpmHandler);
  node.addFilter(PITOT, currentHandler);
  int time_diff;
  CanMessage msg;


  while (1) {
    CanNode::checkForMessages();
    usleep(500);
  }

  return 0;
}

void nodeHandler(CanMessage *msg) {}

void timeHandler(CanMessage *msg) {
  uint32_t data;
  CanState ret = CanNode::getData(msg, &data);
  printf("id: %x, data: %4d ret: %d\n", msg->id, data, (int) ret);
}

void countHandler(CanMessage *msg) {
  uint16_t data;
  CanNode::getData(msg, &data);
  printf("id: %x, data: %d\n", msg->id, data);
}

void rpmHandler(CanMessage *msg) {
  int32_t data;
  CanNode::getData(msg, &data);
  printf("id: %d, data: %d\n", msg->id, data);
}

void currentHandler(CanMessage *msg) {
  uint16_t data;
  CanNode::getData(msg, &data);
  printf("id: %d, data: %d\n", msg->id, data);
}
