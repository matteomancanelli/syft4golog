#include "DFA.h"

DFA::DFA(Cudd *m) {
    mgr = m;
}

DFA::~DFA() {
    delete mgr;
}

void DFA::initialize(string filename, string partfile) {
    read_from_file(filename);
    nbits = state2bin(nstates - 1).length();

    construct_bdd_new();
    read_partfile(partfile);

    initbv = new int[nbits];
    int temp = init;
    for (int i = nbits - 1; i >= 0; i--) {
        initbv[i] = temp % 2;
        temp = temp / 2;
    }
}