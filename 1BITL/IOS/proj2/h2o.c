#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h> //len
#include <semaphore.h>
#include <time.h> //srand
#include <stdbool.h>

#define OK 0
#define ERROR_WRONG_PARAMS 1
#define ERROR_WRONG_NUMBER 2
#define OXYGEN false
#define HYDROGEN true

typedef struct Shared {
  int O_counter; //pocitadlo kysliku
  int H_counter; //pocitadlo vodiu
  FILE* fp; //soubor
  int process_counter; //pocitadlo operaci
  int bonding; //pocitdlo atomu po bonding
  int O_id;//identifikace kysliku
  int H_id;//identifikace vodiku
  int N; //pocet atomu kysliku
  int B; //pocitaldo atomu po bonded
  int do_B; //cas pro upsani pri bonding
} shared_mem;

typedef struct Params //struktura pro uchování parametru
{
  int N; // pocet atomu kysliku
  int gen_H; //upsani kysliku
  int gen_O; //uspani vodiku
  int do_B; //uspani bonding
  int error; //error
} params_t;

params_t do_params(int argc, char **argv); //sortovani parametru
void error(int error_code); //fuknce pro tisk erroru
void gen_hydrogen(params_t parametry, int *list_H); //pomocna funkce pro generovani vodiku
void gen_oxygen(params_t parametry, int *list_O); //pomocna funkce pro generovani kysliku
void hydrogen(); //telo vodiku
void oxygen(); //telo kysliku
int give_sem(); //vytvoreni semaforu
void end(); //zniceni semaforu
void bond(int ID, bool element); //funkce pro 

shared_mem sharedo;
shared_mem *shared = &sharedo;//nelze dat NULL - segfault, nelze nechat bez definice - warning
int shmid_shm = 0;
sem_t*mutex = NULL;
int shmid_mutex = 0;
sem_t*hydrogen_q = NULL;
int shmid_hydrogen_q = 0;
sem_t*finished = NULL;
int shmid_finished = 0;
sem_t*oxygen_q = NULL;
int shmid_oxygen_q = 0;
sem_t*bonded = NULL;
int shmid_bonded = 0;
sem_t*now_bonding = NULL;
int shmid_now_bonding = 0;
sem_t*end_bonding = NULL;
int shmid_end_bonding = 0;

    

int main (int argc, char *argv[])
{
  
  params_t my_params = do_params(argc, argv); //vytvoreni parametru
  
  if(my_params.error) //pokud se spatne zpracuji parametry, tisku error, vracim 1
  {
    error(my_params.error);
    return 1;
  }
  
  int list_H [my_params.N * 2]; //pole pro ulozeni PIDu vodiku
  int list_O [my_params.N]; //pole pro ulozeni PIDu kysliku
  int list_HELP [2]; //pole pro ulozeni PIDu pomocnych procesu
  
  memset(list_H,0,my_params.N * 2 * sizeof(int));
  memset(list_O,0,my_params.N * sizeof(int));
  memset(list_HELP,0,2 * sizeof(int));
  
  srand(time(NULL)); //nahodna cisla
  

/* alokace sdilene pameti */
  shmid_shm = shmget(IPC_PRIVATE , sizeof(shared_mem), IPC_CREAT | IPC_EXCL | 0600); 
  if (shmid_shm == -1) //kontrola spravneho prubehu
  {
    fprintf(stderr, "Error: shmget\n");
    return 2;
  }
  shared = (shared_mem*)shmat(shmid_shm, NULL, 0);
  if (shared == (shared_mem*)-1) //kontrola spravneho prubehu
  {
    fprintf(stderr, "Error: shmat\n");
    return 2;
  }
  
  shared->process_counter = 0; //nastaveni pocitadla procesu na 0
  shared->H_id = 1; //ID prvniho vodiku
  shared->O_id = 1; //ID prvniho kysliku
  shared->H_counter = 0; //pocitaldo vodiku
  shared->O_counter = 0; //pocitadlo kysliku
  shared->bonding = 0; //pocet atomu, ktere provedly bonding
  shared->N = my_params.N; //parametr N
  shared->B = 0; //pocitadlo po provedeni bonded
  shared->do_B = my_params.do_B; //holni mez casu pro uspani bonding
  shared->fp = fopen("h2o.out","w+"); //otevreni souboru pro zapis
  setbuf(shared->fp, NULL);
    
  if(give_sem()) //vytvoreni semaforu, pokud probehbe neuspesne vraci 2
  {
    end();
    return 2;
  }
  
  
  if(fork()) //1. fork pro pomocny proces vodiku
  {  
    if(fork()) //2. fork pro pomocny proces kysliku
    {
    }
    else
    {
      list_HELP[0] = getpid(); //ulozeni PID pomocneho procesu kysliku
      gen_oxygen(my_params, list_O); //zavolani funkce pro generovani kysliku
      exit(0); //ukonceni procesu
    }
  }
  else
  {
    list_HELP[1] = getpid(); //ulozeni PID pomocneho kysliku
    gen_hydrogen(my_params, list_H);//zavolani funkce pro generovani vodiku
    exit(0); //ukonceni procesu
  }
  
  waitpid(list_HELP[0], NULL, 0); //cekam na ukonceni pomocneho procesu pro kyslik
  waitpid(list_HELP[1], NULL, 0); //cekam na ukonceni pomocneho procesu pro vodik
  
  fclose(shared->fp);
  end(); //zniceni semaforu a sdilene pameti
  return 0;
}


