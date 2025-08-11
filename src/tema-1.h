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

#define DIEZ '#'
#define SPACE ' '
#define EROARE "ERROR"
#define LUNGIME_LINIE 20
#define POZ_WRITE_C 6
#define POZ_MV_L_C 15
#define POZ_MV_R_C 16
#define POZ_INSERT_L_C 12
#define POZ_INSERT_R_C 13
#define UNU 1
#define ZERO 0
#define ZRC '0'

int k = ZERO;

typedef struct NodBanda {
    char caracter;
    struct NodBanda *urm;    // adresa catre urmatorul nod din Coada
    struct NodBanda *pred;   // adresa catre nodul anterior din Coada
} NodBanda;


typedef struct LBanda{
    NodBanda *head;       // va memora nodul de inceput al listei
    NodBanda *tail;         // va memora nodul final al listei
    NodBanda *santinela;    // santinela, fundatia listei
    NodBanda *deget;         // va memora nodul catre care degetul indica
} LBanda;


typedef struct Stiva {
    struct Stiva *urm;
    NodBanda *informatie;
} NodStiva, *SStiva;


typedef struct NodCoada {
    char informatie[LUNGIME_LINIE];
    struct NodCoada* urm;    // adresa catre urmatorul nod din Coada
    struct NodCoada* pred;   // adresa catre nodul anterior din Coada
} NodCoada;


typedef struct Coada {
    NodCoada* head;       // va memora nodul de head al listei
    NodCoada* tail;         // va memora nodul tail al listei
} Coada;



NodCoada *new_queue_node(char informatie[LUNGIME_LINIE])
{
    // cream un nou nod
    NodCoada* new_node = malloc(sizeof(NodCoada));
    new_node->urm = new_node->pred = NULL;
    strcpy(new_node->informatie, informatie);
   return new_node;
}


NodBanda *new_banda_node(char caracter)
{
    // cream un nou nod
    NodBanda* new_node = malloc(sizeof(NodBanda));
    new_node->urm = new_node->pred = NULL;
    new_node->caracter = caracter;
    return new_node;
}


LBanda* new_banda()
{
    // creem o banda cu santinela
    LBanda *banda_noua = malloc(sizeof(LBanda));
    banda_noua->head = banda_noua->tail = banda_noua->deget = new_banda_node(DIEZ);
    banda_noua->santinela = new_banda_node(SPACE);
    banda_noua->santinela->urm = banda_noua->head;
    banda_noua->head->pred = banda_noua->santinela;
    return banda_noua;
}


void delete_banda(LBanda *banda)
{
    while (banda->tail != banda->santinela) {
        NodBanda* tmp = banda->tail;
        banda->tail = banda->tail->pred;
        free(tmp);
    }
    free(banda->santinela);
    free(banda);
}


void banda_insert_to_tail(LBanda *banda, char caracter)
{
    if (!banda || !banda->head) {
        // Coada vida, elementul va deveni primul nod al listei
        banda->head = banda->tail = new_banda_node(caracter);
        banda->santinela->urm = banda->head;
        banda->head->pred = banda->santinela;
        return;
    }
    
    // Coada  cel putin un element
    NodBanda* nodul_final = new_banda_node(caracter);
    nodul_final->pred = banda->tail;
    banda->tail-> urm = nodul_final;
    banda->tail = nodul_final;
}


void banda_insert_to_left(FILE *fout, LBanda *banda, char caracter)
{
    // inseram in stanga santinelei
    if (banda->deget == banda->head) {
        // head de banda
        fprintf(fout, "%s\n", EROARE);
        return;
    } else {
        NodBanda* nod_inserare_st = new_banda_node(caracter);
        nod_inserare_st->pred = banda->deget->pred;
        nod_inserare_st->urm = banda->deget;
        banda->deget->pred->urm = nod_inserare_st;
        banda->deget->pred = nod_inserare_st;
        banda->deget = nod_inserare_st;
        return;
    }
}


void banda_insert_to_right(LBanda *banda, char caracter)
{
    // inseram in dreapta santinelei
    if (banda->deget == banda->tail) {
        // inseram la final de banda
        banda_insert_to_tail(banda , caracter);
        banda->deget = banda->tail; 
        return;
    } else {
        NodBanda* new_node = new_banda_node(caracter);
        new_node->pred = banda->deget;
        new_node->urm = banda->deget->urm;
        banda->deget->urm->pred = new_node;
        banda->deget->urm = new_node;
        banda->deget = new_node;
        return;
    }
}


void print_banda(FILE *fout, LBanda *banda)
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



Coada* new_queue()
{
	// cream o Coada noua
	Coada* coada_noua = malloc(sizeof(Coada));
	coada_noua->head = NULL;
	coada_noua->tail = NULL;
	return coada_noua;
}


void queue_push(Coada* queue, char informatie[LUNGIME_LINIE])
{
    if (!queue) {
        // Coada este vida
        queue = new_queue();
        queue->head =  queue->tail = new_queue_node(informatie);
        return;
    } 
    if (queue && !queue->head) {
        // Coada nu este initializata, ea exista, dar nu retine nimic
        queue->head = queue->tail = new_queue_node(informatie);
        return;
    }
    // Coada are cel putin un element
    NodCoada* new_nod = new_queue_node(informatie);
    new_nod->pred = queue->tail;
    queue->tail->urm = new_nod;
    queue->tail = new_nod;
}


void queue_pop(Coada *queue)
{
    // sterge orimul element al listei , dar nu si Coada
    NodCoada* stergator = queue->head;
    queue->head = queue->head->urm;
    if (queue->head) {
        queue->head->pred = NULL;
    }
    free(stergator);
}


void CoadaDeleteLastnod(Coada *queue)
{
    if (!queue) return;
    if (queue->head == queue->tail) {
        // Coada are un singur element
        free(queue->head);
        free(queue->tail);
        return;
    } else {
        // Coada are mai multe elemente
        queue->tail = queue->tail->pred;
        free(queue->tail->urm);
        return;
    }
}


void delete_queue(Coada *queue)
{
	// Guard against young player errors
	if (!queue) {
        free(queue);
        return;
    }
	while (queue->head) {
		// stergem succeiv primul element al listei
		// o privim practic asemenea unei cozi
        // stergere de la inceput
		queue_pop(queue);
	}
	free(queue);
}



void stack_push(SStiva *stiva, NodBanda *varf_nou)
{
    SStiva stiva_noua = (SStiva) malloc(sizeof(NodStiva));
    stiva_noua->informatie = varf_nou;
    stiva_noua->urm = *stiva;
    *stiva = stiva_noua;
}


void stack_pop(SStiva *stiva)
{
    if (stiva != NULL) {
        SStiva tmp = *stiva;
        *stiva = (*stiva)->urm;
        free(tmp);
    }
}


void delete_stack(SStiva *stiva)
{
    while (*stiva != NULL) {
        stack_pop(stiva);
    }
    free(stiva);
}

#endif
