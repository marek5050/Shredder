#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>


static int MAX_QB = 38;
static int MAX_RB = 117;
static int MAX_WR = 150;
static int MAX_TE = 82;
static int MAX_K = 33;
static int MAX_DST = 32;


struct player {
  int rank;
  int salary;
  char name[20];
  char team[20];
  char matchup[20];
  int best_rank;
  int worst_rank;
  double avg_rank;
  double std_dev;
} qb[38], rb[117], wr[150], te[82], k[33], dst[32];


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
  printf ("Done reading!\n");
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
    printf ("%s\n", curplayer);
    char* tok;
    tok = strtok (curplayer, ",");
    //printf ("First: %s\n", tok);


    //sscanf(tok, "%s,%s,%d", NULL,NULL, salary);
    //printf("Salary: %d ", salary );
    copyMe(tok, role);
    copyMe(strtok(NULL, ","), name);
  ///  printf ("Name: %s\n", name);
    //printf ("name: %s\n", playersarr[i].name);
    //printf ("next String: %s\n", strtok(NULL, ","));
    salary = atoi(strtok(NULL, ","));
   /// printf("%d Salary!!!\n" , salary);
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
    }else if(!strcmp(role,"DST")){
	ptr= dst;
	max = MAX_DST;	
    }else if(!strcmp(role,"TE")){
 	ptr = te;
	max = MAX_TE;	
    }
   
    for(i = 0; i < max; i++ ){
  	if(strcmp(name,ptr[i].name)==0){	
	  	printf("%s %d \n",ptr[i].name, salary);	
		ptr[i].salary = salary;
    }
   }
  }
}

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
  printf ("Done populating!\n");
  int j;
  for (j = 0; j < 33; j++){
    printf ("%s\n", k[j].team);
  }
 
  getsalaries();

  for (j = 0; j < 33; j++){
    printf ("%s %s \n",k[j].name,  k[j].salary);
  }

return 0;
}