void gen_hydrogen(params_t parametry, int *list_H)
{
  pid_t pid; //promenna pro ulozeni PIDu atomu vodiku
  int sleep; //promenna pro cas uspani procesu
  
  for (int i = 0; i < (parametry.N*2); ++i) //vytvoreni potrebneho poctu atomu
  {
    sleep = rand() % (parametry.gen_H+1); //generovani casu pro uspani
    usleep(sleep*1000); //uspani na zadany cas
    pid = fork(); //vytvoreni procesu
    if (pid) 
    {
      list_H[i] = pid; //ulozeni PIDu vodiku
      continue;
    } 
    else if (pid == 0) 
    {
      hydrogen(); //zavolani procesu pro vodik
      exit(0); //ukonceni procesu
    } 
    else 
    {
      printf("fork error\n"); //pokud se nepovede fork
      exit(2);
    }
  }
  
  for(int k = 0; k < (parametry.N*2); k++) //cekam na ukonceni vsech atomu vodiku
  {
    waitpid(list_H[k], NULL, 0);
  }
  
  return;
}

void gen_oxygen(params_t parametry, int *list_O)
{
  pid_t pid; //pro ulozeni PID atomu kysliku
  int sleep; //promenna pro cas uspani procesu
  
  for (int i = 0; i < parametry.N; ++i) //vytvoreni potrebneho poctu atomu
  {
    sleep = rand() % (parametry.gen_O+1); //generovani casu pro uspani
    usleep(sleep*1000);//uspani na zadany cas
    pid = fork(); //vytvoreni procesu
    if (pid) 
    {
      list_O[i] = pid; //ulozeni PIDu atomu kysliku
      continue;
    }
    else if (pid == 0)
    {
      oxygen(); //zavolani tela atomu kysliku
      exit(0); //ukonceni procesu
    } 
    else 
    {
      printf("fork error\n");
      exit(2);
    }
  }
  
    for(int l = 0; l < (parametry.N); l++) //cekam na ukonceni vsech atomu vodiku
    {
      waitpid(list_O[l], NULL, 0);
    }
  
  return;
}

