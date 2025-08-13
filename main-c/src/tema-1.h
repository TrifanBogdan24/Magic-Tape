/*TRIFAN BOGDAN-CRISTIAN , 312CD*/
#ifndef TEMA_1_H
#define TEMA_1_H
#include <stdio.h>                            // needed for printf
#include <stdlib.h>                           // needed for alloc, realloc, free

// UPDATE operations:
#define MOVE_LEFT "MOVE_LEFT"
#define MOVE_RIGHT "MOVE_RIGHT"
#define MOVE_LEFT_CHAR "MOVE_LEFT_CHAR"
#define MOVE_RIGHT_CHAR "MOVE_RIGHT_CHAR"
#define WRITE_CHAR "WRITE"
#define INSERT_CHAR_TO_LEFT "INSERT_LEFT"
#define INSERT_CHAR_TO_RIGHT "INSERT_RIGHT"

// QUERY operations:
#define SHOW_CURRENT "SHOW_CURRENT"
#define SHOW_ALL "SHOW"
#define EXECUTE "EXECUTE"
#define UNDO "UNDO"
#define REDO "REDO"

#define DIEZ_CHAR '#'
#define SPACE_CHAR ' '
#define ERROR "ERROR"
#define LINE_LENGTH 20


typedef struct NodBanda {
    char caracter;
    struct NodBanda *urm;    // pointer la urmatorul nod din lista (next)
    struct NodBanda *pred;   // pointer la nodul anterior din lista (previous)
} NodBanda;

typedef struct Banda {
    NodBanda *head;         // pointer la primul nod al listei
    NodBanda *tail;         // pointer la ultimul nod al listei
    NodBanda *santinela;    // pointer la nodul santinela (marcheaza baza listei)
    NodBanda *deget;        // pointer la nodul curent (pozitia "degetului")
} Banda;


typedef struct Stack {
    struct Stack *urm;
    NodBanda *pos_deget;
} StackNode, *Stack;


typedef struct QueueNode {
    char operation[LINE_LENGTH];
    struct QueueNode *urm;
} QueueNode;


typedef struct Queue {
    QueueNode *head;         // pointer la nodul de inceput al cozii
    QueueNode *tail;         // pointer la nodul de la sfarsitul cozii
} Queue;





NodBanda *new_banda_node(char caracter)
{
    // cream un nou nod
    NodBanda* new_node = malloc(sizeof(NodBanda));
    new_node->urm = new_node->pred = NULL;
    new_node->caracter = caracter;
    return new_node;
}


Banda* new_banda()
{
    // creem o banda cu santinela
    Banda *banda_noua = malloc(sizeof(Banda));
    banda_noua->head = banda_noua->tail = banda_noua->deget = new_banda_node(DIEZ_CHAR);
    banda_noua->santinela = new_banda_node(SPACE_CHAR);
    banda_noua->santinela->urm = banda_noua->head;
    banda_noua->head->pred = banda_noua->santinela;
    return banda_noua;
}


void delete_banda(Banda *banda)
{
    while (banda->tail != banda->santinela) {
        NodBanda* tmp = banda->tail;
        banda->tail = banda->tail->pred;
        free(tmp);
    }
    free(banda->santinela);
    free(banda);
}


void banda_insert_to_tail(Banda *banda, char caracter)
{
    if (!banda || !banda->head) {
        // Banda este vida, elementul va deveni primul nod al listei
        banda->head = banda->tail = new_banda_node(caracter);
        banda->santinela->urm = banda->head;
        banda->head->pred = banda->santinela;
        return;
    }
    
    // Banda are cel putin un element
    NodBanda* nodul_final = new_banda_node(caracter);
    nodul_final->pred = banda->tail;
    banda->tail-> urm = nodul_final;
    banda->tail = nodul_final;
}


void banda_insert_to_left(FILE *fout, Banda *banda, char caracter)
{
    // inseram in stanga santinelei
    if (banda->deget == banda->head) {
        // banda nu contine nicio informatie
        fprintf(fout, "%s\n", ERROR);
        return;
    }

    // Banda nevida
    NodBanda* new_node = new_banda_node(caracter);
    new_node->pred = banda->deget->pred;
    new_node->urm = banda->deget;
    banda->deget->pred->urm = new_node;
    banda->deget->pred = new_node;
    banda->deget = new_node;
}


void banda_insert_to_right(Banda *banda, char caracter)
{
    // inseram in dreapta santinelei
    if (banda->deget == banda->tail) {
        // banda nu contine nicio informatie
        // inseram la final de banda
        banda_insert_to_tail(banda , caracter);
        banda->deget = banda->tail; 
        return;
    }

    // Banda nevida
    NodBanda* new_node = new_banda_node(caracter);
    new_node->pred = banda->deget;
    new_node->urm = banda->deget->urm;
    banda->deget->urm->pred = new_node;
    banda->deget->urm = new_node;
    banda->deget = new_node;
}


void print_banda(FILE *fout, Banda *banda)
{
    for (NodBanda* iter = banda->head; iter; iter = iter->urm) {
        if (iter == banda->deget) {
            fprintf(fout , "|%c|" , iter->caracter);
        } else {
            fprintf(fout , "%c" , iter->caracter);
        }
    }
    fprintf(fout, "\n");
}


QueueNode *new_queue_node(char operation[LINE_LENGTH])
{
    // cream un nou nod
    QueueNode* new_node = malloc(sizeof(QueueNode));
    new_node->urm = NULL;
    strcpy(new_node->operation, operation);
    return new_node;
}


Queue* new_queue()
{
	// cream o coada noua
	Queue* queue = malloc(sizeof(Queue));
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}


void queue_push(Queue *queue, char operation[LINE_LENGTH])
{
    if (!queue) {
        // coada este vida
        queue = new_queue();
        queue->head = queue->tail = new_queue_node(operation);
        return;
    } 
    if (queue && !queue->head) {
        // coada nu este initializata, ea exista, dar nu retine nimic
        queue->head = queue->tail = new_queue_node(operation);
        return;
    }
    // coada are cel putin un element
    QueueNode* new_nod = new_queue_node(operation);
    queue->tail->urm = new_nod;
    queue->tail = new_nod;
}


void queue_pop(Queue *queue)
{
    if (!queue || !queue->head) return;

    // sterge primul element al coada, dar nu si coada in sine
    QueueNode* tmp = queue->head;
    queue->head = queue->head->urm;
    free(tmp);
}



void delete_queue(Queue *queue)
{
	// Guard against young player errors
	if (!queue) {
        free(queue);
        return;
    }

    // stergem succeiv primul element al listei
    while (queue->head) {
        // stergere de la inceput
		queue_pop(queue);
	}
	free(queue);
}

void stack_push(Stack *stack, NodBanda *new_pos_deget)
{
    Stack new_stack = (Stack) malloc(sizeof(StackNode));
    new_stack->pos_deget = new_pos_deget;
    new_stack->urm = *stack;
    *stack = new_stack;
}


void stack_pop(Stack *stack)
{
    if (!stack) return;

    // Stiva nu este vida:
    Stack tmp = *stack;
    *stack = (*stack)->urm;
    free(tmp);
}


void delete_stack(Stack *stack)
{
    while (*stack != NULL) {
        stack_pop(stack);
    }
    free(stack);
}

#endif
