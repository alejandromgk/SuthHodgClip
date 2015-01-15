/*
 * Karol Alejandro Mancera Guapo
 * Primavera-Verano 2014
 * DICIS - UGTO
 * Gráficos por Computadora
 */

#include <cstdio>
#include <cmath>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <list>
#include <algorithm>
#include "edgetable.h"
#include "point.h"
#include "matrix.h"

void init();
void processMouse(int button, int state, int x, int y);
void processMouseActiveMotion(int x , int y);
void customDisplay();

/* Scanline polygon fill */
void fillScan(int, std::list<Node>);
void scanlinePolyFill(PointArray Po);

/* Clipping */
int getcode(Point P);
PointArray CohenSuthClip(const PointArray Line);
bool isInside(const Point &P, const Point &EdgeP1, const Point &EdgeP2);
Point getIntersection(const Point P1, const Point P2, const Point &EdgeP1, const Point &EdgeP2);
PointArray SuthHodgClip(const PointArray polygonVertices);
inline void drawClipArea();

int _x1 = -200; 
int _x2 = 200; 
int _y1 = 50; 
int _y2 = 50;
int xmin = -319, xmax = 320, ymin = -239, ymax = 240;
PointArray Poly, lPts;
Point tmpP, tmpQ;


int main(int argc, char **argv)
{
  tmpQ.x = _x1; tmpQ.y = _y1; lPts.pt.push_back(tmpQ);
  tmpQ.x = _x2; tmpQ.y = _y2; lPts.pt.push_back(tmpQ);

  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_SINGLE |GLUT_RGB);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(640,480);
  glutCreateWindow("P2 - Polygon Clipping");
  init();
  glutDisplayFunc(customDisplay);
  glutMouseFunc(processMouse);
//   glutMotionFunc(processMouseActiveMotion);
  glutMainLoop();
  return 0;
}

void init()
{
  glClearColor(0.0,0.0,0.0,0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-319,320,-239,240.0);
}

/* Función para procesar la entrada del mouse con el fin de determinar un área de recorte.
 */
void processMouse(int button, int state, int x, int y)
{
  int w, h, x1 = xmin, x2 = xmax, y1 = ymin, y2 = ymax;
  w = glutGet(GLUT_WINDOW_WIDTH);
  h = glutGet(GLUT_WINDOW_HEIGHT);

  /* Presionar el mouse para obtener la primera coordenada del área de recorte, 
   * arrastrar y luego soltar donde se quiere que finalice el área de recorte. 
   */
  if (button == GLUT_LEFT_BUTTON){
    if (state == GLUT_DOWN){
      x1 = x2 = x - w/2;
      y1 = y2 = h/2 - y;
    }
    if (state == GLUT_UP){
      x2 = x - w/2;
      y2 = h/2 - y;
    }
  }
  
  /* Coordenadas máximas y mínimas del área de recorte. */
  xmin = (x1 < x2) ? x1 : x2;
  xmax = (x1 > x2) ? x1 : x2;
  ymin = (y1 < y2) ? y1 : y2;
  ymax = (y1 > y2) ? y1 : y2;

  glutPostRedisplay();
}

/*
void processMouseActiveMotion(int x, int y)
{
  int specialKey;
  specialKey = glutGetModifiers();
  if (specialKey == GLUT_ACTIVE_CTRL){
    std::printf("x,y: %d, %d", x, y);
  }
}*/

void customDisplay()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(0 , 1, 1);
  PointArray Poly;
  Point p;
  
  p.x = -100; p.y = -130; Poly.pt.push_back(p);
  p.x = -190; p.y = 150; Poly.pt.push_back(p);
  p.x = 10; p.y = 10; Poly.pt.push_back(p);
  p.x = 150; p.y = 150; Poly.pt.push_back(p);
  p.x = 180; p.y = -70; Poly.pt.push_back(p);
  
//   lPts = CohenSuthClip(lPts); // Recortado de línea
  
  Poly = SuthHodgClip(Poly); // Recorte del polígono
  if (!Poly.pt.empty()) {
    scanlinePolyFill(Poly); // Rellenado del polígono
  }
  
  drawClipArea();
  glFlush();
}


/* Función auxiliar del algoritmo de Cohen-Sutherland para el recortado de polígonos.
 * Determina si un punto está dentro del área de recorte.
 */
bool isInside(const Point& P, const Point &EdgeP1, const Point &EdgeP2)
{
  int x, y, x1, x2, y1, y2;
  x = P.x;
  y = P.y;
  x1 = EdgeP1.x;
  y1 = EdgeP1.y;
  x2 = EdgeP2.x;
  y2 = EdgeP2.y;
  
  return (x1 - x2)*(y - y2) > (y1 - y2)*(x - x2);
}

/* Función auxiliar del algoritmo de Cohen-Sutherland para el recortado de polígonos.
 * Regresa la intersección de una recta con uno de los bordes del área de recorte.
 */
