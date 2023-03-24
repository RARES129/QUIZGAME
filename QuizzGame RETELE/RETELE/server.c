#include "server.h"

static void *CreareThread(void *);
int logare(struct inf_thread thClient);
int VerificareLogare(const char*, const char*);
void selectareOptiune(struct inf_thread thClient);
int executie_joc(struct inf_thread thClient);
void asteptareClient();

void asteptareClient()
{
    while (1)
    {
      int player;
      inf_thread * thr;   
      int lungime = sizeof (addr);

      waitPort();

      if ( (player = accept (socketDescriptor, (struct sockaddr *) &addr, &lungime)) < 0)
	{
	  eroareAccept();
	  continue;
	}

	thr=(struct inf_thread*)malloc(sizeof(struct inf_thread));	
	thr->th_id=contor;
    contor++;
	thr->descriptor=player;

	pthread_create(&th[contor], NULL, &CreareThread, thr);	      		
	}  
}

static void *CreareThread(void * arg)
{		
		struct inf_thread thClient; 
		thClient= *((struct inf_thread*)arg);	
        int conectat=0;

        waitThread(thClient.th_id);

		pthread_detach(pthread_self());		

        conectat=logare(thClient);
        if(conectat == 1)
		    selectareOptiune(thClient);

		close ((intptr_t)arg);
		return(NULL);		
};


int logare(struct inf_thread thClient) {

    char id_primit[128];
    char parola_primita[128];

    int logat;
    while (logat!=1) 
    {
        if (write (thClient.descriptor, "USERNAME: ", sizeof("USERNAME: ")) <= 0) 
            eroareScriere(thClient.th_id);
        if (read (thClient.descriptor, id_primit, sizeof(id_primit)) <= 0)
            eroareCitire(thClient.th_id);
        if (write (thClient.descriptor, "PASSWORD: ", sizeof("PASSWORD: ")) <= 0) 
            eroareScriere(thClient.th_id);
        if (read (thClient.descriptor, parola_primita, sizeof(parola_primita)) <= 0) 
            eroareCitire(thClient.th_id);

        logat = VerificareLogare(id_primit, parola_primita);

        if (write (thClient.descriptor, &logat, sizeof(int)) <= 0) 
            eroareScriere(thClient.th_id);
    }
    if (logat==1) 
     {
        logatCorect();
        conectati++;
        strcpy(castigator[thClient.th_id],id_primit);
	  	return 1;
     }
    else return 0;
}


int VerificareLogare(const char* id, const char* parola) 
{
    SQL(id,parola);
    char *msgEroare = 0;
    struct sqlite3 *dataBase;
    int result = sqlite3_open("baza.db", &dataBase);
    
    if(result)
    {
        fprintf(stderr, "N-AM PUTUT DESCHIDE BAZA DE DATE %s\n", 
        sqlite3_errmsg(dataBase));
        sqlite3_close(dataBase);
        return 0;
    }
    else
    {
      fprintf(stderr, "AM DESCHIS BAZA DE DATE CU SUCCES\n");
    }

    struct sqlite3_stmt *statement;
    result = sqlite3_prepare_v2(dataBase, SQL_comanda, -1, &statement, NULL);

    if(result == SQLITE_OK)
    {
       if (sqlite3_step(statement) == SQLITE_ROW)
       {
            sqlite3_finalize(statement);
            sqlite3_close(dataBase);
            return 1;
       }
       else
       {
            fprintf(stderr, "EROARE SQL %s\n", msgEroare);
            sqlite3_free(msgEroare);
            sqlite3_close(dataBase);
            return 0;
       }
    }
   
}


void selectareOptiune(struct inf_thread thClient) 
{

    printf("CONECTATI: %d\n",conectati);
    int o=0;
	if(write(thClient.descriptor, meniu, sizeof(meniu)) <= 0)
     	{
            	eroareScriere(thClient.th_id);
                conectati--;
                printf("CONECTATI: %d\n",conectati);
                o=1;
        }

	while(o==0) 
    {
		int optiune;
		if(read(thClient.descriptor, &optiune, sizeof(optiune)) <= 0) 
        {
           	eroareCitire(thClient.th_id);
            conectati--;
             printf("CONECTATI: %d\n",conectati);
            break;
        }
        if(optiune == 1) 
        {
            jucatori++ ;
            while (	jucatori < 2 || jucatori!=conectati);
			    sleep(1);

            printf("\n PLAY PLAY PLAY\n");
            executie_joc(thClient);
            jucatori=0;
            conectati=0;
            break;
        }
        else if( optiune == 2)
        {
            conectati--;
            printf("CONECTATI: %d\n",conectati);
        	break;
        }
	}
}


