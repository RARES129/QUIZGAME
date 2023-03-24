#include "client.h"

void joc()
{
            asteapta1();
            i=0;
			while(i<10)
            {
			
			if(read( socket_descriptor,&question,sizeof(question)) < 0){
			    eroareCitire();
            	exit(0);
            }
            
            system("clear");
            printf("%s \n",question.question);
            fflush(stdout);
            printf("%s\n",question.r1);	
            fflush(stdout);
            printf("%s\n",question.r2);	
            fflush(stdout);
            printf("%s\n",question.r3);	
            fflush(stdout);
            printf("%s\n",question.r4);
            fflush(stdout);
            
            sec10();
            
            if(poll(&timer, 1, 10000))
                {
                    scanf("%s", answer);

                    if(strcmp(answer,"exit")==0)
                        parasit();
                }
            else
                {
                    timp0();
                    sleep(1);
                }
            while(strcmp(answer,"a")!=0 && strcmp(answer,"b")!=0 && strcmp(answer,"c")!=0 && strcmp(answer,"d")!=0 && strcmp(answer,"")!=0)
                {
                    formulatGresit();
                }


            if(strcmp(answer,"")!=0 && strcmp(answer,question.rc)!=0)
                {
                    gresit();
                    sleep(1);
                }
            else if(strcmp(answer,"")!=0 && strcmp(answer,question.rc)==0)
                {
			        corect();
			        sleep(1);
                }
            answer[0]='\0';   
            i++;
           }

           if(write( socket_descriptor,&scor,sizeof(scor)) <= 0)
            {
                system("clear");
			    eroareScriere();
            	exit(0);
            }

            asteapta2();

            if(read(socket_descriptor,castigator,sizeof(castigator)) < 0)
            {
                system("clear");
			    eroareCitire();
            	exit(0);
            }
            loc1(castigator);
}

void meniu()
{
    while(1) 
    {
	i=0;
    while(i<3)
	    {
            printf("%s\n", meniu_principal[i]); 
            fflush(stdout);
            i++;
        }
		
        selectMeniu();

		scanf("%d", &button);

		if(write( socket_descriptor, &button, sizeof(button)) < 0) 
        {
            eroareCitire();
            exit(0);
        }
			

        if (button == 2)	
        {
            parasit(); 
        }
		else if(button == 1) 
        {
			joc();
            break;
        }
         system("clear");
	}
}

int main (int argc, char *argv[])
{

  port_server = atoi (argv[2]);

    socket_descriptor = socket (AF_INET, SOCK_STREAM, 0);
  if (socket_descriptor == -1)
    {
      eroareSocket();
      exit(0);
    }
  str_server.sin_family = AF_INET;
  str_server.sin_addr.s_addr = inet_addr(argv[1]);
  str_server.sin_port = htons (port_server);

  if (connect ( socket_descriptor, (struct sockaddr *) &str_server,sizeof (struct sockaddr)) == -1)
    {
      eroareConnServer();
      exit(0);
    }
    system("clear");
    
    while (1)
    {
        if (read( socket_descriptor, mesaj, sizeof(mesaj)) < 0) 
        {
            eroareCitire();
            exit(0);
        }
        printf("%s", mesaj);
        fflush(stdout);
        scanf("%s", ID);
        if (write( socket_descriptor, ID,sizeof(ID)) <= 0) 
        {
            eroareScriere();
            exit(0);
        }
        if (read( socket_descriptor, mesaj, sizeof(mesaj)) < 0) 
        {
            eroareCitire();
            exit(0);
        }
        system("clear");
        printf("%s", mesaj);fflush(stdout);
        scanf("%s", password);
        if (write( socket_descriptor, password, sizeof(password)) <= 0) 
        {
            eroareScriere();
            exit(0);
        }
        if (read( socket_descriptor, &conectat, sizeof(int)) < 0) 
        {
            eroareCitire();
            exit(0);
        }
        if (conectat!=1) 
        {
            connGresit();
        }
        else 
        {
            system("clear");
	        break;
        }
   }
    	if(read( socket_descriptor, meniu_principal, sizeof(meniu_principal)) < 0)
        {
    		eroareCitire();
            exit(0);
    	}	

    meniu();
	
  close (socket_descriptor);
}

