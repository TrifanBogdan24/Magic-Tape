/*TRIFAN BOGDAN-CRISTIAN , 312CD*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema-1.h"


void exec_operation(FILE* fout, Queue *coada, Banda *banda, Stack *undo, Stack *redo)
{
    if (coada->head == NULL) return;

    char cmd_to_exec[LINE_LENGTH];
    strcpy(cmd_to_exec, coada->head->operation);
    queue_pop(coada);

    if (strstr(cmd_to_exec, WRITE_CHAR)) {
        int idx = strlen(WRITE_CHAR) + 1;

        banda->deget->caracter = cmd_to_exec[idx];
        while (*undo) stack_pop(undo);
        while (*redo) stack_pop(redo);
    } else if (strstr(cmd_to_exec, MOVE_LEFT_CHAR)) {
        int idx = strlen(MOVE_LEFT_CHAR) + 1;

        NodBanda* aux = banda->deget;
        while (aux) {
            if (cmd_to_exec[idx] == aux->caracter) {
                banda->deget = aux;
                return;
            }
            aux = aux->pred;
        }
        fprintf(fout, "%s\n", ERROR);
    } else if (strstr(cmd_to_exec, MOVE_RIGHT_CHAR)) {
        int idx = strlen(MOVE_RIGHT_CHAR) + 1;

        while (banda->deget && banda->deget->caracter != cmd_to_exec[idx]) {
            banda->deget = banda->deget->urm;
        }
        if (!banda->deget) {
            banda_insert_to_tail(banda, DIEZ_CHAR);
            banda->deget = banda->tail;
        }
    } else if (strstr(cmd_to_exec, INSERT_CHAR_TO_LEFT)) {
        int idx = strlen(INSERT_CHAR_TO_LEFT) + 1;
        banda_insert_to_left(fout, banda, cmd_to_exec[idx]);
    } else if (strstr(cmd_to_exec, INSERT_CHAR_TO_RIGHT)) {
        int idx = strlen(INSERT_CHAR_TO_RIGHT) + 1;
        banda_insert_to_right(banda, cmd_to_exec[idx]);
    } else if (strstr(cmd_to_exec, MOVE_LEFT)) {
        if (banda->deget != banda->head) {
            stack_push(undo, banda->deget);
            banda->deget = banda->deget->pred;
        }
    } else if (strstr(cmd_to_exec, MOVE_RIGHT)) {
        stack_push(undo, banda->deget);
        if (banda->deget == banda->tail) {
            banda_insert_to_tail(banda, DIEZ_CHAR);
            banda->deget = banda->tail;
        } else {
            banda->deget = banda->deget->urm;
        }
    }
}


int main(void)
{
    Banda* banda = new_banda();
    Queue* queue_execute_update = new_queue();
    Stack stack_undo = (Stack) calloc(1, sizeof(StackNode));
    Stack stack_redo = (Stack) calloc(1, sizeof(StackNode));

    FILE *fin = fopen("tema1.in" , "r");
    FILE *fout= fopen("tema1.out" , "w");

    int num_operations = 0;
    fscanf(fin, "%d", &num_operations);
    char line[LINE_LENGTH];

    for (int i = 0; i <= num_operations; i++) {
        fgets(line, LINE_LENGTH , fin);

        if (strstr(line , MOVE_RIGHT_CHAR) != NULL) {
            queue_push(queue_execute_update , line);
        } else if (strstr(line ,  MOVE_LEFT_CHAR) != NULL) {
            queue_push(queue_execute_update , line);
        } else if (strstr(line , MOVE_RIGHT) != NULL) {
            queue_push(queue_execute_update , line);
        } else if (strstr(line , MOVE_LEFT) != NULL) {
            queue_push(queue_execute_update , line);
        } else if (strstr(line , INSERT_CHAR_TO_LEFT) != NULL) {
            queue_push(queue_execute_update , line);
        } else if (strstr(line , INSERT_CHAR_TO_RIGHT) != NULL) {
            queue_push(queue_execute_update , line);
        } else if (strstr(line , SHOW_CURRENT) != NULL) {
            fprintf(fout , "%c\n" , banda->deget->caracter);
        } else if (strstr(line , SHOW_ALL) != NULL) {
            print_banda(fout , banda);
        } else if (strstr(line , UNDO) != NULL) {
            stack_push(&stack_redo , banda->deget);   // punem in varful stivei REDO nodul curent al degetului
            banda->deget = stack_undo->pos_deget;     // extragem pointerul din varful stivei UNDO
            stack_pop(&stack_undo);                   // stergem varful stivei UNDO
        } else if (strstr(line , REDO) != NULL ) {
            stack_push(&stack_undo , banda->deget);   // punem in varful stivei UNDO nodul curent al degetului
            banda->deget = stack_redo->pos_deget;     // extragem pointerul din varful stivei REDO
            stack_pop(&stack_redo);                   // stergem varful stivei REDO
        } else if (strstr(line , WRITE_CHAR) != NULL) {
            queue_push( queue_execute_update , line);
        } else if (strstr(line , EXECUTE) != NULL) {
            exec_operation(fout, queue_execute_update, banda, &stack_undo, &stack_redo);
        }
    }

    delete_banda(banda);
    delete_queue(queue_execute_update);

    while (stack_undo != NULL) {
        stack_pop(&stack_undo);  
    }
    while (stack_redo!= NULL) {
        stack_pop(&stack_redo);
    }

    free(stack_undo);
    free(stack_redo);

    fclose(fin);
    fclose(fout);
    return 0;
}
