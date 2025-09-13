# 🎞️ Magic Tape

> **Mașina Turing** implementată folosind **liste înlănțuite**.

**Banda Magică** este un experiment de programare inspirat de *mașina Turing*:
o bandă „infinită” unde poți scrie, citi și naviga liber,
mutând cursorul la stânga, la dreapta sau direct la următorul simbol dorit.
În spate, funcționează o structură de tip **listă dublu înlănțuită cu santinelă**,
gândită pentru eficiență și ușurință în manipularea datelor.

Un exercițiu perfect de logică, algoritmi și gândire „out of the box”.


## Operații de tip UPDATE

| Operație | Descriere |
| :--- | :--- |
| `MOVE_LEFT`  | Mută degetul cu o poziție la stanga |
| `MOVE_RIGHT` | Mută degetul cu o poziție la dreapta |
| `MOVE_LEFT_CHAR \<C\>`  | Mută degetul pe prima apariție a caracterului \<C\> din stânga poziției sale curente |
| `MOVE_RIGHT_CHAR \<C\>` | Mută degetul pe prima apariție a caracterului \<C\> din dreapta poziției sale curente |
| `INSERT_LEFT \<C\>` | Inserează caracterul \<C\> în stânga degetului și mută degetul pe noul nod |
| `INSERT_RIGHT \<C\>` | Inserează caracterul \<C\> în dreapta degetului și mută degetul pe noul nod |
| `WRITE \<C\>` | Suprascrie caracterul curent cu \<C\> |



## Operații de tip QUERY

| Operație | Descriere |
| :--- | :--- |
| `SHOW_CURRENT` | Afișează caraceterul din poziția degetului |
| `SHOW` | Afișează conținutul întregii benzii (de la început până la final) |

## Operații de tip UNDO/REDO

| Operație | Descriere |
| :--- | :--- |
| `UNDO` | Anulează ultima operație de `MOVE_LEFT`/`MOVE_RIGHT` |
| `REDO` | Reface ultima operație de `MOVE_LEFT`/`MOVE_RIGHT` |

## Operația EXECUTE

| Operație | Descriere |
| :--- | :--- |
| `EXECUTE` | Extrage o comandă din coadă și o execută |


## ⚠️ Corner Case-uri

- `MOVE_LEFT`:
    nu se va întâmpla nimic dacă degetul este pe prima poziție
- `MOVE_RIGHT`:
    dacă degetul este plasat pe ultimul nod din bandă,
    se va insera caracterul `#` la finalul listei,
    iar degetul va fi poziționat pe noul caracter adăugat

- `MOVE_LEFT_CHAR <C>`:
    dacă nu există niciun caracter \<C\> în stânga degetului,
    poziția acestuia nu se va modifica
    și se va afișa mesajul `ERROR`
- `MOVE_RIGHT_CHAR <C>`:
    dacă nu există niciun caracter \<C\> în dreapta degetului,
    se va insera `#` la finaul benzii
    și poziția degetului va fi pe `#`-ul inserat

- `INSERT_LEFT <C>`:
    dacă degetul este deja la începutul benzii,
    se va afișa mesajul `ERROR` și nu va modifica nimic



## Structuri de Date și Algoritmi

- Listă simplu/dublu înlănțuită
- Stivă
- Coadă

### ⛓️ Banda
---

> ⚠️ ATENȚIE:
>
> **Santinela** este practic un nod plasat la începutul listei;
> ea nu face parte din informația propriu-zisă și nu poate fi modificată in vreun fel.


La începutul programului, banda conține doar santinela,
urmată de caracterul `#` aflat pe prima celulă, unde se află și degetul.

Caracterul `#` este folosit pentru a marca un nod alocat din bandă,
asupra căruia nu s-a realizat nicio operație de scriere.


### 🚶‍♂️🚶‍♂️🚶‍♂️ Coada
---

Am implementat **coada** sub forma unei **liste simplu înlănțuite**,
ce reține ca informație efectivă numele operației de executat.

Pentru simplitate, am ales să construiesc structura de date pentru **coadă** folosind 2 pointeri:
unul către începutul listei și al doilea către sfârșitul ei.

> 🎯 **Avantaj**: `push()`/`pop()` se realizează in `θ(1)`.
> 
> Acestea nu mai necesită iterarea **cozii**, chiar dacă au loc la capete diferite ale ei.

Pe parcurs ce operațiile de tip **UPDATE** sunt citite din fișier,
acestea sunt adăugate la finalul unei cozii
pentru a fi executate in viitor, la întâlnirea instrucțiunii `EXECUTE`.


### 📚 Stiva
---

Cele două stive utilizate pentru `UNDO`/`REDO`
sunt practic două **liste simplu înlănțuite**,
având ca informație propriu-zisă 
**pointeri către nodurile prin care degetul s-a deplasat** pe banda magică.


> 🎯 Operațiile de `push()`/`pop()` asupra acestora
> au loc doar la căpatul de început al listei, in `θ(1)`.

Cel mai simplu mod de a înțelege operațiile `UNDO` și `REDO`  
este să le privim ca pe **săgețile de navigare din istoricul unui browser**.


## 📝 Input/Output

Instrucțiunile de executat asupra *"mașinii Turing"* sunt citite și interpretate,
linie cu linie, dintr-un fișier de intrare `*.in`.

Programul va genera mai apoi un fișier `*.out` care va conține
mesajele de eroare și rezultatele comenzilor de tip **QUERY**.

## Implementări multiple

Acest proiect include două implementări complet independente,
fiecare rezolvând aceleași cerințe.

Codul sursă este organizat în foldere separate, corespunzătoare fiecărui limbaj de programare:


| Lang. | Working Directory |
| :---: | :---: |
| C | [`method-c/`](method-c) |
| Rust | [`method-rs/`](method-rs) |


## 🧪 GitHub Actions | CI Pipeline

**Integrarea Continuă** este o parte esențială a procesului de testare.

În acest proiect, **GitHub Actions** rulează automat testele
ori de câte ori se face *push* în repository sau se deschide un *pull request*.

Workflow-urile sunt definite în următoarele fișiere:
- Pentru implementarea din `method-c`: [.github/workflows/CI-test-method-C.yml](.github/workflows/CI-test-method-C.yml)
- Pentru `method-rs`: [.github/workflows/CI-test-method-Rust.yml](.github/workflows/CI-test-method-Rust.yml)


## 🌃 Overnight Testing

Testele nu numai că sunt rulate la fiecare **commit**/**pull request**,  
dar, mai mult decât atât, am extins workflow-ul de CI  
să includă o rulare **programată în fiecare noapte** (`cron job`),  
care execută integral testele în **GitHub Actions**.

> În **DevOps**, acest tip de execuție programată poartă numele de *"overnight testing"*.



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
