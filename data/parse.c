#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>

#define MAX_QB 38
#define MAX_RB 117
#define MAX_WR 150
#define MAX_TE 82
#define MAX_K 33
#define MAX_DST 32
#define MAX_ROSTERS 8589220
#define MAX_PLAYS 1000
#define SALARY_CAP 60000

static int idx1h = 0;
static int idx2h = 0;

struct player {
  int rank;
  int salary;
  char name[30];
  char team[20];
  char matchup[20];
  int best_rank;
  int worst_rank;
  double avg_rank;
  double std_dev;
} qb[38], rb[117], wr[150], te[82], k[33], dst[32];

struct roster {
  int qb;
  int rb1;
  int rb2;
  int wr1;
  int wr2;
  int wr3;
  int te;
  int k;
  int dst;
  int total_amount;
  int accept_ratio;
} rosters1h[MAX_ROSTERS], rosters2h[MAX_ROSTERS];

struct plays {
  char role1[3];
  int player1;
  int pts1;
  char role2[3];
  int player2;
  int pts2;
};


void copyMe(char *from, char *to){
  while(*to++=*from++);
}

void populateData (FILE *fp, struct player * playersarr, int l) {
  char curplayer[300];
  int i = 0;
  
  fgets(curplayer, 300, fp);

  while (fgets(curplayer, 300, fp) != EOF){
    //printf ("%s\n", curplayer);    
    char* tok;
    tok = strtok (curplayer, ",");
    //printf ("First: %s\n", tok);
    playersarr[i].rank = atoi(tok);

    copyMe(strtok(NULL, ","), playersarr[i].name);
    
    //printf ("name: %s\n", playersarr[i].name);
    copyMe(strtok(NULL, ","), playersarr[i].team);
    copyMe(strtok(NULL, ","), playersarr[i].matchup);
    playersarr[i].best_rank = atoi(strtok(NULL, ","));
    playersarr[i].worst_rank = atoi(strtok(NULL, ","));
    tok = strtok(NULL, ",");
    sscanf(tok,"%lf", &playersarr[i].avg_rank);
    //tok = strtok(NULL, ",");
    sscanf(curplayer, "%lf", &playersarr[i].std_dev);
    i++;
        
    if (i == l){
      break;
    }
    
  }
  //printf ("Done reading!\n");
}

void readPlays (FILE *fp, struct plays * playsarr, int l){
  char curplay[300];
  int i = 0;
  //char role[4];
  while (fgets(curplay, 300, fp) != EOF) {
    char* tok;
    tok = strtok(curplay, ",");
    copyMe(tok, playsarr[i].role1);
    playsarr[i].player1 = atoi(strtok(NULL, ","));
    playsarr[i].pts1 = atoi(strtok(NULL, ","));
    copyMe(strtok(NULL, ","), playsarr[i].role1);
    playsarr[i].player2 = atoi(strtok(NULL, ","));
    playsarr[i].pts2 = atoi(strtok(NULL, ","));
    i++;

    if (i == l) {
      break;
    }
  }
}

void getsalaries(){
  char curplayer[300];
  FILE *fp; 
  fp = fopen("Fantasy_Week13_Salaries.csv","r");
  int salary;
  char role[4];
  char name[30];
  int row = 0;
  int i;

  while (fgets(curplayer, 300, fp) != EOF){
   if(row++ == 213) break;
  
    char* tok;
    tok = strtok (curplayer, ",");
    
    copyMe(tok, role);
    copyMe(strtok(NULL, ","), name);
    salary = atoi(strtok(NULL, ","));
    struct player *ptr;
    int max; 

    if(!strcmp(role,"QB")){
	ptr = qb;
	max = MAX_QB;	
    }else if(!strcmp(role,"RB")){
	ptr = rb;
	max = MAX_RB;	
    }else if(!strcmp(role,"WR")){
   	ptr = wr;
	max = MAX_WR;	
    }else if(!strcmp(role,"K")){
 	ptr = k;
	max = MAX_K;	
    }else if(!strcmp(role,"D")){
	ptr= dst;
	max = MAX_DST;	
    }else if(!strcmp(role,"TE")){
 	ptr = te;
	max = MAX_TE;	
    }
   
    for(i = 0; i < max; i++ ){
  	if(strcmp(name,ptr[i].name)==0){	
	 // 	printf("%s %d \n",ptr[i].name, salary);	
		ptr[i].salary = salary;
    }
   }
  }
}

