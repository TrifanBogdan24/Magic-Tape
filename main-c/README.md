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


## ⚠️ Corner Case-uri

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

- Lista simplu/dublu inlantuita
- Stiva
- Coada


### ⛓️ Banda
---

> ⚠️ ATENTIE:
>
> **Santinela** este practic un nod plasat la inceputul listei;
> ea nu face parte din informatia propriu-zisa si nu poate fi modificata in vreun fel.



La inceputul programului, banda contine doar santinela,
urmata de caracterul `#` aflat pe prima celula, unde se afla si degetul.

Caracterul `#` este folosit pentru a marca un nod alocat din banda,
asupra caruia nu s-a realizat nicio operatie de scriere.


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

Am implementat **coada** sub forma unei **liste simplu inlantuite**,
ce retine ca informatie efectiva numele operatiei de executat.

Pentru simplitate, am ales sa construiesc structura de date pentru **coada** folosind 2 pointeri:
unul catre inceputul listei si al doilea catre sfarsitul ei.

> 🎯 **Avantaj**: `push()`/`pop()` se realizeaza in `θ(1)`.
> 
> Acestea nu mai necesita iterarea **cozii**, chiar daca au loc la capete diferite ale ei.

Pe parcurs ce operatiile de tip **UPDATE** sunt citite din fisier,
acestea sunt adaugate la finalul unei cozii
pentru a fi executate in viitor, la intalnirea instructiunii `EXECUTE`.


```c
typedef struct QueueNode {
    char operation[LINE_LENGTH];
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

Cele doua stive utilizate pentru `UNDO`/`REDO`
sunt practic doua **liste simplu inlantuite**,
avand ca informatie propriu-zisa pointeri catre
noduri de pe banda unde s-a aflat degetul.

> 🎯 Operatiile de `push()`/`pop()` asupra acestora
> au loc doar la capatul de inceput de listei, in `θ(1)`.




```c
typedef struct Stack {
    struct Stack *urm;
    NodBanda *pos_deget;
} StackNode, *Stack;
```


```c
void stack_push(Stack *stack, NodBanda *new_pos_deget);
void stack_pop(Stack *stack);
void delete_stack(Stack *stack);
```


Cel mai bine este sa privim operatiile de `UNDO`/`REDO`
sub forma sagetilor din navigarea in istoricului unui browser.

Acestea fiind completementare, ce se intampla in codul de mai jos
devine practic un *joc de "ping-pong"* intre cele doua stive:
cand adaugam degetul curent in varful uneia, extragem pointerul din cealalta.



```c
if (strstr(line , UNDO) != NULL ) {
    stack_push(&stack_redo , banda->deget);
    banda->deget = stack_undo->pos_deget;
    stack_pop(&stack_undo);
} else if (strstr(line , REDO) != NULL ) {
    stack_push(&stack_undo , banda->deget);
    banda->deget = stack_redo->pos_deget;
    stack_pop(&stack_redo);
}
```


## 📝 Input/Output

Instructiunile de executat asupra *"masinii Turing"* sunt citite si interpretate,
linie cu linie, din fisierul `tema1.in`.

Programul va genera mai apoi un fisier denumit `tema1.out` care va contine
mesajele de eroare si rezultatele comenzilor de tip **QUERY**.

## 📥 Instalare dependinte

```sh
sudo apt install -y build-essential valgrind
```

## ⚙️ Rulare program

```sh
cd src/
make build   # Compilare
make run     # Executie
make clean   # Curatare
```


## ✅🔁 Testare Automata

- Testare implementarii:
```sh
cd tests/
chmod +x checker.sh
./checker.sh
```

- Testare coding style:
```sh
# Din radacina repository-ului:
chmod +x tests/coding-style/cs.sh
./tests/coding-style/cs.sh src/*
```

### 🧪 GitHub Actions | CI Pipeline
---

**Integrarea Continuă** este o parte esențială a procesului de testare.

În acest proiect, **GitHub Actions** rulează automat testele
ori de câte ori se face *push* în repository sau se deschide un *pull request*.

Workflow-ul este definit in urmatorul fisier:
[.github/workflows/CI-tests.yml](./.github/workflows/CI-tests.yml)


### 🌃 Overnight Testing
---

Testele nu numai ca sunt rulate la fiecare **commit**/**pull request**,
dar mai mult decat atat, am extins workflow-ul de CI
sa include o rulare **programata in fiecare noapte** (`cron job`)
care execute integral testele in **GitHub Actions**.

> In **DevOps**, acest tip de executie programata poarta numele de *"overnight testing"*.


```yml
on:
  schedule:
    # Overnight: run tests every day at 21:30 UTC
    # Usage: cron <minute> <hour> <day-of-month> <day-of-week (sunday=0)>
    - cron: "30 21 * * *"
```

Pentru a distinge clar rulările programate de cele on-demand (**commit**/**PR**),  
workflow-ul **overnight** utilizează un **label** dedicat
vizibil în lista job-urilor **GitHub Actions**.


```yml
run-name: >
  ${{ github.event_name == 'schedule'
      && '🌃 Running Overnight Tests'
      || (
        github.event_name == 'push'
        && format(github.event.head_commit.message)
        || (
          github.event_name == 'pull_request'
          && format(github.event.pull_request.title)
        )
      )
  }}
```