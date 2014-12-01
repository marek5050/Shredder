#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

struct player {
  int rank;
  char name[20];
  char team[20];
  char matchup[20];
  int best_rank;
  int worst_rank;
  double avg_rank;
  double std_dev;
};

void copyMe(char *from, char *to){
  while(*to++=*from++);
}

int main () {
  FILE *fp;
  fp = fopen("FantasyPros_2014_Week_12_QB_Rankings.csv", "r");
  struct player playersarr[36]; //= malloc (36 * sizeof (struct player*));
  //struct player * pointme;
  char curplayer[300];
  int i = 0;
  //fscanf (fp, "%s\n", curplayer);
  fgets(curplayer, 300, fp);

  while (fgets(curplayer, 300, fp) != EOF){
    //fscanf (fp, "%100s", curplayer);
    printf ("%s\n", curplayer);
    
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
    tok = strtok(NULL, ",");
    sscanf(tok, "%lf", &playersarr[i].std_dev);
   
    i++;
    if (i == 36){
      break;
    }
  }
  printf ("Done reading!\n");
  int j;
  for (j = 0; j < 36; j++){
    printf ("%s\n", playersarr[j].team);
  }
   
}
