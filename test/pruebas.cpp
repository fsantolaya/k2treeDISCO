//
// Created by Carlos on 01-08-2022.
//
#include "gtest/gtest.h"
#include "Storage.h"
#include "LFUCacheStorage.h"
#include "LRUCacheStorage.h"
#include "ExternalBitvect.h"
#include "ExternalK2Tree.h"
#include "ExternalAdjacencyMatrix.h"

TEST(Expect64Test, Expect64){
    ASSERT_EQ(8,sizeof(uint64_t)) << "No se reconoce uint64_t como entero de 64 bit";
}

//Probamos el header Page.h (bloque de memoria)
TEST(StorageTest, testPage){
    //Creamos página de 10 palabras y obtenemos su puntero
    Page<uint64_t> pagina(10);
    pagina.createBlock();
    uint64_t* puntero = pagina.data();

    //LLenamos la página con basura
    for (int i = 0; i < 10; i++)
        pagina[i] = UINT32_MAX;

    //Verificamos que tenga esa basura
    for(int i=0; i < 10; i++)
        EXPECT_EQ(puntero[i],UINT32_MAX);
}

// Probamos el header Storage.h, el cual administra paginas
TEST(StorageTest, testMainStorage){
    //Se crea una página para poder insertarla en el objeto Storage
    Page<uint64_t> buffer(10);
    buffer.createBlock();

    //Se crea una página para poder leer una desde el objeto Storage
    Page<uint64_t> bufferRead(10);
    bufferRead.createBlock(true);

    //LLenamos una página con basura
    for (int i = 0; i < 10; i++)
        buffer[i] = 111111111;

    //Creamos el objeto Storage a testear.
    Storage<uint64_t> pruebaStorage("pruebaStorage.bin", 10);

    //Creamos y abrimos el archivo
    ASSERT_TRUE(pruebaStorage.create(true)); //si existe lo elimina y crea de nuevo
    ASSERT_TRUE(pruebaStorage.open());

    // Adjuntamos 10 páginas a Storage.
    for (int i = 0; i < 10; i++) {
        pruebaStorage.appendPage(buffer);
    }

    // Leemos pagina 6 (la 7ma pagina contando desde el 1)
    bufferRead = pruebaStorage.readPage(6);

    //Verificamos si lo leido es igual a lo que insertamos
    for (int i = 0; i < 10; i++)
        EXPECT_EQ(bufferRead[i],111111111);

    //LLenamos una página con basura distinta
    for (int i = 0; i < 10; i++)
        buffer[i] = 222222222;

    //Acutalizamos la página 6
    pruebaStorage.updatePage(buffer,6);

    // Leemos pagina 6 (la 7ma pagina contando desde el 1)
    bufferRead = pruebaStorage.readPage(6);

    //Verificamos si lo leido es igual a lo que insertamos
    for (int i = 0; i < 10; i++)
        EXPECT_EQ(bufferRead[i],222222222);

    //Cerramos el archivo
    pruebaStorage.close();
}

// Probamos el header LFUCacheStorage
TEST(StorageTest, TestLFUCache){
    //Se crea una página para poder insertarla en el objeto Storage
    Page<uint64_t> buffer(10);
    buffer.createBlock();

    //Se crea una página para poder leer una desde el objeto Storage
    Page<uint64_t> bufferRead(10);
    bufferRead.createBlock(true);

    //LLenamos una página con basura
    for (int i = 0; i < 10; i++)
        buffer[i] = 333333333;

    //Creamos el objeto Storage a testear con 3 paginas de cache.
    LFUCacheStorage<uint64_t> pruebaStorage("pruebaStorage.bin", 10, 3);

    //Creamos y abrimos el archivo
    ASSERT_TRUE(pruebaStorage.create(true)); //si existe lo elimina y crea de nuevo
    ASSERT_TRUE(pruebaStorage.open());

    // Adjuntamos 10 páginas a Storage.
    for (int i = 0; i < 10; i++) {
        pruebaStorage.appendPage(buffer);
    }

    // Leemos pagina 6 (la 7ma pagina contando desde el 1)
    bufferRead = pruebaStorage.readPage(6);

    //Verificamos si lo leido es igual a lo que insertamos
    for (int i = 0; i < 10; i++)
        EXPECT_EQ(bufferRead[i],333333333);

    //LLenamos una página con basura distinta
    for (int i = 0; i < 10; i++)
        buffer[i] = 444444444;

    //Acutalizamos la página 6
    pruebaStorage.updatePage(buffer,6);

    // Leemos pagina 6 (la 7ma pagina contando desde el 1)
    bufferRead = pruebaStorage.readPage(6);

    //Verificamos si lo leido es igual a lo que insertamos
    for (int i = 0; i < 10; i++)
        EXPECT_EQ(bufferRead[i],444444444);

    //Cerramos el archivo
    pruebaStorage.close();
}

