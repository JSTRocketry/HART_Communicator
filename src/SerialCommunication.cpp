#include "SerialCommunication.h"

void SerialCommunication::setPossibleLocations(string p[], int num){
  for(int i = 0; i < num; i ++) ports[i] = p[i];
  numPossiblePorts = num;
}

int SerialCommunication::begin(int baud){
  int i = 0;
  while((!port | (port < 0)) && i < numPossiblePorts){
    port = open( ports[i].c_str(), O_RDWR| O_NOCTTY );
    if(port < 0) cout << "Port " << ports[i] << " failed" << endl;
    else cout << "Port " << ports[i] << " conection success!" << endl;
    i ++;
  }
  if(port != -1){
    speed_t rate = B9600;
    switch(baud){
      case(115200):
        rate = B115200;
        break;
    }
    struct termios tty;
    struct termios tty_old;
    memset (&tty, 0, sizeof tty);
    if ( tcgetattr ( port, &tty ) != 0 ) {
       std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
    }
    tty_old = tty;
    cfsetospeed (&tty, rate);
    cfsetispeed (&tty, rate);
    tty.c_cflag     &=  ~PARENB;
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;
    tty.c_cflag     &=  ~CRTSCTS;
    tty.c_cc[VMIN]   =  1;
    tty.c_cc[VTIME]  =  5;
    tty.c_cflag     |=  CREAD | CLOCAL;
    cfmakeraw(&tty);
    tcflush( port, TCIFLUSH );
    if ( tcsetattr ( port, TCSANOW, &tty ) != 0) {
       std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    }
  }
  return port;
}

int SerialCommunication::serialWrite(char *a){
    int n = write(port,a,1);
    return n;
}
int SerialCommunication::serialRead(char *a){
  int n = read(port,&a,1);
  return n;
}

int SerialCommunication::serialReadString(string *str){
  int n = 0,
    spot = 0;
  char buf = '\0';
  do {
      n = read( port, &buf, 1 );
      //cout << n << endl;
      *str += buf;
      spot += n;
  } while( buf != '\r' && n > 0);
  return n;
}

int SerialCommunication::serialWriteString(string *str){
    int n = 0;
    for(unsigned int i = 0; i < strlen(str->c_str());i ++){
      n = write(port, &str[i],1);
    }
    return n;
}

int SerialCommunication::serialWriteString(string str){
    int n = 0;
    for(unsigned int i = 0; i < strlen(str.c_str());i ++){
      n = write(port, &str[i],1);
    }
    return n;
}
