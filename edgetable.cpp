#include "edgetable.h"

EdgeTable::EdgeTable()
{

}

EdgeTable::~EdgeTable()
{

}

void EdgeTable::buildET ( const PointArray& Pts)
{
  Node n;
  std::list<Node>::iterator iterator = Edges.begin();
  int i;
  
  /* Último punto con el primero */
  if (Pts.pt[Pts.pt.size()-1].y != Pts.pt[0].y){
    if ( Pts.pt[Pts.pt.size()-1].y > Pts.pt[0].y){
	  n.x = Pts.pt[0].x;
	  n.yMin = Pts.pt[0].y;
	  n.yMax = Pts.pt[Pts.pt.size()-1].y;
	  n.mInversa = (float)(Pts.pt[Pts.pt.size()-1].x - Pts.pt[0].x)/(Pts.pt[Pts.pt.size()-1].y - Pts.pt[0].y);
	  //std::printf("mInversa: %f", n.mInversa);
	} else {
	  n.x = Pts.pt[Pts.pt.size()-1].x;
	  n.yMin = Pts.pt[Pts.pt.size()-1].y;
	  n.yMax = Pts.pt[0].y;
	  n.mInversa = (float)(Pts.pt[0].x - Pts.pt[Pts.pt.size()-1].x)/(Pts.pt[0].y - Pts.pt[Pts.pt.size()-1].y);
	}
	Edges.push_back(n);
  }
      
  /* Se itera en todos los puntos, por cada par de puntos se comprueba que el par 
   * no esté en la misma vertical (mismo valor de ymin) y de ser así, no se guardará en 
   * la Edge Table, finalmente se guardan los  valores de x, yMin, yMax y la pendiente 
   * con su signo correcto según sea el caso.
   */
  for ( i = 0; i < Pts.pt.size()-1; i++ ){
    if (Pts.pt[i].y != Pts.pt[i+1].y){
      if ( Pts.pt[i+1].y > Pts.pt[i].y){
	n.x = Pts.pt[i].x;
	n.yMin = Pts.pt[i].y;
	n.yMax = Pts.pt[i+1].y;
	n.mInversa = (float)(Pts.pt[i+1].x - Pts.pt[i].x)/(Pts.pt[i+1].y - Pts.pt[i].y);
      } else {
	n.x = Pts.pt[i+1].x;
	n.yMin = Pts.pt[i+1].y;
	n.yMax = Pts.pt[i].y;
	n.mInversa = (float)(Pts.pt[i].x - Pts.pt[i+1].x)/(Pts.pt[i].y - Pts.pt[i+1].y);
      }
      Edges.push_back(n);
    }
  }
  /* Se ordena respecto a x en forma ascendente. */
  Edges.sort();
}

void EdgeTable::buildAET(std::list<Node> & AET, std::list< Node > &ET, const int scanline)
{
  std::list<Node>::iterator i = ET.begin();
  
  while (i != ET.end()){
    if((*i).yMin == scanline ){
      AET.push_back((*i));
      ET.erase(i);
      i=ET.begin();
    } else 
      i++;
  }
  AET.sort();
}

/* Se borran los nodos cuya yMáxima es igual o mayor al valor actual del scanline
 * sino solo se actualiza x para todos los nodos que hay en la AET.
 */

void EdgeTable::updateAET(std::list<Node> & AET, const int scanline)
{
  std::list<Node>::iterator it = AET.begin();
  
  while (it != AET.end()){
    if (scanline >= (*it).yMax){
      AET.erase(it);
      AET.sort(sortYminmax);
      
      it=AET.begin();
    } else {
      (*it).x += (*it).mInversa;
      it++;
    }
  }
  AET.sort();
}

void EdgeTable::printET()
{
  std::list<Node>::iterator it = Edges.begin();
  for ( it; it != Edges.end(); it++) 
  std::printf("\nElementos: %d, %d, %f, %f", (*it).yMin, (*it).yMax, (*it).x, (*it).mInversa); 
}


// Se sobrecarga el operador < para poder usar la función sort de la STL para ordenar respecto a x.
bool Node::operator<( const Node n )
{
  return (this->x < n.x);
}

bool EdgeTable::sortYminmax(Node n1, Node n2)
{
  return (n1.yMax < n2.yMax);
}
