# 🎞️ Magic Tape

> **Mașina Turing** implementată în `Rust` 🦀 folosind **liste înlănțuite**.

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


```rs
#[derive(Debug)]
pub struct NodBanda {
    pub caracter: char,
    pub urm: Option<Rc<RefCell<NodBanda>>>,
    pub pred: Option<Weak<RefCell<NodBanda>>>,
}

#[derive(Debug)]
pub struct Banda {
    pub head: Option<Rc<RefCell<NodBanda>>>,
    pub tail: Option<Rc<RefCell<NodBanda>>>,
    pub santinela: Option<Rc<RefCell<NodBanda>>>,
    pub deget: Option<Rc<RefCell<NodBanda>>>,
}
```


```rs
impl Banda {
    pub fn new() -> Self;
    pub fn insert_to_tail(&mut self, c: char);
    pub fn insert_to_left(&mut self, c: char) -> Result<(), &'static str>;
    pub fn insert_to_right(&mut self, c: char);
    pub fn print_to_file(&self, fout: &mut impl Write);
}
```


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


```rs
#[derive(Debug)]
pub struct QueueNode {
    pub operation: String,
    pub urm: Option<Box<QueueNode>>,
}

#[derive(Debug)]
pub struct Queue {
    pub head: Option<Box<QueueNode>>,
    pub tail: *mut QueueNode,
}
```


```rs
impl Queue {
    pub fn new() -> Self {
        Queue { head: None, tail: std::ptr::null_mut() }
    }
    pub fn push(&mut self, op: String);
    pub fn pop(&mut self);
}
```

### 📚 Stiva
---

Cele două stive utilizate pentru `UNDO`/`REDO`
sunt practic două **liste simplu înlănțuite**,
având ca informație propriu-zisă 
**pointeri către nodurile prin care degetul s-a deplasat** pe banda magică.


> 🎯 Operațiile de `push()`/`pop()` asupra acestora
> au loc doar la căpatul de început al listei, in `θ(1)`.




```rs
#[derive(Debug)]
pub struct StackNode {
    pub urm: Option<Box<StackNode>>,
    pub pos_deget: Option<Rc<RefCell<NodBanda>>>,
}

pub type Stack = Option<Box<StackNode>>;
```


```rs
impl StackNode {
    pub fn push(stack: &mut Stack, pos: Option<Rc<RefCell<NodBanda>>>);
    pub fn top(stack: &Stack) -> Option<Rc<RefCell<NodBanda>>>;
    pub fn pop(stack: &mut Stack);
}
```


Cel mai simplu mod de a înțelege operațiile `UNDO` și `REDO`  
este să le privim ca pe **săgețile de navigare din istoricul unui browser**.


Fiind complementare, logica din codul de mai jos funcționează  
ca un *joc de ping-pong* între cele două stive:  
când adăugăm poziția curentă a degetului în vârful uneia,
extragem ultimul nod din cealaltă.



```rs
if line.contains(UNDO) {
    StackNode::push(&mut stack_redo, banda.deget.clone());
    banda.deget = StackNode::top(&stack_undo);
    StackNode::pop(&mut stack_undo);
} else if line.contains(REDO) {
    StackNode::push(&mut stack_undo, banda.deget.clone());
    banda.deget = StackNode::top(&stack_redo);
    StackNode::pop(&mut stack_redo);
}
```


## 📝 Input/Output

Instrucțiunile de executat asupra *"mașinii Turing"* sunt citite și interpretate,
linie cu linie, din fisierul `file.in`.

Programul va genera mai apoi un fișier denumit `file.out` care va conține
mesajele de eroare și rezultatele comenzilor de tip **QUERY**.

## 📥 Instalarea dependințelor

```sh
sudo apt install -y build-essential curl
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh      # Instalare rustc & cargo
```

## ⚙️ Rularea programului

Din rădăcina repository-ului:

```sh
cargo build    # Compilare
cargo run      # Execuție
cargo clean    # Curățare
```



## Detalii de implementare în `Rust` 🦀


- `Option`: reprezintă o valoare care poate fi prezentă (`Some`) sau absentă (`None`), înlocuind `NULL` din C
- `Rc`:
  - referință partajată cu număr de referințe
  - **permite mai multe pointere către aceeași valoare**
- `RefCell`:
  - permite mutabilitate interioară chiar dacă structura exterioară este imutabilă
  - verifică împrumuturile la runtime
- `Box`:
  - ownership exclusiv al unei valori stocate pe heap
  - se folosește pentru alocare dinamică și pentru **tipuri recursive** (e.x. **lista**)


## Concluzii generale asupra implementării

Hai să facem o analiză de ansamblu a implementării în Rust, comparativ cu C,
și să tragem câteva concluzii

### 1️⃣ Managementul memoriei

- În C, se foloseau `malloc` și `free` pentru toate structurile (`NodBanda`, `Banda`, `Queue`, `Stack`)
- În Rust, am folosit `Rc<RefCell<T>>` pentru noduri și `Option` pentru a marca lipsa unui nod
  - `Rc` gestionează referințe multiple și contorizează automat, eliminând nevoia de `free`
  - `RefCell` permite mutabilitate interioară (`interior mutability`) chiar dacă structura este imobilă la exterior



### 2️⃣ Borrow checker și mutabilitate

- În C, structurile puteau fi modificate liber în funcții
- În Rust, orice modificare a unui `RefCell` trebuie să respecte reguli stricte:
  - `borrow()` = împrumut imutabil
  - `borrow_mut()` = împrumut mutabil



### 3️⃣ Option și None vs NULL

- În C, se folosea `NULL` pentru a marca sfârșitul listei sau lipsa unui nod
- În Rust, se folosește `Option<Rc<RefCell<NodBanda>>>`:
  - `Some(node)` = nod existent
  - `None` = lista este goală sau nu există nod



### 4️⃣ Scrierea în fișiere și bufferizare

- În C, `fprintf` scria direct în fișier
- În Rust, `File` este bufferizat:
  - `write!` și `writeln!` scriu în buffer, care poate rămâne netratat la sfârșit
  - Este necesar `fout.flush().unwrap()` pentru a se asigura că datele apar în fișier

**Probleme frecvente:**  
- Output-ul gol sau incomplet dacă nu se face flush


### 5️⃣ Traducerea logicii C => Rust

- Logica de manipulare a listei (`insert_to_tail`, `insert_to_left`, `insert_to_right`) rămâne aproape identică
- Diferențele majore sunt tipurile și modul de acces la noduri:  
  - `node->urm` în C => `node.borrow().urm.clone()` în Rust  
  - `node->pred` => `node.borrow().pred.as_ref().and_then(|w| w.upgrade())`  


---

## Concluzii finale

1. Rust impune claritate și siguranță:
    nimic nu poate fi mutat sau împrumutat fără să fie explicit; nimic nu poate fi null fără să fie `Option`.
2. `Rc<RefCell>` rezolvă problema pointerilor dubli și a mutabilității interioare, dar adaugă overhead de clone și upgrade.
3. Bufferizarea în scrierea în fișiere trebuie tratată explicit (`flush`).
4. Cele mai frecvente erori pentru acest tip de program în Rust:  
   - E0506 (împrumut simultan și mutare)  
   - E0433/E0432 (tipuri neimportate: `Rc`, `RefCell`)  
   - Uitarea de `clone()` la `Rc`  
   - Output gol din cauza buffer-ului neflushed  

**Rezumat:** Rust aduce siguranță foarte mare și elimină multe erori tipice din C,
dar necesită obiceiuri clare privind ownership-ul, mutabilitatea și tratarea explicită a valorilor opționale.