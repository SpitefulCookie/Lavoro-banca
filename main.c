#include <stdio.h>
//#include <stdlib.h>
#include <strings.h>
#include <ctype.h> //toupper()
#include <synchapi.h> //Sleep()
#include <time.h>
#include <dirent.h>

#define PATH_SUFFIX "BANCHE\\"


                                         //      TIPI

typedef struct Data {

    short int gg;

    short int mm;

    short int aaaa;

}Data;

typedef struct Cliente {  

    int numero_conto;
    
    char cognome[30];

    char nome[30];

    char username[30];

    char password[30];

    Data data_nascita; 

    char sesso;

    double prelievo_max;

    double saldo;

}Cliente;

typedef struct Movimenti {

    char tipo[15]; 

    Data data_movimento;

    double importo;

}Movimenti;

                                         //      PROTOTIPI

//void datasystem(int *, int *, int *);

int ottieniBanca(char*, char*, char * );

Cliente caricaCliente(char *);

int getConto(char *);

int checkInt(void);

int creaFileMovimenti(int* , char* );

int creaFilePath (char * , char * );

                                         //      CODICE

void main(int argc, char* argv[]){

    char password[30];

    char path[100];

    char pathMovimenti[100];

    char bancaSelezionata[30]="\0", conferma = '\0';

    int scelta, disconnesso = 1, fileok = 0;

    do{

        system("cls");

        printf("\n\t\t   SISTEMA BANCARIO\n\n\tSelezionare il portale a cui accedere:\n\n\t\t[1] Portale impiegati\n\t\t[2] Portale Clienti\n\t\t[0] Termine programma\n\n\tScelta: ");

        fflush(stdin);

        scelta = checkInt(); 
    
        switch(scelta){

            case 1:

                //do{

                    system("cls");

                    printf("\n\t\tLOGIN PORTALE IMPIEGATI\n\n\t Username: impiegato_05\n\t Password: ");

                    fflush(stdin);

                    gets(password);

                    if(strcmp(password, "3PDINFO")){

                        printf("\n\t La password immessa e' errata!\n\t ");

                        Sleep(1400);

                    }

                //}while(strcmp(password, "3PDINFO")); //debug inserire nuovamente

                printf("\n\t Login effettuato, benvenuto impiegato_05!\n\t ");

                disconnesso = 0;

                strcpy(path, PATH_SUFFIX);

                Sleep(1400);

                do{

                    system("cls");

                    printf("\n\t\t   PORTALE IMPIEGATI");

                    if(fileok){

                        printf("\n\n\tDatabase bancario aperto: %s", bancaSelezionata);

                    }

                    printf("\n\n\tSelezionare operazione da eseguire:\n\n\t\t[1] Apri database bancario\n\t\t[2] Crea nuovo database\n\t\t[0] Disconnetti\n\n\tScelta: ");

                    fflush(stdin);

                    scelta = checkInt();

                    switch(scelta){

                        case 1:
                           
                            if(fileok){

                                printf("\n\tUn database bancario e' gia' caricato, caricarne uno nuovo? [y/n] ");

                                fflush(stdin);
                                
                                conferma = getchar();

                                if(toupper(conferma)== 'Y'){

                                    strcpy(bancaSelezionata, "\0");

                                    fileok = 0; 

                                }

                            } else{

                                if(ottieniBanca(path, pathMovimenti, bancaSelezionata)){

                                    fileok = 1;

                                    printf("\tIl database bancario %s e' stato caricato con successo!", bancaSelezionata);

                                    Sleep(1400);

                                }  
                            
                            }

                            break;

                        case 2:

                            //CREA NUOVO DATABASE

                            if(fileok){

                                printf("\n\tUn database bancario e' gia' stato caricato, crearne uno nuovo? [y/n] ");

                                fflush(stdin);
                                
                                conferma = getchar();

                                if(toupper(conferma)== 'Y'){

                                    strcpy(bancaSelezionata, "\0");

                                    fileok = 0; //far si che parta direttamente la creazione di un nuovo database

                                }

                            } else{
                                                              
                                mkdir(path); //<- controllare meglio la logica quando la cartella e gia presente, possono esserci dei comportamenti anomali. se ci sono usare puntatore a directory
                                                            
                                if(creaDatabase(path, pathMovimenti, bancaSelezionata)){

                                    fileok = 1;

                                    printf("\tIl database bancario %s e' stato creato con successo!", bancaSelezionata);

                                    Sleep(1400);

                                }

                            }

                            break;

                        case 0:

                            disconnesso = 1;

                            printf("\n\tDisconnessione in corso...");

                            Sleep(1400);

                            scelta = -1;

                            break;
                        
                        default:

                            printf("\n\tErrore! Scelta non consentita!");
            
                            Sleep(1400);

                            break;

                    }

                }while(!disconnesso);

                break;

            case 2:

                //portale clienti

                break;

            case 0:

                printf("\n\tTermine programma...");

                Sleep(1400);

                break;

            default:

                printf("\n\tErrore! Scelta non consentita!");
            
                Sleep(1400);

                break;

        }

    }while(scelta); 
   
}

