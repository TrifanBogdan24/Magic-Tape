/*TRIFAN BOGDAN-CRISTIAN , 312CD*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema-1.h"


void exec_operation(FILE* fout, Coada *coada, LBanda *banda, SStiva *undo, SStiva *redo)
{
    if (coada->head == NULL) return;

    char cmd_to_exec[LUNGIME_LINIE];
    strcpy(cmd_to_exec, coada->head->informatie);
    queue_pop(coada);

    if (strstr(cmd_to_exec, WRITE_CHAR)) {
        banda->deget->caracter = cmd_to_exec[POZ_WRITE_C];
        while (*undo) stack_pop(undo);
        while (*redo) stack_pop(redo);

    } else if (strstr(cmd_to_exec, MOVE_LEFT_CHAR)) {
        NodBanda* aux = banda->deget;
        while (aux) {
            if (cmd_to_exec[POZ_MV_L_C] == aux->caracter) {
                banda->deget = aux;
                return;
            }
            aux = aux->pred;
        }
        fprintf(fout, "%s\n", EROARE);

    } else if (strstr(cmd_to_exec, MOVE_RIGHT_CHAR)) {
        while (banda->deget && banda->deget->caracter != cmd_to_exec[POZ_MV_R_C]) {
            banda->deget = banda->deget->urm;
        }
        if (!banda->deget) {
            banda_insert_to_tail(banda, DIEZ);
            banda->deget = banda->tail;
        }

    } else if (strstr(cmd_to_exec, INSERT_CHAR_TO_LEFT)) {
        banda_insert_to_left(fout, banda, cmd_to_exec[POZ_INSERT_L_C]);

    } else if (strstr(cmd_to_exec, INSERT_CHAR_TO_RIGHT)) {
        banda_insert_to_right(banda, cmd_to_exec[POZ_INSERT_R_C]);

    } else if (strstr(cmd_to_exec, MOVE_LEFT)) {
        if (banda->deget != banda->head) {
            stack_push(undo, banda->deget);
            banda->deget = banda->deget->pred;
        }

    } else if (strstr(cmd_to_exec, MOVE_RIGHT)) {
        stack_push(undo, banda->deget);
        if (banda->deget == banda->tail) {
            banda_insert_to_tail(banda, DIEZ);
            banda->deget = banda->tail;
        } else {
            banda->deget = banda->deget->urm;
        }
    }
}


int main()
{
    int nr = ZERO;
    Coada* coada_execute_update = new_queue();    // retine numele functii
    SStiva undo_stiva = (SStiva) calloc(1, sizeof(NodStiva));               // retine numele functiei
    SStiva redo_stiva = (SStiva) calloc(1, sizeof(NodStiva));              // retine numele functiei
    LBanda* banda_merge = new_banda();   // retine banda 

    FILE *fin = fopen("tema1.in" , "r");
    FILE *fout= fopen("tema1.out" , "w");
    fscanf(fin, "%d", &nr);

    char  linie[LUNGIME_LINIE];
    int i = ZERO;
    for (i = ZERO ; i <= nr ; i++) {
        fgets(linie, LUNGIME_LINIE , fin);

        if (strstr(linie , MOVE_RIGHT_CHAR) != NULL) {
            queue_push(coada_execute_update , linie);
        } else if (strstr(linie ,  MOVE_LEFT_CHAR) != NULL) {
            queue_push(coada_execute_update , linie);
        } else if (strstr(linie , MOVE_RIGHT) != NULL) {
            queue_push(coada_execute_update , linie);
        } else if (strstr(linie , MOVE_LEFT) != NULL) {
            queue_push(coada_execute_update , linie);
        } else if (strstr(linie , INSERT_CHAR_TO_LEFT) != NULL) {
            queue_push(coada_execute_update , linie);
        } else if (strstr(linie , INSERT_CHAR_TO_RIGHT) != NULL) {
            queue_push(coada_execute_update , linie);
        } else if (strstr(linie , SHOW_CURRENT) != NULL) {
            fprintf(fout , "%c\n" , banda_merge->deget->caracter);
        } else if (strstr(linie , SHOW_ALL) != NULL) {
            print_banda(fout , banda_merge);
        } else if (strstr(linie , UNDO) != NULL ) {
            if( undo_stiva != NULL && undo_stiva->informatie != NULL) {
                stack_push(&redo_stiva , banda_merge->deget); // punem in varful stivei REDO varful lui UNDO
                banda_merge->deget = undo_stiva->informatie;    // extragem pointerul din varful stivei UNDO
                stack_pop(&undo_stiva);  // stergem varful stivei UNDO
            }
        } else if (strstr(linie , REDO) != NULL ) {
            if (redo_stiva != NULL && redo_stiva->informatie != NULL) {
                stack_push(&undo_stiva , banda_merge->deget);     // punem in varful stivei UNDO varful lui REDO
                banda_merge->deget = redo_stiva->informatie;  // extragem pointerul din varful stivei REDO
                stack_pop(&redo_stiva);  // stergem varful stivei REDO
            }
        } else if( strstr(linie , WRITE_CHAR) != NULL) {
            queue_push( coada_execute_update , linie);
        } else if (strstr(linie , EXECUTE) != NULL) {
            exec_operation(fout, coada_execute_update, banda_merge, &undo_stiva, &redo_stiva);
        }
    }

    delete_banda(banda_merge);
    delete_queue(coada_execute_update);

    while (undo_stiva != NULL) {
        stack_pop(&undo_stiva);  
    }
    while (redo_stiva!= NULL) {
        stack_pop(&redo_stiva);
    }

    free(undo_stiva);
    free(redo_stiva);

    fclose(fin);
    fclose(fout);
    return 0;
}
