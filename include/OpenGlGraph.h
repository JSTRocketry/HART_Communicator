#ifndef _OPENGL_GRAPH_H_
#define _OPENGL_GRAPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <string.h>

using namespace std;

struct OpenGlGraphSettings{
  string graphTitle;
  string xAxisTag;
  string yAxisTag;
  float graphOriginX = 0;
  float graphOriginY = 0;
  float graphWidth = .25;
  float graphHeight = .25;
  float dataMinX = 0, dataMaxX = 10;
  float dataMinY = 0, dataMaxY = 10;
  int numHorizontalLines = 5;
  int numVerticalLines = 5;
  float dataPointColor[4] = {1,1,1,1};
  float dataLineColor[4] = {1,1,0,1};
  float graphColor[4] = {1,1,1,5};
};

struct Point2D{
  float x;
  float y;
};


class OpenGlGraph{
public:
  OpenGlGraph(){}
  OpenGlGraph(OpenGlGraphSettings graphSettings);
  //void drawString(float x, float y, string line);
  void draw();
  void setDataRange(float minY, float maxX);
  void setDataDomain(float minX, float maxX);
  int addDataPoint(Point2D);
  vector <Point2D> dataPoints;
  //void
private:
  void drawString(float x, float y, string line);
  void drawStringCenteredAt(float x, float y, string line);
  string truncateFloat(float toTruncate, int numDecimal);
  void drawOutline();
  void drawData(bool connect);

  OpenGlGraphSettings settings;
};


#endif
