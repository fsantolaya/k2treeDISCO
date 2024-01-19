//
// Created by omen on 27-03-23.
//

#ifndef BITVECTOR_TESTBITVECTOR_H
#define BITVECTOR_TESTBITVECTOR_H
#include "ExternalBitvect.h"
#include "LRUCacheStorage.h"
#include "LFUCacheStorage.h"
#include "Storage.h"

#endif //BITVECTOR_TESTBITVECTOR_H


void test_ExternalBitVector() {
    // Objetos a probar (solo access en bitvector)
    //no me gusta que el objeto quede libre por fuera
    Storage<uint64_t> vectorStorage("Testing.bin", 10);
    ExternalBitvector<uint64_t> pruebaVector(vectorStorage);

    pruebaVector.create(50000); //1000 bits.
    cout << "bits en 1: 512-515" << endl;
    pruebaVector.bitSet(512);
    pruebaVector.bitSet(513);
    pruebaVector.bitSet(514);
    pruebaVector.bitSet(515);
    pruebaVector.print(100);
    cout << "cantidad de paginas: "<< vectorStorage.getPages()<< endl;
    cout
            << "Print es basado en acces. La siguiente prueba evalua access individual de los bits en 1 512-515 (se espera 1 1 1 1 )"
            << endl;
    cout << pruebaVector.access(512) << " ";
    cout << pruebaVector.access(513) << " ";
    cout << pruebaVector.access(514) << " ";
    cout << pruebaVector.access(515) << " ";
    cout << endl;
    cout << "se limpian los 4 bits anteriores y se revisan con rank se espera (0 0 0 0)" << endl;
    pruebaVector.bitClean(513);
    pruebaVector.bitClean(515);
    pruebaVector.bitClean(512);
    pruebaVector.bitClean(514);
    cout << pruebaVector.access(512) << " ";
    cout << pruebaVector.access(513) << " ";
    cout << pruebaVector.access(514) << " ";
    cout << pruebaVector.access(515) << " ";
    cout << endl;

    cout << "poniendo en 1 cada 4 bits desde el 1°" << endl;
    for (uint64_t i = 0; i < pruebaVector.getLength(); i += 4) {
        pruebaVector.bitSet(i);
    }
    uint64_t pos = 0;
    pruebaVector.print();

    cout << "prueba de build rank y rank (se espera 250)" << endl;
    pruebaVector.buildRank();
    cout << pruebaVector.rank((uint64_t)49999 ) << endl;

}

void test_bits() {
    uint64_t bits = 0, i = 0;
    int bit;
    cout << ((BitBasic::access(&bits, (uint64_t) 0, 64)) ? 1 : 0) << endl;
    cout << "set(0)" << endl;
    BitBasic::BitSet<uint64_t>(&bits, i, 64);
    cout << ((BitBasic::access(&bits, (uint64_t) 0, 64)) ? 1 : 0) << endl;
    for (uint64_t i = 0; i < 64; i++) {
        bit = (BitBasic::access(&bits, i, 64) ? 1 : 0);
        cout << bit;
    }
    cout << endl;
    cout << bitset<64>(bits) << endl;

    i = 2;
    cout << ((BitBasic::access(&bits, i, 64)) ? 1 : 0) << endl;
    cout << "set(2)" << endl;
    BitBasic::BitSet<uint64_t>(&bits, i, 64);
    cout << ((BitBasic::access(&bits, i, 64)) ? 1 : 0) << endl;

    for (int i = 0; i < 64; i++) {
        cout << (BitBasic::access<uint64_t>(&bits, i, 64)) ? 1 : 0;
    }
    cout << endl;

    cout << bitset<64>(bits) << endl;

    i = 4;
    cout << ((BitBasic::access(&bits, i, 64)) ? 1 : 0) << endl;
    cout << "set(4)" << endl;
    BitBasic::BitSet<uint64_t>(&bits, i, 64);
    cout << ((BitBasic::access(&bits, i, 64)) ? 1 : 0) << endl;

    for (int i = 0; i < 64; i++) {
        cout << ((BitBasic::access<uint64_t>(&bits, i, 64)) ? 1 : 0);
    }
    cout << endl;
    cout << bitset<64>(bits) << endl;
    BitBasic::BitClear(&bits, (uint64_t) 0, 64);
    BitBasic::BitClear(&bits, (uint64_t) 2, 64);
    BitBasic::BitClear(&bits, (uint64_t) 4, 64);
    for (int i = 0; i < 64; i++) {
        cout << ((BitBasic::access<uint64_t>(&bits, i, 64)) ? 1 : 0);
    }
    cout << endl;
    cout << bitset<64>(bits) << endl;
    cout << endl;
    BitBasic::BitSet(&bits, (uint64_t) 10, 64);
    BitBasic::BitSet(&bits, (uint64_t) 11, 64);
    BitBasic::BitSet(&bits, (uint64_t) 12, 64);
    BitBasic::BitSet(&bits, (uint64_t) 13, 64);
    cout << bitset<64>(bits) << endl;
    for (int i = 0; i < 64; i++) {
        cout << ((BitBasic::access<uint64_t>(&bits, i, 64)) ? 1 : 0);
    }
    cout << endl;
    i = 13;
    cout << "rank(13)= " << BitBasic::Rank(&bits, i, 64) << "(esperado 4) " << endl;
}

void test_ExternalBitVectorWithLRU() {
    // Objetos a probar (solo access en bitvector)
    //no me gusta que el objeto quede libre por fuera
    //Storage<uint64_t> vectorStorage("Testing.bin", 10);
    LRUCacheStorage<uint64_t> vectorStorage("Testing.bin", 10, 20);
    ExternalBitvector<uint64_t> pruebaVector(vectorStorage);

    pruebaVector.create(1000);
    cout << "bits en 1: 512-515" << endl;
    pruebaVector.bitSet(512);
    pruebaVector.bitSet(513);
    pruebaVector.bitSet(514);
    pruebaVector.bitSet(515);
    pruebaVector.print(100);
    cout
            << "Print es basado en acces. La siguiente prueba evalua access individual de los bits en 1 512-515 (se espera 1 1 1 1 )"
            << endl;
    cout << pruebaVector.access(512) << " ";
    cout << pruebaVector.access(513) << " ";
    cout << pruebaVector.access(514) << " ";
    cout << pruebaVector.access(515) << " ";
    cout << endl;
    cout << "se limpian los 4 bits anteriores y se revisan con rank se espera (0 0 0 0)" << endl;
    pruebaVector.bitClean(513);
    pruebaVector.bitClean(515);
    pruebaVector.bitClean(512);
    pruebaVector.bitClean(514);
    cout << pruebaVector.access(512) << " ";
    cout << pruebaVector.access(513) << " ";
    cout << pruebaVector.access(514) << " ";
    cout << pruebaVector.access(515) << " ";
    cout << endl;

    cout << "poniendo en 1 cada 4 bits desde el 1°" << endl;
    for (uint64_t i = 0; i < pruebaVector.getLength(); i += 4) {
        pruebaVector.bitSet(i);
    }
    uint64_t pos = 0;
    pruebaVector.print();

    cout << "prueba de build rank y rank (se espera 250)" << endl;
    pruebaVector.buildRank();
    cout << pruebaVector.rank((uint64_t) 999) << endl;

}
