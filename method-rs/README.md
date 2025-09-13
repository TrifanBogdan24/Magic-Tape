# 🎞️ Magic Tape

> **Mașina Turing** implementată în `Rust` 🦀 folosind **liste înlănțuite**.


## Structuri de Date și Algoritmi

- Listă simplu/dublu înlănțuită
- Stivă
- Coadă


### ⛓️ Banda
---

> ⚠️ ATENȚIE:
> **Santinela** este un nod plasat la începutul listei.

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

Am implementat **coada** sub forma unei **liste simplu înlănțuite**.


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
sunt practic două **liste simplu înlănțuite**.



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
- Fișier de intrare: `file.in`
- Fișier de ieșire: `file.in`


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