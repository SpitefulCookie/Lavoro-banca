#include <stdio.h>
#include <strings.h>
#include <ctype.h> //toupper()
#include <synchapi.h> //Sleep()
#include <time.h>
#include <dirent.h>

#define PATH_SUFFIX "BANCHE\\"
#define PASSWORD "3PDINFO"
#define PRELIEVO_MINIMO 1.00 // Assegnare solamente valori positivi
#define DEPOSITO_MINIMO 1.00 // Assegnare solamente valori positivi 
#define ANNO_MIN 1900
#define MIN_PRELEVABILE 500.00  //Costante valutata all'interno dell'inserimento del cliente. Nonostante la semantica ingannevole, indica il valore minimo che prelievomax puo' assumere (MIN_PRELIEVOMAX era abbastanza atroce come nome)
#define GIORNO_MIN 1
#define GIORNO_MAX 31
#define MESE_MIN 1
#define MESE_MAX 12

                                         //      TIPI

typedef struct Data {

    unsigned short int gg;

    unsigned short int mm;

    unsigned short int aaaa;

}Data;

typedef struct Cliente {  

    unsigned int numero_conto;
    
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

    int numero_operazione;
    
    char tipo[15]; // "Prelievo" / "Versamento"

    Data data_movimento;

    double importo;

}Movimenti;

                                         //      PROTOTIPI

void datasystem(Data *);

int getBanca(char*, char*, char * );

int caricaCliente(char *, char *, char *);

int getNextConto(char *);

int checkInt(void);

int creaFileMovimenti(char*, int , char* );

int creaArchivio(char*, char *, char *);

int creaFilePath (char * , char * );

int esisteFile(char * );

int scriviClienteFILE(char*, Cliente);

void visualizzaRecordStruct(Cliente);

int visualizzaArchivio(char *);

void menuRicerca(char *);

int visualizzaRecordPosizione(char * , long );

void menuVisualizzazioneMovimenti(char *);

long ricercaUsername(char * , char * );

long ricercaNominativo(char * , char * , char * );

long ricercaData(char * , Data );

long ricercaNumeroConto(char * , int );

void generaPathMovimenti(char * , char *, int);

Cliente getCliente(char * , long );

int setCliente(char *, Cliente);

void printMovimento(Movimenti );

int eseguiOperazione(char* , Cliente *, char * );

double checkDouble(void);

int visualizzaMovimenti(char *);

int visualizzaMovimentiTipo(char *, char *);

int visualizzaMovimentiMese(char *, Data);

void resetBanche(char * , char * , char * );

int loginCliente(char * , long * );

                                         //      CODICE

