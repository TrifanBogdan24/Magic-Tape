/*TRIFAN BOGDAN-CRISTIAN , 312CD*/
#ifndef TEMA_1_H
#define TEMA_1_H
#include <stdio.h>                                          // needed for printf
#include <stdlib.h>                           // needed for alloc, realloc, free
#define UPDATE_MV_L "MOVE_LEFT"
#define UPDATE_MV_R "MOVE_RIGHT"
#define UPDATE_MV_L_C "MOVE_LEFT_CHAR"
#define UPDATE_MV_R_C "MOVE_RIGHT_CHAR"
#define UPDATE_WRITE_C "WRITE"
#define UPDATE_INSERT_L_C "INSERT_LEFT"
#define UPDATE_INSERT_R_C "INSERT_RIGHT"
#define SHOW_CURRENT "SHOW_CURRENT"
#define SHOW_ALL "SHOW"
#define EXECUTE "EXECUTE"
#define UNDO "UNDO"
#define REDO "REDO" 
#define DIEZ '#'
#define SPACE ' '
#define EROARE "ERROR"
#define LUNGIME_LINII 20
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
    struct NodBanda* urm;    // adresa catre urmatorul nod din Coada
    struct NodBanda* pred;   // adresa catre nodul anterior din Coada
} NodBanda;


typedef struct LBanda{
    NodBanda* inceput;       // va memora nodul de inceput al listei
    NodBanda* final;         // va memora nodul final al listei
    NodBanda* santinela;    // santinela, fundatia listei
    NodBanda* deget;         // va memora nodul catre care degetul indica
} LBanda;


typedef struct Stiva {
    struct Stiva* urm;
    NodBanda* informatie;
} NodStiva, *SStiva;


typedef struct NodCoada {
    char informatie[LUNGIME_LINII];
    struct NodCoada* urm;    // adresa catre urmatorul nod din Coada
    struct NodCoada* pred;   // adresa catre nodul anterior din Coada
} NodCoada;


typedef struct Coada {
    NodCoada* inceput;       // va memora nodul de inceput al listei
    NodCoada* final;         // va memora nodul final al listei

} Coada;



NodCoada *CreareNodCoada(char informatie[LUNGIME_LINII]) {
    // cream un nou nod
    NodCoada* nod_nou = malloc( sizeof(NodCoada));
    nod_nou -> urm = nod_nou -> pred = NULL;
    strcpy(nod_nou -> informatie, informatie);
   return nod_nou;
}


NodBanda *CreareNodBanda(char caracter) {
    // cream un nou nod
    NodBanda* nod_nou = malloc(sizeof(NodBanda));
   
    nod_nou -> urm = nod_nou -> pred = NULL;
    nod_nou -> caracter = caracter;
   return nod_nou;
}


LBanda* CreareBanda(void) {
    // creem o banda cu santinela
    LBanda* banda_noua = malloc(sizeof(LBanda));
    banda_noua->inceput = banda_noua->final = banda_noua->deget = CreareNodBanda(DIEZ);
    banda_noua->santinela = CreareNodBanda(SPACE);
    banda_noua->santinela->urm = banda_noua->inceput;
    banda_noua->inceput->pred = banda_noua->santinela;
    return banda_noua;
}


void BandaStergere(LBanda* banda_input) {
    while (banda_input->final != banda_input->santinela) {
        NodBanda* stergator = banda_input->final;
        banda_input->final = banda_input->final->pred;
        free(stergator);
    }
    free(banda_input->santinela);
    free(banda_input);
}


void BandaInserareFinal(LBanda* banda_input ,char caracter) {
    
    if (banda_input == NULL || banda_input->inceput == NULL) {
        // Coada vida , elementul va deveni primul nod al listei
        banda_input -> inceput = banda_input -> final = CreareNodBanda(caracter);
        banda_input->santinela->urm = banda_input->inceput;
        banda_input->inceput->pred = banda_input->santinela;
        return;
    }
    
    // Coada  cel putin un element
    NodBanda* nodul_final = CreareNodBanda(caracter);
    nodul_final -> pred = banda_input -> final;
    banda_input -> final -> urm = nodul_final;
    banda_input -> final = nodul_final;

    return ;
   
}


void BandaInserareStanga(FILE* fout , LBanda *banda_input , char caracter) {
    // inseram in stanga santinelei
    if (banda_input->deget == banda_input->inceput) {
        // inceput de banda

        fprintf(fout, "%s\n" , EROARE);
        return;
    } else {
        NodBanda* nod_inserare_st = CreareNodBanda(caracter);
        nod_inserare_st->pred = banda_input->deget->pred;
        nod_inserare_st->urm = banda_input->deget;
        banda_input->deget->pred->urm = nod_inserare_st;
        banda_input->deget->pred = nod_inserare_st;
        banda_input->deget = nod_inserare_st;
        return;
    }
}


