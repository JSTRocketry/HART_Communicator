#ifndef _SERIAL_COMMUNICATION_H
#define _SERIAL_COMMUNICATION_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

using namespace std;


class SerialCommunication{
public:
  SerialCommunication(){

  }
  void setPossibleLocations(string p[], int num);
  int begin( int baud);
  int serialWrite(char *a);
  int serialRead(char *a);
  int serialReadString(string *str);
  int serialWriteString(string *str);
  int serialWriteString(string str);
private:
  int port = -1;
  int numPossiblePorts = 0;
  string ports[5];
};

#endif