void main(int argc, char* argv[]){

    char password[30];

    char pathBanca[100];

    char pathMovimenti[100];

    char bancaSelezionata[50]="\0", conferma;

    int scelta, disconnesso = 1, fileok = 0;

    strcpy(pathBanca, PATH_SUFFIX); // Inizializzo il mio pathBanca a BANCHE\

    mkdir(pathBanca); // Creo una directory "BANCHE" dove salverò tutti gli archivi bancari

    system("color f0"); // nero su bianco, puramente per motivi d'aspetto 
    
    do{

        system("cls");

        printf("\n\t\t   SISTEMA BANCARIO\n\n\t"
                "Selezionare il portale a cui accedere:\n\n\t\t"
                "[1] Portale Impiegati\n\t\t"
                "[2] Portale Clienti\n\t\t"
                "[0] Termine programma\n\n\t"
                "Scelta: ");

        fflush(stdin);

        scelta = checkInt(); 
    
        switch(scelta){

            case 1: { // PORTALE IMPIEGATI

                do{

                    system("cls");

                    printf("\n\t\t\tLOGIN PORTALE IMPIEGATI\n");

                    printf("\t___________________________________________________\n");
                    
                    printf("\n\t\tUsername: impiegato_05\n\t\tPassword: ");

                    fflush(stdin);

                    gets(password);

                    if(strcmp(password, PASSWORD)){

                        printf("\n\tLa password immessa e' errata!\n\t ");

                        Sleep(1400); //

                    }

                } while(strcmp(password, PASSWORD)); //

                printf("\n\tLogin effettuato, benvenuto impiegato_05!\n\t ");

                disconnesso = 0;

                Sleep(1400); //

                do{

                    system("cls");

                    printf("\n\t\t   PORTALE IMPIEGATI\n");

                    if(fileok){

                        printf("\n\tArchivio bancario aperto: %s", bancaSelezionata);
                        //printf("\n\tPath dell'archivio bancario: %s", pathBanca); //debug
                        //printf("\n\tPath dei movimenti bancari:  %s", pathMovimenti); //debug
                        printf("\n\t_________________________________\n");

                    }

                    printf("\n\tSelezionare operazione da eseguire:\n\n\t\t"

                                "[1] Apri archivio bancario\n\t\t"
                                "[2] Crea nuovo archivio\n\t\t"
                                "[3] Inserisci nuovo cliente nell'archivio\n\t\t"
                                "[4] Visualizza i record nell'archivio\n\t\t"
                                "[5] Ricerca cliente nell'archivio\n\t\t"
                                "[6] Visualizza movimenti conto corrente\n\t\t"
                                "[0] Disconnetti\n\n\t"

                                "Scelta: ");

                    fflush(stdin);

                    scelta = checkInt();

                    switch(scelta){

                        case 1: { // APRI ARCHIVIO
                           
                            if(fileok){

                                printf("\n\tUn archivio bancario e' gia' caricato, caricarne uno nuovo? [y/n] ");

                                fflush(stdin);
                                
                                conferma = getchar();

                                if(toupper(conferma) == 'Y'){

                                    strcpy(bancaSelezionata, "\0");

                                    strcpy(pathBanca, PATH_SUFFIX); 

                                    strcpy(pathMovimenti, "\0");

                                    fileok = 0; 

                                } else{

                                    printf("\n\tOperazione annullata!");

                                    Sleep(1400);
                                    
                                    break;

                                }

                            }

                            system("cls");

                            printf("\n\t\t\t\t   PORTALE IMPIEGATI\n\t\t\tVISUALIZZAZIONE ELENCO ARCHIVI BANCARI");

                            if(getBanca(pathBanca, pathMovimenti, bancaSelezionata)){

                                generaPathMovimenti(pathMovimenti, bancaSelezionata, 0);
                                
                                fileok = 1;

                                printf("\tL' archivio bancario \"%s\" e' stato caricato con successo!", bancaSelezionata);

                                Sleep(1600);

                            }else{

                                printf("\n\tNon e' stata trovato nessun archivio bancario, crearne uno nuovo? [y/n] "); 

                                fflush(stdin);

                                conferma = getchar();       
                                
                                if(toupper(conferma) == 'Y'){
                                    
                                    if(!creaArchivio(pathBanca, pathMovimenti, bancaSelezionata)){printf("\n\n\tOperazione annullata");}else{
                                        
                                        fileok = 1;

                                        printf("\tL' archivio bancario \"%s\" e' stato creato con successo!", bancaSelezionata);

                                        Sleep(1600);

                                    }
                                    
                                } else{printf("\n\n\tOperazione annullata");}

                            }

                            break;
                        }

                        case 2: { // CREA NUOVO ARCHIVIO

                            if(fileok){

                                printf("\n\tUn archivio bancario e' gia' stato caricato, crearne uno nuovo? [y/n] ");

                                fflush(stdin);
                                
                                conferma = getchar();

                                if(toupper(conferma) == 'Y'){

                                    resetBanche(pathBanca, bancaSelezionata, pathMovimenti);

                                    fileok = 0; //il flag viene impostato a 0 poiche', qualora la creazione (eseguita qui sotto) non vadi a buon fine, il programma pensa che e' stato selezionato un file mentre in bancaSelezionata non vi e' nulla

                                } else{break;} //se non voglio creare un nuovo archivio interrompo il case

                            } 
                                                                                                                          
                            if(creaArchivio(pathBanca, pathMovimenti, bancaSelezionata)){

                                fileok = 1;

                                printf("\tL'archivio bancario \"%s\" e' stato creato con successo!\n\t", bancaSelezionata);

                            } else{

                                resetBanche(NULL, bancaSelezionata, NULL); //reimposto bancaSelezionata a valore nullo (il suo stato viene modificato nella funzione creaArchivio)

                            }

                            system("pause");

                            break;

                        }

                        case 3: { // INSERISCI CLIENTE

                            if(fileok){
                                
                                if(caricaCliente(pathBanca, pathMovimenti, bancaSelezionata)){

                                    printf("\n\tInserimento cliente avvenuto con successo!\n\t");

                                } else{

                                    printf("\n\tInserimento cliente fallito!\n\t");

                                }

                            } else{

                                printf("\n\tErrore! e' necessario aprire o creare un archivio prima di eseguire questa funzione!\n\t");

                            }

                            system("pause");

                            break;

                        }

                        case 4: { // VISUALIZZA I RECORD NELL'ARCHIVIO

                            if(fileok){

                                if(!visualizzaArchivio(pathBanca)){printf("\n\tL'archivio selezionato non contiene nessun record");}

                            }else{

                                printf("\n\tErrore! e' necessario aprire o creare un archivio prima di eseguire questa funzione!");

                            }

                            printf("\n\t");
                            
                            system("pause");

                            break;

                        }

                        case 5: { // RICERCA CLIENTE NELL'ARCHIVIO

                            if (fileok){menuRicerca(pathBanca);} else{printf("\n\tErrore! e' necessario aprire o creare un archivio prima di eseguire questa funzione!\n\t"); system("pause");}

                            break;

                        }

                        case 6: { // VISUALIZZA MOVIMENTI CONTO CORRENTE

                            if (fileok){

                                system("cls");

                                printf("\n\t\t\t\t   VISUALIZZAZIONE MOVIMENTI CONTO\n");

                                printf("\n\tInserire il numero di conto di cui si vogliono visualizzare i movimenti, 0 per annullare: ");

                                scelta = checkInt();

                                if(!scelta){
                                    
                                    printf("\n\n\tOperazione annullata\n\t");
                                    
                                    system("pause"); 

                                    break;
                                    
                                } else{

                                    generaPathMovimenti(pathMovimenti, bancaSelezionata, scelta);

                                    Sleep(500);

                                    if(esisteFile(pathMovimenti)){ //non vi e' alcun controllo sulla presenza o meno di un cliente con numero di conto x nell'archivio principale

                                        menuVisualizzazioneMovimenti(pathMovimenti);
                                    
                                    } else{

                                        printf("\n\tErrore! Il conto desiderato o il suo record movimenti non risultano presenti nell'archivio\n\t"); 

                                        system("pause");

                                    }

                                }
                                
                            } else{

                                printf("\n\tErrore! e' necessario aprire o creare un archivio prima di eseguire questa funzione!\n\t");

                                system("pause");

                            }

                            break;

                        }

                        case 0: { // DISCONNETTI

                            disconnesso = 1;

                            printf("\n\tDisconnessione in corso...");

                            Sleep(1400);

                            resetBanche(pathBanca, bancaSelezionata, pathMovimenti);

                            fileok = 0;

                            scelta = -1; //valore impostato a -1 per la successiva valutazione della variabile nel menu principale

                            break;

                        }

                        default: {

                            printf("\n\tErrore! Scelta non consentita!");
            
                            Sleep(1400);

                            break;

                        }

                    }

                }while(!disconnesso);

                break;

            }

            case 2: { // PORTALE CLIENTI

                long pos; 
                
                system("cls");

                printf("\n\t\t\tLOGIN PORTALE CLIENTI");

                printf("\n\n\tElenco archivi bancari presenti:");
                
                if(getBanca(pathBanca, pathMovimenti, bancaSelezionata)){

                    Sleep(300);

                    system("cls");

                    printf("\n\t\t\tLOGIN BANCA %s\n", strupr(bancaSelezionata));

                    printf("\t___________________________________________________\n");
                    
                    if(loginCliente(pathBanca, &pos)){

                        Cliente cliente_Letto;

                        cliente_Letto = getCliente(pathBanca, pos);

                        printf("\n\tLogin effettuato, benvenuto %s! ", cliente_Letto.nome);

                        generaPathMovimenti(pathMovimenti, bancaSelezionata, cliente_Letto.numero_conto);
                        
                        Sleep(1400);

                        disconnesso = 0;

                        do{
                            
                            system("cls");

                            printf("\n\t\t\tPORTALE CLIENTI\n");

                            printf("\n\tSelezionare operazione da eseguire:\n\n\t\t"
                                    
                                    "[1] Effettua operazione\n\t\t"
                                    "[2] Visualizza movimenti\n\t\t"
                                    "[3] Visualizza informazioni conto\n\t\t"
                                    "[0] Logout\n\n\t"
                                    
                                    "Scelta: ");

                            fflush(stdin);

                            scelta = checkInt();

                            switch(scelta){
                                
                                case 1: { // EFFETTUA OPERAZIONE

                                    system("cls");

                                    printf("\n\t\t\tEFFETTUA OPERAZIONE BANCARIA\n");

                                    printf("\n\tSeleziona il tipo di operazione da eseguire\n\n\t\t"
                                            
                                            "[1] Prelievo \n\t\t"
                                            "[2] Deposito\n\n\t"
                                            
                                            "Scelta: ");

                                    do{

                                        scelta = checkInt();

                                        switch(scelta){

                                            case 1: {

                                                if(eseguiOperazione(pathMovimenti, &cliente_Letto, "Prelievo")){

                                                    setCliente(pathBanca, cliente_Letto); // La funzione setCliente salva le modifiche apportate da eseguiOperazione (saldo)

                                                    printf("\n\tOperazione andata a buon fine! Verrai ora reindirizzato al portale clienti...");

                                                    Sleep(2400);

                                                }

                                                break;

                                            }

                                            case 2: {

                                                if(eseguiOperazione(pathMovimenti, &cliente_Letto, "Deposito")){

                                                    setCliente(pathBanca, cliente_Letto); // La funzione setCliente salva le modifiche apportate da eseguiOperazione (saldo)

                                                    printf("\n\tOperazione andata a buon fine! Verrai ora reindirizzato al portale clienti...");

                                                    Sleep(2400);

                                                }

                                                break;

                                            }

                                            default: {

                                                if(scelta){printf("\n\tErrore! Scelta non consentita");}

                                                Sleep(2400);

                                                break;

                                            }

                                        }

                                    } while(scelta<0 || scelta>2);

                                    break;

                                }

                                case 2: { // VISUALIZZA MOVIMENTI

                                    menuVisualizzazioneMovimenti(pathMovimenti);

                                    break;

                                }

                                case 3: { // VISUALIZZA INFORMAZIONI

                                    system("cls");

                                    printf("\n\t\t\tVISUALIZZAZIONE INFORMAZIONI CONTO\n");

                                    visualizzaRecordStruct(cliente_Letto);

                                    printf("\n\t");

                                    system("pause");

                                    break;
                                }

                                case 0: { // LOGOUT

                                    disconnesso = 1;

                                    printf("\n\tDisconnessione in corso...");

                                    resetBanche(pathBanca, bancaSelezionata, pathMovimenti);

                                    Sleep(1400);

                                    scelta = -1; //valore impostato a -1 per la successiva valutazione della variabile nel menu principale

                                    break;

                                }

                                default: {
                                    
                                    printf("\n\tErrore! Scelta non consentita!");
                
                                    Sleep(1400);

                                    break;

                                }

                            }

                        }while(!disconnesso);

                    }           

                } else{
                   
                    printf("\n\tErrore! Impossibile caricare gli archivi bancari, contattare un impiegato. \n\n\t(Non sono presenti archivi bancari su disco, e' necessario crearne uno tramite il portale impiegato)\n\t"); // come far venire un infarto al cliente 

                    system("pause"); 
                
                }

                resetBanche(pathBanca, bancaSelezionata, NULL);

                break; 

            }

            case 0: { // TERMINE PROGRAMMA

                printf("\n\tTermine programma...");

                Sleep(1400);

                break;

            }

            default: {

                printf("\n\tErrore! Scelta non consentita!");
            
                Sleep(1400);

                break;

            }

        }   

    }while(scelta); 
   
}