int callback(void *DB, int argc, char **argument, char **numeCol) 
{
    
        intrebare[nrx].nr = atoi(argument[0]);
        strcpy(intrebare[nrx].question,argument[1]);
        strcpy(intrebare[nrx].r1,argument[2]);
        strcpy(intrebare[nrx].r2,argument[3]);
        strcpy(intrebare[nrx].r3,argument[4]);
        strcpy(intrebare[nrx].r4,argument[5]);
        strcpy(intrebare[nrx].rc,argument[6]);
        nrx++;
        return 0;
}

void citireIntrebari()
{
    char *msgEroare = 0;
    struct sqlite3 *dataBase;
    int result = sqlite3_open("baza.db", &dataBase);
    
    if(result)
    {
        fprintf(stderr, "(intrebari) N-AM PUTUT DESCHIDE BAZA DE DATE %s\n", 
        sqlite3_errmsg(dataBase));
        sqlite3_close(dataBase);
        exit(0);
    }
    else
    {
      fprintf(stderr, "(intrebari) AM DESCHIS BAZA DE DATE CU SUCCES\n");
    }

    result = sqlite3_exec(dataBase, "SELECT * FROM INTREBARI", callback, 0, &msgEroare);
    

    if(result == SQLITE_OK)
    {
       fprintf(stderr, "AM SELECTAT INTREBARILE CU SUCCES\n");
       sqlite3_close(dataBase);
    }
    else
    {
        fprintf(stderr, "NU AM REUSIT SA SELECTAM INTREBARILE CU SUCCES\n");
        fprintf(stderr, "EROARE : %s\n", msgEroare);
        sqlite3_free(msgEroare);
        sqlite3_close(dataBase);
        sleep(1);
        exit(0);
    }
}



int executie_joc(struct inf_thread thClient)
{
    int folosit;
    int intrebariUzate[11] = {0};
    int m;
    int q;
    punctajMaxim = 0;
    id_punctajMaxim = 0;
    contorIntrebari = 0;
    int punctaj;
    char id[128];
    
     m=1;
    while(m<=10) 
    {
        while(1) 
        {
            iar:
            folosit = 0;
            srand(time(NULL));
            n = rand() % 20;

            q=1;
            while(q<=contorIntrebari)
            {
                if(n == intrebariUzate[q])
                    {
                        folosit = 1; 
                        break;
                    }
                q++;
            }
            
            if( folosit == 1 )
                goto iar;
            else 
            {
                intrebariUzate[++contorIntrebari] = n;
                break;
            }
        }                     
        if(write(thClient.descriptor,&intrebare[n],sizeof(intrebare[n])) < 0)
     	    eroareScriere(thClient.th_id);
    m++; 	
    }

    if(read(thClient.descriptor,&punctaj,sizeof(punctaj)) <= 0)
    {
			    eroareCitire(thClient.th_id);
            	return errno;
    }

    punctaje[thClient.th_id] = punctaj;
    ct_finish++;
    
    ast:
    if(ct_finish == conectati)
    {   
        Winner();
        if(write(thClient.descriptor,&castigator[id_punctajMaxim],sizeof(castigator[id_punctajMaxim])) <= 0)
     	   			eroareScriere(thClient.th_id);
    }
    else
        goto ast;
    	afisareWinner();
}

int main ()
{
    signal(SIGINT, ctrlC);
    
    printf ("PENTRU A OPRI SERVERUL -> 'CTRL + C'\n");

    citireIntrebari();
    
    strcpy(meniu[0],"---------------------------ASTA E MENIUL BAROSANE !!!---------------------------");
	strcpy(meniu[1],"                             1. PLAAAAAAAAAAAAAY            ");
	strcpy(meniu[2],"             2. EXIT (ca sa iesi in timpul jocului, doar scrie 'exit')       ");

  if ((socketDescriptor = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      eroareSocket();
      exit(0);
    }
  
  setsockopt(socketDescriptor,SOL_SOCKET,SO_REUSEADDR,&x,sizeof(x));

    bzero (&server, sizeof (server));
    bzero (&addr, sizeof (addr));

    server.sin_family = AF_INET;	
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);
  
  if (bind (socketDescriptor, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      eroareBind();
      exit(0);
    }

  if (listen (socketDescriptor, 3) == -1)
    {
      eroareListen();
      exit(0);
    }

    asteptareClient();
    return 0;
}	