/*
void datasystem(int *g, int *m, int *a){

    struct tm *now= NULL;

    long tempo;

    time(&tempo);

    now=localtime(&tempo);

    *g=now->tm_mday;

    *m=now->tm_mon+1;

    *a=now->tm_year+1900;
    
}
*/

int getConto(char * banca){ //posso passare il path al posto di banca

    FILE * flogico;

    Cliente cliente_letto;

    int numero_conto;

    flogico = fopen(banca, "ab");

    if(!ftell(flogico)){
        
        fclose(flogico);

        return 1;

    } else{

        fseek(flogico, -(int)sizeof(Cliente), SEEK_CUR);

        fread(&cliente_letto, sizeof(Cliente), 1, flogico);

        numero_conto = cliente_letto.numero_conto + 1;

        fclose(flogico);

        return numero_conto;

    }
    
}

Cliente caricaCliente(char * banca){
    
                                             //      VARIABILI
    
    Cliente cliente = {};

    int controllo = 0;

                                             //       CODICE

    cliente.numero_conto = getConto(banca);

    creaFileMovimenti(&cliente.numero_conto, banca);

    cliente.saldo = 0;

    printf("\n Inserisci la quantita' massima di denaro prelevabile: ");
            
    fflush(stdin);

    cliente.prelievo_max = checkInt();

    printf("\n Inserisci il cognome del cliente: ");

    do{
        
        fflush(stdin);

        gets(cliente.cognome);

        if(!stricmp(cliente.cognome, "") || !stricmp(cliente.cognome, " ")){

            printf(" Errore! Inserire un nome valido!\n Inserisci nome cliente: ");

        }

    }while (!stricmp(cliente.cognome, "") || !stricmp(cliente.cognome, " "));

    printf(" Inserisci nome del cliente: ");

    do{

        fflush(stdin);

        gets(cliente.nome);
        
        if(!stricmp(cliente.nome, "") || !stricmp(cliente.nome, " ")){

            printf(" Errore! Inserire un nome valido!\n Inserisci nome cliente: ");

        }

    }while (!stricmp(cliente.nome, "") || !stricmp(cliente.nome, " "));   

    printf(" Inserisci il giorno di nascita (gg): ");

    do{ 

        cliente.data_nascita.gg = checkInt();

        if (cliente.data_nascita.gg < 1 || cliente.data_nascita.gg > 31){

            printf(" Errore! Giorno invalido, inserire nuovamente: ");

        }

    } while (cliente.data_nascita.gg < 1 || cliente.data_nascita.gg > 31);
    
	printf(" Inserisci il mese di nascita (mm): ");

    do{ 

        cliente.data_nascita.mm = checkInt();

        if (cliente.data_nascita.mm < 1 || cliente.data_nascita.mm > 12){

            printf("\n Errore! Mese invalido, inserire nuovamente: ");

        }
    } while (cliente.data_nascita.mm < 1 || cliente.data_nascita.mm > 12);
    
	printf(" Inserisci l'anno di nascita (aaaa): ");

    do{ 

        cliente.data_nascita.aaaa = checkInt();

        if (cliente.data_nascita.aaaa < 1900 || cliente.data_nascita.aaaa > 2025){

            printf(" Errore! Anno invalido, inserire anno compreso tra [1900-2025]: ");

        }
    } while (cliente.data_nascita.aaaa < 1900 || cliente.data_nascita.aaaa > 2025);

    printf(" Inserisci sesso dell'cliente: ");

    do{

        fflush(stdin);

        cliente.sesso = toupper(getchar());

        if (!(cliente.sesso != 'M')){ 

            controllo = 0;

        }
        else{

            if ((cliente.sesso != 'M')){ 

                controllo = 0;

            }

            else{

                printf(" Errore! Valore invalido! Utilizzare [M/F]");

            }
        }

    }while(controllo == 1);

    controllo = 1;   

    printf("\n Inserisci l'username del cliente: ");

    do{
        
        fflush(stdin);

        gets(cliente.username);

        if(!stricmp(cliente.username, "") || !stricmp(cliente.username, " ")){

            printf(" Errore! Inserire un username valido!\n Inserisci l'username del cliente: ");

        }

    }while (!stricmp(cliente.username, "") || !stricmp(cliente.username, " "));

    printf("\n Inserisci la password del cliente: ");

    do{
        
        fflush(stdin);

        gets(cliente.password);

        if(!stricmp(cliente.password, "") || !stricmp(cliente.password, " ")){

            printf(" Errore! Inserire una password valida!\n Inserisci la password del cliente: ");

        }

    }while (!stricmp(cliente.password, "") || !stricmp(cliente.password, " "));

    return cliente;

}

int checkInt(void){

    int var, is_int;

	do{
        
        fflush(stdin);
        
        if (scanf("%d", &var)){

            is_int = 1;

        }
        else {

            printf(" Valore invalido, inserire un numero intero! ");

        }
        
	} while (!is_int);
	
    is_int = 0;

	return var;
   
}

