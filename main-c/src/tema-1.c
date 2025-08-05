/*TRIFAN BOGDAN-CRISTIAN , 312CD*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema-1.h"


void exec_operation(FILE* fout, Coada* coada, LBanda* banda, SStiva* undo, SStiva* redo) {
    if (coada->inceput == NULL) return;

    char cmd_to_exec[LUNGIME_LINII];
    strcpy(cmd_to_exec, coada->inceput->informatie);
    CoadaDeleteFirstNode(coada);

    if (strstr(cmd_to_exec, UPDATE_WRITE_C)) {
        banda->deget->caracter = cmd_to_exec[POZ_WRITE_C];
        while (*undo) StivaPop(undo);
        while (*redo) StivaPop(redo);

    } else if (strstr(cmd_to_exec, UPDATE_MV_L_C)) {
        NodBanda* aux = banda->deget;
        while (aux) {
            if (cmd_to_exec[POZ_MV_L_C] == aux->caracter) {
                banda->deget = aux;
                return;
            }
            aux = aux->pred;
        }
        fprintf(fout, "%s\n", EROARE);

    } else if (strstr(cmd_to_exec, UPDATE_MV_R_C)) {
        while (banda->deget && banda->deget->caracter != cmd_to_exec[POZ_MV_R_C]) {
            banda->deget = banda->deget->urm;
        }
        if (!banda->deget) {
            BandaInserareFinal(banda, DIEZ);
            banda->deget = banda->final;
        }

    } else if (strstr(cmd_to_exec, UPDATE_INSERT_L_C)) {
        BandaInserareStanga(fout, banda, cmd_to_exec[POZ_INSERT_L_C]);

    } else if (strstr(cmd_to_exec, UPDATE_INSERT_R_C)) {
        BandaInserareDreapta(banda, cmd_to_exec[POZ_INSERT_R_C]);

    } else if (strstr(cmd_to_exec, UPDATE_MV_L)) {
        if (banda->deget != banda->inceput) {
            StivaPush(undo, banda->deget);
            banda->deget = banda->deget->pred;
        }

    } else if (strstr(cmd_to_exec, UPDATE_MV_R)) {
        StivaPush(undo, banda->deget);
        if (banda->deget == banda->final) {
            BandaInserareFinal(banda, DIEZ);
            banda->deget = banda->final;
        } else {
            banda->deget = banda->deget->urm;
        }
    }
}


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

    char  linie[LUNGIME_LINII];
    int i = ZERO;
    for (i = ZERO ; i <= nr ; i++) {
        fgets(linie, LUNGIME_LINII , fin);

        if (strstr(linie , UPDATE_MV_R_C) != NULL) {
            CoadaInserareFinal(coada_execute_update , linie);
        } else if (strstr(linie ,  UPDATE_MV_L_C) != NULL) {
            CoadaInserareFinal(coada_execute_update , linie);
        } else if (strstr(linie , UPDATE_MV_R) != NULL) {
            CoadaInserareFinal(coada_execute_update , linie);
        } else if (strstr(linie , UPDATE_MV_L) != NULL) {
            CoadaInserareFinal(coada_execute_update , linie);
        } else if (strstr(linie , UPDATE_INSERT_L_C) != NULL) {
            CoadaInserareFinal(coada_execute_update , linie);
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
        } else if (strstr(linie , EXECUTE) != NULL) {
            exec_operation(fout, coada_execute_update, banda_merge, &undo_stiva, &redo_stiva);
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

    fclose(fin);
    fclose(fout);
    return 0;
}
