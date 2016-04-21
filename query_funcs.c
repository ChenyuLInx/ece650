#include "query_funcs.h"
#include <string.h>

void exit_with_error(MYSQL *conn)
{
  fprintf(stderr, "%s\n", mysql_error(conn));
  mysql_close(conn);
  exit(1);
}


void add_player(MYSQL *conn, int team_id, int jersey_num, char *first_name, char *last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
  char buf[1000];
  sprintf(buf,"INSERT INTO PLAYER(TEAM_ID,UNIFORM_NUM,FIRST_NAME,LAST_NAME,MPG, PPG,RPG, APG, SPG, BPG) VALUES(%d,%d,'%s','%s',%d,%d,%d,%d,%f,%f)",team_id,jersey_num,first_name,last_name,mpg,ppg,rpg,apg,spg,bpg);
  //printf("%s\n",buf);
  if (mysql_query(conn, buf)) {
    exit_with_error(conn);
  }

}


void add_team(MYSQL *conn, char *name, int state_id, int color_id, int wins, int losses)
{
  char buf[1000];
  sprintf(buf,"INSERT INTO TEAM(NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES('%s',%d,%d,%d,%d)",name,state_id,color_id,wins,losses);
  //printf("%s\n",buf);
  if (mysql_query(conn, buf)) {
    exit_with_error(conn);
  }

}


void add_state(MYSQL *conn, char *name)
{
  char buf[1000];
  sprintf(buf,"INSERT INTO STATE(NAME) VALUES('%s')",name);
  //printf("%s\n",buf);
  if (mysql_query(conn, buf)) {
    exit_with_error(conn);
  }

}


void add_color(MYSQL *conn, char *name)
{
  char buf[1000];
  sprintf(buf,"INSERT INTO COLOR(NAME) VALUES('%s')",name);
  //printf("%s\n",buf);
  if (mysql_query(conn, buf)) {
    exit_with_error(conn);
  }

}


void query1(MYSQL *conn,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
  char input[512] = "SELECT * FROM PLAYER WHERE ";
  if(use_mpg != 0){
    char tmp[100];
    sprintf(tmp,"MPG>=%d AND MPG<=%d ",min_mpg,max_mpg);
    strcat(input,tmp);
  }
  if(use_ppg != 0){
    char tmp[100];
    sprintf(tmp,"PPG>=%d AND PPG<=%d ",min_ppg,max_ppg);
    if(use_mpg !=0){
      strcat(input,"AND ");
    }
    strcat(input,tmp);
  }
  if(use_rpg != 0){
    char tmp[100];
    sprintf(tmp,"RPG>=%d AND RPG<=%d ",min_rpg,max_rpg);
    if((use_mpg+use_ppg) != 0){
      strcat(input,"AND ");
    }
    strcat(input,tmp);
  }
  if(use_apg != 0){
    char tmp[100];
    sprintf(tmp,"APG>=%d AND APG<=%d ",min_apg,max_apg);
    if((use_mpg+use_ppg+use_rpg) != 0){
      strcat(input,"AND ");
    }
    strcat(input,tmp);
  }
  if(use_spg != 0){
    char tmp[100];
    sprintf(tmp,"SPG>=%f AND SPG<=%f ",min_spg,max_spg);
    if((use_mpg+use_ppg+use_rpg+use_apg) != 0){
      strcat(input,"AND ");
    }
    strcat(input,tmp);
  }
  if(use_bpg != 0){
    char tmp[100];
    sprintf(tmp,"BPG>=%f AND BPG<=%f ",min_bpg,max_bpg);
    if((use_mpg+use_ppg+use_rpg+use_apg+use_spg) != 0){
      strcat(input,"AND ");
    }
    strcat(input,tmp);
  }
  //printf("%s",input);
  if (mysql_query(conn, input)) {
    exit_with_error(conn);
  }
  MYSQL_RES *result = mysql_store_result(conn);
  if (result == NULL){
      exit_with_error(conn);
  }
  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  MYSQL_FIELD *field;
  while (field = mysql_fetch_field(result)) {
    printf("%s ", field->name);
  }
  printf("\n");

  while ((row = mysql_fetch_row(result))){
    for(int i = 0; i < num_fields; i++){
      /* if (i == 0) {
        while (field = mysql_fetch_field(result)) {
          printf("%s ", field->name);
        }
        printf("\n");
	}*/
      printf("%s ", row[i] ? row[i] : "NULL");
    }
    printf("\n");
  }
  mysql_free_result(result);
}