// Probamos el header LRUCacheStorage
TEST(StorageTest, TestLRUCache){
    //Se crea una página para poder insertarla en el objeto Storage
    Page<uint64_t> buffer(10);
    buffer.createBlock();

    //Se crea una página para poder leer una desde el objeto Storage
    Page<uint64_t> bufferRead(10);
    bufferRead.createBlock(true);

    //LLenamos una página con basura
    for (int i = 0; i < 10; i++)
        buffer[i] = 555555555;

    //Creamos el objeto Storage a testear con 3 paginas de cache.
    LRUCacheStorage<uint64_t> pruebaStorage("pruebaStorage.bin", 10, 3);

    //Creamos y abrimos el archivo
    ASSERT_TRUE(pruebaStorage.create(true)); //si existe lo elimina y crea de nuevo
    ASSERT_TRUE(pruebaStorage.open());

    // Adjuntamos 10 páginas a Storage.
    for (int i = 0; i < 10; i++) {
        pruebaStorage.appendPage(buffer);
    }

    // Leemos pagina 6 (la 7ma pagina contando desde el 1)
    bufferRead = pruebaStorage.readPage(6);

    //Verificamos si lo leido es igual a lo que insertamos
    for (int i = 0; i < 10; i++)
        EXPECT_EQ(bufferRead[i],555555555);

    //LLenamos una página con basura distinta
    for (int i = 0; i < 10; i++)
        buffer[i] = 666666666;

    //Acutalizamos la página 6
    pruebaStorage.updatePage(buffer,6);

    // Leemos pagina 6 (la 7ma pagina contando desde el 1)
    bufferRead = pruebaStorage.readPage(6);

    //Verificamos si lo leido es igual a lo que insertamos
    for (int i = 0; i < 10; i++)
        EXPECT_EQ(bufferRead[i],666666666);

    //Cerramos el archivo
    pruebaStorage.close();
}

// Probamos el header BitBasic.h
TEST(BitvectorTest,testBits) {
    int bit;

    //bits es nuestro bitvector e i la posición
    uint64_t bits = 0, i = 0;

    //vemos si el bit i vale 0
    EXPECT_EQ(BitBasic::access(&bits, i, 64),0);
    //lo prendemos
    BitBasic::BitSet<uint64_t>(&bits, i, 64);
    //vemos si ahora el bit i vale 1
    EXPECT_EQ(BitBasic::access(&bits, i, 64),1);

    //Verificamos si los demas bits valen 0
    for (uint64_t j = 1; j < 64; j++)
        EXPECT_EQ(BitBasic::access(&bits, j, 64),0);

    // Esperemos que valga 1 seguido de 63 ceros.
    EXPECT_EQ(bitset<64>(bits),0x8000000000000000);

    i = 2;

    //prendemos el bit 2
    EXPECT_EQ(BitBasic::access(&bits, i, 64),0);
    BitBasic::BitSet<uint64_t>(&bits, i, 64);
    EXPECT_EQ(BitBasic::access(&bits, i, 64),1);

    //Esperamos que el bit 0 y 2 valgan 1 y los demas 0
    for (uint64_t j = 0; j < 64; j++)
        if(j == 0 || j == 2)
            EXPECT_EQ(BitBasic::access(&bits, j, 64), 1);
        else
            EXPECT_EQ(BitBasic::access(&bits, j, 64), 0);

    //Esperamos que el valor sea 1010 (A en hex) seguido de 60 ceros
    EXPECT_EQ(bitset<64>(bits),0xA000000000000000);

    i = 4;
    //Repetimos el proceso con el bit 4
    EXPECT_EQ(BitBasic::access(&bits, i, 64),0);
    BitBasic::BitSet<uint64_t>(&bits, i, 64);
    EXPECT_EQ(BitBasic::access(&bits, i, 64),1);

    for (uint64_t j = 0; j < 64; j++)
        if(j == 0 || j == 2 || j == 4)
            EXPECT_EQ(BitBasic::access(&bits, j, 64), 1);
        else
            EXPECT_EQ(BitBasic::access(&bits, j, 64), 0);

    // Esperamos que valga 1010 1000 seguido de 56 ceros
    EXPECT_EQ(bitset<64>(bits),0xA800000000000000);

    //Apagamos los 3 bits que prendimos
    BitBasic::BitClear(&bits, (uint64_t) 0, 64);
    BitBasic::BitClear(&bits, (uint64_t) 2, 64);
    BitBasic::BitClear(&bits, (uint64_t) 4, 64);

    //Verificamos que todos valgan 0
    for (int j = 0; j < 64; j++)
        EXPECT_EQ(BitBasic::access<uint64_t>(&bits, j, 64),0);

    //Esperamos que tenga valor 0
    EXPECT_EQ(bitset<64>(bits),0);

    //Prendemos 10,11,12,13
    BitBasic::BitSet(&bits, (uint64_t) 10, 64);
    BitBasic::BitSet(&bits, (uint64_t) 11, 64);
    BitBasic::BitSet(&bits, (uint64_t) 12, 64);
    BitBasic::BitSet(&bits, (uint64_t) 13, 64);

    //Verificamos que tenga el valor esperado
    EXPECT_EQ(bitset<64>(bits),0x003C000000000000);

    for (int j = 0; j < 64; j++)
        if(j>=10 && j<=13)
            EXPECT_EQ(BitBasic::access<uint64_t>(&bits, j, 64),1);
        else
            EXPECT_EQ(BitBasic::access<uint64_t>(&bits, j, 64),0);


    i = 13;
    //Calculamos Rank a 13 y vemos si tiene el valor esperado,
    //como prendimos 4 bits (10,11,12,13) este debe ser 4.
    EXPECT_EQ(BitBasic::Rank(&bits, i, 64),4);
}