void hydrogen()
{
  bool element = HYDROGEN; //priznak pro prvek
  
  sem_wait(mutex); //uzamceni pristupu do sdilene pameti
    int ID = shared->H_id; //ulozim ID atomu
    shared->H_id++; //zvysim pocitadlo ID atomu
    shared->process_counter++; //zvyseni pocitadla operaci
    fprintf(shared->fp,"%d\t: H %d\t: started\n",shared->process_counter,ID);
  sem_post(mutex); //otevreni pristupu do sdilene pameti
  
  
  sem_wait(now_bonding); //pokud probiha bonding cekam na signal
  
  shared->H_counter++; //zvyseni pocitadla vodiku, ktere jsou pripravene
  
  if(shared->H_counter >=2 && shared->O_counter >= 1) //pokud je potrebny pocet atomu
  {
    sem_wait(mutex); //uzamceni pristupu do sdilene pameti
      shared->process_counter++; //zvyseni pocitadla operaci
      fprintf(shared->fp,"%d\t: H %d\t: ready\n",shared->process_counter,ID);
    sem_post(mutex); //otevreni pristupu do sdilene pameti
    
    sem_post(hydrogen_q); //posilam signal do fronty vodiku
    sem_post(hydrogen_q); //posilam signal do fronty vodiku
    
    shared->H_counter -= 2; //snizim pocet pripravenych vodiku o 2
    
    sem_post(oxygen_q); //posilam signal do fronty kysliku
    
    shared->O_counter--; //snizim pocet pripravenych kysliku o 1
  }
  else
  {
    sem_wait(mutex); //uzamceni pristupu do sdilene pameti
      shared->process_counter++; //zvyseni pocitadla operaci
      fprintf(shared->fp,"%d\t: H %d\t: waiting\n",shared->process_counter,ID);
    sem_post(mutex); //otevreni pristupu do sdilene pameti
    
    sem_post(now_bonding); //nespustil se bonding -> posilam signal pro dalsi atom ke kontrole
  }
  
  sem_wait(hydrogen_q); //cekam ve fronte dokud neprijde signal
  bond(ID, element); //spoustim bonding
  
  sem_wait(bonded); //cekam dokud neprijde signal, ze byl bonding proveden 3x
  sem_post(bonded); //posilam signal pro dalsi atomy, co cekaji
  
  sem_wait(mutex); //uzamceni pristupu do sdilene pameti
    shared->process_counter++; //zvyseni pocitadla operaci
    fprintf(shared->fp,"%d\t: H %d\t: bonded\n",shared->process_counter,ID);
    shared->B++; //pocitadlo bonded
  
    if((shared->B%3) == 0) //pokud probehl bonded 3x
    {
      sem_post(end_bonding); //posilam signal, ze probehl 3x a bonding muze byt ukoncen
    }
  sem_post(mutex); //otevreni pristupu do sdilene pameti

  
  
  sem_wait(mutex); //uzamceni pristupu do sdilene pameti
  if(shared->B == (shared->N*3)) //cekam, az budou vsechny procesy atomu provedeny
    sem_post(finished); //poslu signal pro tisk finnish
  sem_post(mutex); //otevreni pristupu do sdilene pameti
  
  sem_wait(finished); //cekam na signal, ze byly provedeny vsechny procesy atomu
  sem_post(finished); //posilam signal pro dalsi proces atomu
    
  sem_wait(mutex); //uzamceni pristupu do sdilene pameti
    shared->process_counter++; //zvyseni pocitadla operaci
    fprintf(shared->fp,"%d\t: H %d\t: finished\n",shared->process_counter,ID);
  sem_post(mutex); //otevreni pristupu do sdilene pameti
}