void finalizeDST (int qbi, int rb1i, int rb2i, int wr1i, int wr2i, int wr3i, int tei, int ki){
  //printf ("In function finalizeDST");
  int dsti;
  int amount = qb[qbi].salary + rb[rb1i].salary + rb[rb2i].salary + wr[wr1i].salary + wr[wr2i].salary + wr[wr3i].salary + te[tei].salary + k[ki].salary;
 int sum;
 struct roster * rosters;
 int idx;
 for (dsti = 0; dsti < 15; dsti++) {
    sum = amount + dst[dsti].salary;
    if (dst[dsti].salary != 0 && sum <= SALARY_CAP && sum >= 59000){
      //printf("qbi: %d rbi1: %d rb2i: %d wr1i: %d wr2i %d wr3i %d  tei %d ki %d \n",qbi,rb1i,rb2i,wr1i,wr2i,wr3i,tei,ki);
      if (idx1h < MAX_ROSTERS){
	rosters = rosters1h;
	idx = idx1h;
	idx1h++;
      }
      else {
        rosters = rosters2h;
	idx = idx2h;
	idx1h++;
      } 
      rosters[idx].qb = qbi;
      rosters[idx].rb1 = rb1i;
      rosters[idx].rb2 = rb2i;
      rosters[idx].wr1 = wr1i;
      rosters[idx].wr2 = wr2i;
      rosters[idx].wr3 = wr3i;
      rosters[idx].te = tei;
      rosters[idx].k = ki;
      rosters[idx].dst = dsti;
      rosters[idx].total_amount = sum;
      
      
      //printf ("index: %d\n", idx);
    }
  }
}

void finalizeK (int qbi, int rb1i, int rb2i, int wr1i, int wr2i, int wr3i, int tei){
  //printf ("In function finalizeK");
  int ki;
  for (ki = 0; ki < MAX_K; ki++) {
    if (k[ki].salary != 0){
      finalizeDST (qbi, rb1i, rb2i, wr1i, wr2i, wr3i, tei, ki);
    }
  }
}

void finalizeTE (int qbi, int rb1i, int rb2i, int wr1i, int wr2i, int wr3i){
  //printf ("In function finalizeTE");
  int tei;
  for (tei = 0; tei < MAX_TE; tei++) {
    if (te[tei].salary != 0){
      finalizeK (qbi, rb1i, rb2i, wr1i, wr2i, wr3i, tei);
    }
  }
}

void finalizeWR3 (int qbi, int rb1i, int rb2i, int wr1i, int wr2i) {
  //printf ("In function finalizeWR3");
  int wr3i;
  for (wr3i = wr2i + 1; wr3i < 50; wr3i++) {
    if ( wr[wr3i].salary != 0) {
      finalizeTE (qbi, rb1i, rb2i, wr1i, wr2i, wr3i);
    }
  }
}


void finalizeWR2 (int qbi, int rb1i, int rb2i, int wr1i) {
  //printf ("In function finalizeWR2");
  int wr2i;
  for (wr2i = wr1i + 1; wr2i < 20; wr2i++) {
    if (wr[wr2i].salary != 0) {
      finalizeWR3 (qbi, rb1i, rb2i, wr1i, wr2i);
	}
  }
}


void finalizeWR1 (int qbi, int rb1i, int rb2i){
  //printf ("In function finalizeWR1");
  int wr1i;
  for (wr1i = 0; wr1i < 20; wr1i++) {
    if (wr[wr1i].salary != 0){
      finalizeWR2 (qbi, rb1i, rb2i, wr1i);
    }
  }
}


void finalizeRB2 (int qbi, int rb1i) {
  //printf ("In function finalizeRB2");
  int rb2i;
  for (rb2i = rb1i + 1; rb2i < 20; rb2i++) {
    if (rb2i != rb1i && rb[rb2i].salary != 0) {
      finalizeWR1 (qbi, rb1i, rb2i);
    }
  }
}

