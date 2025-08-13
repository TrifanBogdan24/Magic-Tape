# 🎞️ Magic Tape

> **Masina Turing** implementata in C folosind **liste inlantuite**.


## Structuri de Date si Algoritmi

- Banda
- Stiva
- Coada


### ⛓️ Banda
---

> Lista dublu inlantuita.

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


### 🚶‍♂️🚶‍♂️🚶‍♂️ Coada
---

> Lista simpla inlantuita.

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

### 📚 Stiva
---

> Lista simpla inlantuita.


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