void oxygen()
{
  bool element = OXYGEN; //priznak pro prvek
  
  sem_wait(mutex); //uzamceni pristupu do sdilene pameti
    int ID = shared->O_id; //ulozeni ID atomu
    shared->O_id++; //zvyseni ID atomu kysliku
    shared->process_counter++; //zvyseni pocitadla operaci
    fprintf(shared->fp,"%d\t: O %d\t: started\n",shared->process_counter,ID);
  sem_post(mutex); //otevreni pristupu do sdilene pameti
  
  sem_wait(now_bonding); //pokud probiha bonding cekam na signal
  
  shared->O_counter++; //zvyseni pocitadla kysliku, ktere jsou pripravene
  
  if(shared->H_counter >=2) //pokud je potrebny pocet atomu
  {
    sem_wait(mutex);//uzamceni pristupu do sdilene pameti
      shared->process_counter++;//zvyseni pocitadla operaci
      fprintf(shared->fp,"%d\t: O %d\t: ready\n",shared->process_counter,ID);
    sem_post(mutex);//otevreni pristupu do sdilene pameti
    
    sem_post(hydrogen_q); //posilam signal do fronty vodiku
    sem_post(hydrogen_q); //posilam signal do fronty vodiku
    
    shared->H_counter -= 2; //snizim pocet pripravenych vodiku o 2
    
    sem_post(oxygen_q); //posilam signal do fronty kysliku
    
    shared->O_counter--; //snizim pocet pripravenych kysliku o 1
  }
  else
  {
    sem_wait(mutex); //uzamceni pristupu do sdilene pameti
      shared->process_counter++; //zvyseni pocitadla operaci
      fprintf(shared->fp,"%d\t: O %d\t: waiting\n",shared->process_counter,ID);
    sem_post(mutex); //otevreni pristupu do sdilene pameti
    
    sem_post(now_bonding); //nespustil se bonding -> posilam signal pro dalsi atom ke kontrole
  }
  
  sem_wait(oxygen_q); //cekam ve fronte, dokud neprijde signal
  bond(ID, element); //spoustim bonding
  
  sem_wait(bonded); //cekam dokud neprijde signal, ze byl bonding proveden 3x
  sem_post(bonded); //posilam signal pro dalsi atomy, co cekaji
  
  
  sem_wait(mutex); //uzamceni pristupu do sdilene pameti
    shared->process_counter++; //zvyseni pocitadla operaci
    fprintf(shared->fp,"%d\t: O %d\t: bonded\n",shared->process_counter,ID);
    shared->B++; //pocitadlo bonded
  
    if((shared->B%3) == 0) //pokud probehl bonded 3x
    {
      sem_post(end_bonding); //posilam signal, ze probehl 3x a bonding muze byt ukoncen
    }
  sem_post(mutex); //otevreni pristupu do sdilene pameti
  
  
  sem_wait(end_bonding); //cekam na signal, ze bonded probehl 3x
  sem_wait(bonded); //uzamykam semafor pro bonded
  sem_post(now_bonding); //ukoncuji bonding, posilam signal pro kontrolu dalsich atomu
  
  sem_wait(mutex); //uzamceni pristupu do sdilene pameti
    if(shared->B == (shared->N*3)) //cekam, az budou vsechny procesy provedeny
      sem_post(finished); //poslu signal pro tisk finnish   
  sem_post(mutex); //otevreni pristupu do sdilene pameti
  
  sem_wait(finished); //cekam na signal, ze byly provedeny vsechny procesy atomu
  sem_post(finished); //posilam signal pro dalsi proces atomu
    
  sem_wait(mutex); //uzamceni pristupu do sdilene pameti
    shared->process_counter++; //zvyseni pocitadla operaci
    fprintf(shared->fp,"%d\t: O %d\t: finished\n",shared->process_counter,ID);
  sem_post(mutex); //otevreni pristupu do sdilene pameti
    
}

