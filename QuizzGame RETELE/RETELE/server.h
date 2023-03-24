#include <poll.h>
#include <strings.h>
#include <sqlite3.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#define PORT 3000

extern int errno;
int jucatori = 0;

typedef struct inf_thread{
	int th_id; 
	int descriptor; 
}inf_thread;

struct questions{
    int nr;
    char question[256];
    char r1[256];
    char r2[256];
    char r3[256];
    char r4[256];
    char rc[64];
}intrebare[20];

const char* data = "Callback function called";
int conectati=0;
int nrx=0;
int x=1;
int n;
char meniu[4][100];
char SQL_comanda[256];

struct sockaddr_in server;
struct sockaddr_in addr;	
int socketDescriptor;		
pthread_t th[100];    
int contor=1;
int punctajMaxim = -1;
int id_punctajMaxim = 0;
int contorIntrebari = 0;

int ct_finish = 0;
int punctaje[20] = {0};
char castigator[20][128];


void  ctrlC(int sig)
{
     char  Z;
     signal(sig, SIG_IGN);
     printf("\nSUNTETI SIGUR CA DORITI SA INCHIDETI SERVERUL? [Y/N]\n");
     Z = getchar();
     if (Z == 'y' || Z == 'Y')
          exit(0);
     else
          signal(SIGINT, ctrlC);
     getchar();
}

int eroareSocket()
{

      perror ("\nTI-A DAT EROARE BRO, IA GHICI UNDE... LA SOCKET().\n");
      return errno;
}

int eroareBind()
{

      perror ("\nTI-A DAT EROARE BRO, IA GHICI UNDE... LA BIND().\n");
      return errno;
}

int eroareListen()
{

      perror ("\nTI-A DAT EROARE BRO, IA GHICI UNDE... LA LISTEN().\n");
      return errno;
}

int eroareAccept()
{

      perror ("\nTI-A DAT EROARE BRO, IA GHICI UNDE... LA ACCEPT().\n");
      return errno;
}

void waitPort()
{
      printf ("ASTEAPTAM CONECTAREA CLIENTILOR LA PORTUL %d...\n",PORT);
      fflush (stdout);
}

void waitThread(int th_id)
{
      printf ("ASTEPTAM SA SE CONECTEZE ... THREAD %d\n", th_id);
	fflush (stdout);	
}

int eroareCitire(int th_id)
{
    printf("THREAD %d ... \n", th_id);
    perror ("SORRY BRO, N-AM PUTUT CITI DE LA SERVER.... \n");
    return errno;
}

int eroareScriere(int th_id)
{
    printf("THREAD %d ... \n", th_id);
    perror("SORRY BRO, N-AM PUTUT TRANSMITE SPRE SERVER.... \n");
    return errno;
}

void logatCorect()
{
      printf("Utilizator logat!\n"); 
      fflush(stdout);
}

void SQL(const char* id,const char* parola)
{
    strcpy(SQL_comanda, "select * from users where ");
    strcat(SQL_comanda, "'");
    strcat(SQL_comanda, parola);
    strcat(SQL_comanda, "'");
    strcat(SQL_comanda, " = USERS.password and ");
    strcat(SQL_comanda, "'");
    strcat(SQL_comanda, id);
    strcat(SQL_comanda, "'");
    strcat(SQL_comanda, " = USERS.username");
}

void Winner()
{
      int j=1;
    	while(j<=conectati)
    	{	
    		if(punctaje[j] > punctajMaxim) {
    			punctajMaxim = punctaje[j];
    			id_punctajMaxim = j;
    		}
            j++;
    	}
}

void afisareWinner()
{
      printf("CASTIGATOR: %s\n",castigator[id_punctajMaxim]); 
      fflush(stdout);
      sleep(1);
     	ct_finish = 0;
}