// Probamos el header ExternalBitvect.h
TEST(BitvectorTest,testExternalBitVector){
    //Creamos el objeto Storage que administrará las páginas donde se escribirá el bitvector.
    Storage<uint64_t> vectorStorage("MiBitvector.bin", 10);

    //Creamos el objeto Bitvector, de tamaño 1000 y prendemos 4 bits.
    ExternalBitvector<uint64_t> pruebaVector(vectorStorage);
    pruebaVector.create(1000);
    pruebaVector.bitSet(512);
    pruebaVector.bitSet(513);
    pruebaVector.bitSet(514);
    pruebaVector.bitSet(515);

    //Verificamos que estos bits esten prendidos
    EXPECT_EQ(pruebaVector.access(512),1);
    EXPECT_EQ(pruebaVector.access(513),1);
    EXPECT_EQ(pruebaVector.access(514),1);
    EXPECT_EQ(pruebaVector.access(515),1);

    //Los apagamos
    pruebaVector.bitClean(513);
    pruebaVector.bitClean(515);
    pruebaVector.bitClean(512);
    pruebaVector.bitClean(514);

    //Verificamos que esten apagados
    EXPECT_EQ(pruebaVector.access(512),0);
    EXPECT_EQ(pruebaVector.access(513),0);
    EXPECT_EQ(pruebaVector.access(514),0);
    EXPECT_EQ(pruebaVector.access(515),0);

    //Prendemos los bits en posición multiplo de 4
    for (uint64_t i = 0; i < pruebaVector.getLength(); i += 4)
        pruebaVector.bitSet(i);

    //Verificamos que estos bits valgan 1 y los demas 0
    for (uint64_t i = 0; i < pruebaVector.getLength(); i ++)
        if(i%4==0)
            EXPECT_EQ(pruebaVector.access(i),1);
        else
            EXPECT_EQ(pruebaVector.access(i),0);

    //Construimos y verificamos el rank
    pruebaVector.buildRank();
    EXPECT_EQ(pruebaVector.rank((uint64_t)999),250);
}

