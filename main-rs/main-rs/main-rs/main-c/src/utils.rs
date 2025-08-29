// src/utils.rs

use std::cell::RefCell;
use std::rc::{Rc, Weak};
use std::io::Write;

// Constante
pub const DIEZ_CHAR: char = '#';
pub const SPACE_CHAR: char = ' ';
pub const ERROR: &str = "ERROR";

/* ---------------------- NodBanda & Banda ---------------------- */
#[derive(Debug)]
pub struct NodBanda {
    pub caracter: char,
    pub urm: Option<Rc<RefCell<NodBanda>>>,
    pub pred: Option<Weak<RefCell<NodBanda>>>,
}

impl NodBanda {
    pub fn new(c: char) -> Rc<RefCell<NodBanda>> {
        Rc::new(RefCell::new(NodBanda {
            caracter: c,
            urm: None,
            pred: None,
        }))
    }
}

#[derive(Debug)]
pub struct Banda {
    pub head: Option<Rc<RefCell<NodBanda>>>,
    pub tail: Option<Rc<RefCell<NodBanda>>>,
    pub santinela: Option<Rc<RefCell<NodBanda>>>,
    pub deget: Option<Rc<RefCell<NodBanda>>>,
}

impl Banda {
    pub fn new() -> Self {
        let head = NodBanda::new(DIEZ_CHAR);
        let santinela = NodBanda::new(SPACE_CHAR);

        head.borrow_mut().pred = Some(Rc::downgrade(&santinela));
        santinela.borrow_mut().urm = Some(Rc::clone(&head));

        Banda {
            head: Some(Rc::clone(&head)),
            tail: Some(Rc::clone(&head)),
            deget: Some(Rc::clone(&head)),
            santinela: Some(santinela),
        }
    }

    pub fn insert_to_tail(&mut self, c: char) {
        let nod = NodBanda::new(c);
        match self.tail.take() {
            Some(tail) => {
                tail.borrow_mut().urm = Some(Rc::clone(&nod));
                nod.borrow_mut().pred = Some(Rc::downgrade(&tail));
                self.tail = Some(nod);
            }
            None => {
                self.head = Some(Rc::clone(&nod));
                self.tail = Some(Rc::clone(&nod));
                if let Some(s) = &self.santinela {
                    nod.borrow_mut().pred = Some(Rc::downgrade(s));
                    s.borrow_mut().urm = Some(Rc::clone(&nod));
                }
            }
        }
    }

    pub fn insert_to_left(&mut self, c: char) -> Result<(), &'static str> {
        let deget = match &self.deget {
            Some(d) => Rc::clone(d),
            None => return Err("Nu exista deget"),
        };

        if Some(Rc::as_ptr(&deget)) == self.head.as_ref().map(|h| Rc::as_ptr(h)) {
            return Err(ERROR);
        }

        let nod = NodBanda::new(c);
        {
            let mut d = deget.borrow_mut();
            if let Some(pred) = d.pred.as_ref().and_then(|w| w.upgrade()) {
                nod.borrow_mut().pred = Some(Rc::downgrade(&pred));
                nod.borrow_mut().urm = Some(Rc::clone(&deget));
                pred.borrow_mut().urm = Some(Rc::clone(&nod));
                d.pred = Some(Rc::downgrade(&nod));
            }
        }
        self.deget = Some(nod);
        Ok(())
    }

    pub fn insert_to_right(&mut self, c: char) {
        let deget = match &self.deget {
            Some(d) => Rc::clone(d),
            None => return,
        };

        if Some(Rc::as_ptr(&deget)) == self.tail.as_ref().map(|t| Rc::as_ptr(t)) {
            self.insert_to_tail(c);
            self.deget = self.tail.clone();
            return;
        }

        let nod = NodBanda::new(c);
        {
            let mut d = deget.borrow_mut();
            if let Some(urm) = d.urm.as_ref() {
                nod.borrow_mut().urm = Some(Rc::clone(urm));
                nod.borrow_mut().pred = Some(Rc::downgrade(&deget));
                urm.borrow_mut().pred = Some(Rc::downgrade(&nod));
                d.urm = Some(Rc::clone(&nod));
            }
        }
        self.deget = Some(nod);
    }

    pub fn print_to_file(&self, fout: &mut impl Write) {
        let mut node_opt = self.head.clone();
        while let Some(node) = node_opt {
            let n = node.borrow();
            if Some(Rc::as_ptr(&node)) == self.deget.as_ref().map(|d| Rc::as_ptr(d)) {
                write!(fout, "|{}|", n.caracter).unwrap();
            } else {
                write!(fout, "{}", n.caracter).unwrap();
            }
            node_opt = n.urm.clone();
        }
        writeln!(fout).unwrap();
        fout.flush().unwrap();
    }
}

/* ---------------------- Queue ---------------------- */
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

impl Queue {
    pub fn new() -> Self {
        Queue { head: None, tail: std::ptr::null_mut() }
    }

    pub fn push(&mut self, op: String) {
        let mut new_node = Box::new(QueueNode { operation: op, urm: None });
        let raw: *mut _ = &mut *new_node;

        if self.head.is_none() {
            self.head = Some(new_node);
            self.tail = raw;
        } else {
            unsafe { (*self.tail).urm = Some(new_node); }
            self.tail = raw;
        }
    }

    pub fn pop(&mut self) {
        if let Some(mut head) = self.head.take() {
            self.head = head.urm.take();
            if self.head.is_none() {
                self.tail = std::ptr::null_mut();
            }
        }
    }
}

/* ---------------------- Stack ---------------------- */
#[derive(Debug)]
pub struct StackNode {
    pub urm: Option<Box<StackNode>>,
    pub pos_deget: Option<Rc<RefCell<NodBanda>>>,
}

pub type Stack = Option<Box<StackNode>>;

impl StackNode {
    pub fn push(stack: &mut Stack, pos: Option<Rc<RefCell<NodBanda>>>) {
        let new_node = Box::new(StackNode {
            pos_deget: pos,
            urm: stack.take(),
        });
        *stack = Some(new_node);
    }

    pub fn top(stack: &Stack) -> Option<Rc<RefCell<NodBanda>>> {
        stack.as_ref().and_then(|n| n.pos_deget.clone())
    }

    pub fn pop(stack: &mut Stack) {
        if let Some(mut node) = stack.take() {
            *stack = node.urm.take();
        }
    }
}
