//
// Created by omen on 27-03-23.
//

#ifndef BITVECTOR_TESTSSTORAGE_H
#define BITVECTOR_TESTSSTORAGE_H
#include "Storage.h"

#endif //BITVECTOR_TESTSSTORAGE_H

void test_Storage() {
    cout << "Size of uint64_t = " << sizeof(uint64_t) << endl;

    Page<uint64_t> buffer(10);
    buffer.createBlock();
    Page<uint64_t> bufferRead(10);
    bufferRead.createBlock(true);
    //LLenamos un buffer con basura
    for (int i = 0; i < 10; i++) {
        buffer[i] = UINT32_MAX;
        cout << hex << "buffer[" << i << "] = " << buffer[i] << endl;
    }
    Storage<uint64_t> pruebaStorage("pruebaStorage.bin", 10);
    pruebaStorage.create();//lo crea solo sino existe;
    pruebaStorage.open();
    // LLenamos 10 paginas con basura
    for (int i = 0; i < 10; i++) {
        pruebaStorage.appendPage(buffer);
    }

    // Leemos pagina 6 (la 7ma pagina contando desde el 1)
    bufferRead = pruebaStorage.readPage(6);
    for (int i = 0; i < 10; i++) {
        cout << "bufferRead[" << i << "] = " << hex << bufferRead[i] << dec << endl;
    }
}
