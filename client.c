/* cliTCPIt.c - Exemplu de client TCP
   Trimite un numar la server; primeste de la server numarul incrementat.
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;
void handle_sigint(int sig)
{
    printf("\nV-ati deconectat! %d\n", sig);
    signal(SIGINT,SIG_DFL);

}
int main (int argc, char *argv[])
{
    int sd;			// descriptorul de socket
    struct sockaddr_in server;	// structura folosita pentru conectare
    // mesajul trimis

    /* exista toate argumentele in linia de comanda? */
    if (argc != 3)
    {
        printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    /* stabilim portul */
    port = atoi (argv[2]);

    /* cream socketul */
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("Eroare la socket().\n");
        return errno;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(argv[1]);
    /* portul de conectare */
    server.sin_port = htons (port);

    /* ne conectam la server */
    if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
        perror ("[client]Eroare la connect().\n");
        return errno;
    }

    /* citirea mesajului */

    char raspuns[1500];
    char buf[1000];

    while(1){

        printf ("[client]mesaj: ");
        bzero(buf,sizeof(buf));
        bzero(raspuns,sizeof(raspuns));
        signal(SIGINT, handle_sigint);
        fflush (stdout);

        read (0, buf, sizeof(buf));
        //scanf("%d",&nr);
        /* trimiterea mesajului la server */
        if (write (sd,&buf,sizeof(buf)) <= 0)
        {
            perror ("[client]Eroare la write() spre server.\n");
            return errno;
        }
        /* citirea raspunsului dat de server
           (apel blocant pina cind serverul raspunde) */
        bzero(raspuns,sizeof(raspuns));

        if (read (sd, &raspuns,sizeof(raspuns)) < 0)
        {
            perror ("[client]Eroare la read() de la server.\n");
            return errno;
        }

        int lungime=strlen(raspuns);

        if(strcmp(raspuns,"Acum ca v-ati creat un username valabil va puteti creea si o parola!") == 0 )
        {	printf ("[client]Mesajul primit este: %s\n", raspuns);
            printf("Create password: ");
            char buf1[100],raspuns1[100];

            fgets ( buf1, 100, stdin );
            printf("Am citit parola! %s\n",buf1);
            int lungime1=strlen(buf1);
            buf1[lungime1-1]='\0';

            int lungime2=strlen(buf1);

            write(sd,buf1,lungime2);
            read(sd,&raspuns1,sizeof(raspuns1));
            printf ("[client]Mesajul primit este: %s\n", raspuns1);
        }
        else if(strcmp(raspuns,"Acum va rugam introduceti parola!")==0)
        {

            printf("Password: ");
            char buf2[100],raspuns2[100];

            bzero(raspuns2,sizeof(raspuns2));
            fgets ( buf2, 100, stdin );
            printf("Am citit parola! %s\n",buf2);
            int lungime3=strlen(buf2);
            buf2[lungime3-1]='\0';

            int lungime4=strlen(buf2);

            write(sd,buf2,lungime4);

            read(sd,&raspuns2,sizeof(raspuns2));

            printf ("[client]Mesajul primit este: %s\n", raspuns2);
        }

        else if(strcmp(raspuns,"Multumim ca ati utilizat aplicatia! O zi buna!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n", raspuns);
            close(sd);
            return 0;
        }
        else if(strcmp(raspuns,"Acum ca ati adaugat o piesa trebuie sa ii adaugati si link-ul de pe youtube!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n", raspuns);
            printf("Adaugati link-ul: ");
            char buf3[100],raspuns3[100];
            ;
            bzero(raspuns3,sizeof(raspuns3));
            fgets ( buf3, 100, stdin );
            printf("Am citit link-ul! %s\n",buf3);
            int lungime5=strlen(buf3);
            buf3[lungime5-1]='\0';

            int lungime6=strlen(buf3);

            write(sd,buf3,lungime6);

            read(sd,&raspuns3,sizeof(raspuns3));
            printf("%s\n",raspuns3);
            printf ("[client]Mesajul primit este: %s\n", raspuns3);
            if(strcmp(raspuns3,"Url added!")==0)
            {
                char buf4[100],raspuns4[100];
                printf("Acum adaugati genurile muzicale ale piesei!\n");
                printf("Gendre: ");
                bzero(raspuns4,sizeof(raspuns4));
                fgets ( buf4, 100, stdin );
                printf("Am citit genurile ! %s\n",buf4);
                int lungime7=strlen(buf4);
                buf4[lungime7-1]='\0';

                int lungime8=strlen(buf4);

                write(sd,buf4,lungime8);

                read(sd,&raspuns4,sizeof(raspuns4));
                printf("%s\n",raspuns4);
                printf ("[client]Mesajul primit este: %s\n", raspuns4);
                if(strcmp(raspuns4,"Gendre added!")==0)
                {
                    char buf5[100],raspuns5[100];
                    printf("Acum adaugati descrierea piesei!\n");
                    printf("Descriere: ");
                    bzero(raspuns5,sizeof(raspuns5));
                    fgets ( buf5, 100, stdin );
                    printf("Am citit descrierea! %s\n",buf5);
                    int lungime9=strlen(buf5);
                    buf5[lungime9-1]='\0';
                    //printf("%d",lungime1);
                    int lungime10=strlen(buf5);

                    write(sd,buf5,lungime10);

                    read(sd,&raspuns5,sizeof(raspuns5));
                    printf("[client]Mesajul primit este: %s\n",raspuns5);

                }
            }
        }
        else if(strcmp(raspuns,"Sunteti deja logat, va rugam sa va delogati daca doriti sa utilizati alt cont!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strcmp(raspuns,"Nu sunteti logat ca sa va puteti deloga!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strcmp(raspuns,"Piesa a fost stearsa!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strcmp(raspuns,"Nu exista nicio piesa cu acest id!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strcmp(raspuns,"Am adaugat un vot la piesa!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strcmp(raspuns,"Nu sunteti administrator pentru a utiliza aceasta comanda!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strcmp(raspuns,"Nu sunteti logat pentru a utiliza comenzile!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strcmp(raspuns,"Nu puteti creea conturi in timp ce sunteti logat!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strcmp(raspuns,"Nu aveti drept de vot!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strcmp(raspuns,"Ati scris o comanda gresita!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strcmp(raspuns,"Piesa exista deja in aplicatie!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strcmp(raspuns,"V-ati delogat!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }

        else if(strcmp(raspuns,"Nu sunteti logat ca sa va puteti deloga!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strstr(raspuns,"SongId =")!=NULL)
        {
            printf ("[client]Mesajul primit este: \n%s\n",raspuns);
        }
        else if(strcmp(raspuns,"Nu exista comentarii la piesa cu acest id!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strstr(raspuns,"Username:")!=NULL)
        {
            printf ("[client]Mesajul primit este: \n%s\n",raspuns);
        }
        else if(strstr(raspuns,"Acestea sunt comenzile!")!=NULL)
        {
            printf ("[client]Mesajul primit este:%s\n",raspuns);
        }
        else if(strcmp(raspuns,"Usernameul nu exista, va rugam sa va creati un cont in acest scop!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strcmp(raspuns,"Usernameul exista deja, va rugam folositi alt username!")==0)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
        }
        else if(strstr(raspuns,"Am restrictionat userul")!=NULL)
        {
            printf ("[client]Mesajul primit este:%s\n",raspuns);
        }
        else if(strstr(raspuns,"este deja restrictionat!")!=NULL)
        {
            printf ("[client]Mesajul primit este:%s\n",raspuns);
        }
        else if(strstr(raspuns,"Am unrestrictionat userul")!=NULL)
        {
            printf ("[client]Mesajul primit este:%s\n",raspuns);
        }
        else if(strstr(raspuns,"este deja unrestricted!")!=NULL)
        {
            printf ("[client]Mesajul primit este:%s\n",raspuns);
        }
        else if(strstr(raspuns,"Ati ales sa scrieti un comentariu la piesa")!=NULL)
        {
            printf ("[client]Mesajul primit este: %s\n",raspuns);
            printf("Scrie un comentariu: ");
            char buf2[100],raspuns2[100];
            bzero(raspuns2,sizeof(raspuns2));
            fgets ( buf2, 100, stdin );
            int lungime3=strlen(buf2);
            buf2[lungime3-1]='\0';

            int lungime4=strlen(buf2);

            write(sd,buf2,lungime4);

        }
        else {
            printf ("[client]Mesajul primit este: Afisam trendingul! \n");
            printf("%s\n",raspuns);
        }
        /* afisam mesajul primit */
    }


    /* inchidem conexiunea, am terminat */
    close (sd);
}