mod utils;

use utils::*;

use std::rc::Rc;
use std::fs::File;
use std::io::{BufRead, BufReader, Write};


// Constante (echivalentul macro-urilor din C)
const WRITE_CHAR: &str = "WRITE";
const MOVE_LEFT_CHAR: &str = "MOVE_LEFT_CHAR";
const MOVE_RIGHT_CHAR: &str = "MOVE_RIGHT_CHAR";
const INSERT_CHAR_TO_LEFT: &str = "INSERT_LEFT";
const INSERT_CHAR_TO_RIGHT: &str = "INSERT_RIGHT";
const MOVE_LEFT: &str = "MOVE_LEFT";
const MOVE_RIGHT: &str = "MOVE_RIGHT";
const SHOW_CURRENT: &str = "SHOW_CURRENT";
const SHOW_ALL: &str = "SHOW";
const UNDO: &str = "UNDO";
const REDO: &str = "REDO";
const EXECUTE: &str = "EXECUTE";

fn exec_operation(
    fout: &mut impl Write,
    coada: &mut Queue,
    banda: &mut Banda,
    undo: &mut Stack,
    redo: &mut Stack,
) {
    if coada.head.is_none() {
        return;
    }

    // Extragem prima operatie din coada
    let cmd_to_exec = coada.head.as_ref().unwrap().operation.clone();
    coada.pop();

    if cmd_to_exec.contains(WRITE_CHAR) {
        let idx = WRITE_CHAR.len() + 1;
        if let Some(d) = &banda.deget {
            d.borrow_mut().caracter = cmd_to_exec.chars().nth(idx).unwrap_or(DIEZ_CHAR);
        }
        while undo.is_some() { StackNode::pop(undo); }
        while redo.is_some() { StackNode::pop(redo); }

    } else if cmd_to_exec.contains(MOVE_LEFT_CHAR) {
        let idx = MOVE_LEFT_CHAR.len() + 1;
        let target_char = cmd_to_exec.chars().nth(idx).unwrap_or(DIEZ_CHAR);
        let mut aux = banda.deget.clone();
        let mut found = false;

        while let Some(a) = aux {
            if a.borrow().caracter == target_char {
                banda.deget = Some(a);
                found = true;
                break;
            }
            aux = a.borrow().pred.as_ref().and_then(|w| w.upgrade());
        }
        if !found {
            writeln!(fout, "{}", ERROR).unwrap();
            fout.flush().unwrap();
        }

    } else if cmd_to_exec.contains(MOVE_RIGHT_CHAR) {
        let idx = MOVE_RIGHT_CHAR.len() + 1;
        let target_char = cmd_to_exec.chars().nth(idx).unwrap_or(DIEZ_CHAR);

        while let Some(d) = &banda.deget {
            if d.borrow().caracter == target_char {
                break;
            }
            let urm = d.borrow().urm.clone();
            banda.deget = urm;
        }

        if banda.deget.is_none() {
            banda.insert_to_tail(DIEZ_CHAR);
            banda.deget = banda.tail.clone();
        }

    } else if cmd_to_exec.contains(INSERT_CHAR_TO_LEFT) {
        let idx = INSERT_CHAR_TO_LEFT.len() + 1;
        let c = cmd_to_exec.chars().nth(idx).unwrap_or(DIEZ_CHAR);
        banda.insert_to_left(c).unwrap_or_else(|e| {
            writeln!(fout, "{}", e).unwrap();
            fout.flush().unwrap();
        });

    } else if cmd_to_exec.contains(INSERT_CHAR_TO_RIGHT) {
        let idx = INSERT_CHAR_TO_RIGHT.len() + 1;
        let c = cmd_to_exec.chars().nth(idx).unwrap_or(DIEZ_CHAR);
        banda.insert_to_right(c);

    } else if cmd_to_exec.contains(MOVE_LEFT) {
        if let Some(d) = &banda.deget {
            if Some(Rc::as_ptr(d)) != banda.head.as_ref().map(|h| Rc::as_ptr(h)) {
                StackNode::push(undo, banda.deget.clone());
                let pred = d.borrow().pred.as_ref().and_then(|w| w.upgrade());
                banda.deget = pred;
            }
        }

    } else if cmd_to_exec.contains(MOVE_RIGHT) {
        StackNode::push(undo, banda.deget.clone());
        if let Some(d) = &banda.deget {
            if Some(Rc::as_ptr(d)) == banda.tail.as_ref().map(|t| Rc::as_ptr(t)) {
                banda.insert_to_tail(DIEZ_CHAR);
                banda.deget = banda.tail.clone();
            } else {
                let urm = d.borrow().urm.clone();
                banda.deget = urm;
            }
        }
    }
}



fn main() {
    let mut banda = Banda::new();
    let mut queue_execute_update = Queue::new();
    let mut stack_undo: Stack = None;
    let mut stack_redo: Stack = None;

    let fin = File::open("tema1.in").unwrap();
    let fout = File::create("tema1.out").unwrap();
    let mut fout = fout;

    let reader = BufReader::new(fin);
    let mut lines = reader.lines();

    let num_operations: usize = lines.next().unwrap().unwrap().trim().parse().unwrap();

    for line in lines.take(num_operations) {
        let line = line.unwrap();

        if line.contains(MOVE_RIGHT_CHAR) || line.contains(MOVE_LEFT_CHAR)
            || line.contains(MOVE_RIGHT) || line.contains(MOVE_LEFT)
            || line.contains(INSERT_CHAR_TO_LEFT) || line.contains(INSERT_CHAR_TO_RIGHT)
            || line.contains(WRITE_CHAR)
        {
            queue_execute_update.push(line);
        } else if line.contains(SHOW_CURRENT) {
            if let Some(d) = &banda.deget {
                writeln!(fout, "{}", d.borrow().caracter).unwrap();
            }
        } else if line.contains(SHOW_ALL) {
            banda.print_to_file(&mut fout);
        } else if line.contains(UNDO) {
            StackNode::push(&mut stack_redo, banda.deget.clone());
            banda.deget = StackNode::top(&stack_undo);
            StackNode::pop(&mut stack_undo);
        } else if line.contains(REDO) {
            StackNode::push(&mut stack_undo, banda.deget.clone());
            banda.deget = StackNode::top(&stack_redo);
            StackNode::pop(&mut stack_redo);
        } else if line.contains(EXECUTE) {
            exec_operation(&mut fout, &mut queue_execute_update, &mut banda, &mut stack_undo, &mut stack_redo);
        }
    }


    // Forteaza scrierea in fisierul de iesire:
    fout.flush().unwrap();
}