Point getIntersection(const Point P1, const Point P2, const Point &EdgeP1, const Point &EdgeP2)
{
  Point A, B, intersection;
  float K, L;
  float m;
  
  A = P1 - P2;
  B = EdgeP1 - EdgeP2;
  K = (P1.x * P2.y) - (P1.y * P2.x);
  L = (EdgeP1.x * EdgeP2.y) - (EdgeP1.y * EdgeP2.x);
  m = 1 / (A.x * B.y - A.y * B.x);
  
  intersection.x = m * (K * B.x - L * A.x);
  intersection.y = m * (K * B.y - L * A.y);
  
  return intersection;
}

/*
 * Recortado de polígonos mediante el algoritmo de Sutherland–Hodgman.
 * Recibe una arreglo con los vértices del polígono y regresa una nueva lista
 * con los nuevos vértices del polígono ya recortado en base al área de recorte.
 */
PointArray SuthHodgClip(const PointArray polygonVertices)
{
  PointArray outputList = polygonVertices;
  PointArray inputList;
  Point EdgePoint;
  PointArray Edges;
  int E1[4], E2[4];
  E1[0] = 0; E1[1] = 1; E1[2] = 2; E1[3] = 3;
  E2[0] = 1; E2[1] = 2; E2[2] = 3; E2[3] = 0;
  
  /* Bordes del área de recorte. */
  EdgePoint.x = xmin; EdgePoint.y = ymax; Edges.pt.push_back(EdgePoint);
  EdgePoint.x = xmax; EdgePoint.y = ymax; Edges.pt.push_back(EdgePoint);
  EdgePoint.x = xmax; EdgePoint.y = ymin; Edges.pt.push_back(EdgePoint);
  EdgePoint.x = xmin; EdgePoint.y = ymin; Edges.pt.push_back(EdgePoint);
  
  /* Caso 1 : Ambos puntos dentro del área de recorte, se guarda el punto.
   * Caso 2 : P1 dentro y P2 fuera del área, se determina la intersección de P2 con el borde.
   * Caso 3 : Ambos puntos fuera del área de recorte, no se guarda nada.
   * Caso 4 : P1 fuera y P2 dentro, se determina la intersección de P1 con el borde.
   */
  for (int itEdges = 0; itEdges < Edges.pt.size(); itEdges++) {
    inputList = outputList;
    outputList.pt.clear();
    Point S = inputList.pt.back();
    for (int itPointE = 0; itPointE < inputList.pt.size(); itPointE++) {
      if(isInside(inputList.pt.at(itPointE), Edges.pt.at(E1[itEdges]), Edges.pt.at(E2[itEdges]))) {
	if(isInside(S, Edges.pt.at(E1[itEdges]), Edges.pt.at(E2[itEdges]))) {
	  outputList.pt.push_back(inputList.pt.at(itPointE)); // Caso 1
	} else { // Caso 4
	  Point tmp;
	  Point E = inputList.pt.at(itPointE);
	  tmp = getIntersection(S, E, Edges.pt.at(E1[itEdges]), Edges.pt.at(E2[itEdges]) );
	  outputList.pt.push_back(tmp);
	  outputList.pt.push_back(inputList.pt.at(itPointE)); 
	}
      } else {
	  if(isInside(S, Edges.pt.at(E1[itEdges]), Edges.pt.at(E2[itEdges]))) { // Caso 2.
	    Point tmp;
	    Point E = inputList.pt.at(itPointE);
	    tmp = getIntersection(S, E, Edges.pt.at(E1[itEdges]), Edges.pt.at(E2[itEdges]) );
	    outputList.pt.push_back(tmp);
	  }
	}
	// En el caso 3 no se guarda ningún vértice, porque ambos puntos están fuera del área de recorte.
	S = inputList.pt.at(itPointE);
    }
  }
  return outputList;
}

/* Función auxiliar del algoritmo de Cohen-Sutherland para el recortado de rectas.
 * Determina si un punto está dentro de un área de recorte definida.
 * Tiene cinco codificaciones que ayudan a determinar la posición 
 * del punto respecto al área de recorte.
 */
int computeOutCode(Point P)
{
  int outcode = 0; // 0000 Está dentro del área.
  
  /* Mediante la operación OR se puede obtener las distintas combinaciones de la ubicación del punto.
   * Eg. 1010 indica que el punto está en la esquina superior derecha.
   */
  
  if (P.y > ymax) outcode |= 0x8;	// 1000 - Top
  if (P.y < ymin) outcode |= 0x4; 	// 0100 - Bottom
  if (P.x > xmax) outcode |= 0x2; 	// 0010 - Right
  if (P.x < xmin) outcode |= 0x1; 	// 0001 - Left

  return outcode;
}

