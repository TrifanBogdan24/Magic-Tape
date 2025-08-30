#!/usr/bin/bash

NUM_TOTAL_TESTS=$(ls -1 ../../test_data/Input/*.in | wc -l)
NUM_PASSED_TESTS=0

function readmeExists() {
    if [[ -f "README" || -f "README.md" ]] ; then
        echo "README: [exists]"
    else
        echo "README: [deosn't exist]"
    fi
}


function testInput() {
    cat ../test_data/Input/test$1.in > file.in
    cargo run 2> compile.info

    if [ "$?" -ne 0 ] ; then
        echo "[ERROR] test$1.in"
        cat compile.info
        return
    fi

    diff ../test_data/Reference/test$1.ref file.out > /dev/null 2>&1
    if [ "$?" -eq 0 ] ; then
        echo "test$1.in: [OK]"
        NUM_PASSED_TESTS=$((NUM_PASSED_TESTS+1))
    else
        echo "test$1.in: [WRONG]"
    fi
}


function main() {
    echo " -= Magic Tape (RS) =-"
    echo

    # Go from tests/ to repo's root
    cd ../
    touch file.in file.out

    readmeExists
    
    cargo clean
    cargo build


    for i in $(seq 1 $NUM_TOTAL_TESTS) ; do
        testInput $i
    done

    rm -f compile.info
    cargo clean

    echo ""
    echo "Passed tests: $NUM_PASSED_TESTS / $NUM_TOTAL_TESTS"
    rm -f file.in file.out
    
}

main