// UTILITY

void generaPathMovimenti(char * pathMovimenti, char * banca, int n_conto){ //Passando 0 come ultimo parametro, la funzione si limiterà a generare un path nel formato BANCHE\\MOVIMENTI_<banca>\\ utile per la generazione di un nuovo archivio

    
    strcpy(pathMovimenti, PATH_SUFFIX);            //pathMovimenti::    BANCHE\  
    
    strcat(pathMovimenti, "MOVIMENTI_");           //pathMovimenti::    BANCHE\MOVIMENTI_

    strcat(pathMovimenti, banca);                  //pathMovimenti::    BANCHE\MOVIMENTI_<banca>

    strcat(pathMovimenti, "\\");                   //pathMovimenti::    BANCHE\MOVIMENTI_<banca>\ 

    if(n_conto){

        strcat(pathMovimenti, banca);               //pathMovimenti::    BANCHE\MOVIMENTI_<banca>\<banca>

        strcat(pathMovimenti, "_");                 //pathMovimenti::    BANCHE\MOVIMENTI_<banca>\<banca>_

        char buffer [50];

        itoa(n_conto, buffer, 10);

        strcat(pathMovimenti, buffer);              //pathMovimenti::    BANCHE\MOVIMENTI_<banca>\<banca>_<n_conto>
        
    }
    
}

int checkInt(void){

    int var, isInt = 0;

	do{
        
        fflush(stdin);
        
        if (scanf("%d", &var)){

            isInt = 1;

        } else {

            printf("\n\tValore invalido, inserire un numero intero! Inserire nuovamente: ");

        }
        
	} while (!isInt);

	return var;
   
}

double checkDouble(void){

    double var;

    int isDouble;

	do{
        
        fflush(stdin);
        
        if (scanf("%lf", &var)){

            isDouble = 1;

        }
        else {

            printf("\n\tValore invalido, inserire un numero decimale valido! Inserire nuovamente: ");

        }
        
	} while (!isDouble);
	
	return var;

}

void datasystem(Data *data){

    struct tm *now= NULL;

    time_t tempo;

    time(&tempo);

    now = localtime(&tempo);

    data->gg=now->tm_mday;

    data->mm=now->tm_mon+1;

    data->aaaa=now->tm_year+1900;
    
}

void resetBanche(char * pathBanca, char * bancaSelezionata, char * pathMovimenti){ //Passare NULL come parametro dove il reset non e' necessario

    if(pathBanca!=NULL){strcpy(pathBanca, PATH_SUFFIX);}

    if(bancaSelezionata!=NULL){strcpy(bancaSelezionata, "\0");}

    if(pathMovimenti!=NULL){strcpy(pathMovimenti, "\0");}

}

// OPERAZIONI SU FILES

int loginCliente(char * pathBanca, long * pos){

    FILE * flogico;

    Cliente clienteLetto;

    char bufferUsername [50];

    char bufferPassword [50];

    flogico = fopen(pathBanca, "rb");

    fseek(flogico, 0, SEEK_END);

    if(!ftell(flogico)){

        printf("\n\tL'archivio selezionato risulta essere vuoto\n\t");

        system("pause");

    } else{
    
        printf("\n\t\tUsername: ");

        fflush(stdin);
        
        gets(bufferUsername);

        printf("\t\tPassword: ");

        fflush(stdin);

        gets(bufferPassword);

        *pos = ricercaUsername(pathBanca, bufferUsername); //il valore di pos varierà anche nel main

        if(*pos!=-1){

            fseek(flogico, *pos, SEEK_SET);

            fread(&clienteLetto, sizeof(Cliente), 1, flogico);

            if(!strcmp(clienteLetto.username, bufferUsername) && !strcmp(clienteLetto.password, bufferPassword)){

                fclose(flogico);

                return 1;

            }else{printf("\n\tLa password immessa e' errata!\n\t"); system("pause"); return 0;}

        } else {

            printf("\n\tL'username inserito non e' presente nell'archivio\n\t");

            system("pause");

            return 0;

        }

    }

}

int caricaCliente(char * path, char* pathMovimenti , char * banca){ //pathMovimenti deve essere BANCHE\\MOVIMENTI_<banca> 
    
    Cliente cliente;

    Data dataAttuale;

    datasystem(&dataAttuale);
    
    cliente.numero_conto = getNextConto(path);

    system("cls");

    printf("\n\t\t   INSERIMENTO CLIENTE\n");
    
    cliente.saldo = 0;

    printf("\n\tDATI ANAGRAFICI:\n");
    
    printf("\n\tInserisci il cognome del cliente: ");

    do{
        
        fflush(stdin);

        gets(cliente.cognome);

        if(!stricmp(cliente.cognome, "") || !stricmp(cliente.cognome, " ")){

            printf("\n\tErrore! Inserire un nome valido!\n\tInserisci nome cliente: ");

        }

    }while (!stricmp(cliente.cognome, "") || !stricmp(cliente.cognome, " "));

    printf("\tInserisci nome del cliente: ");

    do{

        fflush(stdin);

        gets(cliente.nome);
        
        if(!stricmp(cliente.nome, "") || !stricmp(cliente.nome, " ")){

            printf("\n\tErrore! Inserire un nome valido!\n\tInserisci nome cliente: ");

        }

    }while (!stricmp(cliente.nome, "") || !stricmp(cliente.nome, " "));   

    printf("\tInserisci il giorno di nascita (gg): ");

    do{ 

        cliente.data_nascita.gg = checkInt();

        if (cliente.data_nascita.gg < GIORNO_MIN || cliente.data_nascita.gg > GIORNO_MAX){

            printf("\n\tErrore! Giorno invalido, inserire nuovamente: ");

        }

    } while (cliente.data_nascita.gg < GIORNO_MIN || cliente.data_nascita.gg > GIORNO_MAX);
    
    printf("\tInserisci il mese di nascita (mm): ");

    do{ 

        cliente.data_nascita.mm = checkInt();

        if (cliente.data_nascita.mm < MESE_MIN || cliente.data_nascita.mm > MESE_MAX){

            printf("\n\tErrore! Mese invalido, inserire nuovamente: ");

        }
    } while (cliente.data_nascita.mm < MESE_MIN || cliente.data_nascita.mm > MESE_MAX);
    
    printf("\tInserisci l'anno di nascita (aaaa): ");

    do{ 

        cliente.data_nascita.aaaa = checkInt();

        if (cliente.data_nascita.aaaa < ANNO_MIN || cliente.data_nascita.aaaa > dataAttuale.aaaa){printf("\n\tErrore! Anno invalido, inserire anno compreso tra [%4d-%4d]: ", ANNO_MIN, dataAttuale.aaaa);}

    } while (cliente.data_nascita.aaaa < ANNO_MIN || cliente.data_nascita.aaaa > dataAttuale.aaaa);

    printf("\tInserisci sesso dell'cliente: ");

    do{

        fflush(stdin);

        cliente.sesso = toupper(getchar());

        if ((cliente.sesso != 'M' && cliente.sesso != 'F')){ printf("\n\tErrore! Valore invalido! Utilizzare [M/F] ");}

    }while(cliente.sesso!='M'&&cliente.sesso!='F'); 

    printf("\n\tCREDENZIALI D'ACCESSO:\n");
    
    printf("\n\tInserisci l'username del cliente: ");

    do{
        
        fflush(stdin);

        gets(cliente.username);

        if(!stricmp(cliente.username, "") || !stricmp(cliente.username, " ")){

            printf("\n\tErrore! Inserire un username valido!\n\tInserisci l'username del cliente: ");

        }

    }while (!stricmp(cliente.username, "") || !stricmp(cliente.username, " "));

    printf("\tInserisci la password del cliente: ");

    do{
        
        fflush(stdin);

        gets(cliente.password);

        if(!stricmp(cliente.password, "") || !stricmp(cliente.password, " ")){

            printf("\n\tErrore! Inserire una password valida!\n\tInserisci la password del cliente: ");

        }

    }while (!stricmp(cliente.password, "") || !stricmp(cliente.password, " "));

    printf("\n\tINFORMAZIONI FINANZIARIE:\n");

    printf("\n\tInserisci la quantita' massima di denaro prelevabile per operazione (minimo %.2lf euro): ", MIN_PRELEVABILE);

    do{
        
        cliente.prelievo_max = checkDouble();

        if(cliente.prelievo_max < MIN_PRELEVABILE){printf("\n\tErrore! Inserire un importo maggiore o uguale a %.2lf!\n\tInserire nuovamente: ", MIN_PRELEVABILE);}

    } while(cliente.prelievo_max < MIN_PRELEVABILE);
    
    if(creaFileMovimenti(pathMovimenti, cliente.numero_conto, banca) && scriviClienteFILE(path, cliente)){
        
        return 1;
        
    } else{

        return 0;

    }    

}

