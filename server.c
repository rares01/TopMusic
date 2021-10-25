/* servTCPConcTh2.c - Exemplu de server TCP concurent care deserveste clientii
   prin crearea unui thread pentru fiecare client.
   Asteapta un numar de la clienti si intoarce clientilor numarul incrementat.
    Intoarce corect identificatorul din program al thread-ului.


   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sqlite3.h>
/* portul folosit */
#define PORT 2908

/* codul de eroare returnat de anumite apeluri */
extern int errno;
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    char *a= (char*) NotUsed;

    for(int i = 0; i<argc; i++) {
        char rasp[10000];
        sprintf(rasp,"%s = %s",azColName[i], argv[i] ? argv[i] : "NULL");
        strcat(a,rasp);
        strcat(a,"\n");
    }
    strcat(a,"\n");
    return 0;
}
static int insert_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    char *a= (char*) NotUsed;

    for(int i = 0; i<argc; i++) {
        char rasp[10000];
        sprintf(rasp,"%s\n", argv[i] ? argv[i] : "NULL");
        strcat(a,rasp);
    }
    strcat(a,"\n");
    return 0;
}
static int insert_callback1(void *NotUsed, int argc, char **argv, char **azColName) {
    char *a= (char*) NotUsed;

    for(int i = 0; i<argc; i++) {
        char rasp[10000];
        sprintf(rasp,"%s   ", argv[i] ? argv[i] : "NULL");
        strcat(a,rasp);
    }
    strcat(a,"\n");
    return 0;
}
static int insert_callback2(void *NotUsed, int argc, char **argv, char **azColName) {
    char *a= (char*) NotUsed;

    for(int i = 0; i<argc; i++) {
        char rasp[10000];
        sprintf(rasp,"%s a comentat:\n", argv[i] ? argv[i] : "NULL");
        strcat(a,rasp);
    }
    return 0;
}
static int insert_callback3(void *NotUsed, int argc, char **argv, char **azColName) {
    char *a= (char*) NotUsed;

    for(int i = 0; i<argc; i++) {
        char rasp[10000];
        sprintf(rasp,"%s:  %s",azColName[i], argv[i] ? argv[i] : "NULL");
        strcat(a,rasp);
        strcat(a,"\n");
    }
    strcat(a,"\n");
    return 0;
}
void  createnormal(sqlite3* db,char name[100],int cl)
{
    // 0 e pentru Useri normali si 1 pentru administratori
    // 0 nu e restrictionat ( adica are drept de vot) si 1 e restrictionat)
    int rc1;
    char  sql[100];
    char *zErrMsg=0;
    sprintf(sql,"SELECT * FROM Useri WHERE Username='%s'",name);
    char *sqlque=sql;
    char raspuns1[1000]="\0";
    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sqlque, insert_callback, verificare , &zErrMsg);

    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        if(strstr(verificare,name)==NULL)
        {
            strcpy(raspuns1,"Acum ca v-ati creat un username valabil va puteti creea si o parola!");
            write(cl,raspuns1,strlen(raspuns1));
            char *sqlque1;
            char parola[100],sql1[100],verificare1[100],raspuns2[100];
            bzero(raspuns2,sizeof(raspuns2));
            bzero(sql1,sizeof(sql1));
            bzero(parola,sizeof(parola));
            bzero(verificare1,sizeof(verificare1));


            if (read(cl, &parola, sizeof(parola)) <= 0)
            {
                printf("[Thread ]\n");
                perror("Eroare la read() de la client.\n");

            }

            printf("%s\n",parola);
            printf("Records created!\n");
            int length=strlen(parola);
            printf("%d\n",length);

            char* query = sqlite3_mprintf("insert into Useri values ('%q','%q','%d','%d');", name,parola,0,0);


            sqlque1=sql1;
            int rc2;

            rc2 = sqlite3_exec(db, query, insert_callback , verificare1 , &zErrMsg);

            if( rc2 != SQLITE_OK ){
                sprintf(raspuns2, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            else{
                //printf("%s\n",sqlque1);
                strcpy(raspuns2,"Ati reusit sa va creati un cont! Acum va puteti loga!");
                write(cl,raspuns2,strlen(raspuns2));
            }


        }
        else {
            strcpy(raspuns1,"Usernameul exista deja, va rugam folositi alt username!");
            write(cl,raspuns1,strlen(raspuns1));
        }

    }

}
void  createadmin(sqlite3* db,char name[100],int cl)
{
    // 0 e pentru Useri normali si 1 pentru administratori
    // 0 nu e restrictionat ( adica are drept de vot) si 1 e restrictionat)
    int rc1;
    char  sql[100];
    char *zErrMsg=0;
    sprintf(sql,"SELECT * FROM Useri WHERE Username='%s'",name);
    char *sqlque=sql;
    char raspuns1[1000]="\0";
    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sqlque, insert_callback, verificare , &zErrMsg);

    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        if(strstr(verificare,name)==NULL)
        {
            strcpy(raspuns1,"Acum ca v-ati creat un username valabil va puteti creea si o parola!");
            write(cl,raspuns1,strlen(raspuns1));
            char *sqlque1;
            char parola[100],sql1[100],verificare1[100],raspuns2[100];
            bzero(raspuns2,sizeof(raspuns2));
            bzero(sql1,sizeof(sql1));
            bzero(parola,sizeof(parola));
            bzero(verificare1,sizeof(verificare1));

            if (read(cl, &parola, sizeof(parola)) <= 0)
            {
                printf("[Thread ]\n");
                perror("Eroare la read() de la client.\n");

            }

            printf("%s\n",parola);
            printf("Records created!\n");
            int length=strlen(parola);
            printf("%d\n",length);
            char* query = sqlite3_mprintf("insert into Useri values ('%q','%q','%d','%d');", name,parola,1,0);


            sqlque1=sql1;
            int rc2;

            rc2 = sqlite3_exec(db, query, insert_callback , verificare1 , &zErrMsg);

            if( rc2 != SQLITE_OK ){
                sprintf(raspuns2, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            else{
                //printf("%s\n",sqlque1);
                strcpy(raspuns2,"Ati reusit sa va creati un cont! Acum va puteti loga!");
                write(cl,raspuns2,strlen(raspuns2));
            }


        }
        else {
            strcpy(raspuns1,"Usernameul exista deja, va rugam folositi alt username!");
            write(cl,raspuns1,strlen(raspuns1));
        }

    }

}
void  login(sqlite3* db,char name[100],int cl,int *loging)
{
    int rc1;
    char  sql[100];
    char *zErrMsg=0;
    sprintf(sql,"SELECT * FROM Useri WHERE Username='%s'",name);
    char *sqlque=sql;
    char raspuns1[1000]="\0";
    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    bzero(raspuns1,sizeof(raspuns1));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sqlque, insert_callback, verificare , &zErrMsg);

    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        if(strstr(verificare,name)!=NULL)
        {
            strcpy(raspuns1,"Acum va rugam introduceti parola!");
            write(cl,raspuns1,strlen(raspuns1));
            char *sqlque1;
            char parola[100],sql1[100],verificare1[100],raspuns2[100];
            bzero(raspuns2,sizeof(raspuns2));
            bzero(sql1,sizeof(sql1));
            bzero(parola,sizeof(parola));
            bzero(verificare1,sizeof(verificare1));

            if (read(cl, &parola, sizeof(parola)) <= 0)
            {
                printf("[Thread ]\n");
                perror("Eroare la read() de la client.\n");

            }

            printf("%s\n",parola);
            printf("Parola pusa!\n");
            int length=strlen(parola);

            char* query = sqlite3_mprintf("select * from Useri where Username='%q' and Password='%q'", name,parola);


            sqlque1=sql1;
            int rc2;

            rc2 = sqlite3_exec(db, query, insert_callback , verificare1 , &zErrMsg);

            if( rc2 != SQLITE_OK ){
                sprintf(raspuns2, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            else{

                if(strstr(verificare1,name)!=NULL)
                    //printf("%s\n",sqlque1);
                {
                    strcpy(raspuns2,"");
                    //printf("%s\n",raspuns2);
                    strcpy(raspuns2,"Acum sunteti logat!");
                    //printf("%s\n",raspuns2);
                    write(cl,raspuns2,strlen(raspuns2));
                    printf("%s\n",raspuns2);
                    *loging=1;
                }
                else {
                    strcpy(raspuns2,"");
                    strcpy(raspuns2,"Parola introdusa gresit incercati din nou sa va logati!");
                    write(cl,raspuns2,strlen(raspuns2));
                }
            }


        }
        else {
            strcpy(raspuns1,"Usernameul nu exista, va rugam sa va creati un cont in acest scop!");
            write(cl,raspuns1,strlen(raspuns1));
        }

    }

}
void addsong(sqlite3* db,char name[100],int cl)
{
    int rc1;
    char  sql[100];
    char *zErrMsg=0;
    sprintf(sql,"SELECT * FROM Songs WHERE Song='%s'",name);
    char *sqlque=sql;
    char raspuns1[1000]="\0";
    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    bzero(raspuns1,sizeof(raspuns1));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sqlque, insert_callback, verificare , &zErrMsg);

    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        if(strstr(verificare,name)==NULL)
        {
            strcpy(raspuns1,"Acum ca ati adaugat o piesa trebuie sa ii adaugati si link-ul de pe youtube!");
            write(cl,raspuns1,strlen(raspuns1));
            char *sqlque1;
            char url[100],sql1[100],verificare1[100],raspuns2[100],gendre[100],descriere[100];
            bzero(raspuns2,sizeof(raspuns2));
            bzero(sql1,sizeof(sql1));
            bzero(url,sizeof(url));
            bzero(gendre,sizeof(gendre));
            bzero(descriere,sizeof(descriere));
            bzero(verificare1,sizeof(verificare1));

            if (read(cl, &url, sizeof(url)) <= 0)
            {
                printf("[Thread ]\n");
                perror("Eroare la read() de la client.\n");

            }

            printf("%s\n",url);
            printf("Url added!\n");
            char raspuns3[100]="Url added!";
            write(cl,raspuns3,strlen(raspuns3));

            if (read(cl, &gendre, sizeof(gendre)) <= 0)
            {
                printf("[Thread ]\n");
                perror("Eroare la read() de la client.\n");

            }
            printf("%s\n",gendre);
            printf("Gendre added!\n");
            char raspuns4[100]="Gendre added!";
            write(cl,raspuns4,strlen(raspuns4));
            if (read(cl, &descriere, sizeof(descriere)) <= 0)
            {
                printf("[Thread ]\n");
                perror("Eroare la read() de la client.\n");

            }
            printf("%s\n",descriere);
            printf("Description added!\n");
            char raspuns5[100];
            char* query = sqlite3_mprintf("insert into Songs (Song,Votes,Url,Genre,Description) values ('%q','%d','%q','%q','%q');",name,0,url,gendre,descriere);

            int rc2;

            rc2 = sqlite3_exec(db, query, insert_callback , verificare1 , &zErrMsg);

            if( rc2 != SQLITE_OK ){
                printf("nui bine\n");
                sprintf(raspuns2, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            else{

                printf("S-a adaugat piesa in baza de date!\n");
                strcpy(raspuns5,"Ati reusit sa adaugati piesa in aplicatie!");
                write(cl,raspuns5,strlen(raspuns5));
            }

        } else {
            printf("Piesa exista deja in aplicatie!\n");
            strcpy(raspuns1,"Piesa exista deja in aplicatie!");
            write(cl,raspuns1,strlen(raspuns1));

        }
    }
}
void votesong( sqlite3* db,int songid,int cl)
{
    int rc1;
    char  sql[100];
    char *zErrMsg=0;
    char raspuns1[1000]="\0";
    bzero(raspuns1,sizeof(raspuns1));
    sprintf(sql,"SELECT * FROM Songs WHERE SongId='%d'",songid);
    char *sqlque=sql;

    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sqlque, insert_callback, verificare , &zErrMsg);

    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        if(strcmp(verificare,"") == 0)
        {

            printf("Nu exista piesa!\n");
            strcpy(raspuns1,"Nu exista nicio piesa cu acest id!");
            write(cl,raspuns1,strlen(raspuns1));
        }
        else {
            printf("Exista piesa!\n");

            char* query = sqlite3_mprintf("UPDATE Songs set votes= votes+1 where SongId='%d';",songid);
            int rc2;
            char verificare1[100];
            bzero(verificare1,sizeof(verificare1));
            rc2 = sqlite3_exec(db, query, insert_callback , verificare1 , &zErrMsg);

            if( rc2 != SQLITE_OK ){
                sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            else{
                printf("Am adaugat un vot!\n");
                strcpy(raspuns1,"Am adaugat un vot la piesa!");
                write(cl,raspuns1,strlen(raspuns1));
            }
        }
    }
}
void trending(sqlite3* db,int cl)
{
    int rc1;
    char *zErrMsg=0;
    char raspuns1[1000]="\0";
    bzero(raspuns1,sizeof(raspuns1));
    char *sql="SELECT Song,Votes FROM Songs ORDER by Votes DESC;";

    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sql, insert_callback1, verificare , &zErrMsg);

    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printf("Afisam trendingul!\n");
        strcpy(raspuns1,verificare);
        write(cl,raspuns1,strlen(raspuns1));
    }
}
void trendingenre(sqlite3* db,char name[100],int cl)
{
    int rc1;
    char *zErrMsg=0;
    char raspuns1[1000]="\0";
    char verificare[100];
    bzero(raspuns1,sizeof(raspuns1));
    bzero(verificare,sizeof(verificare));

    char sql[1000] = "SELECT Song,Votes FROM Songs where genre like '";
    strcat(sql,"%");
    strcat(sql,name);
    strcat(sql,"%' ORDER by Votes DESC;");
    char * sqlque=sql;

    rc1 = sqlite3_exec(db, sqlque, insert_callback1, verificare , &zErrMsg);

    if( rc1 != SQLITE_OK ){
        printf("Nu merge!\n");
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printf("Afisam trendingul pentru %s!\n",name);
        printf("%s\n",verificare);
        strcpy(raspuns1,verificare);
        write(cl,raspuns1,strlen(raspuns1));
    }

}
void delete(sqlite3* db,int songid,int cl)
{
    int rc1;
    char  sql[100];
    char *zErrMsg=0;
    char raspuns1[1000]="\0";
    bzero(raspuns1,sizeof(raspuns1));

    sprintf(sql,"Select exists ( select 1 from Songs where SongId='%d');",songid);
    char *sqlque=sql;
    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sqlque, insert_callback, verificare , &zErrMsg);

    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        if(strstr(verificare,"1") !=NULL)
        {
            int rc2;
            char sql1[1000];
            bzero(sql1,sizeof(sql1));
            char verificare1[1000]="";
            bzero(verificare1,sizeof(verificare1));
            /* Execute SQL statement */
            sprintf(sql1,"DELETE FROM Songs WHERE SongId='%d'",songid);
            rc2 = sqlite3_exec(db, sql1, insert_callback, verificare1 , &zErrMsg);

            if( rc2 != SQLITE_OK ){
                sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            } else {
                printf("Piesa a fost stearsa!\n");
                strcpy(raspuns1,"Piesa a fost stearsa!");
                write(cl,raspuns1,strlen(raspuns1));
            }
        }
        else {
            printf("Nu exista piesa!\n");
            strcpy(raspuns1,"Nu exista nicio piesa cu acest id!");
            write(cl,raspuns1,strlen(raspuns1));
        }
    }
}
void view(sqlite3* db,int cl)
{
    int rc1;
    char *zErrMsg=0;
    char raspuns1[1000]="\0";
    bzero(raspuns1,sizeof(raspuns1));
    char *sql="SELECT * FROM Songs;";

    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sql, callback, verificare , &zErrMsg);

    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printf("Afisam piesele!\n");
        strcpy(raspuns1,verificare);
        write(cl,raspuns1,strlen(raspuns1));
    }
}
void Unrestrict(sqlite3* db,char name[100],int cl)
{
    //face din 1 -> 0
    int rc1;
    char  sql[100];
    char *zErrMsg=0;
    char raspuns1[1000]="\0";
    bzero(raspuns1,sizeof(raspuns1));
    sprintf(sql,"SELECT * FROM Useri WHERE Username='%s'",name);
    char *sqlque=sql;

    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sqlque, callback, verificare , &zErrMsg);
    printf("%d\n",rc1);
    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        if(strstr(verificare,name)!=NULL)
        {
            int rc2;
            char  sql1[100];
            sprintf(sql1,"SELECT * FROM Useri WHERE Username='%s' AND Restricited=1",name);
            char *sqlque1=sql1;

            char verificare1[1000]="";
            bzero(verificare1,sizeof(verificare1));
            rc2 = sqlite3_exec(db, sql1, insert_callback, verificare1 , &zErrMsg);

            if( rc1 != SQLITE_OK ){
                sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            } else {
                printf("%s\n",verificare1);
                if(strstr(verificare1,name)!=NULL)
                {
                    int rc3;
                    char* query = sqlite3_mprintf("UPDATE Useri set Restricited=0 where Username='%s';",name );
                    char verificare2[1000]="";
                    bzero(verificare2,sizeof(verificare2));
                    char raspuns2[100];
                    bzero(raspuns2,sizeof(raspuns2));
                    rc3 = sqlite3_exec(db, query, insert_callback, verificare2 , &zErrMsg);
                    if( rc3 != SQLITE_OK ){
                        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
                        sqlite3_free(zErrMsg);
                    } else {
                        sprintf(raspuns2,"Am unrestrictionat userul %s !",name);
                        strcpy(raspuns1,raspuns2);
                        write(cl,raspuns1,strlen(raspuns1));
                    }
                }
                else{
                    char raspuns2[100];
                    bzero(raspuns2,sizeof(raspuns2));
                    sprintf(raspuns2,"Userul %s este deja unrestricted!",name);
                    strcpy(raspuns1,raspuns2);
                    write(cl,raspuns1,strlen(raspuns1));
                    //e deja unrestricted
                }
            }

        }
        else {
            //username nu exista
            char raspuns2[100];
            bzero(raspuns2,sizeof(raspuns2));
            sprintf(raspuns2,"Userul %s nu exista!",name);
            strcpy(raspuns1,raspuns2);
            write(cl,raspuns1,strlen(raspuns1));
        }
    }
}
void Restrict(sqlite3* db,char name[100],int cl)
{
    //face din 1 -> 0
    int rc1;
    char  sql[100];
    char *zErrMsg=0;
    char raspuns1[1000]="\0";
    bzero(raspuns1,sizeof(raspuns1));
    sprintf(sql,"SELECT * FROM Useri WHERE Username='%s'",name);
    char *sqlque=sql;

    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sqlque, insert_callback, verificare , &zErrMsg);
    printf("%d\n",rc1);
    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        if(strstr(verificare,name)!=NULL)
        {
            int rc2;
            char  sql1[100];
            sprintf(sql1,"SELECT * FROM Useri WHERE Username='%s' AND Restricited=0",name);
            char *sqlque1=sql1;

            char verificare1[1000]="";
            bzero(verificare1,sizeof(verificare1));
            rc2 = sqlite3_exec(db, sql1, insert_callback, verificare1 , &zErrMsg);

            if( rc2 != SQLITE_OK ){
                sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            } else {
                printf("%s\n",verificare1);
                if(strstr(verificare1,name)!=NULL)
                {
                    int rc3;
                    char* query = sqlite3_mprintf("UPDATE Useri set Restricited=1 where Username='%s';",name );
                    char verificare2[1000]="";
                    bzero(verificare2,sizeof(verificare2));
                    char raspuns2[100];
                    bzero(raspuns2,sizeof(raspuns2));
                    rc3 = sqlite3_exec(db, query, insert_callback, verificare2 , &zErrMsg);
                    if( rc3 != SQLITE_OK ){
                        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
                        sqlite3_free(zErrMsg);
                    } else {
                        sprintf(raspuns2,"Am restrictionat userul %s !",name);
                        strcpy(raspuns1,raspuns2);
                        write(cl,raspuns1,strlen(raspuns1));
                    }
                }
                else{
                    char raspuns2[100];
                    bzero(raspuns2,sizeof(raspuns2));
                    sprintf(raspuns2,"Userul %s este deja restrictionat!",name);
                    strcpy(raspuns1,raspuns2);
                    write(cl,raspuns1,strlen(raspuns1));
                    //e deja unrestricted
                }
            }

        }
        else {
            //username nu exista
            char raspuns2[100];
            bzero(raspuns2,sizeof(raspuns2));
            sprintf(raspuns2,"Userul %s nu exista!",name);
            strcpy(raspuns1,raspuns2);
            write(cl,raspuns1,strlen(raspuns1));
        }
    }
}
void checkadmin(sqlite3* db,char name[100],int cl,int* admin)
{
    int rc1;
    char  sql[100];
    char *zErrMsg=0;
    char raspuns1[1000]="\0";
    bzero(raspuns1,sizeof(raspuns1));
    sprintf(sql,"SELECT * FROM Useri WHERE Username='%s' AND Account=1",name);
    char *sqlque=sql;

    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sqlque, insert_callback, verificare , &zErrMsg);
    printf("%d\n",rc1);
    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        if(strstr(verificare,name)!=NULL)
        {
            *admin=1;
        }
        else{
            *admin=0;
        }
    }
}
void checkrestrict(sqlite3* db,char name[100],int cl,int* restr)
{
    int rc1;
    char  sql[100];
    char *zErrMsg=0;
    char raspuns1[1000]="\0";
    bzero(raspuns1,sizeof(raspuns1));
    sprintf(sql,"SELECT * FROM Useri WHERE Username='%s' AND Restricited=1",name);
    char *sqlque=sql;

    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sqlque, insert_callback, verificare , &zErrMsg);
    printf("%d\n",rc1);
    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        if(strstr(verificare,name)!=NULL)
        {
            *restr=1;
        }
        else{
            *restr=0;
        }
    }
}
void comment(sqlite3* db,int songid,int cl,char name[100])
{
    int rc1;
    char  sql[100];
    char *zErrMsg=0;
    char raspuns1[1000]="\0";
    bzero(raspuns1,sizeof(raspuns1));
    sprintf(sql,"Select exists ( select 1 from Songs where SongId='%d');",songid);
    char *sqlque=sql;

    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sqlque, insert_callback, verificare , &zErrMsg);
    //printf("%d\n",rc1);
    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        if(strstr(verificare,"1") !=NULL)
        {
            int rc2;
            char sql1[1000];
            bzero(sql1,sizeof(sql1));
            char verificare1[1000]="";
            bzero(verificare1,sizeof(verificare1));
            sprintf(sql1,"Select Song from Songs where SongId='%d'",songid);
            char *sqlque1=sql1;
            rc2 = sqlite3_exec(db, sqlque1, insert_callback, verificare1 , &zErrMsg);

            if( rc2 != SQLITE_OK ){
                //printf("%s\n",verificare1);
                sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            } else {

                char comentariu[2000];
                bzero(comentariu,sizeof(comentariu));
                int lenu=strlen(verificare1);
                //printf("%d\n",lenu);
                verificare1[lenu-2]='\0';

                sprintf(raspuns1,"Ati ales sa scrieti un comentariu la piesa %s !", verificare1);
                write(cl,raspuns1,strlen(raspuns1));
                if (read(cl, &comentariu, sizeof(comentariu)) <= 0)
                {
                    printf("[Thread ]\n");
                    perror("Eroare la read() de la client.\n");

                }
                char* query = sqlite3_mprintf("insert into Comments (SongId,Comentariu,Username) values ('%d','%q','%q');", songid,comentariu,name);
                int rc3;
                rc3 = sqlite3_exec(db, query, insert_callback, verificare1 , &zErrMsg);
                //printf("%d\n",rc1);
                if( rc3 != SQLITE_OK ){
                    //printf("Am inserat comentariul!\n");
                    sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                } else {
                    printf("Am inserat comentariul!\n %s \n",comentariu);
                }



            }
        } else{
            strcpy(raspuns1,"Nu exista nicio piesa cu acest id!");
            printf("Nu exista nicio piesa cu acest id!\n");
            write(cl,raspuns1,strlen(raspuns1));
        }
    }

}
void deletecomm(sqlite3* db, int songid, int cl)
{
    int rc1;
    char  sql[100];
    char *zErrMsg=0;
    char raspuns1[1000]="\0";
    bzero(raspuns1,sizeof(raspuns1));
    //sprintf(sql,"DELETE FROM Songs WHERE SongId='%d';",songid);
    sprintf(sql,"Select exists ( select 1 from Songs where SongId='%d');",songid);
    char *sqlque=sql;
    char verificare[1000]="";
    bzero(verificare,sizeof(verificare));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sqlque, insert_callback, verificare , &zErrMsg);

    if( rc1 != SQLITE_OK ){
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        if(strstr(verificare,"1") !=NULL)
        {
            int rc2;
            char sql1[1000];
            bzero(sql1,sizeof(sql1));
            char verificare1[1000]="";
            bzero(verificare1,sizeof(verificare1));
            /* Execute SQL statement */
            sprintf(sql1,"DELETE FROM Comments WHERE SongId='%d'",songid);
            rc2 = sqlite3_exec(db, sql1, insert_callback, verificare1 , &zErrMsg);

            if( rc2 != SQLITE_OK ){
                sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            } else {
                printf("Comentariile au fost sterse!\n");
            }
        }
    }
}
void viewcomm(sqlite3* db, int songid, int cl)
{
    int rc1;
    char  sql[100];
    char* zErrMsg = 0;
    char raspuns1[1000] = "\0";
    bzero(raspuns1, sizeof(raspuns1));
    sprintf(sql, "Select exists ( select 1 from Songs where SongId='%d');", songid);
    char* sqlque = sql;

    char verificare[1000] = "";
    bzero(verificare, sizeof(verificare));
    /* Execute SQL statement */
    rc1 = sqlite3_exec(db, sqlque, insert_callback, verificare, &zErrMsg);

    if (rc1 != SQLITE_OK) {
        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        if (strstr(verificare, "1") != NULL)
        {

            int rc10;
            char  sql10[100];
            sprintf(sql10, "Select exists ( select 1 from Comments where SongId='%d');", songid);
            char* sqlque10 = sql10;

            char verificare10[1000] = "";
            bzero(verificare10, sizeof(verificare10));
            /* Execute SQL statement */
            rc10 = sqlite3_exec(db, sqlque10, insert_callback, verificare10, &zErrMsg);
            //printf("%d\n",rc1);
            if (rc10 != SQLITE_OK) {
                sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            else
            {
                if (strstr(verificare10, "1") != NULL)
                {
                    int rc2;
                    char sql1[1000];
                    bzero(sql1, sizeof(sql1));
                    char verificare1[1000] = "";
                    bzero(verificare1, sizeof(verificare1));
                    sprintf(sql1, "Select Username from Comments where SongId='%d'", songid);
                    char* sqlque1 = sql1;
                    rc2 = sqlite3_exec(db, sqlque1, insert_callback2, verificare1, &zErrMsg);
                    if (rc2 != SQLITE_OK) {

                        sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
                        sqlite3_free(zErrMsg);
                    }
                    else
                    {

                        int rc3;
                        char sql2[1000];
                        bzero(sql2, sizeof(sql2));
                        char verificare2[1000] = "";
                        bzero(verificare2, sizeof(verificare2));
                        sprintf(sql2, "Select Comentariu from Comments where SongId='%d'", songid);
                        char* sqlque2 = sql2;
                        rc3 = sqlite3_exec(db, sqlque2, insert_callback, verificare2, &zErrMsg);
                        if (rc3 != SQLITE_OK)
                        {
                            //printf("%s\n",verificare1);
                            sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
                            sqlite3_free(zErrMsg);
                        }
                        else
                        {
                            /*strcat(verificare1, verificare2);
                            strcpy(raspuns1, verificare1);*/
                            int rc100;
                            char sql100[100];
                            sprintf(sql100," Select Username,Comentariu from Comments where songid='%d';",songid);
                            char * sqlque100=sql100;
                            char verificare4[1000];
                            bzero(verificare4,sizeof(verificare4));
                            rc100 = sqlite3_exec(db, sqlque100, insert_callback3, verificare4, &zErrMsg);
                            if (rc100 != SQLITE_OK)
                            {
                                //printf("%s\n",verificare1);
                                sprintf(raspuns1, "SQL error: %s\n", zErrMsg);
                                sqlite3_free(zErrMsg);
                            }
                            else
                            {
                                strcpy(raspuns1,verificare4);
                                printf("Acestea sunt comentariile!\n");
                                write(cl, raspuns1, strlen(raspuns1));
                            }
                        }
                    }

                }
                else
                {
                    //NU EXISTA COMENTARII
                    strcpy(raspuns1, "Nu exista comentarii la piesa cu acest id!");
                    printf("Nu exista comentarii la piesa cu acest id!\n");
                    write(cl, raspuns1, strlen(raspuns1));
                }
            }
        }
        else { //nu exista
            strcpy(raspuns1, "Nu exista nicio piesa cu acest id!");
            printf("Nu exista nicio piesa cu acest id!\n");
            write(cl, raspuns1, strlen(raspuns1));
        }
    }
}

typedef struct thData {
    int idThread; //id-ul thread-ului tinut in evidenta de acest program
    int cl; //descriptorul intors de accept
    sqlite3* db;
}thData;

static void* treat(void*); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */
void raspunde(void*);

int main()
{
    struct sockaddr_in server;	// structura folosita de server
    struct sockaddr_in from;
    int nr;		//mesajul primit de trimis la client
    int sd;		//descriptorul de socket
    int pid;
    pthread_t th[100];    //Identificatorii thread-urilor care se vor crea
    int i = 0;

    sqlite3* db;
    char* zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);

    if (rc) {
        perror("Can't open database");
        return(0);
    }
    else {
        printf("Opened database successfully\n");
    }
    /* crearea unui socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[server]Eroare la socket().\n");
        return errno;
    }
    /* utilizarea optiunii SO_REUSEADDR */
    int on = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    /* pregatirea structurilor de date */
    bzero(&server, sizeof(server));
    bzero(&from, sizeof(from));

    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
    /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    /* utilizam un port utilizator */
    server.sin_port = htons(PORT);

    /* atasam socketul */
    if (bind(sd, (struct sockaddr*) & server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server]Eroare la bind().\n");
        return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen(sd, 5) == -1)
    {
        perror("[server]Eroare la listen().\n");
        return errno;
    }
    /* servim in mod concurent clientii...folosind thread-uri */
    while (1)
    {
        int client;
        thData* td; //parametru functia executata de thread
        int length = sizeof(from);

        printf("[server]Asteptam la portul %d...\n", PORT);
        fflush(stdout);

        // client= malloc(sizeof(int));
        /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
        if ((client = accept(sd, (struct sockaddr*) & from, &length)) < 0)
        {
            perror("[server]Eroare la accept().\n");
            continue;
        }

        /* s-a realizat conexiunea, se astepta mesajul */

        // int idThread; //id-ul threadului
        // int cl; //descriptorul intors de accept

        td = (struct thData*)malloc(sizeof(struct thData));
        td->idThread = i++;
        td->cl = client;
        td->db = db;//pentru ca orice client sa poata avea acces simultan la baza de date
        pthread_create(&th[i], NULL, &treat, td);

    }//while
};
static void* treat(void* arg)
{
    struct thData tdL;
    tdL = *((struct thData*)arg);
    printf("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
    fflush(stdout);
    pthread_detach(pthread_self());
    raspunde((struct thData*)arg);
    /* am terminat cu acest client, inchidem conexiunea */
    close((intptr_t)arg);
    return(NULL);

};


void raspunde(void* arg)
{
    int  i = 0;
    struct thData tdL;
    char mesaj[1000];
    char copiemesaj[1000];
    char numeutilizator[100];
    bzero(numeutilizator,sizeof(numeutilizator));
    char help[1500]="Acestea sunt comenzile!\nCreate account: username\nCreate admin: username\nLogin: username\n Add song: song\nVote song: songid\nView songs\nComment songid\nComments songid\nTrending\nTrending: gendre\nQuit\nComenzi doar pentru administratori!\nDelete songid\nRestrict username\nUnrestrict username\n";
    char final[100]="Nu sunteti logat pentru a utiliza comenzile!";
    char ptadmin[100]="Nu sunteti administrator pentru a utiliza aceasta comanda!";
    char nuputeti[100]="Nu puteti creea conturi in timp ce sunteti logat!";
    char logarea[100]="Sunteti deja logat, va rugam sa va delogati daca doriti sa utilizati alt cont!";
    char * zErrMsg;
    int quit=1;
    int loging=0;
    int admin=0;
    tdL = *((struct thData*)arg);
    while(quit){
        if (read(tdL.cl, &mesaj, sizeof(mesaj)) <= 0)
        {
            printf("[Thread %d]\n", tdL.idThread);
            perror("Eroare la read() de la client.\n");

        }

        printf("[Thread %d]Mesajul a fost receptionat...%s\n", tdL.idThread, mesaj);
        int realizabil=0;
        /*pregatim mesajul de raspuns */
        int len=strlen(mesaj);
        mesaj[len-1]='\0';
        strcpy(copiemesaj,mesaj);
        //printf("[Thread %d]Trimitem mesajul inapoi...%s\n", tdL.idThread, mesaj);
        char * p=strtok(mesaj," ");
        if(strcmp(p,"Help")==0)
        {  // returnam mesajul clientului
            realizabil=1;
            if (write(tdL.cl, &help, sizeof(help)) <= 0)
            {
                printf("[Thread %d] ", tdL.idThread);
                perror("[Thread]Eroare la write() catre client.\n");
            }
            else{
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
            }
        }

        if(strcmp(p,"Create")==0)
        {

            p=strtok(NULL," ");
            if(strcmp(p,"account:")==0)
            {
                if(loging==0)
                {
                    char name[100];
                    p=strtok(NULL," ");
                    strcpy(name,p);
                    realizabil=1;
                    createnormal(tdL.db,name,tdL.cl);
                }
                else
                {
                    write(tdL.cl, &nuputeti, sizeof(nuputeti));
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
                }

            }
            if(strcmp(p,"admin:")==0)
            {
                if(loging==0)
                {
                    char name1[100];
                    p=strtok(NULL," ");
                    strcpy(name1,p);
                    realizabil=1;
                    createadmin(tdL.db,name1,tdL.cl);
                }
                else
                {
                    write(tdL.cl, &nuputeti, sizeof(nuputeti));
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
                }
            }
        }
        if(strcmp(p,"Login:")==0)
        {
            if(loging==0)
            {
                char name2[100];
                p=strtok(NULL," ");
                strcpy(name2,p);
                realizabil=1;
                login(tdL.db,name2,tdL.cl,&loging);
                if(loging==1)
                {
                    strcpy(numeutilizator,name2);
                    checkadmin(tdL.db,name2,tdL.cl,&admin);
                }
            }
            else {
                if (write(tdL.cl, &logarea, sizeof(logarea)) <= 0)
                {
                    printf("[Thread %d] ", tdL.idThread);
                    perror("[Thread]Eroare la write() catre client.\n");
                }
                else{
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
                }
            }

        }

        if(strcmp(p,"Quit")==0)
        {
            quit=0;
            realizabil=1;
            char raspunsuri[100]="Multumim ca ati utilizat aplicatia! O zi buna!";
            write(tdL.cl,raspunsuri,strlen(raspunsuri));
        }
        if(strcmp(p,"Add")==0)
        {
            p=strtok(NULL," ");
            if(strcmp(p,"song:")==0)
            {
                if(loging==1)
                {
                    char song[100];
                    //p=strtok(NULL," ");
                    //strcpy(song,p);
                    strcpy(song,copiemesaj+10);
                    printf("%s\n",song);
                    realizabil=1;
                    addsong(tdL.db,song,tdL.cl);
                }
                else
                {
                    write(tdL.cl, &final, sizeof(final));
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
                }
            }
        }
        if(strcmp(p,"Vote")==0)
        {
            p=strtok(NULL," ");
            if(strcmp(p,"song:")==0)
            {
                if(loging==1)
                {
                    char songid1[100];
                    p=strtok(NULL," ");
                    strcpy(songid1,p);
                    realizabil=1;
                    int songid=atoi(songid1);
                    int restr=0;
                    checkrestrict(tdL.db,numeutilizator,tdL.cl,&restr);
                    if(restr==0)
                        votesong(tdL.db,songid,tdL.cl);
                    else
                    {
                        char novote[1000]="Nu aveti drept de vot!";
                        write(tdL.cl, &novote, sizeof(novote));
                        printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
                    }

                }
                else
                {
                    write(tdL.cl, &final, sizeof(final));
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
                }
            }
        }
        if(strcmp(p,"Trending")==0)
        {
            if(loging==1)
            {
                realizabil=1;
                trending(tdL.db,tdL.cl);
            }
            else
            {
                write(tdL.cl, &final, sizeof(final));
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
            }
        }
        if(strcmp(p,"Trending:")==0)
        {
            if(loging==1)
            {
                char genre[100];
                p=strtok(NULL," ");
                strcpy(genre,p);
                realizabil=1;
                trendingenre(tdL.db,genre,tdL.cl);
            }
            else
            {
                write(tdL.cl, &final, sizeof(final));
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
            }
        }
        if(strcmp(p,"Delete")==0)
        {
            if(loging==1)
            {
                if(admin==1)
                {
                    char songid1[100];
                    p=strtok(NULL," ");
                    strcpy(songid1,p);
                    realizabil=1;
                    int songid=atoi(songid1);
                    deletecomm(tdL.db,songid,tdL.cl);
                    delete(tdL.db,songid,tdL.cl);
                }
                else
                {
                    write(tdL.cl,&ptadmin, sizeof(ptadmin));
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
                }
            }
            else
            {
                write(tdL.cl, &final, sizeof(final));
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
            }

        }
        if(strcmp(p,"View")==0)
        {
            p=strtok(NULL," ");
            if(strcmp(p,"songs")==0)
            {
                if(loging==1)
                {
                    realizabil=1;
                    view(tdL.db,tdL.cl);
                }
                else
                {
                    write(tdL.cl, &final, sizeof(final));
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
                }
            }
        }
        if(strcmp(p,"Restrict")==0)
        {
            if(loging==1)
            {
                if(admin==1)
                {
                    char name[100];
                    p=strtok(NULL," ");
                    strcpy(name,p);
                    realizabil=1;
                    Restrict(tdL.db,name,tdL.cl);
                }
                else
                {
                    write(tdL.cl,&ptadmin, sizeof(ptadmin));
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
                }
            }
            else
            {
                write(tdL.cl, &final, sizeof(final));
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
            }
        }
        if(strcmp(p,"Unrestrict")==0)
        {
            if(loging==1)
            {
                if(admin==1)
                {
                    char name[100];
                    p=strtok(NULL," ");
                    strcpy(name,p);
                    realizabil=1;
                    Unrestrict(tdL.db,name,tdL.cl);
                }
                else
                {
                    write(tdL.cl,&ptadmin, sizeof(ptadmin));
                    printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
                }
            }
            else
            {
                write(tdL.cl, &final, sizeof(final));
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
            }
        }

        /*if(strcmp(p,"Check")==0)
        {
            char name[100];
            p=strtok(NULL," ");
            strcpy(name,p);
            realizabil=1;
            int admin=0;
            checkadmin(tdL.db,name,tdL.cl,&admin);
            if(admin==0)
                printf("User normal!\n");
            if(admin==1)
                printf("Admin!\n");
        }*/
        /*if(strcmp(p,"CheckR")==0)
        {
            char name[100];
            p=strtok(NULL," ");
            strcpy(name,p);
            realizabil=1;
            int restr=0;
            checkrestrict(tdL.db,name,tdL.cl,&restr);
            if(restr==0)
                printf("Normal!\n");
            if(restr==1)
                printf("Restrictionat!\n");
        }*/
        if(strcmp(p,"Logout")==0)
        {
            realizabil=1;
            if(loging==0)
            {
                char raspunsuri1[100]="Nu sunteti logat ca sa va puteti deloga!";
                write(tdL.cl,raspunsuri1,strlen(raspunsuri1));
            }
            if(loging==1)
            {
                loging=0;
                char raspunsuri2[100]="V-ati delogat!";
                write(tdL.cl,raspunsuri2,strlen(raspunsuri2));
            }
        }
        if(strcmp(p,"Comment")==0)
        {
            if(loging==1)
            {
                char songid2[100];
                p=strtok(NULL," ");
                strcpy(songid2,p);
                realizabil=1;
                int songid=atoi(songid2);
                comment(tdL.db,songid,tdL.cl,numeutilizator);
            }
            else
            {
                write(tdL.cl, &final, sizeof(final));
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
            }
        }
        if(strcmp(p,"Comments")==0)
        {
            if(loging==1)
            {
                char songid3[100];
                p=strtok(NULL," ");
                strcpy(songid3,p);
                realizabil=1;
                int songid=atoi(songid3);
                viewcomm(tdL.db,songid,tdL.cl);
            }
            else
            {
                write(tdL.cl, &final, sizeof(final));
                printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
            }
        }
        if(realizabil==0)
        {
            char gresita[100]="Ati scris o comanda gresita!";
            write(tdL.cl, &gresita, sizeof(gresita));
            printf("[Thread %d]Mesajul a fost trasmis cu succes.\n", tdL.idThread);
        }
        /*if(strcmp(p,"Deletecomm")==0)
        {
            char songid4[100];
            p=strtok(NULL," ");
            strcpy(songid4,p);
            realizabil=1;
            int songid=atoi(songid4);
            deletecomm(tdL.db,songid,tdL.cl);
        }*/

    }




}