void bond(int ID, bool element)
{
  int sleep; //promenna pro generovani casu na uspani
  
  sem_wait(mutex); //uzamceni pristupu do sdilene pameti
    sleep = rand() % (shared->do_B+1); //generovani nahodneho cisla
    usleep(sleep*1000); //uspani procesu
    shared->bonding++; //zvyseni pocitadla bondingu, ktere probehly
    shared->process_counter++; //zvyseni pocitadla operaci
    if(element) 
    {
      fprintf(shared->fp,"%d\t: H %d\t: begin bonding\n",shared->process_counter,ID);
    }
    else
    {
      fprintf(shared->fp,"%d\t: O %d\t: begin bonding\n",shared->process_counter,ID);
    }
  
    if((shared->bonding%3) ==0) //pokud probehl 3. bonding
    {
      sem_post(bonded); //posilam signal, ze muze probihat bonded
    }
  sem_post(mutex); //otevreni pristupu do sdilene pameti
  
}


int give_sem()
{
  shmid_mutex = shmget(IPC_PRIVATE , sizeof(sem_t), IPC_CREAT | IPC_EXCL | 0600);
  if (shmid_mutex == -1)
  {
    fprintf(stderr, "Error: shmget\n");
    return 1;
  }
  mutex = (sem_t *)shmat(shmid_mutex, NULL, 0);
  if (mutex == (sem_t *)-1)
  {
    fprintf(stderr, "Error: shmat(sem)n");
    return 1;
  }
  sem_init(mutex, 1, 1);
    
    
  shmid_hydrogen_q = shmget(IPC_PRIVATE , sizeof(sem_t), IPC_CREAT | IPC_EXCL | 0600);
  if (shmid_hydrogen_q == -1)
  {
    fprintf(stderr, "Error: shmget\n");
    return 1;
  }
  hydrogen_q = (sem_t *)shmat(shmid_hydrogen_q, NULL, 0);
  if (hydrogen_q == (sem_t *)-1)
  {
    fprintf(stderr, "Error: shmat(sem)n");
    return 1;
  }
  sem_init(hydrogen_q, 1, 0);
    
    
  shmid_finished = shmget(IPC_PRIVATE , sizeof(sem_t), IPC_CREAT | IPC_EXCL | 0600);
  if (shmid_finished == -1)
  {
    fprintf(stderr, "Error: shmget\n");
    return 1;  
  }
  finished = (sem_t *)shmat(shmid_finished, NULL, 0);
  if (finished == (sem_t *)-1)
  {
    fprintf(stderr, "Error: shmat(sem)n");
    return 1;
  }
  sem_init(finished, 1, 0);
 
  
  shmid_oxygen_q = shmget(IPC_PRIVATE , sizeof(sem_t), IPC_CREAT | IPC_EXCL | 0600);
  if (shmid_oxygen_q == -1)
  {
    fprintf(stderr, "Error: shmget\n");
    return 1;
  }
  oxygen_q = (sem_t *)shmat(shmid_oxygen_q, NULL, 0);
  if (oxygen_q == (sem_t *)-1)
  {
    fprintf(stderr, "Error: shmat(sem)n");
    return 1;
  }     
  sem_init(oxygen_q, 1, 0);
    
  
  shmid_bonded = shmget(IPC_PRIVATE , sizeof(sem_t), IPC_CREAT | IPC_EXCL | 0600);
  if (shmid_bonded == -1)
  {
    fprintf(stderr, "Error: shmget\n");
    return 1;
  }
  bonded = (sem_t *)shmat(shmid_bonded, NULL, 0);
  if (bonded == (sem_t *)-1)
  {
    fprintf(stderr, "Error: shmat(sem)n");
    return 1;
  }    
  sem_init(bonded, 1, 0);
    
  
  shmid_now_bonding = shmget(IPC_PRIVATE , sizeof(sem_t), IPC_CREAT | IPC_EXCL | 0600);
  if (shmid_now_bonding == -1)
  {
    fprintf(stderr, "Error: shmget\n");
    return 1;
  }
  now_bonding = (sem_t *)shmat(shmid_now_bonding, NULL, 0);
  if (now_bonding == (sem_t *)-1)
  {
    fprintf(stderr, "Error: shmat(sem)n");
    return 1;
  }    
  sem_init(now_bonding, 1, 1);
    
  
  shmid_end_bonding = shmget(IPC_PRIVATE , sizeof(sem_t), IPC_CREAT | IPC_EXCL | 0600);
  if (shmid_end_bonding == -1)
  {
    fprintf(stderr, "Error: shmget\n");
    return 1;
  }
  end_bonding = (sem_t *)shmat(shmid_end_bonding, NULL, 0);
  if (end_bonding == (sem_t *)-1)
  {
    fprintf(stderr, "Error: shmat(sem)n");
    return 1;
  }    
  sem_init(end_bonding, 1, 0);
    
    
  return 0;
}