int getNextConto(char * path){ // ritorna un integer > 0 no indice!

    FILE * flogico;

    Cliente cliente_letto;

    int numero_conto;

    flogico = fopen(path, "rb");

    fseek(flogico, 0, SEEK_END);

    if(!ftell(flogico)){ //se 0 (ovvero non ho alcun record nell'archvio) ritorno 1
        
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

int creaFileMovimenti(char * pathMovimenti, int numero_conto, char* banca){ //pathMovimenti sara' BANCHE\\MOVIMENTI_<banca>\\ al momento della chiamata

    FILE * flogico;

    char nomefile [70];

    char conto [6]; 

    strcpy(nomefile, pathMovimenti);            //nomefile::   BANCHE\\MOVIMENTI_<banca>\\

    strcat(nomefile, banca);                    //nomefile::   BANCHE\\MOVIMENTI_<banca>\\<banca>

    strcat(nomefile, "_");                      //nomefile::   BANCHE\\MOVIMENTI_<banca>\\<banca>_

    itoa(numero_conto, conto, 10);

    strcat(nomefile, conto);                    //nomefile::   BANCHE\\MOVIMENTI_<banca>\\<banca>_<numero_conto>

    if(!esisteFile(nomefile)){ //correggere!!

        flogico = fopen(nomefile, "wb");

        if(!(flogico == NULL)){

            fclose(flogico);

            return 1;

        }

    } else{

        return 0;

    }

}

int getBanca(char * pathBanca, char * pathMovimenti, char * banca){ 

                                         //      VARIABILI

    DIR *cartella;

    struct dirent *record;

    int trovato = 0, i = 0;

    char conferma;

    char listaBanche [30][30];

                                         //      CODICE

    cartella = opendir(pathBanca);

    if(cartella == NULL){ // se per qualche motivo non vi e' una directory "BANCHE", la creo

        mkdir(pathBanca);

        cartella = opendir(pathBanca);

    }

    while( (record = readdir(cartella)) && i<30 ){ //leggo la directory e ottengo la lista delle banche finché il mio array non e' pieno

        if(strcmp(record->d_name,".") && strcmp(record->d_name,"..")){ //esclude '.' e ".." 

            if(strncmp(record->d_name,"MOVIMENTI_", 10)){ // Se i primi 10 caratteri del nome sono DIVERSI da "MOVIMENTI_" (cartella MOVIMENTI_<banca>) ho trovato quello che volevo

                strcpy(listaBanche[i], record->d_name);

                i++;

                trovato = 1;

            }
            
        }
    
    }

    closedir(cartella);
    
    if(i==30){printf("\n\tAttenzione e' stato possibile leggere solamente i primi 30 elementi nella directory. \n\tIl record desiderato potrebbe non essere presente all'interno della lista\n");}

    printf("\n");

    int j;

    for (j=0;j<i; j++){ //print a schermo delle banche trovate, l'if permette il print su due colonne

        if((j+2)%2==0){printf("\n\t\t");}else{printf("\t");}

        printf("[%2d] %18s %9s ", j+1, listaBanche[j], " "); // %9s e' stato inserito per problemi legati all'allineamento delle colonne

    }

    if(trovato){

        printf("\n\n\tImmettere l'archivio bancario desiderato: ");

        int scelta;

        do{

            scelta = checkInt();

            if(scelta <1 || scelta > i){printf("\tErrore! inserire un valore compreso tra 1 e %d: ", i);}

        }while( scelta <1 || scelta > i );

        strcpy(banca, listaBanche[scelta-1]);

        strcat(pathBanca, banca);                       //pathBanca::             BANCHE\\<banca>

        return 1;

    } else{return 0;}

}

int creaArchivio(char * pathBanca, char* pathMovimenti, char * banca){

    char conferma;

    char buffer[100];
    
    do{ 

        system("cls");

        printf("\n\t\t\t      PORTALE IMPIEGATI\n\t\t\tCREAZIONE ARCHIVIO BANCARIO\n");

        printf("\n\tInserire il nome dell'archivio da creare: "); 

        fflush(stdin);
        
        gets(buffer);

        if(buffer == " " || buffer == "\n" || buffer == "\0"){ //controllo che non sia stato inserito un nome "vuoto" <<-- non funziona!!

            printf("\n\tErrore! inserire un valore valido!");

        } else{

            printf("\tIl nome del nuovo archivio sara' \"%s\", confermare? [y/n] ", buffer);

            fflush(stdin);

            conferma = getchar();

            if(toupper(conferma) == 'Y'){

                if(creaFilePath(buffer, pathBanca)){ 

                    strcpy(banca, buffer);                              //  Copio il nome desiderato in bancaSelezionata nel main

                    strcat(pathBanca, buffer);                          //  pathBanca:: BANCHE\<banca>

                    generaPathMovimenti(buffer, banca, 0);              //  buffer::    BANCHE\MOVIMENTI_<banca>\ 

                    if(!mkdir(buffer)){strcpy(pathMovimenti, buffer);}  //  Creo una directory che conterrà tutti i miei file movimenti per la banca
                    
                    return 1;

                } else{

                    printf("\n\tErrore! Il file potrebbe essere gia' esistente\n\t");

                    return 0;

                }
                
            } else{

                printf("\tOperazione annullata!");

                return 0;

            }

        }

    }while(toupper(conferma)!='Y' && toupper(conferma)!='N'); 

    return 0;

}

int scriviClienteFILE(char * path, Cliente nuovoCliente){ //Scrittura in coda
    
    FILE * flogico;

    flogico = fopen(path, "ab");

    fwrite(&nuovoCliente, sizeof(Cliente), 1, flogico);

    fclose(flogico);

    return 1;

}

int creaFilePath(char * nomefisico, char * path){
    
    FILE *flogico;

    char buffer[30];

    strcpy(buffer, path);            

    strcat(buffer, nomefisico); 

    if(!esisteFile(buffer)){ //se NON ho trovato il file, lo creo

        flogico = fopen(buffer, "wb");

        if(flogico != NULL){
            
            fclose(flogico);

            return 1; 
        
        } 

    } else{

        return 0;

    }

}

int esisteFile(char * path){
    
    FILE * flogico;
    
    flogico = fopen(path, "rb");
    
    if(flogico != NULL){ // flogico avra' valore null se NON ho trovato il file

        fclose(flogico);
        
        return 1;

    } else {return 0;}

}

Cliente getCliente(char * pathBanca, long pos){

    FILE * flogico;

    Cliente clientLetto;

    flogico = fopen(pathBanca, "rb");

    fseek(flogico, pos, SEEK_SET);

    fread(&clientLetto, sizeof(Cliente), 1, flogico);

    fclose(flogico);

    return clientLetto;

}

int setCliente(char * pathBanca, Cliente cliente){ // Salva il cliente passato nell'archivio eseguendo la ricerca tramite il numero di conto (identificatore univoco)

    FILE * flogico;

    flogico = fopen(pathBanca, "rb+");

    long pos = ricercaNumeroConto(pathBanca, cliente.numero_conto);

    fseek(flogico, pos, SEEK_SET);

    fwrite(&cliente, sizeof(cliente), 1, flogico);

    fclose(flogico);

    return 1;

}

int eseguiOperazione(char* pathMovimenti, Cliente *datiCliente, char * operazione){ //al momento della chiamata pathMovimenti sara' gia' completo (BANCHE\\MOVIMENTI_<banca>\\<banca>_<n_conto>)
    
    FILE * fileMovimenti;

    char conferma;

    double input;

    Movimenti movimentoLetto;

    if(esisteFile(pathMovimenti)){
        
        fileMovimenti = fopen(pathMovimenti, "rb+");

        fseek(fileMovimenti, 0, SEEK_END);

        if(ftell(fileMovimenti)){ 

            fseek(fileMovimenti, -(long)sizeof(Movimenti), SEEK_CUR);
            
            fread(&movimentoLetto, sizeof(Movimenti), 1, fileMovimenti);

            movimentoLetto.numero_operazione++;

        } else{

            movimentoLetto.numero_operazione = 1;

        }

        strcpy(movimentoLetto.tipo, operazione);

        if(!strcmp(operazione, "Prelievo")){

            system("cls");

            printf("\n\t\t\t\tEFFETTUA PRELIEVO BANCARIO\n");
            
            if(datiCliente->saldo && datiCliente->saldo >= PRELIEVO_MINIMO){
                
                printf("\n\tSaldo disponibile: %8.2lf eur\t\tPrelievo massimo per operazione: %8.2lf eur\n\n\tInserire la quantita' di denaro da prelevare, inserire 0 per annullare l'operazione.\n\n\tImporto: ",  datiCliente->saldo, datiCliente->prelievo_max);

                input = checkDouble();

                if(input<=datiCliente->prelievo_max && input <= datiCliente->saldo && input >= PRELIEVO_MINIMO){

                    if(!input){

                        printf("\n\tOperazione annullata");

                        Sleep(1200);

                        fclose(fileMovimenti);

                        return 0;

                    }
                    
                    movimentoLetto.importo = input;

                    datasystem(&movimentoLetto.data_movimento);
                    
                    printMovimento(movimentoLetto);

                    printf("\n\tConfermi di voler eseguire il movimento? [y/n]\n\n\tScelta: ");

                    do{

                        fflush(stdin);
                        
                        conferma = toupper(getchar());

                        switch(conferma){

                            case 'Y': {

                                //printf("\n\nPuntatore prima di fseek: %ul\n\n", ftell(fileMovimenti)); //debug

                                //system("pause"); //debug
                                
                                fseek(fileMovimenti, 0, SEEK_END); 
                                
                                    /*
                                    * Non ho idea del motivo per cui la scrittura su file non funzioni senza questo fseek. I valori 
                                    * del puntatore prima e dopo l'esecuzione restano gli stessi eppure senza di essa il codice non funziona. 
                                    * Ho tentato a modificare l'IF a riga 1322 (l'unico punto in cui il puntatore viene modificato prima dell'fwrite) in
                                    *   
                                    *   if(!fseek(fileMovimenti, -(long)sizeof(Movimenti), SEEK_END)){ 
                                    *        
                                    *        fread(&movimentoLetto, sizeof(Movimenti), 1, fileMovimenti);
                                    *   
                                    *        movimentoLetto.numero_operazione++;
                                    *   }
                                    * 
                                    * poiche' quanto fseek restituisce 0 in caso di successo, ma i risultati non sono cambiati.
                                    * 
                                    * Errno non restituisce alcun errore.
                                    * 
                                    */

                                //printf("\n\nPuntatore dopo di fseek: %ul\n\n", ftell(fileMovimenti)); //debug

                                //system("pause"); //debug

                                if(fwrite(&movimentoLetto, sizeof(Movimenti), 1, fileMovimenti)){
                                    
                                    datiCliente->saldo-=input;
                                
                                    fclose(fileMovimenti);

                                    return 1;

                                } else{
                                
                                    printf("\n\tErrore! impossibile eseguire l'operazione!\n\t");

                                    system("pause");

                                    fclose(fileMovimenti);

                                    return 0;
                                    
                                }

                                break;

                            }

                            case 'N': {

                                printf("\n\tOperazione annullata!\n\t");

                                fclose(fileMovimenti);

                                break;

                            }

                            default: {

                                printf("\n\tErrore! Scelta non consentita, inserire nuovamente: ");

                                break;

                            }

                        }

                    } while(conferma != 'Y' && conferma != 'N');

                } else{

                    if(input>datiCliente->prelievo_max){ 
                        
                        printf("\n\tImpossibile eseguire il prelievo, l'importo immesso eccede la quantita' di denaro prelevabile per operazione\n\t");

                    } else{ 
                        
                        if(input<PRELIEVO_MINIMO){
                            
                            printf("\n\tErrore! il prelievo minimo consentito e' %.2lf eur\n\t", PRELIEVO_MINIMO);

                        } else{
                            
                            printf("\n\tImpossibile eseguire il prelievo, l'importo immesso eccede la quantita' di denaro presente sul conto\n\t"); 

                        }

                    } 

                }

            } else{

                if(!datiCliente->saldo){
                    
                    printf("\n\tErrore! Impossibile eseguire un prelievo, il saldo sul conto e' 0 euro\n\t");
                    
                } else{
                    
                    printf("\n\tErrore! Impossibile eseguire un prelievo, il saldo sul conto e' inferiore alla quantita' minima di denaro prelevabile per operazione\n\t");
                
                }

            }

            system("pause");

            fclose(fileMovimenti);

            return 0; 

        } else{

            system("cls");

            printf("\n\t\t   EFFETTUA DEPOSITO BANCARIO\n");
            
            printf("\n\tInserire la quantita' di denaro da depositare, inserire 0 per annullare l'operazione.\n\n\tImporto: ");

            do{
                
                input = checkDouble();

                if(input>=DEPOSITO_MINIMO){

                    movimentoLetto.importo = input; 

                    datasystem(&movimentoLetto.data_movimento);
                
                    printMovimento(movimentoLetto);

                    printf("\n\tConfermi di voler eseguire il movimento? [y/n]\n\n\tScelta: ");

                    do{

                        fflush(stdin);
                        
                        conferma = toupper(getchar());

                        switch(conferma){

                            case 'Y': {

                                //printf("\n\nPuntatore prima di fseek: %ul\n\n", ftell(fileMovimenti)); //debug

                                //system("pause"); //debug
                                
                                fseek(fileMovimenti, 0, SEEK_END); //   VEDI COMMENTO RIGA 1389
                                
                                //printf("\n\nPuntatore dopo di fseek: %ul\n\n", ftell(fileMovimenti)); //debug

                                //system("pause"); //debug

                                if(fwrite(&movimentoLetto, sizeof(Movimenti), 1, fileMovimenti)){
                                
                                    datiCliente->saldo+=input;
                                
                                    fclose(fileMovimenti);

                                    return 1;

                                } else{

                                    printf("\n\tErrore! impossibile eseguire l'operazione!");

                                    system("pause");

                                    fclose(fileMovimenti);

                                    return 0;
                                
                                }

                                break;
                            }

                            case 'N': {

                                printf("\n\tOperazione annullata!");

                                fclose(fileMovimenti);

                                break;

                            }

                            default: {

                                printf("\n\tErrore! Scelta non consentita, inserire nuovamente: ");

                                break;

                            }

                        }

                    } while(conferma!='Y' && conferma != 'N');

                } else{

                    if(!input){

                        printf("\n\tOperazione annullata!");

                        fclose(fileMovimenti);

                        Sleep(1200);

                        return 0;

                    } 

                    printf("\n\tErrore! il deposito minimo consentito e' %.2lf eur\n\t", DEPOSITO_MINIMO);

                    system("pause");

                }

            } while(input<0);

        }
    
    } else{

        printf("\n\tErrore! Non è stato possibile trovare il file movimenti");

        return 0;

    }

}

// RICERCA

void menuRicerca(char * pathBanca){

    int scelta, numeroconto;

    long pos;

    char nome[50];

    char cognome[50];

    char username[50];

    Data data, dataAttuale;
    
    do{

        pos = -1;

        datasystem(&dataAttuale);

        system("cls");

        printf("\n\t\t   RICERCA CLIENTE ARCHIVIO BANCARIO\n");

        printf("\n\tSelezionare criterio di ricerca:\n\n\t\t"
               
                "[1] Ricerca per nominativo\n\t\t"
                "[2] Ricerca per data di nascita\n\t\t"
                "[3] Ricerca per numero di conto\n\t\t"
                "[4] Ricerca per username\n\t\t"
                "[0] Indietro\n\n\t"
                
                "Scelta: ");

        fflush(stdin);

        scelta = checkInt();

        switch(scelta){

            case 1: {//ricerca per nominativo
                    
                system("cls");

                printf("\n\t\t   RICERCA CLIENTE PER NOMINATIVO\n");

                printf("\n\tInserire il nome del cliente da ricercare: ");

                fflush(stdin);
                
                gets(nome);

                printf("\tInserire il cognome del cliente da ricercare: ");

                fflush(stdin);
                
                gets(cognome);
               
                pos = ricercaNominativo(pathBanca, nome, cognome);

                if (pos != -1){visualizzaRecordPosizione(pathBanca, pos);} else{printf("\n\tNon e' stato trovato alcun record che corrispondeva ai criteri desiderati");}

                break;

            }

            case 2: {//ricerca per data di nascita
                    
                system("cls");

                printf("\n\t\t   RICERCA CLIENTE PER DATA DI NASCITA\n");

                printf("\n\tInserire il giorno di nascita del cliente da ricercare: ");

                do {

                    data.gg = checkInt();

                    if(data.gg < GIORNO_MIN || data.gg > GIORNO_MAX){

                        printf("\n\tErrore! inserire un valore compreso tra %d e %d\n\tInserire nuovamente: ", GIORNO_MIN, GIORNO_MAX);

                    }

                } while(data.gg < GIORNO_MIN || data.gg > GIORNO_MAX);

                printf("\tInserire il mese di nascita del cliente da ricercare: ");

                do {

                    data.mm = checkInt();

                    if(data.mm < MESE_MIN || data.mm > MESE_MAX){

                        printf("\n\tErrore! inserire un valore compreso tra %d e %d\n\tInserire nuovamente: ", MESE_MIN, MESE_MAX);

                    }

                } while(data.mm < MESE_MIN || data.mm > MESE_MAX);

                printf("\tInserire l'anno di nascita del cliente da ricercare: ");

                do {

                    data.aaaa = checkInt();

                    if(data.aaaa < ANNO_MIN || data.aaaa > dataAttuale.aaaa){

                        printf("\n\tErrore! inserire un valore compreso tra %d e %d\n\tInserire nuovamente: ", ANNO_MIN, dataAttuale.aaaa);

                    }

                } while(data.aaaa < ANNO_MIN || data.aaaa > dataAttuale.aaaa);

                pos = ricercaData(pathBanca, data);

                if (pos != -1){visualizzaRecordPosizione(pathBanca, pos);} else{printf("\n\tNon e' stato trovato alcun record che corrispondeva ai criteri desiderati");}

                break;

            }

            case 3: { //ricerca per numero del conto
                    
                system("cls");

                printf("\n\t\t   RICERCA CLIENTE PER NUMERO DI CONTO\n");

                printf("\n\tInserire il numero di conto del cliente da ricercare, 0 per annullare l'operazione: ");

                do{
                    numeroconto = checkInt();

                    if(numeroconto<0){

                        printf("\n\tErrore! inserire un valore maggiore o uguale a 0!\n\tInserire nuovamente: ");

                    }

                }while(numeroconto < 0);

                if(numeroconto){

                    pos = ricercaNumeroConto(pathBanca, numeroconto);

                    if (pos != -1){visualizzaRecordPosizione(pathBanca, pos);} else{printf("\n\tNon e' stato trovato alcun record che corrispondeva ai criteri desiderati");}

                    Sleep(1400);

                } else {

                    printf("\n\tOperazione annullata");

                }
                
                break;

            }

            case 4: { //ricerca per username 
                system("cls");

                printf("\n\t\t   RICERCA CLIENTE PER USERNAME\n");

                printf("\n\tInserire l'username del cliente da ricercare: ");

                fflush(stdin);
                
                gets(username);

                pos = ricercaUsername(pathBanca, username);

                if(pos != -1){visualizzaRecordPosizione(pathBanca, pos);} else{printf("\n\tNon e' stato trovato alcun record che corrispondeva ai criteri desiderati");}

                break;
            }

            default: {

                if(scelta){printf("\n\tErrore! Scelta non consentita!");}
                
                break;

            }

        }

        if(scelta){ //selezione puramente per motivi di formattazione 

            printf("\n\t");

            system("pause");

        } 
    
    } while(scelta);
    
}

long ricercaUsername(char * pathBanca, char * username){

    FILE * flogico;

    Cliente cliente_letto;

    long pos = -1;

    flogico = fopen(pathBanca, "rb");

    fread(&cliente_letto, sizeof(Cliente), 1, flogico);

    while(!feof(flogico)){

        if(!strcmp(cliente_letto.username, username)){ 

            pos = ftell(flogico) - (long)sizeof(Cliente); 

            break;

        } else {fread(&cliente_letto,sizeof(Cliente), 1, flogico);}

    }

    fclose(flogico);

    return pos;

}

long ricercaNominativo(char * pathBanca, char * nome, char * cognome){ 

    FILE * flogico;

    Cliente cliente_letto;

    long pos =- 1;
    
    flogico = fopen(pathBanca, "rb");

    fread(&cliente_letto, sizeof(Cliente), 1, flogico);

    while(!feof(flogico)){

        if(!stricmp(cliente_letto.nome, nome)&&!stricmp(cliente_letto.cognome, cognome)){

            pos = ftell(flogico) - sizeof(Cliente); 

            break;

        } else {fread(&cliente_letto,sizeof(Cliente), 1, flogico);}

    }

    fclose(flogico);

    return pos;

}

long ricercaData(char * pathBanca, Data data){

    FILE * flogico;

    Cliente cliente_letto;

    long pos = -1;

    flogico = fopen(pathBanca, "rb");
        
    fread(&cliente_letto, sizeof(Cliente), 1, flogico);

    while(!feof(flogico)){

        if(cliente_letto.data_nascita.gg==data.gg && cliente_letto.data_nascita.mm==data.mm && cliente_letto.data_nascita.aaaa==data.aaaa){

            pos = ftell(flogico) - sizeof(Cliente);

            break;

        } else{fread(&cliente_letto, sizeof(Cliente), 1, flogico);}

    }

    fclose(flogico);

    return pos;

}

long ricercaNumeroConto(char * pathBanca, int numeroConto){

    FILE * flogico;

    Cliente cliente_letto;

    long pos = -1;

    flogico = fopen(pathBanca, "rb");

    fread(&cliente_letto, sizeof(Cliente), 1, flogico);

    while(!feof(flogico)){

        if(cliente_letto.numero_conto == numeroConto){

            pos = ftell(flogico) - sizeof(Cliente);

            break;

        } else{

            if(cliente_letto.numero_conto>numeroConto){break;} else{fread(&cliente_letto, sizeof(Cliente), 1, flogico);}
            
            /* 
            * poiché in quest'ambito i numeri di conto sono ad incrementi di 1, e soprattutto sono in ordine crescente, non appena andro' 
            * a valutare un numero di conto superiore a quello desiderato posso interrompere il ciclo
            */

        }

    }

    fclose(flogico);

    return pos;  

}

// VISUALIZZAZIONI 

void visualizzaRecordStruct(Cliente id){

    printf("\n\tDATI ANAGRAFICI:\n");
    
    printf("\n\tCognome: %s", id.cognome);

    printf("\n\tNome: %s", id.nome);

    printf("\n\tData di nascita: %d/%d/%d", id.data_nascita.gg, id.data_nascita.mm, id.data_nascita.aaaa);

    printf("\n\tSesso: %c\n", id.sesso);

    printf("\n\tCREDENZIALI D'ACCESSO:\n");
    
    printf("\n\tUsername: %s", id.username);

    printf("\n\tPassword: %s\n", id.password);

    printf("\n\tINFORMAZIONI FINANZIARIE:\n");

    printf("\n\tNumero di conto: %d", id.numero_conto);
    
    printf("\n\tSaldo sul conto: %.2lf eur",  id.saldo);
    
    printf("\n\tPrelievo massimo eseguibile: %.2lf eur\n", id.prelievo_max);

    printf("\n\t________________________\n");

}

int visualizzaRecordPosizione(char * pathBanca, long pos){

    FILE * flogico;

    Cliente cliente_letto;

    if(esisteFile(pathBanca)){

        /* 
         *  In teoria non sarebbe necessario effettuare questo controllo in quanto viene eseguito un controllo a monte 
         *  dell'esistenza del file, tuttavia questa funzione mi permette di verificare implicitamente qualora il file risulti
         *  aperto in qualche altra porzione di codice
         * 
         */

        flogico = fopen(pathBanca, "rb");

        fseek(flogico, pos, SEEK_SET);

        fread(&cliente_letto, sizeof(Cliente), 1, flogico);

        visualizzaRecordStruct(cliente_letto);

        fclose(flogico);

        return 1;

    } else {return -1;}

}

int visualizzaArchivio(char * path){

    FILE * flogico;

    Cliente cliente_letto;

    int haRecord = 0;

    if(esisteFile(path)){ 
        
        /* 
         *  In teoria non sarebbe necesario effettuare questo controllo in quanto viene eseguito un controllo a monte 
         *  dell'esistenza del file, tuttavia questa funzione mi permette di verificare implicitamente qualora il file risulti
         *  aperto in qualche altra porzione di codice
         * 
         */

        system("cls");

        printf("\n\t\t   VISUALIZZAZIONE ARCHIVIO BANCARIO\n");
        
        flogico = fopen(path, "rb");

        fread(&cliente_letto, sizeof(Cliente), 1, flogico);

        while(!feof(flogico)){

            haRecord = 1;

            visualizzaRecordStruct(cliente_letto);

            fread(&cliente_letto, sizeof(Cliente), 1, flogico);

        }

        fclose(flogico);

        return haRecord;

    } else{

        printf("\n\tErrore! Impossibile visualizzare l'archvio");

        return -1;

    }

}

void printMovimento(Movimenti movimento){

    printf("\n\t____________________________________________________________");

    printf("\n\n\tNumero operazione: %6d\n\n\tTipo operazione: %s\t Data operazione: %d/%d/%d\n\n\t", movimento.numero_operazione, movimento.tipo, movimento.data_movimento.gg,movimento.data_movimento.mm, movimento.data_movimento.aaaa);

    printf("Importo: %12.2lf eur\n", movimento.importo);

    printf("\n\t____________________________________________________________\n");

}

void menuVisualizzazioneMovimenti(char *pathMovimenti){

    int scelta;

    Data dataAttuale;

    datasystem(&dataAttuale);
    
    do{

        system("cls");

        printf("\n\t\t\tVISUALIZZAZIONE MOVIMENTI BANCARI\n");

        printf("\n\tSeleziona il criterio di visualizzazione dei movimenti bancari:\n\n\t\t"
                
                "[1] Visualizza tutti \n\t\t"
                "[2] Visualizza per mese\n\t\t"
                "[3] Visualizza per tipo\n\t\t"
                "[0] Indietro\n\n\t"
                
                "Scelta: ");
        
        scelta = checkInt();

        switch(scelta){

            case 1: {

                system("cls");

                printf("\n\t\t\tVISUALIZZA TUTTI I MOVIMENTI\n");

                visualizzaMovimenti(pathMovimenti);

                printf("\n\n\t");

                system("pause");
                
                break;

            }

            case 2:{

                system("cls");

                printf("\n\t\t\tVISUALIZZA MOVIMENTI PER MESE\n");

                printf("\n\tInserire mese [%d-%d]: ", MESE_MIN, MESE_MAX);

                Data data;

                do {

                    data.mm = checkInt();

                    if(data.mm < MESE_MIN || data.mm > MESE_MAX){

                        printf("\n\tErrore! inserire un valore compreso tra %d e %d\n\tInserire nuovamente: ", MESE_MIN, MESE_MAX);

                    }

                } while(data.mm < MESE_MIN || data.mm > MESE_MAX);

                printf("\tInserire anno [%d-%d]: ", ANNO_MIN, dataAttuale.aaaa);

                do {

                    data.aaaa = checkInt();

                    if(data.aaaa < ANNO_MIN || data.aaaa > dataAttuale.aaaa){

                        printf("\n\tErrore! inserire un valore compreso tra %d e %d\n\tInserire nuovamente: ", ANNO_MIN, dataAttuale.aaaa);

                    }

                } while(data.aaaa < ANNO_MIN || data.aaaa > dataAttuale.aaaa);

                visualizzaMovimentiMese(pathMovimenti, data);

                printf("\n\n\t");

                system("pause");
                
                break;

            }

            case 3: {

                system("cls");

                printf("\n\t\t\tVISUALIZZA MOVIMENTI PER TIPO\n");

                printf("\n\tVisualizza:\n\n\t\t"
                        
                        "[1] Prelievi \n\t\t"
                        "[2] Depositi\n\t\t"
                        "[0] Indietro\n\n\t"
                        
                        "Scelta: ");

                scelta = checkInt();

                if(scelta == 1){
                    
                    visualizzaMovimentiTipo(pathMovimenti, "Prelievo");
                
                } else{
                    
                    if(scelta){ //Escludo lo 0 poiche' verra' utilizzato per annullare
                        
                        visualizzaMovimentiTipo(pathMovimenti, "Deposito");
                    
                    }else{printf("\n\tOperazione annullata");}

                }

                printf("\n\n\t");

                system("pause");

                break;

            }

            default: {

                if(scelta){
                    
                    printf("\n\tErrore! Scelta non consentita");
                
                    printf("\n\t");

                    system("pause");
                
                }else{Sleep(800);}

                break;

            }

        }
        
    }while(scelta);

}

int visualizzaMovimenti(char * pathMovimenti){

    FILE * filemovimenti;

    Movimenti movimentoLetto;

    int haRecord = 0;

    if(esisteFile(pathMovimenti)){

        /* 
         *  In teoria non sarebbe necesario effettuare questo controllo in quanto viene eseguito un controllo a monte 
         *  dell'esistenza del file, tuttavia questa funzione mi permette di verificare implicitamente qualora il file risulti
         *  aperto in qualche altra porzione di codice
         * 
         */

        filemovimenti = fopen(pathMovimenti, "rb");

        fread(&movimentoLetto, sizeof(Movimenti), 1, filemovimenti);

        if(!feof(filemovimenti)){

            do{ 

                printMovimento(movimentoLetto);

                haRecord = 1;

                fread(&movimentoLetto, sizeof(Movimenti), 1, filemovimenti);

            }while(!feof(filemovimenti));

        }else{printf("\n\tNon vi sono movimenti presenti all'interno del conto");}
        
        fclose(filemovimenti);

        return haRecord;

    } else{

        return -1;

    }

}

int visualizzaMovimentiMese(char * pathMovimenti, Data data){ // aggiungere controllo sulla data?

    FILE * filemovimenti;

    Movimenti movimentoLetto;

    Movimenti listaMovimenti[10];

    int ultimoRecordLetto = 0, i = 0, ultimoRecordArchivio, j, trovato = 0;

    char mese[10];

    switch(data.mm){

        case 1: strcpy(mese, "Gennaio"); break;
        case 2: strcpy(mese, "Febbraio"); break;
        case 3: strcpy(mese, "Marzo"); break;
        case 4: strcpy(mese, "Aprile"); break;
        case 5: strcpy(mese, "Maggio"); break;
        case 6: strcpy(mese, "Giugno"); break;
        case 7: strcpy(mese, "Luglio"); break;
        case 8: strcpy(mese, "Agosto"); break;
        case 9: strcpy(mese, "Settembre"); break;
        case 10: strcpy(mese, "Ottobre"); break;
        case 11: strcpy(mese, "Novembre"); break;
        case 12: strcpy(mese, "Dicembre"); break;

    }

    if(esisteFile(pathMovimenti)){

        /* 
         *  In teoria non sarebbe necessario effettuare questo controllo in quanto viene eseguito un controllo a monte 
         *  dell'esistenza del file, tuttavia questa funzione mi permette di verificare implicitamente qualora il file risulti
         *  aperto in qualche altra porzione di codice
         * 
         */
    
        filemovimenti = fopen(pathMovimenti, "rb");

        fread(&movimentoLetto, sizeof(Movimenti), 1, filemovimenti); 
        
        /*
        * Poiche' feof() non funziona senza effettuare una lettura prima sono costretto a leggere il primo record.
        * Un alternativa sarebbe stata if(fseek(filemovimenti, -(long)sizeof(Movimenti), SEEK_END)) come nella
        * funzione eseguiOperazione tuttavia, come gia' menzionato in un commento precedente, l'uso di questa istruzione
        * sembrerebbe avere dei comportamenti a me anomali che impediscono il corretto funzionamento di fwrite().
        * 
        */

        if(!feof(filemovimenti)){

            fseek(filemovimenti, -(long)sizeof(Movimenti), SEEK_END);

            fread(&movimentoLetto, sizeof(Movimenti), 1, filemovimenti); //leggo l'ultimo record nel file

            ultimoRecordArchivio = movimentoLetto.numero_operazione;

            rewind(filemovimenti);

            fread(&movimentoLetto, sizeof(Movimenti), 1, filemovimenti); //leggo nuovamente il primo record

        } else{

            printf("\n\tNon sono presenti movimenti all'interno del file.");

            return trovato;

        }

        system("cls");

        printf("\n\t\t\tVISUALIZZA MOVIMENTI PER MESE\n");

        printf("\n\tMovimenti effettuati nel mese di %s %d:\n", mese, data.aaaa); 

        printf("\n\t____________________________________________\n");
        
        while(ultimoRecordLetto<ultimoRecordArchivio){
            
            while(!feof(filemovimenti) && i < 10){

                if(movimentoLetto.data_movimento.mm == data.mm && movimentoLetto.data_movimento.aaaa ==data.aaaa){

                    listaMovimenti[i] = movimentoLetto;

                    i++;

                    trovato = 1;

                }

                ultimoRecordLetto = movimentoLetto.numero_operazione;

                fread(&movimentoLetto, sizeof(Movimenti), 1, filemovimenti);

            }
            
            if(i){

                for(j = 0; j < i; j++){printMovimento(listaMovimenti[j]);}

                i = 0;

            } 
        
        } 

        if(!trovato){

            printf("\n\tNon sono stati trovati movimenti che corrispondevano ai criteri di ricerca.");

            return trovato;

        }

        return trovato;

    } else{

        return -1;

    }
}

int visualizzaMovimentiTipo(char * pathMovimenti, char * tipo){

    FILE * filemovimenti;

    Movimenti movimentoLetto;

    Movimenti listaMovimenti[10];

    int ultimoRecordLetto = 0, i = 0, ultimoRecordArchivio, j, trovato = 0;

    if(esisteFile(pathMovimenti)){

        /* 
         *  In teoria non sarebbe necessario effettuare questo controllo in quanto viene eseguito un controllo a monte 
         *  dell'esistenza del file, tuttavia questa funzione mi permette di verificare implicitamente qualora il file risulti
         *  aperto in qualche altra porzione di codice
         * 
         */
    
        filemovimenti = fopen(pathMovimenti, "rb");

        fread(&movimentoLetto, sizeof(Movimenti), 1, filemovimenti); 
        
        /*
        * Poiche' feof() non funziona senza effettuare una lettura prima sono costretto a leggere il primo record.
        * Un alternativa sarebbe stata if(fseek(filemovimenti, -(long)sizeof(Movimenti), SEEK_END)) come nella
        * funzione eseguiOperazione tuttavia, come gia' menzionato in un commento precedente, l'uso di questa istruzione
        * sembrerebbe avere dei comportamenti a me anomali che impediscono il corretto funzionamento di fwrite().
        * 
        */

        if(!feof(filemovimenti)){

            fseek(filemovimenti, -(long)sizeof(Movimenti), SEEK_END);

            fread(&movimentoLetto, sizeof(Movimenti), 1, filemovimenti); //leggo l'ultimo record nel file

            ultimoRecordArchivio = movimentoLetto.numero_operazione;

            rewind(filemovimenti);

            fread(&movimentoLetto, sizeof(Movimenti), 1, filemovimenti); //leggo nuovamente il primo record

        } else{

            printf("\n\tNon sono presenti movimenti all'interno del file.");

            return trovato;

        }

        system("cls");

        printf("\n\t\t\tVISUALIZZA MOVIMENTI PER TIPO\n");

        if(!strcmp(tipo, "Prelievo")){printf("\n\tPrelievi effettuati: \n");}else{printf("\n\tDepositi effettuati: \n");}
        
        while(ultimoRecordLetto<ultimoRecordArchivio){
            
            while(!feof(filemovimenti) && i < 10){

                if(!strcmp(movimentoLetto.tipo, tipo)){

                    listaMovimenti[i] = movimentoLetto;

                    i++;

                    trovato = 1;

                }

                ultimoRecordLetto = movimentoLetto.numero_operazione;

                fread(&movimentoLetto, sizeof(Movimenti), 1, filemovimenti);

            }
            
            if(i){

                for(j = 0; j < i; j++){printMovimento(listaMovimenti[j]);}

                i = 0;

            } 
        
        } 

        if(!trovato){

            printf("\n\tNon sono stati trovati movimenti che corrispondevano ai criteri di ricerca.");

            return trovato;

        }

        return trovato;

    } else{

        return -1;

    }

}
