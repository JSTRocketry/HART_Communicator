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
    vector <float> data;
};

vector <RocketData> flightData;




int openFile(string fileLoc){
    file.open(fileLoc);
    if(!file) return -1;
}

int parseLine(string line, RocketData *data){
    //parse the data and return a RocketData struct
    //if the data is not legit, return a null
    printf("Parsing New Line: %s\n", line.c_str());
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
    }
    file.close();
    printf("Parsing complete!\n");
    printf("Generating Report......\n");
    //float maxAltitude =

    return 0;
}