void finalizeRB1 (int qbi) {
  //printf ("In function finalizeRB1");
  int rb1i;
  for (rb1i = 0; rb1i < 10; rb1i++) {
    if (rb[rb1i].salary != 0){
      finalizeRB2 (qbi, rb1i);
    }
  }
}


void makeRosters(){
  //printf ("In function makeRosters");
  int qbi;
  for (qbi = 0; qbi < 15; qbi++) {
    if (qb[qbi].salary != 0){
      finalizeRB1(qbi);
    }
  }
}

struct plays *teams[32];

int main (){
  FILE *fpqb;
  FILE *fprb;
  FILE *fpwr;
  FILE *fpte;
  FILE *fpk;
  FILE *fpdst;
  fpqb = fopen("FantasyPros_2014_Week_13_QB_Rankings.csv", "r");
  fprb = fopen("FantasyPros_2014_Week_13_RB_Rankings.csv", "r");
  fpwr = fopen("FantasyPros_2014_Week_13_WR_Rankings.csv", "r");
  fpte = fopen("FantasyPros_2014_Week_13_TE_Rankings.csv", "r");
  fpk = fopen("FantasyPros_2014_Week_13_K_Rankings.csv", "r");
  fpdst = fopen("FantasyPros_2014_Week_13_DST_Rankings.csv", "r");
  populateData(fpqb, qb, 38);
  populateData(fprb, rb, 117);
  populateData(fpwr, wr, 150);
  populateData(fpte, te, 82);
  populateData(fpk, k, 33);
  populateData(fpdst, dst, 32);
  // printf ("Done populating!\n");
  
  int i, j;
  /*
  for (j = 0; j < 33; j++){
    printf ("%s\n", k[j].team);
  }
  */
  getsalaries();
  /*
  for (j = 0; j < 33; j++){
    printf ("%s %d \n",k[j].name,  k[j].salary);
  }
  */
 // makeRosters();
 
  int  z = 0;
  for(;z < 32 ; z++){
    struct plays teamPlays[100];
    teams[z] = &teamPlays;
  }

char *prefix = "playcsv/Fantasy_";
char str[80];
//strcat (str, foo);


  FILE *teamplays;
  for (z = 0; z < 32; z++) {
	strcpy(str,  prefix);
	strcat (str, dst[z].name);
	strcat( str, ".csv");
	
	printf("%s \n" , str);
       
	if(access(str, F_OK) != -1){
	  printf("Yes \n");
 	  teamplays = fopen(str,"r" );
	  readPlays (teamplays,teams[z], 20);
	
    	  fclose(teamplays);
	}else{
	  printf("No \n");

	}
  }
/*
  for (i = 0; i < 5; i++) {
    j = i * 100000;
    printf ("QB: %s Salary: %d\n", qb[rosters1h[j].qb].name, qb[rosters1h[j].qb].salary);
    printf ("RB: %s Salary: %d\n", rb[rosters1h[j].rb1].name, rb[rosters1h[j].rb1].salary);
    printf ("RB: %s Salary: %d\n", rb[rosters1h[j].rb2].name, rb[rosters1h[j].rb2].salary);
    printf ("WR: %s Salary: %d\n", wr[rosters1h[j].wr1].name, wr[rosters1h[j].wr1].salary);
    printf ("WR: %s Salary: %d\n", wr[rosters1h[j].wr2].name, wr[rosters1h[j].wr2].salary);
    printf ("WR: %s Salary: %d\n", wr[rosters1h[j].wr3].name, wr[rosters1h[j].wr3].salary);
    printf ("TE: %s Salary: %d\n", te[rosters1h[j].te].name, te[rosters1h[j].te].salary);
    printf ("K: %s Salary: %d\n", k[rosters1h[j].k].name, k[rosters1h[j].k].salary);
    printf ("DST: %s Salary: %d\n", dst[rosters1h[j].dst].name, dst[rosters1h[j].dst].salary);
    printf ("Total Salary: %d\n\n", rosters1h[j].total_amount);
  }
 */ 
 // printf ("\nNumber of rosters!!!!! %d\n", idx1h + idx2h);
return 0;
}
