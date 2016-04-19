#include "exerciser.h"

void exercise(MYSQL *conn)
{
  query1(conn,0,2,3,0,3,10,1,0,10,0,1,5,1,0.2,1.5,1,0.3,0.6);  
  query2(conn,"Gold");
  query3(conn,"Miami");
  query4(conn,"NC","Gold");
  query5(conn,10);
}