void query2(MYSQL *conn, char *team_color)
{
  char input[512];
  sprintf(input,"SELECT COLOR_ID FROM COLOR WHERE NAME='%s'",team_color);
  if (mysql_query(conn, input)) {
    exit_with_error(conn);
  }
  MYSQL_RES *result = mysql_store_result(conn);
  if (result == NULL) {
      exit_with_error(conn);
    }
  MYSQL_ROW row = mysql_fetch_row(result);
  MYSQL_FIELD *field;
  int color_id = atoi(row[0]);
  sprintf(input,"SELECT NAME FROM TEAM WHERE COLOR_ID=%d",color_id);
  if (mysql_query(conn, input)) {
    exit_with_error(conn);
  }
  result = mysql_store_result(conn);
  if (result == NULL){
    exit_with_error(conn);
  }
  int num_fields = mysql_num_fields(result);
  while (field = mysql_fetch_field(result)) {
    printf("%s ", field->name);
  }
  printf("\n");
  while ((row = mysql_fetch_row(result))){
    for(int i = 0; i < num_fields; i++){
      /*if (i == 0) {
        while (field = mysql_fetch_field(result)) {
          printf("%s ", field->name);
        }
        printf("\n");
	}*/
      printf("%s ", row[i] ? row[i] : "NULL");
    }
    printf("\n");
  }
  mysql_free_result(result);

}


void query3(MYSQL *conn, char *team_name)
{
  char input[512];
  sprintf(input,"SELECT TEAM_ID FROM TEAM WHERE NAME='%s'",team_name);
  if (mysql_query(conn, input)) {
    exit_with_error(conn);
  }
  MYSQL_RES *result = mysql_store_result(conn);
  if (result == NULL) {
    exit_with_error(conn);
  }
  MYSQL_FIELD *field;
  MYSQL_ROW row = mysql_fetch_row(result);
  int team_id = atoi(row[0]);
  sprintf(input,"SELECT FIRST_NAME,LAST_NAME FROM PLAYER WHERE TEAM_ID=%d ORDER BY PPG DESC",team_id);
  if (mysql_query(conn, input)) {
    exit_with_error(conn);
  }
  result = mysql_store_result(conn);
  if (result == NULL){
    exit_with_error(conn);
  }
  int num_fields = mysql_num_fields(result);
  while (field = mysql_fetch_field(result)) {
    printf("%s ", field->name);
  }
  printf("\n");
  while ((row = mysql_fetch_row(result))){
    for(int i = 0; i < num_fields; i++){
      /*if (i == 0) {
        while (field = mysql_fetch_field(result)) {
          printf("%s ", field->name);
        }
        printf("\n");
	}*/
      printf("%s ", row[i] ? row[i] : "NULL");
    }
    printf("\n");
  }
  mysql_free_result(result);
}


void query4(MYSQL *conn, char *team_state, char *team_color)
{
  char input[512];                      
  sprintf(input,"SELECT FIRST_NAME,LAST_NAME,UNIFORM_NUM FROM PLAYER,TEAM,COLOR,STATE WHERE PLAYER.TEAM_ID=TEAM.TEAM_ID AND TEAM.STATE_ID=STATE.STATE_ID AND TEAM.COLOR_ID=COLOR.COLOR_ID AND COLOR.NAME ='%s' AND STATE.NAME='%s'",team_color,team_state);
  if (mysql_query(conn, input)) {
    exit_with_error(conn);
  }
  MYSQL_RES *result = mysql_store_result(conn);
  if (result == NULL){
    exit_with_error(conn);
  }
  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  MYSQL_FIELD *field;
  while (field = mysql_fetch_field(result)) {
    printf("%s ", field->name);
  }
  printf("\n");
  while ((row = mysql_fetch_row(result))){
    for(int i = 0; i < num_fields; i++){
      /*if (i == 0) {
	while (field = mysql_fetch_field(result)) {
	  printf("%s ", field->name);
	}
	printf("\n");
	}*/
      printf("%s ", row[i] ? row[i] : "NULL");
    }
    printf("\n");
  }
  mysql_free_result(result);
}


void query5(MYSQL *conn, int num_wins){
  char input[512];
  sprintf(input,"SELECT FIRST_NAME,LAST_NAME,TEAM.NAME,TEAM.WINS FROM PLAYER,TEAM WHERE PLAYER.TEAM_ID=TEAM.TEAM_ID AND TEAM.WINS>%d",num_wins);
  if (mysql_query(conn, input)) {
    exit_with_error(conn);
  }
  MYSQL_RES *result = mysql_store_result(conn);
  if (result == NULL){
    exit_with_error(conn);
  }
  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  MYSQL_FIELD *field;
  while (field = mysql_fetch_field(result)) {
    printf("%s ", field->name);
  }
  printf("\n");
  while (row = mysql_fetch_row(result)) {
    for (int i=0; i < num_fields; i++) {
      /*if (i == 0) {
	while (field = mysql_fetch_field(result)) {
	  printf("%s ", field->name);
	}
	printf("\n");
	}*/
      printf("%s ", row[i] ? row[i] : "NULL");
    }
    printf("\n");
  }
  mysql_free_result(result);
}
