#include <poll.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

struct questions{
    int nr;
    char question[256];
    char r1[256];
    char r2[256];
    char r3[256];
    char r4[256];
    char rc[64];
};
int port_server;
extern int errno;
  int socket_descriptor;			
  struct sockaddr_in str_server;
  struct questions question;
  char answer[20];
  char final_answer[64];
  int conectat;
  int scor = 0;
  char ID[128];
  char password[128];
  char mesaj[64];
  char castigator[128];
  char meniu_principal[4][100];
  int button;
  int i;
  struct pollfd timer = { STDIN_FILENO, POLLIN|POLLPRI };

int eroareConnServer()
{
    system("clear");
    perror ("UFFFF, NU S-A PUTUT CONECTA LA SERVER.\n");
    return errno;
}

int eroareSocket()
{
      system("clear");
      perror ("TI-A DAT EROARE BRO, IA GHICI UNDE... LA SOCKET().\n");
      return errno;
}

int eroareCitire()
{
    system("clear");
    perror ("SORRY BRO, N-AM PUTUT CITI DE LA SERVER.\n");
    return errno;
}

int eroareScriere()
{
    system("clear");
    perror("SORRY BRO, N-AM PUTUT TRANSMITE SPRE SERVER.\n");
    return errno;
}

void parasit()
{
    system("clear");
    printf("M-AI PARASIT, :( BYE BYE \n");
    fflush(stdout);
    exit(1);
}

void connGresit()
{
    system("clear");
    printf("MOAAAAAAAMA, AI GRESIT ID-UL SAU PAROLA, IA SI MAI BAGA O FISA !!!\n");
    fflush(stdout);
}

void gresit()
{
    printf("GINIOOOOOOOOOON, GRESIT GRESIT GRESIT\n"  );
    fflush(stdout);
}

void corect()
{
    printf("URAAAAAAAAA, CORECT CORECT CORECT\n"  );
	fflush(stdout);		        
    scor++;
}

void timp0()
{
    system("clear");
    printf("GINIOOOOOON, S-A DUS TIMPUL BRO.\n"  );
    fflush(stdout);
}

void asteapta2()
{
    system("clear");
    printf("\nASTEAPTA SA TERMINE SI CEILALTI !\n"); 
    fflush(stdout);
}

void asteapta1()
{
    system("clear");
    printf("\nHAI SA MAI STAM PUTIN SA SE CONECTEZE SI RESTU .\n");
    fflush(stdout);
}

void formulatGresit()
{
    printf("\nDOAR O LITERA DINTRE ASTEA MAN: (a,b,c,d)\n");            
    fflush(stdout);
    scanf("%s", answer);
                
    if(strcmp(answer,"exit")==0)
         parasit();
}

void sec10()
{
    printf("\nAI 10 SECUNDE, BAGA MARE !!!\n"  );
        fflush(stdout);
}

void loc1(char Castigator[])
{
    system("clear");
    printf("\n\n********************************************************************************\n                   CASTIHATORUL ESTEEEEEEEEEE : ---->  ");         

    printf("%s",Castigator);

    printf("  <----\n********************************************************************************\n");
    fflush(stdout);
}

void selectMeniu()
{
    printf("\nCARE-I URMATOAREA MISCARE MAN? : "); 
        fflush(stdout);
}