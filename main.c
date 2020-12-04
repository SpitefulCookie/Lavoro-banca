#include <stdio.h>
//#include <stdlib.h>
#include <strings.h>
#include <ctype.h> //toupper()
#include <synchapi.h>//Sleep()
#include <time.h>


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

Cliente caricaCliente(char *);

int getConto(char *);

int checkInt(void);

int creaFileMovimenti(int* , char* );

                                         //      CODICE

void main(int argc, char* argv[]){

    char password[30];

    int scelta, disconnesso = 1;

    do{
        
        system("cls");

        printf("\n\t\t   SISTEMA BANCARIO\n\n\tSelezionare il portale a cui accedere:\n\n\t\t[1] Portale impiegati\n\t\t[2] Portale Clienti\n\t\t[0] Termine programma\n\n\tScelta: ");

        fflush(stdin);

        scelta = checkInt(); 
    
        switch(scelta){

            case 1:

                do{

                    system("cls");

                    printf("\n\t\tLOGIN PORTALE IMPIEGATI\n\n\t Username: impiegato_05\n\t Password: ");

                    fflush(stdin);

                    gets(password);

                    if(strcmp(password, "3PDINFO")){

                        printf("\n\t La password immessa e' errata!\n\t ");

                        Sleep(1500);

                    }

                }while(strcmp(password, "3PDINFO"));

                printf("\n\t Login effettuato, benvenuto impiegato_05!\n\t ");

                disconnesso = 0;

                Sleep(1200);

                do{

                    system("cls");

                    printf("\n\t\t   PORTALE IMPIEGATI");
                    
                    printf("\n\n\tSelezionare operazione da eseguire:\n\n\t\t[1] Apri database bancario\n\t\t[2] Crea nuovo database\n\t\t[0] Disconnetti\n\n\tScelta: ");

                    fflush(stdin);

                    scelta = checkInt();

                    switch(scelta){

                        case 1:

                            //APRI DATABASE 

                            break;
                        case 2:

                            //CREA NUOVO DATABASE

                            break;

                        case 0:

                            disconnesso = 1;

                            printf("\n\tDisconnessione in corso...");

                            Sleep(1500);

                            break;
                        
                        default:

                            printf("\n\tErrore! Scelta non consentita!");
            
                            Sleep(1500);

                            break;
                    }

                }while(!disconnesso);

                break;

            case 2:

                //portale clienti

                break;

            case 0:

                printf("\n\tTermine programma...");

                Sleep(1200);

                break;

            default:

                printf("\n\tErrore! Scelta non consentita!");
            
                Sleep(1200);

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


int getConto(char * banca){

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
