//
// Created by fernando s on 27-03-23.
//

#ifndef BITVECTOR_TESTSKTREE_H
#define BITVECTOR_TESTSKTREE_H
#include "ExternalBitvect.h"
#include "LRUCacheStorage.h"
#include "LFUCacheStorage.h"
#include "ExternalK2Tree.h"
#include "Storage.h"

#endif //BITVECTOR_TESTSKTREE_H

string intVec2Str(vector<uint64_t> vec) {
    string output;
    output.append("[ ");
    for (auto i = vec.begin(); i != vec.end(); ++i) {
        output.append(to_string(*i));
        output.append(" ");

    }
    output.append("]");
    return output;
}
bool compareELements(auto primero, auto segundo, auto tercero){
 //   cout << "["<<primero << " " << segundo << " " << tercero <<"]";
    bool pass = true;
    if((primero == segundo) && (primero != tercero))
        pass = false;
  //  cout << " PASS: "<< pass << endl;
    return pass;
}
void testK2TREEall(char *argv[]){
    //int n = 256;
    int n = atoi(argv[2]);
    // std::string dataset ="../../dataset/DH_1_256.txt";
    std::string dataset = argv[1];
    int cantPUNTOS=atoi(argv[3]);
    //int cantPUNTOS=55000;


    cout << " --------------------- TEST K2TREE ALL BUFFERS---------------------"<< endl;
    cout << "BUILDING ...." << endl;
    Storage<uint64_t> NOCACHE("BIN_TESTS/k2t-NOCAHE.bin", 10);
    LRUCacheStorage<uint64_t> LRU("BIN_TESTS/k2t-LRU.bin", 10,50);
    LFUCacheStorage<uint64_t> LFU("BIN_TESTS/k2t-LFU.bin", 10,50);
    ExternalK2Tree<uint64_t> k2t_nocache(2,NOCACHE);
    ExternalK2Tree<uint64_t> k2t_lru(2,LRU);
    ExternalK2Tree<uint64_t> k2t_lfu(2,LFU);
    k2t_nocache.create(dataset.c_str());
    k2t_lru.create(dataset.c_str());
    k2t_lfu.create(dataset.c_str());
    k2t_nocache.open();
    k2t_lru.open();
    k2t_lfu.open();

    /*************************************************************************************************************/
    cout << "**** TESTING EDGE COUNT ¿ESTAN TODOS LOS PUNTOS? ****";

    long long nocacheEDGE = k2t_nocache.getEdges();//strtoll(NOCACHE.getMeta("EDGE_COUNT").c_str(),NULL,10);
    long long lruEDGE = k2t_lru.getEdges();
    long long lfuEDGE = k2t_lfu.getEdges();

    if(cantPUNTOS == nocacheEDGE && cantPUNTOS == lruEDGE && cantPUNTOS == lfuEDGE){
        cout << BOLD << GREEN << " PASS " << RESET <<endl;
    }else{
        cout << BOLD << RED << " FAIL "<< RESET <<endl;
        cout << "NO CACHE COUNT: "<< nocacheEDGE <<"/ LFU: "<< lfuEDGE << "/ LRU: "<< lruEDGE;
        cout << BOLD << GREEN << " DEBIERA SER: "<< cantPUNTOS << RESET << endl;
        k2t_nocache.close();
        k2t_lfu.close();
        k2t_lru.close();
        exit(1);
    }
    /*************************************************************************************************************/
    cout << "**** TESTING RANGE COUNT ¿ESTAN TODOS LOS PUNTOS, SEGUN CONSULTA POR RANGO? ****";
    sleep(1);
    auto nodes = n;
    auto output1 = k2t_nocache.range(0, nodes - 1, 0, nodes - 1); //(0,0) AL (n-1,n-1)
    auto output2 = k2t_lfu.range(0, nodes - 1, 0, nodes - 1);
    auto output3 = k2t_lru.range(0, nodes - 1, 0, nodes - 1);

    if(cantPUNTOS == output1.size() && cantPUNTOS == output2.size() && cantPUNTOS == output3.size()){
        cout << BOLD << GREEN << " PASS " << RESET <<endl;
    }else{
        cout << BOLD << RED << " FAIL "<< RESET <<endl;
        cout << "NO CACHE COUNT: "<< output1.size() <<"/ LFU: "<< output2.size() << "/ LRU: "<< output3.size();
        cout << BOLD << GREEN << " DEBIERA SER: "<< cantPUNTOS << RESET << endl;
        k2t_nocache.close();
        k2t_lfu.close();
        k2t_lru.close();
        exit(1);
    }
    /*************************************************************************************************************/
    cout << "**** TESTING CHECKLINK ¿COINCIDE PUNTO A PUNTO? ****" ;

    bool pass = true;
    for (auto i = 0; i <= n-1; i++) {
        for (auto j = 0; j <= n-1; j++) {
            //   cout << "i = "<< i << " j = "<< j << " " ;
            pass = compareELements2(k2t_nocache.checkLink(i, j), k2t_lfu.checkLink(i, j), k2t_lru.checkLink(i, j));
        }
    }

    if(pass){
        cout << BOLD << GREEN << " PASS " << RESET <<endl;
    }else{
        cout << BOLD << RED << " FAIL "<< RESET <<endl;
        k2t_nocache.close();
        k2t_lfu.close();
        k2t_lru.close();
        exit(2);
    }
    /*************************************************************************************************************/
    cout << "**** TESTING RANGE QUERY ¿COINCIDE PUNTO A PUNTO? ****";

    bool pass2 = true;
    auto itA = output1.begin();
    auto itB = output2.begin();
    auto itC = output3.begin();
    while(itA != output1.end() || itB != output2.end()|| itC != output3.end()){
        pass2 = compareELements2(itA->first,itB->first,itC->first);
        ++itA;
        ++itB;
        ++itC;
    }

    if(pass2){
        cout << BOLD << GREEN << " PASS " << RESET <<endl;
    }else{
        cout << BOLD << RED << " FAIL "<< RESET <<endl;
        k2t_nocache.close();
        k2t_lfu.close();
        k2t_lru.close();
    }

    k2t_nocache.close();
    k2t_lru.close();
    k2t_lfu.close();

}
void testK2TREEall2(char *argv[]){
    //int n = 256;
    int n = atoi(argv[2]);
    // std::string dataset ="../../dataset/DH_1_256.txt";
    std::string dataset = argv[1];
    int cantPUNTOS=atoi(argv[3]);
    //int cantPUNTOS=55000;

    cout << " --------------------- TEST K2TREE ALL BUFFERS---------------------"<< endl;
    cout << "BUILDING ...." << endl;
    Storage<uint64_t> NOCACHE("k2t-NOCAHE.bin", 10);
   // LRUCacheStorage<uint64_t> LRU("BIN_TESTS/k2t-LRU.bin", 10,50);
   // LFUCacheStorage<uint64_t> LFU("BIN_TESTS/k2t-LFU.bin", 10,50);
    ExternalK2Tree<uint64_t> k2t_nocache(2,NOCACHE);
   // ExternalK2Tree<uint64_t> k2t_lru(2,LRU);
    //ExternalK2Tree<uint64_t> k2t_lfu(2,LFU);
    k2t_nocache.create2();
    //k2t_lru.create2();
    //k2t_lfu.create2();
    k2t_nocache.open();
    //k2t_lru.open();
    //k2t_lfu.open();

    /*************************************************************************************************************/
    /*cout << "**** TESTING EDGE COUNT ¿ESTAN TODOS LOS PUNTOS? ****";

    long long nocacheEDGE = k2t_nocache.getEdges();//strtoll(NOCACHE.getMeta("EDGE_COUNT").c_str(),NULL,10);
   // long long lruEDGE = k2t_lru.getEdges();
   // long long lfuEDGE = k2t_lfu.getEdges();

    if(cantPUNTOS == nocacheEDGE ){
        cout << BOLD << GREEN << " PASS " << RESET <<endl;
    }else{
        cout << BOLD << RED << " FAIL "<< RESET <<endl;
        cout << "NO CACHE COUNT: "<< nocacheEDGE ;
        cout << BOLD << GREEN << " DEBIERA SER: "<< cantPUNTOS << RESET << endl;
        k2t_nocache.close();
        exit(1);
    }*/
    /*************************************************************************************************************/
    cout << "**** TESTING RANGE COUNT ¿ESTAN TODOS LOS PUNTOS, SEGUN CONSULTA POR RANGO? ****";
    sleep(1);
    auto nodes = n;
    auto output1 = k2t_nocache.range(0, nodes - 1, 0, nodes - 1); //(0,0) AL (n-1,n-1)

    if(cantPUNTOS == output1.size()){
        cout << BOLD << GREEN << " PASS " << RESET <<endl;
    }else{
        cout << BOLD << RED << " FAIL "<< RESET <<endl;
        cout << "NO CACHE COUNT: "<< output1.size();
        cout << BOLD << GREEN << " DEBIERA SER: "<< cantPUNTOS << RESET << endl;
        k2t_nocache.close();
        exit(1);
    }
    /*************************************************************************************************************/
   /* cout << "**** TESTING CHECKLINK ¿COINCIDE PUNTO A PUNTO? ****" ;

    bool pass = true;
    for (auto i = 0; i <= n-1; i++) {
        for (auto j = 0; j <= n-1; j++) {
            //   cout << "i = "<< i << " j = "<< j << " " ;
            pass = compareELements2(k2t_nocache.checkLink(i, j), k2t_lfu.checkLink(i, j), k2t_lru.checkLink(i, j));
        }
    }

    if(pass){
        cout << BOLD << GREEN << " PASS " << RESET <<endl;
    }else{
        cout << BOLD << RED << " FAIL "<< RESET <<endl;
        k2t_nocache.close();
        k2t_lfu.close();
        k2t_lru.close();
        exit(2);
    }
    *//*************************************************************************************************************//*
    cout << "**** TESTING RANGE QUERY ¿COINCIDE PUNTO A PUNTO? ****";

    bool pass2 = true;
    auto itA = output1.begin();
    auto itB = output2.begin();
    auto itC = output3.begin();
    while(itA != output1.end() || itB != output2.end()|| itC != output3.end()){
        pass2 = compareELements2(itA->first,itB->first,itC->first);
        ++itA;
        ++itB;
        ++itC;
    }

    if(pass2){
        cout << BOLD << GREEN << " PASS " << RESET <<endl;
    }else{
        cout << BOLD << RED << " FAIL "<< RESET <<endl;
        k2t_nocache.close();
        k2t_lfu.close();
        k2t_lru.close();
    }*/

    k2t_nocache.close();
}
void testK2treeGOG(){
    int n = 256;
  //  int n = atoi(argv[2]);
    // std::string dataset ="../../dataset/DH_1_256.txt";
   // std::string dataset = argv[1];
    int cantPUNTOS=1000;
    //int cantPUNTOS=55000;

    cout << " --------------------- TEST K2TREE ALL BUFFERS---------------------"<< endl;
    cout << "BUILDING ...." << endl;
    Storage<uint64_t> NOCACHE("BIN/extK2tree_DH_256_8.bin", 10);
    // LRUCacheStorage<uint64_t> LRU("BIN_TESTS/k2t-LRU.bin", 10,50);
    // LFUCacheStorage<uint64_t> LFU("BIN_TESTS/k2t-LFU.bin", 10,50);
    ExternalK2Tree<uint64_t> k2t_nocache(8,NOCACHE);
    // ExternalK2Tree<uint64_t> k2t_lru(2,LRU);
    //ExternalK2Tree<uint64_t> k2t_lfu(2,LFU);
    //k2t_nocache.create2();
    //k2t_lru.create2();
    //k2t_lfu.create2();
    k2t_nocache.open();
    auto output1 = k2t_nocache.range(0, n - 1, 0, n - 1); //(0,0) AL (n-1,n-1)
        k2t_nocache.checkLink(0,255);
        k2t_nocache.direct(5);
        k2t_nocache.reverse(10);
    cout << "cantidad de puntos encontrados: " << output1.size() <<endl;
}
void testExternalK2treeSinCache() {
    cout << " --------------------- TEST K2TREE CON LRU ---------------------"<<endl;
    cout << "creando el k2tree externo" << endl;
    Storage<uint64_t> storage("BIN_TESTS/k2tree.bin", 10);
    ExternalK2Tree<uint64_t> externalK2Tree(2, storage);
    externalK2Tree.create("../../dataset/DH_256.txt");
    cout << "abriendo el k2tree externo" << endl;
    externalK2Tree.open();

    /*cout << "Matriz de adyacencia construida con check link..." << endl;
    //check link
    cout << endl << "    ";
    int tamMatrix = 8;
    for (int i = 0; i < tamMatrix; i++) {
        cout << (i < 10 ? "  " : " ") << i << " ";
    }
    cout << endl << "   ";
    for (int i = 0; i < tamMatrix; i++) {
        cout << "____";
    }
    cout << endl;
    for (int i = 0; i < tamMatrix; i++) {
        cout << ((i < 10) ? " " : "") << i << "| ";
        for (int j = 0; j < tamMatrix; j++) {
            cout << "  " << externalK2Tree.checkLink(i, j) << " ";
        }
        cout << endl;
    }
    cout << "test de cada punto con check link..." << endl;
    cout << "(0,0) = " << externalK2Tree.checkLink(0, 0) << endl;
    cout << "(0,7) = " << externalK2Tree.checkLink(0, 7) << endl;
    cout << "(5,2) = " << externalK2Tree.checkLink(5, 2) << endl;
    cout << "(6,6) = " << externalK2Tree.checkLink(6, 6) << endl;
    cout << "(1,2) = " << externalK2Tree.checkLink(1, 2) << endl;
    cout << "(3,4) = " << externalK2Tree.checkLink(3, 4) << endl;
    cout << "(5,5) = (NO EXISTE)" << externalK2Tree.checkLink(5, 5) << endl;

    cout << "Direct and Reverse Neighbors..." << endl;
    cout << "Direct Neighbors of 0: Esperado: [ 0 7] Obtenido: " << intVec2Str(externalK2Tree.direct(0)) << endl;
    cout << "Direct Neighbors of 5: Esperado: [ 2 ] Obtenido: " << intVec2Str(externalK2Tree.direct(5)) << endl;
    cout << "Direct Neighbors of 6: Esperado: [ 6 ] Obtenido: " << intVec2Str(externalK2Tree.direct(6)) << endl;
    cout << "Direct Neighbors of 1: Esperado: [ 2] Obtenido: " << intVec2Str(externalK2Tree.direct(1)) << endl;

    cout << "Reverse Neighbors of 7:  Esperado: [ 0] Obtenido: " << intVec2Str(externalK2Tree.reverse(7))
         << endl;
    cout << "Reverse Neighbors of 6: Esperado: [ 6] Obtenido: " << intVec2Str(externalK2Tree.reverse(6))
         << endl;
    cout << "Reverse Neighbors of 2:  Esperado: [1 5 ] Obtenido: " << intVec2Str(externalK2Tree.reverse(2)) << endl;
    cout << " Range query" << endl;
*/
    auto nodes = externalK2Tree.getNodes();
    auto output = externalK2Tree.range(0, nodes - 1, 0, nodes - 1);
    //save output
   // ofstream outfile;
   // outfile.open("../rangeOutput.txt");
    for (const auto &point: output) {
        cout << point.first << " " << point.second << endl;
    }
    //outfile.close();

    externalK2Tree.close();

}
void testExternalK2treeLRU() {
    cout << " --------------------- TEST K2TREE CON LRU ---------------------"<<endl;
    cout << "creando el k2tree externo" << endl;
    LRUCacheStorage<uint64_t> storage("BIN_TESTS/k2tree.bin", 512,20);
    ExternalK2Tree<uint64_t> externalK2Tree(2, storage);
    externalK2Tree.create("../../dataset/DH_1_8192.txt");
    cout << "abriendo el k2tree externo" << endl;
    externalK2Tree.open();

   /* cout << "Matriz de adyacencia construida con check link..." << endl;
    //check link
    cout << endl << "    ";
    int tamMatrix = 8;
    for (int i = 0; i < tamMatrix; i++) {
        cout << (i < 10 ? "  " : " ") << i << " ";
    }
    cout << endl << "   ";
    for (int i = 0; i < tamMatrix; i++) {
        cout << "____";
    }
    cout << endl;
    for (int i = 0; i < tamMatrix; i++) {
        cout << ((i < 10) ? " " : "") << i << "| ";
        for (int j = 0; j < tamMatrix; j++) {
            cout << "  " << externalK2Tree.checkLink(i, j) << " ";
        }
        cout << endl;
    }*/
   /* cout << "test de cada punto con check link..." << endl;
    cout << "(0,0) = " << externalK2Tree.checkLink(0, 0) << endl;
    cout << "(0,7) = " << externalK2Tree.checkLink(0, 7) << endl;
    cout << "(5,2) = " << externalK2Tree.checkLink(5, 2) << endl;
    cout << "(6,6) = " << externalK2Tree.checkLink(6, 6) << endl;
    cout << "(1,2) = " << externalK2Tree.checkLink(1, 2) << endl;
    cout << "(3,4) = " << externalK2Tree.checkLink(3, 4) << endl;
    cout << "(5,5) = (NO EXISTE)" << externalK2Tree.checkLink(5, 5) << endl;

    cout << "Direct and Reverse Neighbors..." << endl;
    cout << "Direct Neighbors of 0: Esperado: [ 0 7] Obtenido: " << intVec2Str(externalK2Tree.direct(0)) << endl;
    cout << "Direct Neighbors of 5: Esperado: [ 2 ] Obtenido: " << intVec2Str(externalK2Tree.direct(5)) << endl;
    cout << "Direct Neighbors of 6: Esperado: [ 6 ] Obtenido: " << intVec2Str(externalK2Tree.direct(6)) << endl;
    cout << "Direct Neighbors of 1: Esperado: [ 2] Obtenido: " << intVec2Str(externalK2Tree.direct(1)) << endl;

    cout << "Reverse Neighbors of 7:  Esperado: [ 0] Obtenido: " << intVec2Str(externalK2Tree.reverse(7))
         << endl;
    cout << "Reverse Neighbors of 6: Esperado: [ 6] Obtenido: " << intVec2Str(externalK2Tree.reverse(6))
         << endl;
    cout << "Reverse Neighbors of 2:  Esperado: [1 5 ] Obtenido: " << intVec2Str(externalK2Tree.reverse(2)) << endl;
    cout << " Range query" << endl;
*/
    auto nodes = externalK2Tree.getNodes();
    auto output = externalK2Tree.range(0, nodes - 1, 0, nodes - 1);
    //save output
    // ofstream outfile;
    // outfile.open("../rangeOutput.txt");
    for (const auto &point: output) {
      //  cout << point.first << " " << point.second << endl;
    }
    //outfile.close();

    externalK2Tree.close();

}
void testExternalK2treeLFU() {
    cout << " --------------------- TEST K2TREE CON LRU ---------------------"<<endl;
    cout << "creando el k2tree externo" << endl;
    LFUCacheStorage<uint64_t> storage("BIN_TESTS/k2tree.bin", 10,20);
    ExternalK2Tree<uint64_t> externalK2Tree(2, storage);
    externalK2Tree.create("../../dataset/DH_8.txt");
    cout << "abriendo el k2tree externo" << endl;
    externalK2Tree.open();

    cout << "Matriz de adyacencia construida con check link..." << endl;
    //check link
    cout << endl << "    ";
    int tamMatrix = 8;
    for (int i = 0; i < tamMatrix; i++) {
        cout << (i < 10 ? "  " : " ") << i << " ";
    }
    cout << endl << "   ";
    for (int i = 0; i < tamMatrix; i++) {
        cout << "____";
    }
    cout << endl;
    for (int i = 0; i < tamMatrix; i++) {
        cout << ((i < 10) ? " " : "") << i << "| ";
        for (int j = 0; j < tamMatrix; j++) {
            cout << "  " << externalK2Tree.checkLink(i, j) << " ";
        }
        cout << endl;
    }
    cout << "test de cada punto con check link..." << endl;
    cout << "(0,0) = " << externalK2Tree.checkLink(0, 0) << endl;
    cout << "(0,7) = " << externalK2Tree.checkLink(0, 7) << endl;
    cout << "(5,2) = " << externalK2Tree.checkLink(5, 2) << endl;
    cout << "(6,6) = " << externalK2Tree.checkLink(6, 6) << endl;
    cout << "(1,2) = " << externalK2Tree.checkLink(1, 2) << endl;
    cout << "(3,4) = " << externalK2Tree.checkLink(3, 4) << endl;
    cout << "(5,5) = (NO EXISTE)" << externalK2Tree.checkLink(5, 5) << endl;

    cout << "Direct and Reverse Neighbors..." << endl;
    cout << "Direct Neighbors of 0: Esperado: [ 0 7] Obtenido: " << intVec2Str(externalK2Tree.direct(0)) << endl;
    cout << "Direct Neighbors of 5: Esperado: [ 2 ] Obtenido: " << intVec2Str(externalK2Tree.direct(5)) << endl;
    cout << "Direct Neighbors of 6: Esperado: [ 6 ] Obtenido: " << intVec2Str(externalK2Tree.direct(6)) << endl;
    cout << "Direct Neighbors of 1: Esperado: [ 2] Obtenido: " << intVec2Str(externalK2Tree.direct(1)) << endl;

    cout << "Reverse Neighbors of 7:  Esperado: [ 0] Obtenido: " << intVec2Str(externalK2Tree.reverse(7))
         << endl;
    cout << "Reverse Neighbors of 6: Esperado: [ 6] Obtenido: " << intVec2Str(externalK2Tree.reverse(6))
         << endl;
    cout << "Reverse Neighbors of 2:  Esperado: [1 5 ] Obtenido: " << intVec2Str(externalK2Tree.reverse(2)) << endl;
    cout << " Range query" << endl;

    auto nodes = externalK2Tree.getNodes();
    auto output = externalK2Tree.range(0, nodes - 1, 0, nodes - 1);
    //save output
    // ofstream outfile;
    // outfile.open("../rangeOutput.txt");
    for (const auto &point: output) {
        cout << point.first << " " << point.second << endl;
    }
    //outfile.close();

    externalK2Tree.close();

}
void testExternalK2treeALL(){
    cout << " --------------------- TEST K2TREE ALL---------------------"<<endl;
    cout << "1.- creando el k2tree externo" << endl;
    Storage<uint64_t> NOCACHE("BIN_TESTS/k2tree-NOCAHE.bin", 10);
    LRUCacheStorage<uint64_t> LRU("BIN_TESTS/k2treeLRU.bin", 10,20);
    LFUCacheStorage<uint64_t> LFU("BIN_TESTS/k2treeLFU.bin", 10,20);

    ExternalK2Tree<uint64_t> k2treenocache(2, NOCACHE);
    ExternalK2Tree<uint64_t> k2treeLru(2, LRU);
    ExternalK2Tree<uint64_t> k2treeLfu(2, LFU);

    k2treenocache.create("../../dataset/DH_256.txt");
    k2treeLru.create("../../dataset/DH_256.txt");
    k2treeLfu.create("../../dataset/DH_256.txt");
    cout << "2.- abriendo los k2tree externos" << endl;
    k2treenocache.open();
    k2treeLru.open();
    k2treeLfu.open();
    int tamMatrix = 256;
    cout << "3.- testing checklink " << endl;

    bool pass = true;
    for (auto i = 0; i < tamMatrix; i++) {
        for (auto j = 0; j < tamMatrix; j++) {
         //   cout << "i = "<< i << " j = "<< j << " " ;
            pass = compareELements(k2treenocache.checkLink(i, j), k2treeLfu.checkLink(i, j), k2treeLru.checkLink(i, j));
        }
    }

    if(pass){
        cout << BOLD << GREEN << "********** CHECKLINK TEST PASSS **********" << RESET <<endl;
    }else{
        cout << BOLD << RED << "********** CHECKLINK TEST FAIL **********"<< RESET <<endl;
    }

    auto nodes1 = k2treenocache.getNodes();
    auto output1 = k2treenocache.range(0, nodes1 - 1, 0, nodes1 - 1);
    auto nodes2 = k2treeLfu.getNodes();
    auto output2 = k2treeLfu.range(0, nodes2 - 1, 0, nodes2 - 1);
    auto nodes3 = k2treeLru.getNodes();
    auto output3 = k2treeLru.range(0, nodes3 - 1, 0, nodes3 - 1);
    cout << "4.- TESTING GET NODES " << endl;

    bool pass2 = true;
    pass2 = compareELements(nodes1,nodes2,nodes3);
    if(pass2){
        cout << BOLD << GREEN << "********** GETNODES TEST PASSS **********" << RESET <<endl;
    }else{
        cout << BOLD << RED << "********** GETNODES TEST FAIL **********"<< RESET <<endl;
    }

    pass2 = true;
    pass2 = compareELements(output1.size(),output2.size(),output3.size());
    cout << "5.- testing RANGE SIZE " << endl;

    if(pass2){
        cout << BOLD << GREEN << "********** RANGE SIZE TEST PASSS **********" << RESET <<endl;
    }else{
        cout << BOLD << RED << "********** RANGE SIZE TEST FAIL **********"<< RESET <<endl;
    }
    cout << "6.- testing RANGE ELEMENTS " << endl;

    pass2 = true;
    auto itA = output1.begin();
    auto itB = output2.begin();
    auto itC = output3.begin();
    while(itA != output1.end() || itB != output2.end()|| itC != output3.end()){
        pass2 = compareELements(itA->first,itB->first,itC->first);
        ++itA;
        ++itB;
        ++itC;
    }

    if(pass2){
        cout << BOLD << GREEN << "********** RANGE elements TEST PASSS **********" << RESET <<endl;
    }else{
        cout << BOLD << RED << "********** RANGE elements TEST FAIL **********"<< RESET <<endl;
    }

    k2treenocache.close();
    k2treeLru.close();
    k2treeLfu.close();

}
void test2BigExternalK2treeSinCache() {
    cout << "TEST BIG K2TREE SIN CACHE" << endl;
    cout << "creando el k2tree" << endl;
    Storage<uint64_t> storage("BIN_TESTS/NOCACHE-k2treeBIG.bin", 10);
    ExternalK2Tree<uint64_t> externalK2Tree(2, storage);
    externalK2Tree.create("../../dataset/DH_1_8192.txt");
    cout << "abriendo el k2tree externo" << endl;
    externalK2Tree.open();
    ifstream input;
    input.open("../../dataset/DH_1_8192.txt");
    long contador = 0, contador2=0;
    if (!input.is_open()) {
        cout << "ERROR: THE FILE " << "../../dataset/DH_1_32768.txt" << " DOES NOT EXIST" << endl;
        exit(1);
    }
    uint32_t a, b;
    cout << "------------- VERIFICANDO EXISTENCIA DE PUNTOS EN LA ESTRUCTURA ------------- "<<endl;
    cout << "------------- EJECUTANDO CHECKPOINT POR CADA PUNTO DEL ARCHIVO ------------- "<<endl;
    while (input >> a >> b) {
        if(externalK2Tree.checkLink(a, b)){
            contador++;
        }else{
            cout << "EL punto: ("<<a<<","<<b<<") no aparece, DEBIERA!!!"<<endl;
            contador2++;
        }
    }
    cout <<  "cantidad de puntos (contador interno): "<< contador << endl;
    cout <<  "cantidad de puntos no reconocidos: "<< contador2 << endl;
    externalK2Tree.close();
}
void test2BigExternalK2treeLFU() {
    cout << "TEST BIG K2TREE LFU" << endl;
    cout << "creando el k2tree" << endl;
    LFUCacheStorage<uint64_t> storage("BIN_TESTS/LFU-k2tree.bin", 10,20);
    ExternalK2Tree<uint64_t> externalK2Tree(2, storage);
    externalK2Tree.create("../../dataset/DH_1_8192.txt");
    cout << "abriendo el k2tree externo" << endl;
    externalK2Tree.open();


    ifstream input;
    input.open("../../dataset/DH_1_8192.txt");
    long contador = 0, contador2=0;
    if (!input.is_open()) {
        cout << "ERROR: THE FILE " << "../../dataset/DH_1_32768.txt" << " DOES NOT EXIST" << endl;
        exit(1);
    }
    uint32_t a, b;
    cout << "------------- VERIFICANDO EXISTENCIA DE PUNTOS EN LA ESTRUCTURA ------------- "<<endl;
    cout << "------------- EJECUTANDO CHECKPOINT POR CADA PUNTO DEL ARCHIVO ------------- "<<endl;
    while (input >> a >> b) {
        //cout << "contador is: "<<contador<<endl;
        //cout << contador<<"- probando punto: "<< printPoint(a,b)<<endl;
        if(externalK2Tree.checkLink(a, b)){
            contador++;
        }else{
            cout << "EL punto: ("<<a<<","<<b<<") no aparece, DEBIERA!!!"<<endl;
            contador2++;
        }
    }
    cout <<  "cantidad de puntos (contador interno): "<< contador << endl;
    cout <<  "cantidad de puntos no reconocidos: "<< contador2 << endl;

    externalK2Tree.close();
}
void test2BigExternalK2treeLRU() {
    cout << "TEST BIG K2TREE LRU" << endl;
    cout << "creando el k2tree" << endl;
    LRUCacheStorage<uint64_t> storage("BIN_TESTS/LRU-k2tree.bin", 10,20);
    ExternalK2Tree<uint64_t> externalK2Tree(2, storage);
    externalK2Tree.create("../../dataset/DH_1_8192.txt");
    cout << "abriendo el k2tree externo" << endl;
    externalK2Tree.open();
    ifstream input;
    input.open("../../dataset/DH_1_8192.txt");
    long contador = 0, contador2=0;
    if (!input.is_open()) {
        cout << "ERROR: THE FILE " << "../../dataset/DH_1_32768.txt" << " DOES NOT EXIST" << endl;
        exit(1);
    }
    uint32_t a, b;
    cout << "------------- VERIFICANDO EXISTENCIA DE PUNTOS EN LA ESTRUCTURA ------------- "<<endl;
    cout << "------------- EJECUTANDO CHECKPOINT POR CADA PUNTO DEL ARCHIVO ------------- "<<endl;
    while (input >> a >> b) {

        if(externalK2Tree.checkLink(a, b)){
            contador++;
        }else{
            cout << "EL punto: ("<<a<<","<<b<<") no aparece, DEBIERA!!!"<<endl;
            contador2++;
        }
    }
    cout <<  "cantidad de puntos (contador interno): "<< contador << endl;
    cout <<  "cantidad de puntos no reconocidos: "<< contador2 << endl;

    externalK2Tree.close();
}
void test2ExternalK2treeLRU() {
    cout << "creando el k2tree externo" << endl;
    LRUCacheStorage<uint64_t> cacheStorage("adjMatrix_uniform64x64LFU.bin", 10, 20);
    ExternalK2Tree<uint64_t> externalK2Tree(2, cacheStorage);
     externalK2Tree.create("../../dataset/uniform16x16.txt");
    cout << "abriendo el k2tree externo" << endl;
    externalK2Tree.open();

    cout << "Matriz de adyacencia construida con check link..." << endl;
    //check link
    cout << endl << "    ";
    for (int i = 0; i < 16; i++) {
        cout << (i < 10 ? "  " : " ") << i << " ";
    }
    cout << endl << "   ";
    for (int i = 0; i < 16; i++) {
        cout << "____";
    }
    cout << endl;
    for (int i = 0; i < 16; i++) {
        cout << ((i < 10) ? " " : "") << i << "| ";
        for (int j = 0; j < 16; j++) {
            cout << "  " << externalK2Tree.checkLink(i, j) << " ";
        }
        cout << endl;
    }
    cout << "test de cada punto con check link..." << endl;
    cout << "(1,1) = " << externalK2Tree.checkLink(1, 1) << endl;
    cout << "(1,3) = " << externalK2Tree.checkLink(1, 3) << endl;
    cout << "(1,7) = " << externalK2Tree.checkLink(1, 7) << endl;
    cout << "(1,11) = " << externalK2Tree.checkLink(1, 11) << endl;
    cout << "(4,11) = " << externalK2Tree.checkLink(4, 11) << endl;
    cout << "(4,13) = " << externalK2Tree.checkLink(4, 13) << endl;
    cout << "(4,15) = " << externalK2Tree.checkLink(4, 15) << endl;
    cout << "(5,2) = " << externalK2Tree.checkLink(5, 2) << endl;
    cout << "(6,14) = " << externalK2Tree.checkLink(6, 14) << endl;
    cout << "(10,4) = " << externalK2Tree.checkLink(10, 4) << endl;
    cout << "(11,11) = " << externalK2Tree.checkLink(11, 11) << endl;
    cout << "(13,7) = " << externalK2Tree.checkLink(13, 7) << endl;
    cout << "(14,3) = " << externalK2Tree.checkLink(14, 3) << endl;
    cout << "(15,7) = " << externalK2Tree.checkLink(15, 7) << endl;
    cout << "(15,15) = " << externalK2Tree.checkLink(15, 15) << endl;

    cout << "(6,15) Esperado false=" << externalK2Tree.checkLink(6, 15) << endl;
    cout << "Direct and Reverse Neighbors..." << endl;
    cout << "Direct Neighbors of 4: Esperado: [ 11 13 15 ] Obtenido: " << intVec2Str(externalK2Tree.direct(4)) << endl;
    cout << "Direct Neighbors of 1: Esperado: [ 1 3 7 11 ] Obtenido: " << intVec2Str(externalK2Tree.direct(1)) << endl;
    cout << "Direct Neighbors of 11: Esperado: [ 11 ] Obtenido: " << intVec2Str(externalK2Tree.direct(11)) << endl;
    cout << "Direct Neighbors of 12: Esperado: [ ] Obtenido: " << intVec2Str(externalK2Tree.direct(12)) << endl;

    cout << "Reverse Neighbors of 7:  Esperado: [ 1 13 15 ] Obtenido: " << intVec2Str(externalK2Tree.reverse(7))
         << endl;
    cout << "Reverse Neighbors of 11: Esperado: [ 1 4 11 ] Obtenido: " << intVec2Str(externalK2Tree.reverse(11))
         << endl;
    cout << "Reverse Neighbors of 5:  Esperado: [  ] Obtenido: " << intVec2Str(externalK2Tree.reverse(5)) << endl;
    cout << " Range query" << endl;
    auto nodes = externalK2Tree.getNodes();

    auto output = externalK2Tree.range(0, nodes - 1, 0, nodes - 1);
    //save output
    ofstream outfile;
    outfile.open("../rangeOutput.txt");
    for (const auto &point: output) {
        outfile << point.first << " " << point.second << endl;
    }
    outfile.close();


    externalK2Tree.close();

}
void test2ExternalK2treeLFU() {
    cout << "creando el k2tree externo" << endl;
    LFUCacheStorage<uint64_t> cacheStorage("../k2tree.bin", 10, 20);
    ExternalK2Tree<uint64_t> externalK2Tree(2, cacheStorage);
    cacheStorage.resetCacheStat();
    externalK2Tree.create("../../dataset/uniform64x64.txt");
    cout << "abriendo el k2tree externo" << endl;
    externalK2Tree.open();
    cout << "total de accesos a disco: " << cacheStorage.getCacheMissed() << endl;

    ifstream input;
    input.open("../../dataset/uniform64x64.txt");
    long contador = 0, contador2=0;
    if (!input.is_open()) {
        cout << "ERROR: THE FILE " << "../../dataset/DH_1_32768.txt" << " DOES NOT EXIST" << endl;
        exit(1);
    }
    uint32_t a, b;
    cout << "------------- VERIFICANDO EXISTENCIA DE PUNTOS EN LA ESTRUCTURA ------------- "<<endl;
    cout << "------------- EJECUTANDO CHECKPOINT POR CADA PUNTO DEL ARCHIVO ------------- "<<endl;
    while (input >> a >> b) {
        cout << "contador is: "<<contador<<endl;
        cout << contador<<"- probando punto: "<< printPoint(a,b)<<endl;
        if(externalK2Tree.checkLink(a, b)){
            contador++;
        }else{
            cout << "EL punto: ("<<a<<","<<b<<") no aparece, DEBIERA!!!"<<endl;
            contador2++;
        }
    }
    cout <<  "cantidad de puntos (contador interno): "<< contador << endl;
    cout <<  "cantidad de puntos no reconocidos: "<< contador2 << endl;

    externalK2Tree.close();

}

