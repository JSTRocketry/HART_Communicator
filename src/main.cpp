/*
TODO: add a parsing file that defines the syntax, ie "@{PA:<float>;TS:<long>}@ TYPE:PRESSURE_ALTITDUE

*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <vector>
#include "OpenGlGraph.h"
using namespace std;

fstream file;

char title[] = "Rocket Altitude Visual";

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

vector <vector <string> > dataSyntax;

struct DataTypeSyntax{
  int numDataEntries;
  float *dataMembers;
  string *dataSyntax;
  RocketDataType type;
};

vector<DataTypeSyntax> dataLinesToLookFor;

void addDataLineToLookFor(RocketDataType type, int numEntries, string *syntax){
  DataTypeSyntax temp;
  temp.type = type;
  temp.numDataEntries = numEntries;
  temp.dataMembers = new float[numEntries];
  temp.dataSyntax = new string[numEntries];
  for(int i = 0; i < numEntries; i ++){
    temp.dataSyntax[i] = syntax[i];
  }
  dataLinesToLookFor.push_back(temp);
}

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


int parseLineGeneral(string line){
  //determine which data member it is
  //printf("Line %s\n",line.c_str());
  RocketData newMember;
  int dataTypeIndex = -1;
  for(int i = 0; i < (int)dataLinesToLookFor.size(); i ++){
    //printf("index i %d\n",i);
    if(line.find(dataLinesToLookFor[i].dataSyntax[0]) >= 0 && line.find(dataLinesToLookFor[i].dataSyntax[0]) < line.length()){
      dataTypeIndex = i;
      i = dataLinesToLookFor.size();
    }
  }
  if(dataTypeIndex == -1){
    return -1;
  }
  newMember.dataType = dataLinesToLookFor[dataTypeIndex].type;
  //printf("Index: %d",dataTypeIndex);
  int numToStore = dataLinesToLookFor[dataTypeIndex].numDataEntries;
  //printf("Num To Store %d\n",numToStore);
  int starts[numToStore];
  for(int i = 0; i < numToStore; i ++){
    starts[i] = line.find(dataLinesToLookFor[dataTypeIndex].dataSyntax[i]) + dataLinesToLookFor[dataTypeIndex].dataSyntax[i].length();
    if(starts[i] < 0 || starts[i] > (int)line.length()){
      printf("Failed to find %s in data line %s", dataLinesToLookFor[i].dataSyntax[i].c_str(), line.c_str());
      return -1;
    }
  }
  //int startLine = line.find("@{");
  int endLine = line.find("}@");
  string temps[numToStore];
  for(int i = 0; i < numToStore - 1; i ++){
    temps[i] = line.substr(starts[i], starts[i + 1] - dataLinesToLookFor[i].dataSyntax[i+1].length() - starts[i] - 1);
  }
  //printf("Last: %d total len %d", starts[numToStore], endLine);
  temps[numToStore -1] = line.substr(starts[numToStore -1], endLine - starts[numToStore - 1] - 1);
  //printf("Parsed: ");
  for(int i = 0; i < numToStore - 1; i ++){
    newMember.values.push_back(atof(temps[i].c_str()));
    //printf("%s\t", temps[i].c_str());
  }
  newMember.timeStamp = atof(temps[numToStore-1].c_str());
  flightData.push_back(newMember);
  return 0;
}

int parseLineGeneral(string line, float *toStore, int numToStore, string *checks){
  int starts[numToStore];
  for(int i = 0; i < numToStore; i ++){
    starts[i] = line.find(checks[i]) + checks[i].length();
    if(starts[i] < 0 || starts[i] > (int)line.length()){
      printf("Failed to find %s in data line %s", checks[i].c_str(), line.c_str());
      return -1;
    }
  }
  //int startLine = line.find("@{");
  int endLine = line.find("}@");
  string temps[numToStore];
  for(int i = 0; i < numToStore - 1; i ++){
    temps[i] = line.substr(starts[i], starts[i + 1] - checks[i+1].length() - starts[i] - 1);
  }
  //printf("Last: %d total len %d", starts[numToStore], endLine);
  temps[numToStore -1] = line.substr(starts[numToStore -1], endLine - starts[numToStore - 1] - 1);
  //printf("Parsed: ");
  for(int i = 0; i < numToStore; i ++){
    toStore[i] = atof(temps[i].c_str());
    //printf("%s\t", temps[i].c_str());
  }
  return 0;
}

int parseLine(string line, RocketData *data){
    //parse the data and return a RocketData struct
    //if the data is not legit, return a null
    return parseLineGeneral(line);
}


RocketData findMaxAltitude(){
  int index = -1;
  for(int i = 0; i < (int)flightData.size(); i++){
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



void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}



OpenGlGraph graph1;

void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0f, 0.0f, -.5f);
   //printf("Showing Graph!");
   graph1.draw();
   //printf("Done Showing Graph!");
   glutSwapBuffers();
}

string getSegment(string target, string startString, string endString){
  int start = target.find(startString) + startString.length();
  int end = target.find(endString, start + 1);
  return target.substr(start, end - start);
}


/*
void timer(int value) {
   glutPostRedisplay();
   glutTimerFunc(refreshMills, timer, 0);
}
*/
void reshape(GLsizei width, GLsizei height) {
   if (height == 0) height = 1;
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
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
    string pressureSyntax[] = {"PA:","TS:"};
    addDataLineToLookFor(PRESSURE_HEIGHT,2, &pressureSyntax[0]);
    string orientationSyntax[] = {"OX:","OY:","OZ:","TS:"};
    addDataLineToLookFor(ORIENTATION,4, &orientationSyntax[0]);
    string accelSyntax[] = {"AX:","AY:","AZ:","TS:"};
    addDataLineToLookFor(ACCEL,4, &accelSyntax[0]);
    //printf("Num Entries %d Syntax %s %s\n",dataLinesToLookFor[0].numDataEntries,dataLinesToLookFor[0].dataSyntax[0].c_str(),dataLinesToLookFor[0].dataSyntax[1].c_str());
    string currentLine;
    RocketData currentData;
    while(file >> currentLine){
        if(parseLine(currentLine, &currentData) >= 0){
          flightData.push_back(currentData);
          currentData = RocketData();
        }
        else {
          printf("Unrecognized line: %s\n", currentLine.c_str());
        }
    }
     file.close();
    printf("Parsing complete!\n");
    printf("Generating Report......\n");
    //float maxAltitude =
    RocketData maxAltitude = findMaxAltitude();
    if(maxAltitude.dataType != PRESSURE_HEIGHT){
      printf("No Pressure Height data found!");
    }
    else printf("Max Altitude %.2f at time %ld\n",maxAltitude.values[0], maxAltitude.timeStamp);
    OpenGlGraphSettings settings;
    settings.graphTitle = "Altitude";
    settings.xAxisTag = "Time (sec)";
    settings.yAxisTag = "Altitude (m)";
    printf("Settings init");
    graph1 = OpenGlGraph(settings);
    for(int i = 0; i < (int)flightData.size(); i ++){
      if(flightData[i].dataType == PRESSURE_HEIGHT){
        if(flightData[i].values[0] > -100){
          Point2D point;
          point.y = flightData[i].values[0];
          //printf("Height: %f",point.y);
          point.x = flightData[i].timeStamp/1000.0;
          graph1.addDataPoint(point);
        }
      }
    }
     printf("Graph setup done");
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_DOUBLE);
     glutInitWindowSize(640, 480);
     glutInitWindowPosition(50, 50);
     glutCreateWindow(title);
     glutDisplayFunc(display);
     glutReshapeFunc(reshape);
     initGL();
     //glutTimerFunc(0, timer, 0);
     glutMainLoop();
    return 0;
}
