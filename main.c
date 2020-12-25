#include <stdio.h>
//#include <stdlib.h>
#include <strings.h>
#include <ctype.h> //toupper()
#include <synchapi.h> //Sleep()
#include <time.h>
#include <dirent.h>

#define PATH_SUFFIX "BANCHE\\"
#define PASSWORD "3PDINFO"

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

    char tipo[15]; // "prelievo" / "versamento"

    Data data_movimento;

    double importo;

}Movimenti;

                                         //      PROTOTIPI

void datasystem(int *, int *, int *);

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

void menuRicerca(char * );

long ricercaUsername(char * , char * );

long ricercaNominativo(char * , char * , char * );

long ricercaData(char * , Data );

long ricercaNumeroConto(char * , int );

void getPathMovimenti(char * , char *, int);

Cliente getCliente(char * , long );

                                         //      CODICE

void main(int argc, char* argv[]){

    char password[30];

    char pathBanca[100];

    char pathMovimenti[100];

    char bancaSelezionata[30]="\0", conferma = '\0';

    int scelta, disconnesso = 1, fileok = 0;

    long pos;

    strcpy(pathBanca, PATH_SUFFIX);

    mkdir(pathBanca); //Creo una directory "BANCHE" dove salverò tutti gli archivi bancari
    
    do{

        system("cls");

        printf("\n\t\t   SISTEMA BANCARIO\n\n\tSelezionare il portale a cui accedere:\n\n\t\t[1] Portale Impiegati\n\t\t[2] Portale Clienti\n\t\t[0] Termine programma\n\n\tScelta: ");

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

                        printf("\n\tLa password immessa e' errata!\n\t ");

                        //Sleep(1400); //debug inserire nuovamente

                    }

                //}while(strcmp(password, PASSWORD)); //debug inserire nuovamente

                printf("\n\tLogin effettuato, benvenuto impiegato_05!\n\t ");

                disconnesso = 0;

                //Sleep(1400); //debug inserire nuovamente

                do{

                    system("cls");

                    printf("\n\t\t   PORTALE IMPIEGATI\n");

                    if(fileok){

                        printf("\n\tArchivio bancario aperto: %s", bancaSelezionata);
                        //printf("\n\tPath dell'archivio bancario: %s", pathBanca); //debug
                        //printf("\n\tPath dei movimenti bancari:  %s", pathMovimenti); //debug
                        printf("\n\t_________________________________\n");

                    }

                    printf("\n\tSelezionare operazione da eseguire:\n\n\t\t[1] Apri archivio bancario\n\t\t[2] Crea nuovo archivio\n\t\t[3] Inserisci nuovo cliente nell'archivio\n\t\t[4] Visualizza i record nell'archivio\n\t\t[5] Ricerca cliente nell'archivio\n\t\t[0] Disconnetti\n\n\tScelta: ");

                    fflush(stdin);

                    scelta = checkInt();

                    switch(scelta){

                        case 1:
                           
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

                                    break;

                                }

                            }

                            system("cls");

                            printf("\n\t\t\t\t   PORTALE IMPIEGATI\n\t\t\tVISUALIZZAZIONE ELENCO ARCHIVI BANCARI");

                            if(getBanca(pathBanca, pathMovimenti, bancaSelezionata)){

                                getPathMovimenti(pathMovimenti, bancaSelezionata, 0);
                                
                                fileok = 1;

                                printf("\tL' archivio bancario \"%s\" e' stato caricato con successo!", bancaSelezionata);

                                Sleep(1600);

                            }else{

                                printf("\n\tNon e' stata trovato nessun archivio bancario, crearne uno nuovo? [y/n] "); 

                                fflush(stdin);

                                conferma = getchar();       
                                
                                if(toupper(conferma) == 'Y'){
                                    
                                    if(!creaArchivio(pathBanca, pathMovimenti, bancaSelezionata)){printf("\n\n\tOperazione annullata");}
                                    
                                } else{printf("\n\n\tOperazione annullata");}

                            }

                            break;

                        case 2: //CREA NUOVO ARCHIVIO

                            if(fileok){

                                printf("\n\tUn archivio bancario e' gia' stato caricato, crearne uno nuovo? [y/n] ");

                                fflush(stdin);
                                
                                conferma = getchar();

                                if(toupper(conferma)== 'Y'){

                                    strcpy(bancaSelezionata, "\0");

                                    strcpy(pathBanca, PATH_SUFFIX);
                                    
                                    strcpy(pathMovimenti, "\0");

                                    fileok = 0; //il flag viene impostato a 0 poiche', qualora la creazione non vadi a buon fine, il programma pensa che e' stato selezionato un file mentre in bancaSelezionata non vi e' nulla

                                } else{

                                    break; //se non voglio creare un nuovo archivio interrompo il case

                                }

                            } 
                                                                                                                          
                            if(creaArchivio(pathBanca, pathMovimenti, bancaSelezionata)){

                                fileok = 1;

                                printf("\tL'archivio bancario \"%s\" e' stato creato con successo!", bancaSelezionata);

                            } else{

                                printf("\tNon e' stato possibile creare l'archivio!");

                                strcpy(bancaSelezionata, "\0"); //reimposto bancaSelezionata a valore nullo (il suo stato viene modificato nella funzione creaArchivio)

                            }

                            printf("\n\t");

                            system("pause");

                            break;

                        case 3:

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

                        case 4:

                            if(fileok){

                                if(!visualizzaArchivio(pathBanca)){printf("\n\tL'archivio selezionato non contiene nessun record");}

                                printf("\n\t");

                            }else{

                                printf("\n\tErrore! e' necessario aprire o creare un archivio prima di eseguire questa funzione!\n\t");

                            }

                            system("pause");

                            break;

                        case 5:

                            if (fileok){menuRicerca(pathBanca);} else{printf("\n\tErrore! e' necessario aprire o creare un archivio prima di eseguire questa funzione!\n\t");}

                            system("pause");

                            break;

                        case 0:

                            disconnesso = 1;

                            printf("\n\tDisconnessione in corso...");

                            Sleep(1400);

                            strcpy(pathBanca, PATH_SUFFIX);

                            strcpy(bancaSelezionata, "\0");

                            strcpy(pathMovimenti, "\0");

                            fileok = 0;

                            scelta = -1; //valore impostato a -1 per la successiva valutazione della variabile nel menu principale

                            break;
                        
                        default:

                            printf("\n\tErrore! Scelta non consentita!");
            
                            Sleep(1400);

                            break;

                    }

                }while(!disconnesso);

                break;

            case 2:

                system("cls");

                printf("\n\t\tLOGIN PORTALE CLIENTI\n");

                printf("\n\tElenco archivi bancari presenti:");
                
                getBanca(pathBanca, pathMovimenti, bancaSelezionata); 

                Sleep(300);

                system("cls");

                printf("\n\t\tLOGIN BANCA %s\n", strupr(bancaSelezionata));
                
                if(loginCliente(pathBanca, &pos)){

                    Cliente cliente_Letto;

                    cliente_Letto = getCliente(pathBanca, pos);

                    printf("\n\tLogin effettuato, benvenuto %s!\n\t ", cliente_Letto.nome);

                    getPathMovimenti(pathMovimenti, bancaSelezionata, cliente_Letto.numero_conto);
                    
                    Sleep(1400);

                    disconnesso = 0;

                    do{
                        
                        system("cls");

                        printf("\n\t\t   PORTALE CLIENTI\n");

                        printf("\n\tSelezionare operazione da eseguire:\n\n\t\t[1] Effettua operazione\n\t\t[2] Visualizza movimenti\n\t\t[3] Visualizza informazioni conto\n\t\t[0] Logout\n\n\tScelta: ");

                        fflush(stdin);

                        scelta = checkInt();

                        switch(scelta){
                            
                            case 1:

                                // operazione

                                break;

                            case 2:

                                // visualizza movimenti

                                break;
                            case 3:

                                system("cls");

                                printf("\n\t\t   VISUALIZZAZIONE INFORMAZIONI CONTO\n");

                                visualizzaRecordStruct(cliente_Letto);

                                printf("\n\t");

                                system("pause");

                                break;

                            case 0:

                                disconnesso = 1;

                                printf("\n\tDisconnessione in corso...");

                                strcpy(pathBanca, PATH_SUFFIX);

                                strcpy(bancaSelezionata, "\0");

                                strcpy(pathMovimenti, "\0");

                                Sleep(1400);

                                scelta = -1; //valore impostato a -1 per la successiva valutazione della variabile nel menu principale

                                break;

                            default:
                                
                                printf("\n\tErrore! Scelta non consentita!");
            
                                Sleep(1400);

                                break;
                        }

                    }while(!disconnesso);

                }               

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

