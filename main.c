#include <stdlib.h>
#include <stdio.h>
#include "exerciser.h"
#include<string.h>

int main (int argc, char *argv[]) 
{
  //Allocate & initialize a MYSQL object
  MYSQL *conn = mysql_init(NULL); 

  if (conn == NULL) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    return 1;
  }

  //Establish a connection to the database
  //Parameters: connection handler, host name, user name, password,
  //            database name, port numbrer, unix socket, client flag
  if (mysql_real_connect(conn, "localhost", "myuser", "passw0rd", "ACC_BBALL", 0, NULL, 0) == NULL) {
    exit_with_error(conn);
  }


  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  if (mysql_query(conn, "DROP TABLE IF EXISTS PLAYER")) {
    exit_with_error(conn);
  }
  if (mysql_query(conn, "CREATE TABLE PLAYER(PLAYER_ID INT NOT NULL AUTO_INCREMENT, TEAM_ID INT, UNIFORM_NUM INT, FIRST_NAME TEXT, LAST_NAME TEXT, MPG INT, PPG INT, RPG INT, APG INT, SPG FLOAT, BPG FLOAT,PRIMARY KEY (PLAYER_ID))")) {      
    exit_with_error(conn);
  }
  if (mysql_query(conn, "DROP TABLE IF EXISTS TEAM")) {
    exit_with_error(conn);
  }
  if (mysql_query(conn, "CREATE TABLE TEAM (TEAM_ID INT NOT NULL AUTO_INCREMENT, NAME TEXT, STATE_ID INT, COLOR_ID INT, WINS INT, LOSSES INT,PRIMARY KEY (TEAM_ID))")) {
    exit_with_error(conn);
  }
  if (mysql_query(conn, "DROP TABLE IF EXISTS STATE")) {
    exit_with_error(conn);
  }
  if (mysql_query(conn, "CREATE TABLE STATE (STATE_ID INT NOT NULL AUTO_INCREMENT, NAME TEXT,PRIMARY KEY (STATE_ID))")) {
    exit_with_error(conn);
  }
  if (mysql_query(conn, "DROP TABLE IF EXISTS COLOR")) {
    exit_with_error(conn);
  }
  if (mysql_query(conn, "CREATE TABLE COLOR (COLOR_ID INT NOT NULL AUTO_INCREMENT, NAME TEXT,PRIMARY KEY (COLOR_ID))")) {
    exit_with_error(conn);
  }
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  const char s[2] = " ";
  fp = fopen("player.txt", "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {
    //printf("Retrieved line of length %zu :\n", read);
    //printf("%s", line);
    if(read > 1){
      char *curr;
      char input[1000] = "INSERT INTO PLAYER VALUES(";
      curr = strtok(line, s);
      int PLAYER_ID = atoi(curr);
      strcat(input,curr);strcat(input,",");
      curr = strtok(NULL, s);
      strcat(input,curr);strcat(input,",");
      int TEAM_ID = atoi(curr);
      curr = strtok(NULL, s);
      strcat(input,curr);strcat(input,",");
      int UNIFORM_NUM = atoi(curr);
      curr = strtok(NULL, s);
      strcat(input,"'");strcat(input,curr);strcat(input,"'");strcat(input,",");
      char FIRST_NAME[20];
      strcpy(FIRST_NAME,curr); 
      curr = strtok(NULL, s);
      strcat(input,"'");strcat(input,curr);strcat(input,"'");strcat(input,",");
      char LAST_NAME[20];
      strcpy(LAST_NAME,curr);
      curr = strtok(NULL, s);
      strcat(input,curr);strcat(input,",");
      int MPG = atoi(curr);
      curr = strtok(NULL, s);
      strcat(input,curr);strcat(input,",");
      int PPG = atoi(curr);
      curr = strtok(NULL, s);
      strcat(input,curr);strcat(input,",");
      int RPG = atoi(curr);
      curr = strtok(NULL, s);
      strcat(input,curr);strcat(input,",");
      int APG = atoi(curr);
      curr = strtok(NULL, s);
      strcat(input,curr);strcat(input,",");
      double SPG = strtof(curr,NULL);
      curr = strtok(NULL, "\n");
      strcat(input,curr);strcat(input,")");
      double BPG = strtof(curr,NULL);
      //printf("%s\n",input);
      add_player(conn, TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG,RPG, APG, SPG, BPG);
      //printf("PlayerID, %d, TEAM_ID, %d, name, %s %s ,SPG,%f,BPG,%f\n",PLAYER_ID,TEAM_ID, FIRST_NAME,LAST_NAME, SPG,BPG);
       
      //if (mysql_query(conn, input)) {
      //	exit_with_error(conn);
      //}
    }
  }
  fclose(fp);
  fp = fopen("team.txt", "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {
    if(read > 1){
      char *curr;
      char input[1000] = "INSERT INTO TEAM VALUES(";
      curr = strtok(line, s);
      strcat(input,curr);strcat(input,",");
      curr = strtok(NULL, s);
      strcat(input,"'");strcat(input,curr);strcat(input,"'");strcat(input,",");
      char name[20];
      strcpy(name,curr); 
      curr = strtok(NULL, s);
      int state_id = atoi(curr);
      strcat(input,curr);strcat(input,",");
      curr = strtok(NULL, s);
      int color_id = atoi(curr);
      strcat(input,curr);strcat(input,",");
      curr = strtok(NULL, s);
      int wins = atoi(curr); 
      strcat(input,curr);strcat(input,",");
      curr = strtok(NULL, "\n");
      int losses = atoi(curr); 
      strcat(input,curr);strcat(input,")");
      //printf("%s\n",input);
      add_team(conn,name,state_id,color_id,wins,losses);
      //if (mysql_query(conn, input)) {
      //  exit_with_error(conn);
      //}
    }
  }
  fclose(fp);
  fp = fopen("state.txt", "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {
    if(read > 1){
      char *curr;
      char input[1000] = "INSERT INTO STATE VALUES(";
      curr = strtok(line, s);
      strcat(input,curr);strcat(input,",");
      curr = strtok(NULL, "\n");
      char name[20];
      strcpy(name,curr);
      strcat(input,"'");strcat(input,curr);strcat(input,"'");strcat(input,")");
      //printf("%s\n",input);
      add_state(conn,name);
      //if (mysql_query(conn, input)) {
      //  exit_with_error(conn);
      //}
    }
  }
  fclose(fp);
  fp = fopen("color.txt", "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);
  while ((read = getline(&line, &len, fp)) != -1) {
    if(read > 1){
      char *curr;
      char input[1000] = "INSERT INTO COLOR VALUES(";
      curr = strtok(line, s);
      strcat(input,curr);strcat(input,",");
      curr = strtok(NULL, "\n");
      char name[20];
      strcpy(name,curr);
      strcat(input,"'");strcat(input,curr);strcat(input,"'");strcat(input,")");
      //printf("%s\n",input);
      add_color(conn,name);
      //if (mysql_query(conn, input)) {
      //  exit_with_error(conn);
      //}
    }
  }
  fclose(fp);


  
  exercise(conn);


  //Close database connection
  mysql_close(conn);

  return 0;
}