params_t do_params(int argc, char **argv)
{
  params_t my_params;
  my_params.error = OK;
  
  if (argc != 5)
  {
    printf("lamo");
    my_params.error = ERROR_WRONG_PARAMS;
    return my_params;
  }
  else
  {
    char *ptr;
    my_params.N = (int)strtol(argv[1],&ptr,10);
    if(my_params.N > 5000 || my_params.N < 0 || *ptr != '\0')
      my_params.error = ERROR_WRONG_NUMBER;
      
    my_params.gen_H = (int)strtol(argv[2],&ptr,10);
    if(my_params.gen_H > 5000 || my_params.gen_H < 0 || *ptr != '\0')
      my_params.error = ERROR_WRONG_NUMBER;
      
    my_params.gen_O = (int)strtol(argv[3],&ptr,10);
    if(my_params.gen_O > 5000 || my_params.gen_O < 0 || *ptr != '\0')
      my_params.error = ERROR_WRONG_NUMBER;
      
    my_params.do_B = (int)strtol(argv[4],&ptr,10);
    if(my_params.do_B > 5000 || my_params.do_B < 0 || *ptr != '\0')
      my_params.error = ERROR_WRONG_NUMBER;
    
    ptr = NULL;
  }
  
  //printf("N=%d, GH=%d, GO=%d, B=%d\n",my_params.N,my_params.gen_H,my_params.gen_O,my_params.do_B);
  
  return my_params;
}

void error(int error_code) 
{ 
  switch(error_code) 
  { 
    case ERROR_WRONG_PARAMS: 
      fprintf(stderr, "\n\nChybny pocet parametru prikazove radky! Program bude ukoncen. \n\n");  
      break; 
    case ERROR_WRONG_NUMBER: 
      fprintf(stderr, "\n\nZadane cislo mimo rozsah nebo nespravne zadano! Program bude ukoncen. \n\n");  
      break; 
    default: 
      break; 
  } 
  return;
}

void end()
{    

    sem_destroy(mutex);
    sem_destroy(hydrogen_q);
    sem_destroy(finished);
    sem_destroy(oxygen_q);
    sem_destroy(bonded);
    sem_destroy(now_bonding);
    sem_destroy(end_bonding);
    
    shmdt((void *)shared);
    shmctl(shmid_shm, IPC_RMID, NULL);
    shared = NULL;

    shmdt((void *)mutex);
    shmctl(shmid_mutex, IPC_RMID, NULL);
    mutex = NULL;
    
    shmdt((void *)hydrogen_q);
    shmctl(shmid_hydrogen_q, IPC_RMID, NULL);
    hydrogen_q = NULL;
    
    shmdt((void *)finished);
    shmctl(shmid_finished, IPC_RMID, NULL);
    finished = NULL;
    
    shmdt((void *)now_bonding);
    shmctl(shmid_now_bonding, IPC_RMID, NULL);
    now_bonding = NULL; 
 
    shmdt((void *)end_bonding);
    shmctl(shmid_end_bonding, IPC_RMID, NULL);
    end_bonding = NULL;    
 
    shmdt((void *)bonded);
    shmctl(shmid_bonded, IPC_RMID, NULL);
    bonded = NULL;
    
    shmdt((void *)oxygen_q);
    shmctl(shmid_oxygen_q, IPC_RMID, NULL);
    oxygen_q = NULL;
}