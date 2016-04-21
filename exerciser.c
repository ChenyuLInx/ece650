#include "exerciser.h"

void exercise(MYSQL *conn)
{
  query1(conn,1,20,25,1,3,10,1,5,10,1,1,5,1,0.5,2.0,0,0.3,0.6);  
  //query2(conn,"Green");
  //query2(conn,"Orange");
  //query3(conn,"BostonCollege");
  //query3(conn,"UNC");
  query4(conn,"MA","Black");
  query4(conn,"NC","DarkBlue");
  //query5(conn,10);

}
