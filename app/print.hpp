#pragma once


template<class Global>
void print(Global global) {

    for (auto g :  global) {
        g.pretty_print();
    }

}