int creaFileMovimenti(int* numero_conto, char* banca){

    FILE * flogico;

    char nomefile [30];

    char conto[30];

    strcpy(nomefile, banca);

    strcat(nomefile, "_");

    itoa(*numero_conto, conto, 10);

    strcat(nomefile, conto);

    flogico = fopen(nomefile, "wb");

    if(!(flogico == NULL)){

        fclose(flogico);

        return 1;

    } else{

        return 0;

    }

}

int ottieniBanca(char * path, char * pathMovimenti, char * banca){

                                         //      VARIABILI

    DIR *cartella;

    struct dirent *record;

    int trovato = 0, i = 0;

    char conferma;

    char listaBanche [30][30];

                                         //      CODICE

    cartella = opendir(path);

    if(cartella == NULL){ // se non vi e' una directory "BANCHE", la creo

        mkdir(path);

        cartella = opendir(path);

    }

    while( (record = readdir(cartella)) && i<30 ){ //leggo la directory e ottengo la lista delle banche finché il mio array non è pieno

        if(record->d_namlen>2){ //esclude '.' e ".." <-- escludere le cartelle!!!

            if(strncmp(record->d_name,"MOVIMENTI_", 10)){

                strcpy(listaBanche[i], record->d_name);

                i++;

                trovato = 1;

            }
            
        }
    
    }

    closedir(cartella);

    system("cls");

    printf("\n\t\t\t\t   PORTALE IMPIEGATI\n\t\t\tVISUALIZZAZIONE ELENCO DATABASE BANCARI");

    if(i==30){printf("\n\tAttenzione e' stato possibile leggere solamente i primi 30 elementi nella directory. \n\tIl record desiderato potrebbe non essere presente all'interno della lista\n");}

    printf("\n");

    int j;

    for (j=0;j<i; j++){ //print a schermo delle banche trovate, l'if permette il print su due colonne

        if((j+2)%2==0){printf("\n\t\t");}else{printf("\t\t");}

        printf("[%2d] %20s", j+1, listaBanche[j]);

    }

    if(trovato){

        printf("\n\n\tImmettere il database bancario da aprire: ");

        int scelta;

        do{

            scelta = checkInt();

            if(scelta <1 || scelta > i){printf("\tErrore! inserire un valore compreso tra 1 e %d: ", i);}

        }while( scelta <1 || scelta > i );

        strcpy(banca,listaBanche[scelta-1]);

        return 1;

    } else{ //se non è stato trovato almeno un database bancario chiedo all'utente se ne vuole creare uno nuovo
        
        printf("\n\tNon e' stata trovato nessun database bancario, crearne uno nuovo? [y/n] "); 

        fflush(stdin);

        conferma = getchar();       
        
        if(toupper(conferma) == 'Y'){
            
            if(creaDatabase(path, pathMovimenti, banca)){return 1;}else{printf("\n\n\tOperazione annullata");}
            
        } else{printf("\n\n\tOperazione annullata");}

    }

    return 0;

}

int creaDatabase(char * path, char* pathMovimenti, char * banca){

    char conferma = '\0';

    char buffer[30];
    
    do{ 

        printf("\n\tInserire il nome del database da creare: ");

        fflush(stdin);
        
        gets(buffer);

        if(buffer == " " || buffer == "\n" || buffer == "\0"){ //controllo che non sia stato inserito un nome "vuoto" <<-- verificare che funzioni correttamente

            printf("\n\tErrore! inserire un valore valido!");

        } else{

            printf("\n\tIl nome del nuovo database sara' %s, confermare? [y/n] ", buffer);

            fflush(stdin);

            conferma = getchar();

            if(toupper(conferma) == 'Y'){

                if(creaFilePath(buffer, path)){

                    strcpy(banca, buffer);          //  Copio il nome del database all'interno della variabile "bancaSelezionata" nel main

                    strcpy(buffer, path);           //path::    BANCHE\\  <- sovrascrivo buffer e gli assegno il mio path (BANCHE\\)

                    strcat(buffer, "MOVIMENTI_");   //path::    BANCHE\\MOVIMENTI_

                    strcat(buffer, banca);          //path::    BANCHE\\MOVIMENTI_<banca>

                    if(mkdir(buffer)){strcpy(pathMovimenti, buffer);}   //Creo una directory che conterrà tutti i miei file movimenti

                    return 1;

                }
                
            } 

        }

    }while(toupper(conferma)!='Y'&& toupper(conferma)!='N'); 

    return 0;

}

int creaFilePath(char * nomefisico, char * path){
    
    FILE *flogico;

    char buffer[30];

    strcpy(buffer, path);            

    strcat(buffer, nomefisico);      

    flogico = fopen(buffer, "wb");

    if(flogico != NULL){
        
        fclose(flogico);

        return 1;

    } else{

        return 0;

    }

}
