/*TRIFAN BOGDAN-CRISTIAN , 312CD*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema-1.h"





int main()
{
    int nr = ZERO;
    Coada* coada_execute_update = CreareCoada();    // retine numele functii
    SStiva undo_stiva = (SStiva) calloc(1, sizeof(NodStiva));               // retine numele functiei
    SStiva redo_stiva = (SStiva) calloc(1, sizeof(NodStiva));              // retine numele functiei
    LBanda* banda_merge = CreareBanda();   // retine banda 


    FILE *fin = fopen("tema1.in" , "r");
    FILE *fout= fopen("tema1.out" , "w");
    fscanf(fin, "%d", &nr);
    // printf("%d\n" , nr);

    //display(banda_merge, fout);
    /* testare stiva
    SStiva s= (SStiva) malloc(sizeof(NodStiva));   
    Nod* nod1 = CreareNod("1"); StivaPush(&s , nod1);
    Nod* nod2 = CreareNod("2"); StivaPush(&s , nod2);
    Nod* nod3 = CreareNod("3"); StivaPush(&s , nod3);

   printf("%s\n" , s->informatie->informatie);   StivaPop(&s); 
    printf("%s\n" , s->informatie->informatie);  StivaPop(&s);
    printf("%s\n" , s->informatie->informatie); StivaPop(&s);   
   */ 



    char  linie[LUNGIME_LINII];
    // printf("%s = %d" , linie , nr );
    int i = ZERO;
    for (i = ZERO ; i <= nr ; i++) {
        // fscanf(fin , "%s" , linie);
        
        fgets(linie, LUNGIME_LINII , fin);
        // printf("%s\n" ,linie);

        if (strstr(linie , UPDATE_MV_R_C) != NULL) {
            
            CoadaInserareFinal(coada_execute_update , linie);
            //printf("MOVE RIGTH CHAR\n");
             
        } else if (strstr(linie ,  UPDATE_MV_L_C) != NULL) {
            
            CoadaInserareFinal(coada_execute_update , linie);
            // printf("MOVE LEFT CHAR\n");
             
        } else if (strstr(linie , UPDATE_MV_R) != NULL) {
            
            CoadaInserareFinal(coada_execute_update , linie);
            // printf("MOVE RIGTH\n");
             
        } else if (strstr(linie , UPDATE_MV_L) != NULL) {
           
            CoadaInserareFinal(coada_execute_update , linie);
            // printf("MOVE LEFT\n");
             
        } else if (strstr(linie , UPDATE_INSERT_L_C) != NULL) {
            
            CoadaInserareFinal(coada_execute_update , linie);
            // printf("%s\n" , linie);
             
        } else if (strstr(linie , UPDATE_INSERT_R_C) != NULL) {
            
            CoadaInserareFinal(coada_execute_update , linie);
             
        } else if (strstr(linie , SHOW_CURRENT) != NULL) {
            
            fprintf(fout , "%c\n" , banda_merge->deget->caracter);
             
        } else if (strstr(linie , SHOW_ALL) != NULL) {

            display(fout , banda_merge );
             
        } else if (strstr(linie , UNDO) != NULL ) {
            
            if( undo_stiva != NULL && undo_stiva->informatie != NULL) {
                StivaPush(&redo_stiva , banda_merge->deget); // punem in varful stivei REDO varful lui UNDO
                banda_merge->deget = undo_stiva->informatie;    // extragem pointerul din varful stivei UNDO
                StivaPop(&undo_stiva);  // stergem varful stivei UNDO
            }

        } else if (strstr(linie , REDO) != NULL ) {
            
            if (redo_stiva != NULL && redo_stiva->informatie != NULL) {
                StivaPush(&undo_stiva , banda_merge->deget);     // punem in varful stivei UNDO varful lui REDO
                banda_merge->deget = redo_stiva->informatie;  // extragem pointerul din varful stivei REDO
                StivaPop(&redo_stiva);  // stergem varful stivei REDO
            }

        } else if( strstr(linie , UPDATE_WRITE_C) != NULL) {
            
            CoadaInserareFinal( coada_execute_update , linie);
            // printf("%s\n" , coada_execute_update->final->informatie );
            //show(coada_execute_update, fout);
             
        } else if (strstr(linie , EXECUTE) != NULL) {
            
            // executia efectiva a operatiilor memorate
            if (coada_execute_update->inceput != NULL) {
                
                char cmd_to_exec[LUNGIME_LINII] ;
                strcpy(cmd_to_exec , coada_execute_update->inceput->informatie);
                CoadaDeleteFirstNode(coada_execute_update);

                if (strstr(cmd_to_exec , UPDATE_WRITE_C) != NULL) {
                    
                    // modificam santinela
                    banda_merge->deget->caracter = cmd_to_exec[POZ_WRITE_C];

                    while (undo_stiva != NULL) {
                        StivaPop(&undo_stiva);  
                    }
                    
                    while (redo_stiva!= NULL) {
                       StivaPop(&redo_stiva);
                    }
                    // printf("%s" , banda_merge->inceput->informatie);
                     
                } else if (strstr(cmd_to_exec , UPDATE_MV_L_C) != NULL) {
                    
                    // cautam caracterul in stanga degetului
                    // printf("good\n");
                    // printf("Cautam in stanga %c\n", cmd_to_exec[POZ_MV_L_C]);
                    NodBanda* aux = banda_merge->deget;
                    
                    while (aux != NULL) {
                        if (cmd_to_exec[POZ_MV_L_C] == aux->caracter) {
                            banda_merge->deget = aux;
                            break;
                        }
                        aux = aux->pred;
                    }
                    
                    if (aux==NULL) {
                        // afisam eroarea 
                        free(aux);
                        fprintf(fout, "%s\n" , EROARE);
                    }
                   

                } else if (strstr(cmd_to_exec, UPDATE_MV_R_C) != NULL) {

                    // printf("Cautam in dreapta %s\n", caracter);
                    while (banda_merge->deget != NULL && banda_merge->deget->caracter != cmd_to_exec[POZ_MV_R_C]) {
                        banda_merge->deget = banda_merge->deget->urm;
                    }
                    if (banda_merge->deget==NULL) {
                        BandaInserareFinal(banda_merge, DIEZ);
                        banda_merge->deget = banda_merge->final;
                         
                    } 
                } else if (strstr(cmd_to_exec , UPDATE_INSERT_L_C) != NULL) {
                   
                    // inserarea unui caracter la stanga si modificarea santinelei
                    BandaInserareStanga(fout , banda_merge , cmd_to_exec[POZ_INSERT_L_C]);
                     
                } else if (strstr(cmd_to_exec , UPDATE_INSERT_R_C) != NULL) {
                    
                    // inserarea unui caracter in dreapta santinelei si modificarea santinelei
                    BandaInserareDreapta(banda_merge , cmd_to_exec[POZ_INSERT_R_C]);
                     
                } else if (strstr(cmd_to_exec , UPDATE_MV_L) != NULL) {
                    
                    // mutam degetul in stanga
                    if (banda_merge->deget != banda_merge->inceput) {
                        
                        StivaPush(&undo_stiva , banda_merge->deget);
                        banda_merge->deget = banda_merge->deget->pred;
                        
                    }
                    
                } else if (strstr(cmd_to_exec , UPDATE_MV_R) != NULL) {
                    
                    StivaPush(&undo_stiva , banda_merge->deget);

                    if (banda_merge->deget == banda_merge->final) {
                        // inseram la final                      
                        BandaInserareFinal(banda_merge , DIEZ);
                        banda_merge->deget = banda_merge->final;

                    } else {
                        banda_merge->deget= banda_merge->deget->urm;
                    }
                     
                }
                
            } // altminteri nu avem ce execyta
        
        }
     
    }

    BandaStergere(banda_merge);
    CoadaFullDelete(coada_execute_update);

    while (undo_stiva != NULL) {
        StivaPop(&undo_stiva);  
    }
    
    while (redo_stiva!= NULL) {
        StivaPop(&redo_stiva);
    }
    free(undo_stiva);
    free(redo_stiva);
    //DeleteStiva(&undo_stiva);
    //DeleteStiva(&redo_stiva);
    fclose(fin);
    fclose(fout);
    return 0;
}
