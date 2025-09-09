//
// Created by fernando s on 27-03-23.
//

#ifndef BITVECTOR_TESTSADAYACENCIA_H
#define BITVECTOR_TESTSADAYACENCIA_H
#include "ExternalBitvect.h"
#include "LRUCacheStorage.h"
#include "LFUCacheStorage.h"
#include "Storage.h"
#include "ExternalAdjacencyMatrix.h"
#endif //BITVECTOR_TESTSADAYACENCIA_H
using namespace std;
bool compareELements2(auto primero, auto segundo, auto tercero){
    //   cout << "["<<primero << " " << segundo << " " << tercero <<"]";
    bool pass = true;
    if((primero == segundo) && (primero != tercero))
        pass = false;
    //  cout << " PASS: "<< pass << endl;
    return pass;
}
void test_matrizAdyacencia(char *argv[]){
    cout << "Archivo a procesar: "<< argv[1] << endl;
    cout << "Tamaño matriz es: "<< argv[2] << endl;

    double pagePercents;

    pagePercents = 100 / 100.0;
    int pageSize = sysconf(_SC_PAGE_SIZE); //lo devuelve en bytes. la que ocupa el S.O
    int D = ceil(((pageSize * 8.0) / 64.0) * pagePercents); //cantidad de palabras que contiene cada página.
    int n = atoi(argv[2]); //tamaño de la matriz.

    //LRUCacheStorage<uint64_t> storage ("adjMatrix.bin", D, 50);
    cout << "------------- CONSTRUYENDO LA ESTRUCTURA ------------- ";

    Storage<uint64_t> storage ("BIN_TESTS/adjMatrix.bin", D);
    ExternalAdjacencyMatrix<uint64_t> externalAdjacencyMatrix(storage, n);

    externalAdjacencyMatrix.create(argv[1]);
   // externalAdjacencyMatrix.load();
    //externalAdjacencyMatrix.open();
    cout << "OK"<<endl;

    // externalAdjacencyMatrix.printTL(160);
    cout << "------------- DOING TESTING -------------"<< endl;
    cout << "Probando el dataset: " << argv[1] << endl;
    cout << "Tamaño de la matriz: " << n << " X " << n << endl;
    cout << "Numero de words por página: " << D << endl;
    cout << "Cantidad de páginas de disco utilizadas (creadas): "<< storage.getPages() << endl;
    cout << "External bit vector ones: "<< storage.getMeta("EXTERNAL_BIT_VECTOR_ONES") << endl;
    cout << "External bit length: "<< storage.getMeta("EXTERNAL_BIT_VECTOR_LENGTH")<< endl;

    ifstream input;
    const char *fileName = argv[1];
    input.open(fileName);
    long contador = 0, contador2=0;
    if (!input.is_open()) {
        cout << "ERROR: THE FILE " << fileName << " DOES NOT EXIST" << endl;
        exit(1);
    }
    uint32_t a, b;
    cout << "------------- VERIFICANDO EXISTENCIA DE PUNTOS EN LA ESTRUCTURA ------------- "<<endl;
    cout << "------------- EJECUTANDO CHECKPOINT POR CADA PUNTO DEL ARCHIVO ------------- ";
    while (input >> a >> b) {
        if(externalAdjacencyMatrix.isEdge(a,b)){
            contador++;
        }else{
            cout << "EL punto: ("<<a<<","<<b<<") no aparece, DEBIERA!!!"<<endl;
            contador2++;
        }
    }
    input.close();
    cout << "OK "<< endl;

   /* cout << "DIRECTOS DE LA FILA 0 SON:" << endl;
    vector <int> directos =  externalAdjacencyMatrix.direct(0);
    *//*for (int value:directos){
        cout << "(0 , "<< value <<")"<<endl;
    }*//*

    cout << "REVERSOS DE LA COLUMNA 2 SON:" << endl;
    vector <int> reversos =  externalAdjacencyMatrix.reverse(2);
    *//*for (int value:reversos){
        cout << "( "<< value << ", 2)" <<endl;
    }*/

    cout <<  "cantidad de puntos (contador interno): "<< contador << endl;
    cout <<  "cantidad de puntos no reconocidos: "<< contador2 << endl;
    cout <<  "cantidad de puntos (edge count): "<< externalAdjacencyMatrix.getEdgeCount()<< endl;
    //externalAdjacencyMatrix.printRange(0,0,n-1,n-1);
    Cronometer *crono = cCronometer();

    cout << "------------- EJECUTANDO RANGE QUERY 1. ALL WINDOW ------------- "<<endl;
    start_clock(crono);
    vector<pair<uint32_t, uint32_t>> rangePoints = externalAdjacencyMatrix.range(0,n-1,0,n-1);
    double stop = stop_clock(crono) * 1000000.0;

    std::cout << std::setprecision(2) << std::fixed;
    cout <<" Cantidad de puntos encontrados en la consulta range: " << rangePoints.size() << endl;
    cout << "Tiempo demorado por rangeQuery es de : "<< stop <<" microsegundos"<<endl;

    cout << "Verificando que los ptos de range query esten .."<< endl;
    int ctdor=0;
    for(const auto &pto:rangePoints){
        if(externalAdjacencyMatrix.isEdge(pto.first,pto.second)){
            ctdor++;
        }else{
            cout << "EL punto: ("<<pto.first<<","<<pto.second<<") no aparece, DEBIERA!!!"<<endl;
            contador2++;
        }
    }
    cout << "Se verificaron: "<< ctdor<< " puntos"<<endl;
    cout << "-------------------------------------------------------------------------" << endl;
    /*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**/
    cout << "------------- EJECUTANDO RANGE QUERY 2. ALL WINDOW ------------- "<<endl;
    start_clock(crono);
    vector<pair<uint32_t, uint32_t>> rangePoints2 = externalAdjacencyMatrix.range2(0,n-1,0,n-1);
    double stop2 = stop_clock(crono) * 1000000.0;

    std::cout << std::setprecision(2) << std::fixed;
    cout <<" Cantidad de puntos encontrados en la consulta range: " << rangePoints2.size() << endl;
    cout << "Tiempo demorado por rangeQuery es de : "<< stop2 <<" microsegundos"<<endl;

    cout << "Verificando que los ptos de range query esten .."<< endl;
    int ctdor2 = 0;
    for(const auto &pto:rangePoints2){
        if(externalAdjacencyMatrix.isEdge(pto.first,pto.second)){
            ctdor2++;
        }else{
          //  cout << "EL punto: ("<<pto.first<<","<<pto.second<<") no aparece, DEBIERA!!!"<<endl;
            contador2++;
        }
    }
    cout << "Se verificaron: "<< ctdor2<< " puntos"<<endl;

    externalAdjacencyMatrix.close();

}
void testMATADYALL(char *argv[]){
    //int n = 256;
    int n = atoi(argv[2]);
    // std::string dataset ="../../dataset/DH_1_256.txt";
    std::string dataset = argv[1];
    int cantPUNTOS=atoi(argv[3]);
    //int cantPUNTOS=55000;


    cout << " --------------------- TEST MATRIZ DE ADYACENCIA ALL BUFFERS---------------------"<<endl;
    cout << "BUILDING ...." << endl;
    Storage<uint64_t> NOCACHE("BIN_TESTS/ady-NOCAHE.bin", 10); //cada pagina de 6400 bits es decir 800 bytes. 1 pagina = 800 bytes.
    LRUCacheStorage<uint64_t> LRU("BIN_TESTS/ady-LRU.bin", 10,30); //
    LFUCacheStorage<uint64_t> LFU("BIN_TESTS/ady-LFU.bin", 10,30); //
    ExternalAdjacencyMatrix<uint64_t> ady_nocache(NOCACHE, n);
    ExternalAdjacencyMatrix<uint64_t> ady_lru(LRU, n);
    ExternalAdjacencyMatrix<uint64_t> ady_lfu(LFU, n);
    ady_nocache.create(dataset.c_str());
    ady_lru.create(dataset.c_str());
    ady_lfu.create(dataset.c_str());

    ady_nocache.open();
    ady_lru.open();
    ady_lfu.open();
    //*************************************************************************************************************//*
    cout << "**** TESTING EDGE COUNT ¿ESTAN TODOS LOS PUNTOS? ****";

   long long nocacheEDGE = ady_nocache.getEdgeCount();//strtoll(NOCACHE.getMeta("EDGE_COUNT").c_str(),NULL,10);
   long long lruEDGE = ady_lru.getEdgeCount();
   long long lfuEDGE = ady_lfu.getEdgeCount();

   if(cantPUNTOS == nocacheEDGE && cantPUNTOS==lruEDGE && cantPUNTOS==lfuEDGE){
       cout << BOLD << GREEN << " PASS " << RESET <<endl;
   }else{
       cout << BOLD << RED << " FAIL "<< RESET <<endl;
       cout << "NO CACHE COUNT: "<< nocacheEDGE <<"/ LFU: "<<lfuEDGE << "/ LRU: "<< lruEDGE;
       cout << BOLD << GREEN << " DEBIERA SER: "<< cantPUNTOS << RESET << endl;
       ady_nocache.close();
       ady_lfu.close();
       ady_lru.close();
       exit(1);
   }
    //*************************************************************************************************************//*
    cout << "**** TESTING RANGE COUNT ¿ESTAN TODOS LOS PUNTOS, SEGUN CONSULTA POR RANGO? ****";
    auto nodes = n;
    auto output1 = ady_nocache.range2(0, nodes - 1, 0, nodes - 1); //(0,0) AL (n-1,n-1)
    auto output2 = ady_lfu.range2(0, nodes - 1, 0, nodes - 1);
    auto output3 = ady_lru.range2(0, nodes- 1, 0, nodes - 1);
    if(cantPUNTOS == output1.size() && cantPUNTOS==output2.size() && cantPUNTOS==output3.size()){
        cout << BOLD << GREEN << " PASS " << RESET <<endl;
    }else{
        cout << BOLD << RED << " FAIL "<< RESET <<endl;
        cout << "NO CACHE COUNT: "<< output1.size() <<"/ LFU: "<<output2.size() << "/ LRU: "<<output3.size();
        cout << BOLD << GREEN << " DEBIERA SER: "<< cantPUNTOS << RESET << endl;
        ady_nocache.close();
        ady_lfu.close();
        ady_lru.close();
        exit(1);
    }
    //*************************************************************************************************************//*

    cout << "**** TESTING CHECKLINK ¿COINCIDE PUNTO A PUNTO? ****" ;

    bool pass = true;
    for (auto i = 0; i < n; i++) {
        for (auto j = 0; j < n; j++) {
            //   cout << "i = "<< i << " j = "<< j << " " ;
            pass = compareELements2(ady_nocache.isEdge(i, j), ady_lfu.isEdge(i, j), ady_lru.isEdge(i, j));
        }
    }

    if(pass){
        cout << BOLD << GREEN << " PASS " << RESET <<endl;
    }else{
        cout << BOLD << RED << " FAIL "<< RESET <<endl;
        ady_nocache.close();
        ady_lfu.close();
        ady_lru.close();
        exit(2);
    }
    //*************************************************************************************************************//*
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
        ady_nocache.close();
        ady_lfu.close();
        ady_lru.close();
    }

    ady_nocache.close();
    ady_lru.close();
    ady_lfu.close();

}