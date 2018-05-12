#include "OpenGlGraph.h"

int OpenGlGraph::addDataPoint(Point2D point){
  if(dataPoints.size() == 0){
    settings.dataMaxX = point.x;
    settings.dataMinX = point.x;
    settings.dataMaxY = point.y;
    settings.dataMinY = point.y;
    dataPoints.push_back(point);
    return 0;
  }
  if(point.x < settings.dataMinX) settings.dataMinX = point.x;
  if(point.y < settings.dataMinY) settings.dataMinY = point.y;
  if(point.x > settings.dataMaxX) settings.dataMaxX = point.x;
  if(point.y > settings.dataMaxY) settings.dataMaxY = point.y;
  dataPoints.push_back(point);
  return 0;
}

OpenGlGraph::OpenGlGraph(OpenGlGraphSettings graphSettings){
  settings = graphSettings;
}

void OpenGlGraph::draw(){
  drawOutline();
  drawData(false);
}

void OpenGlGraph::drawOutline(){
  //horizontalLines
  glColor4f(settings.graphColor[0],settings.graphColor[1],settings.graphColor[2],settings.graphColor[3]);
  glBegin(GL_LINE_LOOP);
  float startX = settings.graphOriginX - settings.graphWidth/2.0;
  float startY = settings.graphOriginY - settings.graphHeight/2.0;
  //printf("Start X:%f Y:%f\n",startX,startY);
  glVertex2f(startX, startY);
  glVertex2f(startX + settings.graphWidth, startY);
  glVertex2f(startX + settings.graphWidth, startY + settings.graphHeight);
  glVertex2f(startX, startY + settings.graphHeight);
  glEnd();
  glBegin(GL_LINES);
  for(int i = 1; i < settings.numHorizontalLines; i ++){
    glVertex2f(startX, startY + i*settings.graphHeight/settings.numHorizontalLines);
    glVertex2f(startX + settings.graphWidth, startY + i*settings.graphHeight/settings.numHorizontalLines);
  }
  for(int i = 1; i < settings.numVerticalLines; i ++){
    glVertex2f(startX +  i*settings.graphWidth/settings.numVerticalLines, startY);
    glVertex2f(startX +  i*settings.graphWidth/settings.numVerticalLines, startY + settings.graphHeight);
  }
  glEnd();
  for(int i = 0; i < settings.numHorizontalLines + 1; i ++){
    drawString(startX - .02, startY + (i)*settings.graphHeight/settings.numHorizontalLines,truncateFloat(i * (settings.dataMaxY - settings.dataMinY)/settings.numHorizontalLines + settings.dataMinY,1));
  }
  for(int i = 0; i < settings.numVerticalLines + 1; i ++){
    drawString(startX + i*settings.graphWidth/settings.numVerticalLines, startY - .02 ,truncateFloat(i * (settings.dataMaxX - settings.dataMinX)/settings.numVerticalLines + settings.dataMinX,1));
  }
  drawStringCenteredAt(startX + settings.graphWidth/2.0, startY + settings.graphHeight + .01,settings.graphTitle);
  drawStringCenteredAt(startX + settings.graphWidth/2.0, startY - .035,settings.xAxisTag);
  //glRotatef(90, .0, 1.0, 0);
  //drawStringCenteredAt(startX + settings.graphWidth/2.0, startY - .035,settings.yAxisTag);
}

void OpenGlGraph::drawData(bool connect){
  glColor4f(settings.dataLineColor[0],settings.dataLineColor[1],settings.dataLineColor[2],settings.dataLineColor[3]);
  glBegin(GL_LINE_STRIP);
  float tempX, tempY;
  for(int i = 0; i < (int)dataPoints.size(); i ++){
    tempX = (dataPoints[i].x - settings.dataMinX) / (settings.dataMaxX - settings.dataMinX) * settings.graphWidth + settings.graphOriginX - settings.graphWidth/2.0;
    tempY = (dataPoints[i].y - settings.dataMinY) / (settings.dataMaxY - settings.dataMinY) * settings.graphHeight + settings.graphOriginY - settings.graphHeight/2.0;
    glVertex2d(tempX, tempY);
  }
  glEnd();
}



void OpenGlGraph::drawStringCenteredAt(float x, float y, string line){
  float textWidth = line.length() * .005;
  float textHeight = .002;
  drawString(x - textWidth/2.0, y - textHeight,line);
}

void OpenGlGraph::drawString(float x, float y, string line){
  glRasterPos2f(x , y);
  for(int i = 0; i < (int)line.length(); i ++){
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, line.at(i));
  }
}

string OpenGlGraph::truncateFloat(float toTruncate, int numDecimal){
  string temp = to_string(toTruncate);
  string toReturn;
  int decimal = temp.find(".");
  toReturn = temp.substr(0,decimal + 1 + numDecimal);
  //printf(". found: %d len %d",temp.find("."),temp.length() );
  if(temp.find(".") == temp.length()) toReturn += "0";
  return toReturn;
}