int loginCliente(char * pathBanca, long * pos){

    FILE * flogico;

    Cliente clienteLetto;

    char bufferUsername [50];

    char bufferPassword [50];

    flogico = fopen(pathBanca, "rb");

    printf("\n\tUsername: ");

    fflush(stdin);
    
    gets(bufferUsername);

    *pos = ricercaUsername(pathBanca, bufferUsername); //il valore di pos varierà anche nel main

    if(*pos!=-1){

        fseek(flogico, *pos, SEEK_SET);

        fread(&clienteLetto, sizeof(Cliente), 1, flogico);

        printf("\tPassword: ");

        fflush(stdin);

        gets(bufferPassword);

        if(!strcmp(clienteLetto.username, bufferUsername) && !strcmp(clienteLetto.password, bufferPassword)){

            fclose(flogico);

            return 1;

        }else{printf("\n\tLa password immessa e' errata!\n\t "); return 0;}

    } else {

        printf("\n\tL'username inserito non e' presente nell'archivio");

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

Cliente getCliente(char * pathBanca, long pos){

    FILE * flogico;

    Cliente clientLetto;

    flogico = fopen(pathBanca, "rb");

    fseek(flogico, pos, SEEK_SET);

    fread(&clientLetto, sizeof(Cliente), 1, flogico);

    fclose(flogico);

    return clientLetto;

}

int caricaCliente(char * path, char* pathMovimenti , char * banca){
    
                                             //      VARIABILI
    
    Cliente cliente;

    int controllo = 0;

                                             //       CODICE
    
    cliente.numero_conto = getNextConto(path);

    if(creaFileMovimenti(pathMovimenti, cliente.numero_conto, banca)){ //per far si che funzioni il mio pathMovimenti deve essere BANCHE\\MOVIMENTI_<banca> !!

        system("cls");

        printf("\n\t\t   INSERIMENTO CLIENTE\n");
        
        cliente.saldo = 0;

        printf("\n\tDATI ANAGRAFICI:\n");
        
        printf("\n\tInserisci il cognome del cliente: ");

        do{
            
            fflush(stdin);

            gets(cliente.cognome);

            if(!stricmp(cliente.cognome, "") || !stricmp(cliente.cognome, " ")){

                printf(" Errore! Inserire un nome valido!\n Inserisci nome cliente: ");

            }

        }while (!stricmp(cliente.cognome, "") || !stricmp(cliente.cognome, " "));

        printf("\tInserisci nome del cliente: ");

        do{

            fflush(stdin);

            gets(cliente.nome);
            
            if(!stricmp(cliente.nome, "") || !stricmp(cliente.nome, " ")){

                printf(" Errore! Inserire un nome valido!\n Inserisci nome cliente: ");

            }

        }while (!stricmp(cliente.nome, "") || !stricmp(cliente.nome, " "));   

        printf("\tInserisci il giorno di nascita (gg): ");

        do{ 

            cliente.data_nascita.gg = checkInt();

            if (cliente.data_nascita.gg < 1 || cliente.data_nascita.gg > 31){

                printf(" Errore! Giorno invalido, inserire nuovamente: ");

            }

        } while (cliente.data_nascita.gg < 1 || cliente.data_nascita.gg > 31);
        
        printf("\tInserisci il mese di nascita (mm): ");

        do{ 

            cliente.data_nascita.mm = checkInt();

            if (cliente.data_nascita.mm < 1 || cliente.data_nascita.mm > 12){

                printf("\n Errore! Mese invalido, inserire nuovamente: ");

            }
        } while (cliente.data_nascita.mm < 1 || cliente.data_nascita.mm > 12);
        
        printf("\tInserisci l'anno di nascita (aaaa): ");

        do{ 

            cliente.data_nascita.aaaa = checkInt();

            if (cliente.data_nascita.aaaa < 1900 || cliente.data_nascita.aaaa > 2025){

                printf(" Errore! Anno invalido, inserire anno compreso tra [1900-2025]: ");

            }
        } while (cliente.data_nascita.aaaa < 1900 || cliente.data_nascita.aaaa > 2025);

        printf("\tInserisci sesso dell'cliente: ");

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

        printf("\n\tCREDENZIALI D'ACCESSO:\n");
        
        printf("\n\tInserisci l'username del cliente: ");

        do{
            
            fflush(stdin);

            gets(cliente.username);

            if(!stricmp(cliente.username, "") || !stricmp(cliente.username, " ")){

                printf(" Errore! Inserire un username valido!\n Inserisci l'username del cliente: ");

            }

        }while (!stricmp(cliente.username, "") || !stricmp(cliente.username, " "));

        printf("\tInserisci la password del cliente: ");

        do{
            
            fflush(stdin);

            gets(cliente.password);

            if(!stricmp(cliente.password, "") || !stricmp(cliente.password, " ")){

                printf("\n\tErrore! Inserire una password valida!\n\t Inserisci la password del cliente: ");

            }

        }while (!stricmp(cliente.password, "") || !stricmp(cliente.password, " "));

        printf("\n\tINFORMAZIONI FINANZIARIE:\n");

        printf("\n\tInserisci la quantita' massima di denaro prelevabile: ");
                
        fflush(stdin);

        scanf("%d", &cliente.prelievo_max);
        
        if(scriviClienteFILE(path, cliente)){return 1;} 

    }else{return 0;}

}

int creaFileMovimenti(char * pathMovimenti, int numero_conto, char* banca){ //pathmovimenti sara' BANCHE\\MOVIMENTI_<banca>\\ al momento della chiamata

    FILE * flogico;

    char nomefile [70];

    char conto [30];

    strcpy(nomefile, pathMovimenti);            //nomefile::   BANCHE\\MOVIMENTI_<banca>\\

    strcat(nomefile, banca);                    //nomefile::   BANCHE\\MOVIMENTI_<banca>\\<banca>

    strcat(nomefile, "_");                      //nomefile::   BANCHE\\MOVIMENTI_<banca>\\<banca>_

    itoa(numero_conto, conto, 10);

    strcat(nomefile, conto);                    //nomefile::   BANCHE\\MOVIMENTI_<banca>\\<banca>_<numero_conto>

    //printf("\n\nnomefile in creaFileMovimenti: %s\n\n", nomefile); //debug

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

    if(cartella == NULL){ // se non vi e' una directory "BANCHE", la creo

        mkdir(pathBanca);

        cartella = opendir(pathBanca);

    }

    while( (record = readdir(cartella)) && i<30 ){ //leggo la directory e ottengo la lista delle banche finché il mio array non e' pieno

        if(strcmp(record->d_name,".")&& strcmp(record->d_name,"..")){ //esclude '.' e ".." 

            if(strncmp(record->d_name,"MOVIMENTI_", 10)){

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

        if((j+2)%2==0){printf("\n\t\t");}else{printf("\t\t");}

        printf("[%2d] %20s", j+1, listaBanche[j]);

    }

    if(trovato){

        printf("\n\n\tImmettere l'archivio bancario desiderato: ");

        int scelta;

        do{

            scelta = checkInt();

            if(scelta <1 || scelta > i){printf("\tErrore! inserire un valore compreso tra 1 e %d: ", i);}

        }while( scelta <1 || scelta > i );

        strcpy(banca, listaBanche[scelta-1]);

        strcat(pathBanca, banca);                       //path::             BANCHE\\<banca>

        return 1;

    } else{return 0;}

}

void getPathMovimenti(char * pathMovimenti, char * banca, int n_conto){

    
    strcpy(pathMovimenti, PATH_SUFFIX);            //pathMovimenti::    BANCHE\\  
    
    strcat(pathMovimenti, "MOVIMENTI_");           //pathMovimenti::    BANCHE\\MOVIMENTI_

    strcat(pathMovimenti, banca);                  //pathMovimenti::    BANCHE\\MOVIMENTI_<banca>

    strcat(pathMovimenti, "\\");                   //pathMovimenti::    BANCHE\\MOVIMENTI_<banca>\\ 

    if(n_conto){

        strcat(pathMovimenti, banca);               //pathMovimenti::    BANCHE\\MOVIMENTI_<banca>\\<banca>

        strcat(pathMovimenti, "_");                 //pathMovimenti::    BANCHE\\MOVIMENTI_<banca>\\<banca>_

        char buffer [50];

        itoa(n_conto, buffer, 10);

        strcat(pathMovimenti, buffer);              //pathMovimenti::    BANCHE\\MOVIMENTI_<banca>\\<banca>_<n_conto>
        
    }
    
}

int creaArchivio(char * path, char* pathMovimenti, char * banca){

    char conferma = '\0';

    char buffer[30];
    
    do{ 

        system("cls");

        printf("\n\t\t   CREAZIONE ARCHIVIO BANCARIO\n");

        printf("\n\tInserire il nome dell'archivio da creare: "); //bisognerebbe forzare una lunghezza di almeno 3 caratteri

        fflush(stdin);
        
        gets(buffer);

        if(buffer == " " || buffer == "\n" || buffer == "\0"){ //controllo che non sia stato inserito un nome "vuoto" <<-- verificare che funzioni correttamente (non funziona!!)

            printf("\n\tErrore! inserire un valore valido!");

        } else{

            printf("\tIl nome del nuovo archivio sara' \"%s\", confermare? [y/n] ", buffer);

            fflush(stdin);

            conferma = getchar();

            if(toupper(conferma) == 'Y'){

                if(creaFilePath(buffer, path)){ 

                    strcpy(banca, buffer);          //  Copio il nome dell'archivio all'interno della variabile "bancaSelezionata" nel main

                    strcat(path, banca);

                    strcpy(buffer, PATH_SUFFIX);    //buffer::    BANCHE\\  <- sovrascrivo buffer e gli assegno il mio path (BANCHE\\)

                    strcat(buffer, "MOVIMENTI_");   //buffer::    BANCHE\\MOVIMENTI_

                    strcat(buffer, banca);          //buffer::    BANCHE\\MOVIMENTI_<banca>

                    strcat(buffer, "\\");           //buffer::    BANCHE\\MOVIMENTI_<banca>\\ 

                    if(!mkdir(buffer)){strcpy(pathMovimenti, buffer);}   //Creo una directory che conterrà tutti i miei file movimenti per la banca
                    
                    return 1;

                } else{

                    printf("\n\tErrore! Il file potrebbe essere gia' esistente");

                    return 0;

                }
                
            } else{return 0;}

        }

    }while(toupper(conferma)!='Y'&& toupper(conferma)!='N'); 

    return 0;

}

int scriviClienteFILE(char * path, Cliente nuovoCliente){
    
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
    
    char buffer[40];

    strcpy(buffer, path);   

    //creo il path al file e successivamente lo apro in modalita' rb, se esiste ritorno 1

    flogico = fopen(buffer, "rb");
    
    if(flogico == NULL){ //flogico avra' valore diverso da null se NON ho trovato il file

        fclose(flogico);

        return 0;

    }else{

        fclose(flogico);

        return 1;

    }

}

int checkInt(void){

    int var, is_int;

	do{
        
        fflush(stdin);
        
        if (scanf("%d", &var)){

            is_int = 1;

        }
        else {

            printf("\n\tValore invalido, inserire un numero intero! Inserire nuovamente: ");

        }
        
	} while (!is_int);
	
    is_int = 0;

	return var;
   
}

void datasystem(int *g, int *m, int *a){

    struct tm *now= NULL;

    time_t t;

    time(&t);

    now = localtime(&t);

    *g=now->tm_mday;

    *m=now->tm_mon+1;

    *a=now->tm_year+1900;
    
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
    
    printf("\n\tSaldo sul conto: %d eur",  id.saldo);
    
    printf("\n\tPrelievo massimo eseguibile: %d eur\n", id.prelievo_max);

    printf("\n\t________________________\n");

}

int visualizzaRecordPosizione(char * pathBanca, long pos){

    FILE * flogico;

    Cliente client_letto;

    flogico = fopen(pathBanca, "rb");

    if(flogico!=NULL){

        fseek(flogico, pos, SEEK_SET);

        fread(&client_letto, sizeof(Cliente), 1, flogico);

        visualizzaRecordStruct(client_letto);

        fclose(flogico);

        return 1;

    } else {return 0;}

}

int visualizzaArchivio(char * path){

    FILE * flogico;

    Cliente cliente_letto;

    int haRecord = 0;

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

}

// RICERCA

void menuRicerca(char * pathBanca){

    int scelta, numeroconto;

    long pos;

    char nome[50];

    char cognome[50];

    char username[50];

    Data data;
    
    do{

        pos = -1;

        system("cls");

        printf("\n\t\t   RICERCA CLIENTE ARCHIVIO BANCARIO\n");

        printf("\n\tSelezionare criterio di ricerca:\n\n\t\t[1] Ricerca per nominativo\n\t\t[2] Ricerca per data di nascita\n\t\t[3] Ricerca per numero di conto\n\t\t[4] Ricerca per username\n\t\t[0] Indietro\n\n\tScelta: ");

        fflush(stdin);

        scelta = checkInt();

        switch(scelta){

            case 1: //ricerca per nominativo
                    
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

            case 2: //rifcerca per data di nascita
                    
                system("cls");

                printf("\n\t\t   RICERCA CLIENTE PER DATA DI NASCITA\n");

                printf("\n\tInserire il giorno di nascita del cliente da ricercare: ");

                data.gg = checkInt();

                printf("\tInserire il mese di nascita del cliente da ricercare: ");

                data.mm = checkInt();

                printf("\tInserire l'anno di nascita del cliente da ricercare: ");

                data.aaaa = checkInt();

                pos = ricercaData(pathBanca, data);

                if (pos != -1){visualizzaRecordPosizione(pathBanca, pos);} else{printf("\n\tNon e' stato trovato alcun record che corrispondeva ai criteri desiderati");}

                break;

            case 3: //ricerca per numero del conto
                    
                system("cls");

                printf("\n\t\t   RICERCA CLIENTE PER NUMERO DI CONTO\n");

                printf("\n\tInserire il numero di conto del cliente da ricercare: ");

                numeroconto = checkInt();
                
                pos = ricercaNumeroConto(pathBanca, numeroconto);

                if (pos != -1){visualizzaRecordPosizione(pathBanca, pos);} else{printf("\n\tNon e' stato trovato alcun record che corrispondeva ai criteri desiderati");}

                Sleep(1400);

                break;

            case 4: //ricerca per username
                    
                system("cls");

                printf("\n\t\t   RICERCA CLIENTE PER USERNAME\n");

                printf("\n\tInserire l'username del cliente da ricercare: ");

                fflush(stdin);
                
                gets(username);

                pos = ricercaUsername(pathBanca, username);

                if(pos != -1){visualizzaRecordPosizione(pathBanca, pos);} else{printf("\n\tNon e' stato trovato alcun record che corrispondeva ai criteri desiderati");}

                break;

            default:

                if(scelta){printf("\n\tErrore! Scelta non consentita!");}
                
                break; 

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

    long pos =- 1;

    flogico = fopen(pathBanca, "rb");

    fread(&cliente_letto, sizeof(Cliente), 1, flogico);

    while(!feof(flogico)){

        if(!stricmp(cliente_letto.username, username)){

            pos = ftell(flogico) - sizeof(Cliente); 

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

/*int eseguiOperazione(char* pathMovimenti, Cliente datiCliente, const char operazione){

    FILE * fileMovimenti;

}*/