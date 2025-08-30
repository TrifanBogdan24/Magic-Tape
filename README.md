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


## Implementări multiple

Acest proiect include două implementări complet independente,
fiecare rezolvând aceleași cerințe.

Codul sursă este organizat în foldere separate, corespunzătoare fiecărui limbaj de programare:

- [`main-c/`](./main-c/)
- [`main-rs/`](./main-rs/)


## 🧪 GitHub Actions | CI Pipeline

**Integrarea Continuă** este o parte esențială a procesului de testare.

În acest proiect, **GitHub Actions** rulează automat testele
ori de câte ori se face *push* în repository sau se deschide un *pull request*.

Workflow-urile sunt definite în următoarele fișiere:
- Pentru implementarea din `main-c`: [.github/workflows/CI-test-main-C.yml](.github/workflows/CI-test-main-C.yml)
- Pentru `main-rs`: [.github/workflows/CI-test-main-Rust.yml](.github/workflows/CI-test-main-Rust.yml)


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
