/*
TODO: add a parsing file that defines the syntax, ie "@{PA:<float>;TS:<long>}@ TYPE:PRESSURE_ALTITDUE

*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <vector>
#include "SerialCommunication.h"
#include "OpenGlGraph.h"
#include <exception>
using namespace std;

fstream file;
ofstream radioFile;
string possiblePorts[] = {"/dev/ttyACM0","/dev/ttyUSB1"};
SerialCommunication rocket;

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
  if(line.find("@{") > line.length() || line.find("@{") < 0){
    return -1;
  }
  if(line.find("}@") > line.length() || line.find("}@") < 0){
    return -1;
  }
  RocketData newMember;
  int dataTypeIndex = -1;
  for(int i = 0; i < (int)dataLinesToLookFor.size(); i ++){
    //printf("index i %d\n",i);
    if(line.find(dataLinesToLookFor[i].dataSyntax[0]) >= 0 && line.find(dataLinesToLookFor[i].dataSyntax[0]) < line.length()){
      dataTypeIndex = i;
      i = dataLinesToLookFor.size();
      printf("Found! %i", dataTypeIndex);
    }
  }
  if(dataTypeIndex == -1){
    return -1;
  }
  newMember.dataType = dataLinesToLookFor[dataTypeIndex].type;
  cout << newMember.dataType << endl;
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

RocketData currentData;
//string currentLine;
void timer(int value) {
   glutPostRedisplay();
   try{
      string cmd;
      //arduino.serialWriteString("Hello!")
      if(rocket.serialReadString(&cmd) && cmd != ""){

        printf("New line from rocket: %s\n",cmd.c_str());
        radioFile << cmd << endl;
        //cout << cmd << endl;
        //printf("%i", cmd.find("PA:"));
        if(parseLine(cmd, &currentData) >= 0){
          currentData = flightData[flightData.size() - 1];
          switch(currentData.dataType){
            case(PRESSURE_HEIGHT):
              printf("Pressure Height Line!");
              break;
            default:
              printf("Not Handled!: %i\n", currentData.dataType);
          }
          if(currentData.dataType == PRESSURE_HEIGHT){
            printf("New Altitude line!:");
            Point2D point;
            point.y = currentData.values[0];
            //printf("Height: %f",point.y);
            point.x = currentData.timeStamp/1000.0;
            //Point2D previous =graph1.dataPoints[graph1.dataPoints.size() - 1];
            //float previousx = ;
            if(graph1.dataPoints.size() > 1){
              cout << "Previous time stamp:  " << graph1.dataPoints[graph1.dataPoints.size() - 1].x << endl;
              if(point.x >= graph1.dataPoints[graph1.dataPoints.size() - 1].x)
              graph1.addDataPoint(point);
            }
            else{
              graph1.addDataPoint(point);
            }
          }
          flightData.push_back(currentData);
          currentData = RocketData();
        }
        else {
          printf("Unrecognized line: %s\n", cmd.c_str());
        }
      }
      else cout << "ERROR!" << endl;
      cmd = "";
    }
    catch(exception &e){
      cout << e.what() << endl;
    }

   glutTimerFunc(5, timer, 0);
}

void reshape(GLsizei width, GLsizei height) {
   if (height == 0) height = 1;
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}


int main(int argc, char* argv[]){
    /*
    if(argc < 2){
        printf("Usage: ./parser file");
        return -1;
    }

    if(openFile(string(argv[1])) < 0){
        printf("Failed to open file, check name!");
        return -1;
    }
    printf("File opened! Parsing:\n");
    */
    radioFile.open("RADIOSTREAM.txt");
    rocket.setPossibleLocations(possiblePorts,sizeof(possiblePorts)/sizeof(possiblePorts[0]));
    if(rocket.begin(115200) > 0){
   }
   else{
     printf("Failed to connect to rocket!");
     return -1;
   }
   string pressureSyntax[] = {"PA:","TS:"};
   addDataLineToLookFor(PRESSURE_HEIGHT,2, &pressureSyntax[0]);
   OpenGlGraphSettings settings;
   settings.graphTitle = "Altitude";
   settings.xAxisTag = "Time (sec)";
   settings.yAxisTag = "Altitude (m)";
   settings.graphWidth = .5;
   settings.graphHeight = .3;
   printf("Settings init");
   graph1 = OpenGlGraph(settings);

    printf("Graph setup done");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow(title);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    initGL();
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
