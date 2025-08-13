# 🎞️ Magic Tape

> **Masina Turing** implementata in C folosind **liste inlantuite**.

**Banda Magică** este un experiment de programare inspirat de *mașina Turing*:
o bandă „infinită” unde poți scrie, citi și naviga liber,
mutând cursorul la stânga, la dreapta sau direct la următorul simbol dorit.
În spate, funcționează o structură de tip **listă dublu înlănțuită cu santinelă**,
gândită pentru eficiență și ușurință în manipularea datelor.

Un exercițiu perfect de logică, algoritmi și gândire „out of the box”.


## Operatii de tip UPDATE

| Operatie | Descriere |
| :--- | :--- |
| `MOVE_LEFT`  | Muta degetul cu o pozitie la stanga |
| `MOVE_RIGHT` | Muta degetul cu o pozitie la dreapta |
| `MOVE_LEFT_CHAR \<C\>`  | Muta degetul pe prima aparitie a caracterului \<C\> din stanga pozitiei sale curente |
| `MOVE_RIGHT_CHAR \<C\>` | Muta degetul pe prima aparitie a caracterului \<C\> din dreapta pozitiei sale curente |
| `INSERT_LEFT \<C\>` | Insereaza caracterul \<C\> in stanga degetului si muta degetul pe noul nod |
| `INSERT_RIGHT \<C\>` | Insereaza caracterul \<C\> in dreapta degetului si muta degetul pe noul nod |
| `WRITE \<C\>` | Suprascrie caracterul curent cu \<C\> |



## Operatii de tip QUERY

| Operatie | Descriere |
| :--- | :--- |
| `SHOW_CURRENT` | Afiseaza caraceterul din pozitia degetului |
| `SHOW` | Afiseaza continutul intregii benzii (de la inceput pana la final) |

## Operatii de tip UNDO/REDO

| Operatie | Descriere |
| :--- | :--- |
| `UNDO` | Anuleaza ultima operatie de `MOVE_LEFT`/`MOVE_RIGHT` |
| `REDO` | Reface ultima operatie de `MOVE_LEFT`/`MOVE_RIGHT` |

## Operatia EXECUTE

| Operatie | Descriere |
| :--- | :--- |
| `EXECUTE` | Extrage o comanda din coada si o executa |


## Corner Case-uri

- `MOVE_LEFT`:
    nu se va intampla nimic daca degetul este pe prima pozitie
- `MOVE_RIGHT`:
    daca degetul este plasat pe ultimul nod din banda,
    se va insera caracterul `#` la finalul listei,
    iar degetul va fi pozitionat pe noul caracter adaugat

- `MOVE_LEFT_CHAR <C>`:
    daca nu exista niciun caracter \<C\> in stanga degetului,
    pozitia acestuia nu se va modifica
    si se va fisa mesajul `ERROR`
- `MOVE_RIGHT_CHAR <C>`:
    daca nu exista niciun caracter \<C\> in dreapta degetului,
    se va insera `#` la finaul benzii
    si pozitia degetului va fi pe `#`-ul inserat

- `INSERT_LEFT <C>`:
    daca degetul este deja la inceputul benzii,
    se va afisa mesajul `ERROR` si nu va modifica nimic



## Structuri de Date si Algoritmi

- Banda
- Stiva
- Coada


## ⛓️ Banda


> ⚠️ ATENTIE:
>
> **Santinela** este practic un nod plasat la inceputul listei;
> ea nu face parte din informatia propriu-zisa si nu poate fi modificata in vreun fel.


```c
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
```


```c
NodBanda *new_banda_node(char caracter);
Banda* new_banda();
void delete_banda(Banda *banda);
void banda_insert_to_left(FILE *fout, Banda *banda, char caracter);
void banda_insert_to_tail(Banda *banda, char caracter);
void banda_insert_to_right(Banda *banda, char caracter);
void print_banda(FILE *fout, Banda *banda);
```


## 🚶‍♂️🚶‍♂️🚶‍♂️ Coada

> Lista simplu inlantuita.

```c
typedef struct QueueNode {
    char informatie[LINE_LENGTH];
    struct QueueNode *urm;
} QueueNode;


typedef struct Queue {
    QueueNode *head;         // pointer la nodul de inceput al cozii
    QueueNode *tail;         // pointer la nodul de la sfarsitul cozii
} Queue;
```


```c
QueueNode *new_queue_node(char informatie[LINE_LENGTH]);
Queue* new_queue();
void queue_push(Queue *queue, char informatie[LINE_LENGTH]);
void queue_pop(Queue *queue);
void delete_queue(Queue *queue);
```

## 📚 Stiva

> Lista simplu inlantuita.


```c
typedef struct Stack {
    struct Stack *urm;
    NodBanda *informatie;
} StackNode, *Stack;
```


```c
void stack_push(Stack *stack, NodBanda *new_top);
void stack_pop(Stack *stack);
void delete_stack(Stack *stack);
```