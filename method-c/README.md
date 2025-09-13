# 🎞️ Magic Tape

> **Mașina Turing** implementată în C folosind **liste înlănțuite**.


## Structuri de Date și Algoritmi

- Listă simplu/dublu înlănțuită
- Stivă
- Coadă


### ⛓️ Banda
---

> ⚠️ ATENȚIE:
> **Santinela** este un nod plasat la începutul listei.


```c
typedef struct NodBanda {
    char caracter;
    struct NodBanda *urm;    // pointer la următorul nod din listă (next)
    struct NodBanda *pred;   // pointer la nodul anterior din listă (previous)
} NodBanda;

typedef struct Banda {
    NodBanda *head;          // pointer la primul nod al listei
    NodBanda *tail;          // pointer la ultimul nod al listei
    NodBanda *santinela;     // pointer la nodul santinelă (marcheaza baza listei)
    NodBanda *deget;         // pointer la nodul curent (poziția "degetului")
} Banda;
```


```c
NodBanda *new_banda_node(char caracter);
Banda *new_banda();
void delete_banda(Banda *banda);
void banda_insert_to_left(FILE *fout, Banda *banda, char caracter);
void banda_insert_to_tail(Banda *banda, char caracter);
void banda_insert_to_right(Banda *banda, char caracter);
void print_banda(FILE *fout, Banda *banda);
```


### 🚶‍♂️🚶‍♂️🚶‍♂️ Coada
---

Am implementat **coada** sub forma unei **liste simplu înlănțuite**.



```c
typedef struct QueueNode {
    char operation[LINE_LENGTH];
    struct QueueNode *urm;
} QueueNode;


typedef struct Queue {
    QueueNode *head;         // pointer la nodul de la începutul cozii
    QueueNode *tail;         // pointer la nodul de la sfârșitul cozii
} Queue;
```


```c
QueueNode *new_queue_node(char informatie[LINE_LENGTH]);
Queue *new_queue();
void queue_push(Queue *queue, char informatie[LINE_LENGTH]);
void queue_pop(Queue *queue);
void delete_queue(Queue *queue);
```

### 📚 Stiva
---

Cele două stive utilizate pentru `UNDO`/`REDO`
sunt practic două **liste simplu înlănțuite**.



```c
typedef struct Stack {
    struct Stack *urm;
    NodBanda *pos_deget;
} StackNode, *Stack;
```


```c
void stack_push(Stack *stack, NodBanda *new_pos_deget);
NodBanda *stack_top(Stack *stack);
void stack_pop(Stack *stack);
void delete_stack(Stack *stack);
```


Cel mai simplu mod de a înțelege operațiile `UNDO` și `REDO`  
este să le privim ca pe **săgețile de navigare din istoricul unui browser**.


Fiind complementare, logica din codul de mai jos funcționează  
ca un *joc de ping-pong* între cele două stive:  
când adăugăm poziția curentă a degetului în vârful uneia,
extragem ultimul nod din cealaltă.



```c
if (strstr(line, UNDO) != NULL) {
    stack_push(&stack_redo, banda->deget);
    banda->deget = stack_top(stack_undo);
    stack_pop(&stack_undo);
} else if (strstr(line, REDO) != NULL) {
    stack_push(&stack_undo, banda->deget);
    banda->deget = stack_top(stack_redo);
    stack_pop(&stack_redo);
}
```


## 📝 Input/Output
- Fișier de intrare: `tema1.in`
- Fișier de ieșire: `tema1.in`

## 📥 Instalarea dependințelor

```sh
sudo apt install -y build-essential valgrind
```

## ⚙️ Rularea programului

```sh
cd src/
make build   # Compilare
make run     # Execuție
make clean   # Curățare
```

