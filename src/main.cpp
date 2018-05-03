#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

fstream file;

//enumeration for telling the type of data contained
enum RocketDataType{
    GYRO,
    ACCEL,
    ORIENTATION,
    PRESSURE_HEIGHT,
    VELOCITY,
    DISPLACEMENT,
    MAG
};
//struct for containing line worth of data from the rocket
struct RocketData{
    //time stamp according to the rocket's flight computer
    long timeStamp = 0;
    //type of data
    RocketDataType dataType;
    /*
        vector storing all data stored in the line. This is dependent on what is stored.
        i.e.:
            dataType == GYRO
                data[0] = x, data[1] = y, data[2] = z
            dataType == PRESSURE_HEIGHT
                data[0] = height
    */
    vector <float> values;
};

vector <RocketData> flightData;




int openFile(string fileLoc){
    file.open(fileLoc);
    if(!file) return -1;
    return 0;
}

int parseGyroLine(string line, RocketData *data){
  int gXStart = line.find("GX:") + 3;
  int gYStart = line.find("GY:") + 3;
  int gZStart = line.find("GZ:") + 3;
  int tsStart = line.find("TS:") + 3;
  int end = line.find("}@") + 2;
  string xString = line.substr(gXStart, gYStart - 4 - gXStart);
  string yString = line.substr(gYStart, gZStart - 4 - gYStart);
  string zString = line.substr(gZStart, tsStart - 4 - gZStart);
  string tString = line.substr(tsStart, end - 3 - tsStart);
  //printf("xString:%s\n", xString.c_str());
  float gx = atof(xString.c_str());
  float gy = atof(yString.c_str());
  float gz = atof(zString.c_str());
  long ts = atol(tString.c_str());
  data->values.push_back(gx);
  data->values.push_back(gy);
  data->values.push_back(gz);
  data->timeStamp = ts;
  data->dataType = GYRO;
  return 0;
}
int parseAccelLine(string line, RocketData *data){
  int aXStart = line.find("AX:") + 3;
  int aYStart = line.find("AY:") + 3;
  int aZStart = line.find("AZ:") + 3;
  int ts2 = line.find("TS:") + 3;
  int end2 = line.find("}@") + 2;
  string x2String = line.substr(aXStart, aYStart - 4 - aXStart);
  string y2String = line.substr(aYStart, aZStart - 4- aYStart);
  string z2String = line.substr(aZStart, ts2 - 4- aZStart);
  string t2String = line.substr(ts2, end2 - 3- ts2);
  float ax = atof(x2String.c_str());
  float ay = atof(y2String.c_str());
  float az = atof(x2String.c_str());
  long t_s = atol(t2String.c_str());
  data->values.push_back(ax);
  data->values.push_back(ay);
  data->values.push_back(az);
  data->timeStamp = t_s;
  data->dataType = ACCEL;
  return 0;
}
int parsePresAltLine(string line, RocketData *data){
  int altStart = line.find("PA:") + 3;
  int tsStart = line.find("TS:") + 3;
  int end = line.find("}@") + 2;
  string altString = line.substr(altStart, tsStart - 4 - altStart);
  string tsString = line.substr(tsStart, end - 3 - tsStart);
  float alt = atof(altString.c_str());
  long ts = atol(tsString.c_str());
  data->values.push_back(alt);
  data->timeStamp = ts;
  data->dataType = PRESSURE_HEIGHT;
  return 0;
}

int parseLine(string line, RocketData *data){
    //parse the data and return a RocketData struct
    //if the data is not legit, return a null
    if(line.find("GX") >= 0 && line.find("GX") < line.size()){
      return parseGyroLine(line, data);
    }
    if(line.find("PA") >= 0 && line.find("PA") < line.size()){
      return parsePresAltLine(line, data);
    }
    return -1;
}

int parseOrientLine(string line, RocketData *data){
  int oXStart = line.find("OX:") + 3;
  int oYStart = line.find("OY:") + 3;
  int oZStart = line.find("OZ:") + 3;
  int tsStart = line.find("TS:") + 3;
  int end = line.find("}@") + 2;
  string xString = line.substr(oXStart, oYStart - 4 - oXStart);
  string yString = line.substr(oYStart, oZStart - 4 - oYStart);
  string zString = line.substr(oZStart, tsStart - 4 - oZStart);
  string tString = line.substr(tsStart, end - 3 - tsStart);
  //printf("xString:%s\n", xString.c_str());
  float ox = atof(xString.c_str());
  float oy = atof(yString.c_str());
  float oz = atof(zString.c_str());
  long ts = atol(tString.c_str());
  data->values.push_back(ox);
  data->values.push_back(oy);
  data->values.push_back(oz);
  data->timeStamp = ts;
  data->dataType = ORIENTATION;
  return 0;
}

int parseMagLine(string line, RocketData *data){
  int mXStart = line.find("MX:") + 3;
  int mYStart = line.find("MY:") + 3;
  int mZStart = line.find("MZ:") + 3;
  int tsStart = line.find("TS:") + 3;
  int end = line.find("}@") + 2;
  string xString = line.substr(mXStart, mYStart - 4 - mXStart);
  string yString = line.substr(mYStart, mZStart - 4 - mYStart);
  string zString = line.substr(mZStart, tsStart - 4 - mZStart);
  string tString = line.substr(tsStart, end - 3 - tsStart);
  //printf("xString:%s\n", xString.c_str());
  float mx = atof(xString.c_str());
  float my = atof(yString.c_str());
  float mz = atof(zString.c_str());
  long ts = atol(tString.c_str());
  data->values.push_back(mx);
  data->values.push_back(my);
  data->values.push_back(mz);
  data->timeStamp = ts;
  data->dataType = MAG;
  return 0;
}

RocketData findMaxAltitude(){
  int index = -1;
  for(int i = 0; i < flightData.size(); i++){
    if(flightData[i].dataType == PRESSURE_HEIGHT){
      if(index < 0) index = i; //catch the first PRESSURE_HEIGHT line
      else{
        if(flightData[i].values[0] > flightData[index].values[0]){
          index = i;
        }
      }
    }
  }
  if(index < -1) return flightData[0];
  return flightData[index];
}

RocketData findLaunchTime(){

}

int findBurnSart(){

}

int findBurnEnd(){

}

float findMaxVelocity(){

}



int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Usage: ./parser file");
        return -1;
    }
    if(openFile(string(argv[1])) < 0){
        printf("Failed to open file, check name!");
        return -1;
    }
    printf("File opened! Parsing:\n");
    string currentLine;
    RocketData currentData;
    while(file >> currentLine){
        if(parseLine(currentLine, &currentData) >= 0){
          flightData.push_back(currentData);
          currentData = RocketData();
        }
        else printf("Unrecognized line: %s\n", currentLine.c_str());
    }
     file.close();
    printf("Parsing complete!\n");
    printf("Generating Report......\n");
    //float maxAltitude =
    RocketData maxAltitude = findMaxAltitude();
    if(maxAltitude.dataType != PRESSURE_HEIGHT){
      printf("No Pressure Height data found!");
    }
    else printf("Max Altitude %.2f at time %i\n",maxAltitude.values[0], maxAltitude.timeStamp);

    return 0;
}