/* Algoritmo de Cohen-Sutherland para el recortado de rectas.
 * Recibe una línea, con puntos P1 y P2 y regresa otra línea ya recortada tomando en cuenta el área de recorte definida.
 */
PointArray CohenSuthClip(const PointArray Line)
{
  int outcodeP1, outcodeP2, tmpcode, x, y;
  bool done = false;
  Point P1;
  PointArray clippedLine = Line;
  
  outcodeP1 = computeOutCode(Line.pt.at(0));
  outcodeP2 = computeOutCode(Line.pt.at(1));
  do {
    if(!(outcodeP1 | outcodeP2)) {
      // Si ambos puntos están dentro del área de recorte (código 0000), es T/A.
      done = true;
    } else 
      if(outcodeP1 & outcodeP2) 
	done = true; // De otro modo, si el AND de los dos puntos no es cero, está fuera del área de recorte y es T/R.
	else {
      tmpcode = outcodeP1 ? outcodeP1 : outcodeP2; // Se comprueba cual punto está fuera del área de recorte y se procede a recortar.
      // Top area 
      if(tmpcode & 8) {
	x = Line.pt.at(0).x + (Line.pt.at(1).x - Line.pt.at(0).x) * (ymax - Line.pt.at(0).y) / (Line.pt.at(1).y - Line.pt.at(0).y);
	y = ymax;
      } else  // Bottom area
	if(tmpcode & 4) {
	  x = Line.pt.at(0).x + (Line.pt.at(1).x- Line.pt.at(0).x) * (ymin - Line.pt.at(0).y) / (Line.pt.at(1).y - Line.pt.at(0).y);
	  y = ymin;
	} else // Right area
	  if(tmpcode & 2) {
	    y = Line.pt.at(0).y + (Line.pt.at(1).y - Line.pt.at(0).y) * (xmax - Line.pt.at(0).x) / (Line.pt.at(1).x - Line.pt.at(0).x);
	    x = xmax;
	  } else {
	    // Left area
	    y = Line.pt.at(0).y + (Line.pt.at(1).y - Line.pt.at(0).y) * (xmin - Line.pt.at(0).x) / (Line.pt.at(1).x - Line.pt.at(0).x);
	    x = xmin;
	  }
	    // Se revisa cuál punto fue el que se recortó y se actualiza acorde.
	  if (tmpcode == outcodeP1) {
	    clippedLine.pt.at(0).x = x; // x1
	    clippedLine.pt.at(0).y = y; // y1
	    outcodeP1 = computeOutCode(clippedLine.pt.at(0));
	  } else {
	    clippedLine.pt.at(1).x = x; // x2
	    clippedLine.pt.at(1).y = y; // y2
	    outcodeP2 = computeOutCode(clippedLine.pt.at(1));
	  }
	}
  } while(!done);
  
  return clippedLine;
}

/* Función auxiliar para pintar el rectángulo que representa el área de recorte.
 */
inline void drawClipArea()
{
  /* Área de recorte. */
  glColor3f(1 , 0, 1);
  glBegin(GL_LINES);
    glVertex2i(xmin, ymax);
    glVertex2i(xmax, ymax);
    glVertex2i(xmax, ymax);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmin, ymin);
    glVertex2i(xmin, ymin);
    glVertex2i(xmin, ymax);
  glEnd();
}


/* Función que se encarga de construir la Tabla de bordes y la tabla de bordes activos
 * en base a un arrelo de puntos y actualiza ambas tablas mientras itera de Ymin a Ymax.
 */
void scanlinePolyFill(PointArray Po)
{
  EdgeTable ET;
  int minY, maxY;
  int scanline;
  std::list<Node> AET;
  
  ET.buildET(Po);
  minY = Po.yMin();
  maxY = Po.yMax();
  //ET.printET();
  
  for (scanline = minY; scanline < maxY; scanline++) {
    if (!ET.Edges.empty()) {
      ET.buildAET(AET, ET.Edges, scanline);
    }
    if (!AET.empty()) {
       fillScan(scanline, AET);
       ET.updateAET (AET, scanline);
       /* AET.sort(); */
     }
  }
}

/* Pinta una línea horizontal que equivale al scanline actual para ello toma pares de
 * vértices de la Active Edge Table que coincidan con el valor actual del scanline.
 */
void fillScan (int scanline, std::list<Node> AET)
{
  std::list<Node>::iterator iter1 = AET.begin(), iter2;
  int x1,x2;
  
  if(AET.size() % 2 ==0){
    while (iter1 != AET.end())
    { iter2 = iter1;
      iter2++; // Segundo punto.
      x1 = (int)(*iter1).x;
      x2 = (int)(*iter2).x;
      glBegin(GL_LINES);
	glVertex2i(x1,scanline);
	glVertex2i(x2,scanline);
      glEnd();
	// Siguiente par de nodos.
      iter1 = iter2;
      iter1++;
    }
  }
}