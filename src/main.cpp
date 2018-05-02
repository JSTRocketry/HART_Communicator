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
  string x2Start = line.substr(aXStart, aYStart - 4 - aXStart);
  string y2Start = line.substr()
}

int parseLine(string line, RocketData *data){
    //parse the data and return a RocketData struct
    //if the data is not legit, return a null

    printf("Parsing New Line: %s\n", line.c_str());
    if(line.find("GX") >= 0 && line.find("GX") < line.size()){
      return parseGyroLine(line, data);
    }
    return -1;
}


RocketData findMaxAltitude(){

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
        }
        else printf("Unrecognized line: %s\n", currentLine.c_str());
    }
     file.close();
    printf("Parsing complete!\n");
    printf("Generating Report......\n");
    //float maxAltitude =

    return 0;
}