//Prueba de Bitvector con caché.
TEST(BitvectorTest,testExternalBitVectorWithLFU) {
    //Creamos el objeto Storage que administrará las páginas donde se escribirá el bitvector.
    LFUCacheStorage<uint64_t> vectorStorage("MiBitvectorLRU.bin", 10,20);

    //Creamos el objeto Bitvector, de tamaño 1000 y prendemos 4 bits.
    ExternalBitvector<uint64_t> pruebaVector(vectorStorage);
    pruebaVector.create(1000);
    pruebaVector.bitSet(512);
    pruebaVector.bitSet(513);
    pruebaVector.bitSet(514);
    pruebaVector.bitSet(515);

    //Verificamos que estos bits esten prendidos
    EXPECT_EQ(pruebaVector.access(512),1);
    EXPECT_EQ(pruebaVector.access(513),1);
    EXPECT_EQ(pruebaVector.access(514),1);
    EXPECT_EQ(pruebaVector.access(515),1);

    //Los apagamos
    pruebaVector.bitClean(513);
    pruebaVector.bitClean(515);
    pruebaVector.bitClean(512);
    pruebaVector.bitClean(514);

    //Verificamos que esten apagados
    EXPECT_EQ(pruebaVector.access(512),0);
    EXPECT_EQ(pruebaVector.access(513),0);
    EXPECT_EQ(pruebaVector.access(514),0);
    EXPECT_EQ(pruebaVector.access(515),0);

    //Prendemos los bits en posición multiplo de 4
    for (uint64_t i = 0; i < pruebaVector.getLength(); i += 4)
        pruebaVector.bitSet(i);

    //Verificamos que estos bits valgan 1 y los demas 0
    for (uint64_t i = 0; i < pruebaVector.getLength(); i ++)
        if(i%4==0)
            EXPECT_EQ(pruebaVector.access(i),1);
        else
            EXPECT_EQ(pruebaVector.access(i),0);

    //Construimos y verificamos el rank
    pruebaVector.buildRank();
    EXPECT_EQ(pruebaVector.rank((uint64_t)999),250);
}

//Prueba grande de bitvector (crea archivo de 2 Megabyte)
TEST(BitvectorTest, TestLRUCacheBitVect){
    //Creamos el objeto Storage que administrará las páginas donde se escribirá el bitvector.
    LRUCacheStorage<uint64_t> vectorStorage("MiBitvectorLFU.bin", 10,20);

    //Creamos el objeto Bitvector, de tamaño 2 Megabyte y prendemos 4 bits.
    ExternalBitvector<uint64_t> pruebaVector(vectorStorage);
    pruebaVector.create(16000000); //2 megabyte
    pruebaVector.bitSet(512);
    pruebaVector.bitSet(51300);
    pruebaVector.bitSet(514000);
    pruebaVector.bitSet(1550000);

    //Verificamos que estos bits estén prendidos
    EXPECT_EQ(pruebaVector.access(512),1);
    EXPECT_EQ(pruebaVector.access(51300),1);
    EXPECT_EQ(pruebaVector.access(514000),1);
    EXPECT_EQ(pruebaVector.access(1550000),1);

    //Los apagamos
    pruebaVector.bitClean(512);
    pruebaVector.bitClean(51300);
    pruebaVector.bitClean(514000);
    pruebaVector.bitClean(1550000);

    //Verificamos que esten apagados
    EXPECT_EQ(pruebaVector.access(512),0);
    EXPECT_EQ(pruebaVector.access(51300),0);
    EXPECT_EQ(pruebaVector.access(514000),0);
    EXPECT_EQ(pruebaVector.access(1550000),0);

    //Prendemos los bits en posición multiplo de 4
    for (uint64_t i = 0; i < pruebaVector.getLength(); i += 4)
        pruebaVector.bitSet(i);

    //Verificamos que estos bits valgan 1 y los dempas 0
    for (uint64_t i = 0; i < pruebaVector.getLength(); i ++)
        if(i%4==0)
            EXPECT_EQ(pruebaVector.access(i),1);
        else
            EXPECT_EQ(pruebaVector.access(i),0);

    //Construimos y verificamos el rank
    pruebaVector.buildRank();
    EXPECT_EQ(pruebaVector.rank((uint64_t)1599999),4000000);
}
/*
TEST(k2Tree, adjMatrixTest) {

    int n = 64;
    int size = 8;
    Storage<uint64_t> storage("../k2tree.bin", 2);

    ExternalAdjacencyMatrix<uint64_t> pruebaMatrix(storage, size, size);

    pruebaMatrix.create( size * size); //aca falla.
    pruebaMatrix.load("../dataset/uniform64x64.txt");

    pruebaMatrix.print(); //la matriz
    pruebaMatrix.printDirectNeighbors(1,2); //imprime la lista.
   // pruebaMatrix.printDirects(pruebaMatrix.direct(5,2), 5);

}*/