void BandaInserareDreapta(LBanda *banda_input , char caracter) {
    // inseram in dreapta santinelei
    if (banda_input->deget == banda_input->final) {
        // inseram la final
        BandaInserareFinal(banda_input , caracter);
        banda_input->deget = banda_input->final; 
        // printf("%s\n" , banda_input->deget->informatie);
        return;
    } else {
        NodBanda* nod_inserare_dr = CreareNodBanda(caracter);
        nod_inserare_dr->pred = banda_input->deget;
        nod_inserare_dr->urm = banda_input->deget->urm;
        banda_input->deget->urm->pred = nod_inserare_dr;
        banda_input->deget->urm = nod_inserare_dr;
        banda_input->deget = nod_inserare_dr;
        return;
    }
}


void display(FILE* fout , LBanda *banda_input) {
    NodBanda* parcurgere = banda_input -> inceput;
    while (parcurgere != NULL) {
        if (parcurgere == banda_input -> deget) {
            fprintf(fout , "|%c|" , parcurgere -> caracter);
        } else {
            fprintf(fout , "%c" , parcurgere -> caracter);
        }
        parcurgere = parcurgere -> urm;
    }
    fprintf(fout , "\n");
    return ;
}



Coada* CreareCoada(void) {

	// cream o Coada noua
	Coada* coada_noua = malloc(sizeof(Coada));
	coada_noua -> inceput = NULL;
	coada_noua -> final = NULL;
	return coada_noua;
}

void CoadaInserareInceput(Coada* coada_input, char informatie[LUNGIME_LINII]) {

    // Coada are cel putin un element
    NodCoada* new_nod = CreareNodCoada(informatie);
    new_nod->urm = coada_input->inceput;
    if (coada_input->inceput != NULL) {
        coada_input->inceput->pred = new_nod;
    }
    coada_input->inceput = new_nod;
    return ;
}

void CoadaInserareFinal(Coada* coada_input, char informatie[LUNGIME_LINII]) {
    if (coada_input == NULL) {
        // Coada este vida
        coada_input = CreareCoada();
        coada_input->inceput =  coada_input->final = CreareNodCoada(informatie);
        return;
    } 
    if (coada_input != NULL && coada_input->inceput == NULL) {
        // Coada nu este initializata, ea exista, dar nu retine nimic
        coada_input->inceput = coada_input->final = CreareNodCoada(informatie);
        return;
    }
    // Coada are cel putin un element
    NodCoada* new_nod = CreareNodCoada(informatie);
    new_nod->pred = coada_input->final;
    coada_input->final->urm = new_nod;
    coada_input->final = new_nod;
    return ;
}


void CoadaDeleteFirstNode(Coada* coada_input) {
    // sterge orimul element al listei , dar nu si Coada
    NodCoada* stergator = coada_input -> inceput;
    coada_input -> inceput = coada_input -> inceput -> urm;
    if (coada_input -> inceput != NULL) {
        coada_input -> inceput -> pred = NULL;
    }
    free(stergator);
    return ;

}


void CoadaDeleteLastnod(Coada* coada_input) {
    if (coada_input == NULL) return;
    if (coada_input->inceput == coada_input->final) {
        // Coada are un singur element
        // coada_input -> inceput = coada_input -> final = NULL;
        free(coada_input->inceput);
        free(coada_input->final);
        return;
    } else {
        // Coada are mai multe elemente
        coada_input->final = coada_input->final->pred;
        free(coada_input->final->urm);
        return ;
    }
}


void CoadaFullDelete(Coada* coada_input){
	// Guard against young player errors
	if (coada_input == NULL) {
        free(coada_input);
        return ;
    }
	while (coada_input -> inceput != NULL) {
		// stergem succeiv primul element al listei
		// o privim practic asemenea unei cozis
        // stergere de la inceput
		CoadaDeleteFirstNode(coada_input);
	}
	free(coada_input);
	return ;
}



void StivaPush(SStiva *stiva_input, NodBanda* varf_nou) {
    SStiva stiva_noua = (SStiva) malloc(sizeof(NodStiva));
    stiva_noua -> informatie = varf_nou;
    stiva_noua->urm = *stiva_input;
    *stiva_input = stiva_noua;
    return ; 
}
    

void StivaPop(SStiva* stiva_input) {
    if (stiva_input != NULL) {
        SStiva stergator = *stiva_input;
        *stiva_input = (*stiva_input)->urm;
        free(stergator);
    }

}


void DeleteStiva(SStiva* stiva_input) {
    while (*stiva_input != NULL) {
        StivaPop(stiva_input);
    }
    free(stiva_input);
    return;
}

#endif
