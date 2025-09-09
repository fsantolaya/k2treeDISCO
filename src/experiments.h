//
// Created by fernando santolaya-lenovo on 11-01-23.
//
#ifndef BITVECTOR_EXPERIMENTS_H
#define BITVECTOR_EXPERIMENTS_H

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "Storage.h"
#include "LRUCacheStorage.h"
#include "LFUCacheStorage.h"
#include "ExternalBitvect.h"
#include "ExternalK2Tree.h"
#include "ExternalAdjacencyMatrix.h"
#include "TimeMesure.c"
#include "MemoryMesure.c"
#include "utils.h"

#define MAX_VUELTAS 50
#define K2TRE_DISCO 1
#define MAT_ADY 0
const std::string BIN_DIR = "BIN/";
struct Punto {
    uint32_t x;
    uint32_t y;
};
struct Ventana {
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
};
unsigned long int getPeakMemory() {
    struct rusage memory;
    getrusage(RUSAGE_SELF, &memory);
    return memory.ru_maxrss;
}

unsigned long int getPageFaultWithoutIO() {
    struct rusage memory;
    getrusage(RUSAGE_SELF, &memory);
    return memory.ru_minflt;
}

unsigned long int getPageFaultWithIO() {
    struct rusage memory;
    getrusage(RUSAGE_SELF, &memory);
    return memory.ru_majflt;
}

unsigned long int getSwaped() {
    struct rusage memory;
    getrusage(RUSAGE_SELF, &memory);
    return memory.ru_nswap;
}

unsigned long int getReadFromDisk() {
    struct rusage memory;
    getrusage(RUSAGE_SELF, &memory);
    return memory.ru_inblock;
}

unsigned long int getWriteToDisk() {
    struct rusage memory;
    getrusage(RUSAGE_SELF, &memory);
    return memory.ru_oublock;
}

void printUsage() {

    cout << "Uso: ./<ejecutable> [parametros] [archivo]\n" << endl;

    cout << "Ejemplo de Construccion: ./Bitvector_run 2 2 0 0 ../../dataset/" << endl;
    cout << "Ejemplo de Consulta: ./Bitvector_run 2 2 1 5 ./adjMatrix_16x16.txt\n" << endl;
    cout << "Ejemplo de Consulta: ./Bitvector_run 1 0 0 5 ../../dataset/\n" << endl;

    cout << endl << "------------- Estructura de datos ----------------\n" << endl;
    cout << left << setw(30) << "Nombre" << setw(10) << "Número" << endl;
    cout << left << setw(30) << tipo_estructura << setw(10) << tipo_estructura_num << endl;
    cout << left << setw(30) << tipo_estructura2 << setw(10) << tipo_estructura_num2 << endl;
    cout << endl << "\n----------------- Parámetros -------------------\n" << endl;
    cout << "\nOperaciones disponibles: " << endl;
    cout << left << setw(30) << "Nombre" << setw(10) << "Número" << endl;
    cout << left << setw(30) << tipo_operacion << setw(10) << tipo_operacion_num << endl;
    cout << left << setw(30) << tipo_operacion2 << setw(10) << tipo_operacion_num2 << endl;
    cout << left << setw(30) << tipo_operacion3 << setw(10) << tipo_operacion_num3 << endl;
    cout << left << setw(30) << tipo_operacion4 << setw(10) << tipo_operacion_num4 << endl;
    cout << left << setw(30) << tipo_operacion5 << setw(10) << tipo_operacion_num5 << endl;
    cout << "\nTipos de cache disponibles: " << endl;
    cout << left << setw(30) << "Nombre" << setw(10) << "Número" << endl;
    cout << left << setw(30) << tipo_cache << setw(10) << tipo_cache_num << endl;
    cout << left << setw(30) << tipo_cache2 << setw(10) << tipo_cache_num2 << endl;
    cout << left << setw(30) << tipo_cache3 << setw(10) << tipo_cache_num3 << endl;
    cout
            << "\nTamaño de Pagina a usar: Porcentaje del tamaño a usar el cual se especifica en el argumento siguiente al tipo de cache.\nSolo especificar el numero. Si este es 0 se tomara como si se usara el tamaño de pagina completo"
            << endl;
    cout << "\nDirectorio de Archivos de Entrada: " << endl;
    cout << left << setw(30) << "Nombre" << setw(10) << "Descripción" << endl;
    cout << left << setw(30) << "Extension .txt" << setw(10)
         << "Archivo de texto con los datos para construir la estructura" << endl;
    cout << left << setw(30) << "Extension .bin" << setw(10) << "Archivo binario que contiene la estructura de datos"
         << endl;
}

void printHeader() {
    printf("%-12s|%-8s|"
           "%-12s|%-6s|%-15s|"
           "%-15s|%-10s|%-12s|"
           "%-17s|%-15s|%-15s|%-15s|"
           "%-18s|%-18s|%-15s|%-15s|%-15s|"
           "%-14s|%-12s|\n",
           "SET NAME", "CACHE",
           "POINTS (1'S)", "%DENSI", "P.BIN FILE(Kb)",
           "P.DATASET(Kb)", "P.SIZE(SO)", "WORD X PAGE",
           "BUILD TIME(micrs)", "PEAK MEMORY(Kb)", "CACHE HIT", "CACHE MISS",
           "PAGE FAULTS NO-IO", "PAGE FAULTS IO", "SWAPPED", "READ", "WRITE",
           "CACHE HIT RATE", "ALGORITMO");
}

void printHeaderConsultas() {
    printf("%-12s|%-10s|"
           "%-8s|%-10s|%-12s|"
           "%17s|%16s|"
           "%-15s|%-15s|%-15s|"
           "%-18s|%-18s|%-15s|%-15s|%-15s|"
           "%-15s|%-15s|"
           "%-12s|\n",
           "SET NAME", "QUERY NAME",
           "CACHE", "P.SIZE(SO)", "CANT PAGES ",
           "QUERY TIME(micrs)", "PEAK MEMORY2(Kb)",
           "CACHE HIT", "CACHE MISS", "CACHE HIT RATE",
           "PAGE FAULTS NO-IO", "PAGE FAULTS IO", "SWAPPED", "READ", "WRITE",
           "P.BIN FILE(Kb)", "P.DATASET(Kb)",
           "ALGORITMO");
}

void printStats(Stats *stats) {
    printf("%-12s|%-8s|"
           "%12d|%5.01f%%|%15.0f|"
           "%15.0f|%10.0f|%12.0f|"
           "%17.0f|%15.0ld|%15.02f|%15.02f|"
           "%18ld|%18ld|%15ld|%15ld|%15ld|"
           "%14.02f|%-12s|",
           stats->dataset.c_str(), stats->cacheName.c_str(),
           stats->points, stats->density, stats->binFileSpace,
           stats->datasetSpace, stats->pagesize, stats->pageSizeUsed,
           stats->buildTime, stats->peakMemory, stats->cacheHit, stats->cacheMiss,
           stats->pageFaultsWithoutIO, stats->pageFaultWithIO, stats->swaped, stats->readFromDisk, stats->writeToDisk,
           stats->hitRatio, stats->algoritmo.c_str());
    printf("\n");
}

void printStatsConsultas(StatsForQuerys &stats) {
    printf(
            "%-12s|%-10s|"
            "%-8s|%10.0f|%'12.0f|"
            "%'17.02f|%'16.0ld|"
            "%'15.02lf|%'15.02lf|%'15.02lf|"
            "%'18ld|%'18ld|%'15ld|%'15ld|%'15ld|"
            "%15.0f|%15.0f|"
            "%12s|",
            stats.dataset.c_str(), stats.queryName.c_str(),
            stats.cacheName.c_str(), stats.pagesize, stats.pageSizeUsed,
            stats.queryTime, stats.peakMemory,
            stats.cacheHit, stats.cacheMiss, stats.hitRatio,
            stats.pageFaultsWithoutIO, stats.pageFaultWithIO, stats.swaped, stats.readFromDisk, stats.writeToDisk,
            stats.binFileSpace, stats.datasetSpace,
            stats.algoritmo.c_str());
    printf("\n");
}

std::vector<Punto> llenaQueryPoints(char *file){
    ifstream input;
    uint32_t a, b;
    std::vector<Punto> puntos;

    input.open(file);
    if (!input.is_open()) {
        cout << "ERROR: THE FILE " << file << " DOES NOT EXIST" << endl;
        exit(211);
    }
    int i = 0;
    while (input >> a >> b) {
      // cout << "consulta : "<< a<<" /" << b<<endl;
        Punto nuevoPunto;
        nuevoPunto.x = a;
        nuevoPunto.y = b;
        puntos.push_back(nuevoPunto);
    }
    input.close();

    return puntos;
}
std::vector<Ventana> llenaRangeQuerys(char *file){
    ifstream input;
    uint32_t a, b, c, d;
    std::vector<Ventana> ventanas;
    input.open(file);
    if (!input.is_open()) {
        cout << "ERROR: THE FILE " << file << " DOES NOT EXIST" << endl;
        exit(211);
    }
    int i = 0;
    while (input >> a >> b >> c >> d) {
        Ventana nuevaVentana;
        nuevaVentana.a = a;
        nuevaVentana.b = b;
        nuevaVentana.c = c;
        nuevaVentana.d = d;
        ventanas.push_back(nuevaVentana);
    }
    input.close();

    return ventanas;
}

void executeQuery(char* warmupFIle, char *file, StatsForQuerys &stats, int query, Storage<uint64_t> *storage, int tipoEstructura, int n, int K) {
    double stop = 0;
    //levantar la estructura. no construir de nuevo.
    /*************************** LEVANTO LA ESTRUCTURA  ***************************/

    ExternalAdjacencyMatrix<uint64_t> *externalAdjacencyMatrix = 0;//((*storage), n, n);
    ExternalK2Tree<uint64_t> *externalK2Tree = 0;//(2, *storage);
    storage->open();
    if (tipoEstructura == 2) {
        externalAdjacencyMatrix = new ExternalAdjacencyMatrix<uint64_t>((*storage), n);
        externalAdjacencyMatrix->open();
    } else {
        externalK2Tree = new ExternalK2Tree<uint64_t>(K, *storage);
        externalK2Tree->open();
    }
    /*************************** WARM UP ***************************/
    std::vector<Punto> puntos = llenaQueryPoints(warmupFIle);
   for (auto i = puntos.begin(); i != puntos.end(); ++i) {
      if (tipoEstructura == 2) {
           externalAdjacencyMatrix->isEdge(i->x, i->y);
      } else {
        bool val = externalK2Tree->checkLink(i->x, i->y);
       }
  }
    /*************************** fin WARM up  ***************************/
    storage->resetCacheStat();
    Cronometer *crono = cCronometer();
    int conta = 0;

    switch (query) {
        case 1: {//checklink
            stats.queryName = "checklink";
            std::vector<Punto> puntos = llenaQueryPoints(file);
            start_clock(crono);
            for (auto i = puntos.begin(); i != puntos.end(); ++i) {
                if (tipoEstructura == 2) {
                    externalAdjacencyMatrix->isEdge(i->x, i->y);
                } else {
                    externalK2Tree->checkLink(i->x, i->y);
                }
                conta++;
            }
            stop = stop_clock(crono) * 1000000.0;
            break;
        }
        case 2: {//direct
            stats.queryName = "direct";
            std::vector<Punto> puntos = llenaQueryPoints(file);
            start_clock(crono);
            for (auto i = puntos.begin(); i != puntos.end(); ++i) {
                if (tipoEstructura == 2) {
                    externalAdjacencyMatrix->direct(i->x);
                    conta++;
                } else {
                    externalK2Tree->direct(i->x);
                }
                conta++;
            }
            stop = stop_clock(crono) * 1000000.0;
            break;
        }
        case 3: {//reverse
            stats.queryName = "reverse";
            std::vector<Punto> puntos = llenaQueryPoints(file);
            start_clock(crono);
            for (auto i = puntos.begin(); i != puntos.end(); ++i) {
                if (tipoEstructura == 2) {
                    externalAdjacencyMatrix->reverse(i->y);
                } else {
                    externalK2Tree->reverse(i->y);
                }
                conta++;
            }
            stop = stop_clock(crono) * 1000000.0;
            break;
        }
        case 4: {//range
            std::vector<Ventana> ventanas = llenaRangeQuerys(file);
            start_clock(crono);
            for (auto i = ventanas.begin(); i != ventanas.end(); ++i) {
                if (tipoEstructura == 2) {
                    externalAdjacencyMatrix->range(i->a, i->c, i->b, i->d);
                } else {
                    externalK2Tree->range(i->a, i->c, i->b, i->d);
                }
                conta++;
            }
            stop = stop_clock(crono) * 1000000.0;
            std::string filex(file);
            string fileaux = removeExtension(filex);
            //pensando que el archivo viene como eje: queryWindows_8192_10.txt
            stats.queryName = "range" + fileaux.substr(fileaux.find_last_of("_") + 1);
            break;
        }
    }
    stats.queryTime = (double) stop / conta;
    stats.cacheMiss = (double) storage->getCacheMissed() / conta;
    stats.cacheHit = (double) storage->getCacheHit() / conta;
    stats.hitRatio = storage->hitRatio();//(float) stats.cacheHit / ((float) stats.cacheHit + (float) stats.cacheMiss);

    struct rusage memory;
    getrusage(RUSAGE_SELF, &memory);
    stats.peakMemory = memory.ru_maxrss; //memoria usada ; in kilobytes
    stats.pageFaultsWithoutIO = memory.ru_minflt;
    stats.pageFaultWithIO = memory.ru_majflt;
    stats.swaped = memory.ru_nswap;
    stats.readFromDisk = memory.ru_inblock;
    stats.writeToDisk = memory.ru_oublock;

    stats.algoritmo = tipoEstructura == 2 ? "MAY-ADY" : "K2TREE-DISCO";
    printStatsConsultas(stats);

    storage->resetCacheStat();
    if (tipoEstructura == 2) {
        externalAdjacencyMatrix->close();
    } else {
        externalK2Tree->close();
    }
    if (externalAdjacencyMatrix) {
        delete externalAdjacencyMatrix;
    }
    if (externalK2Tree) {
        delete externalK2Tree;
    }
}

void buildStructure(char *filename, Storage<uint64_t> *storage, int n, Stats &stats, int tipoEstructura) {
    double stop = 0;
    unsigned long long points, matrixSize;
    Cronometer *crono = cCronometer();
    /******************* MIDIENDO  ******************/
    if (tipoEstructura == 2) {
        ExternalAdjacencyMatrix<uint64_t> externalAdjacencyMatrix((*storage), n);
        start_clock(crono);
        externalAdjacencyMatrix.create(filename);
        stop = stop_clock(crono) * 1000000.0;
        externalAdjacencyMatrix.close();
        points = stoull(storage->getMeta("EDGE_COUNT")); //cantidad de puntos. funca bien.
        matrixSize = sqrt(stoull(storage->getMeta("MATRIX_SIZE")));
    } else {
        ExternalK2Tree<uint64_t> externalK2Tree(2, *storage);
        start_clock(crono);
        externalK2Tree.create(filename);
        stop = stop_clock(crono) * 1000000.0;
        externalK2Tree.close();
        points = stoull(storage->getMeta("K2tree_EDGES")); //cantidad de puntos cargados..
        matrixSize = stoull(storage->getMeta("K2Tree_MATRIX_SIZE"));
    }

    string msg = (tipoEstructura == 0) ? "Matriz de adyacencia" : "K2tree Disco";

    stats.buildTime = stop;
    stats.peakMemory = getPeakMemory();
    stats.points = points;
    stats.density = ((double) points / ((double) matrixSize * (double) matrixSize)) * 100;
    stats.hitRatio = storage->hitRatio();
    stats.cacheMiss = storage->getCacheMissed();
    stats.cacheHit = storage->getCacheHit();
    stats.pageFaultsWithoutIO = getPageFaultWithoutIO();
    stats.pageFaultWithIO = getPageFaultWithIO();
    stats.swaped = getSwaped();
    stats.readFromDisk = getReadFromDisk();
    stats.writeToDisk = getWriteToDisk();
    stats.algoritmo = tipoEstructura == 2 ? "MAY-ADY" : "K2TREE-DISCO";

}

void reads(const char *fileName, uint32_t &max, vector<uint32_t> &X, vector<uint32_t> &Y) {
    ifstream input;

    input.open(fileName);
    if (!input.is_open()) {
        cerr << "No se pudo abrir el archivo: " << fileName << std::endl;
        return;
    }
    uint32_t a, b;
    max = 0;
    while (input >> a >> b) {
        if (a > max) {
            max = a;
        }
        if (b > max) {
            max = b;
        }
        X.push_back(a);
        Y.push_back(b);

    }
    input.close();
}

void experimentoConstruccion(char *argv[], int op, int tipoEstructura) {
    //Cantidad de bloques de 64 bits que se necesitan para representar la matriz de adyacencia de orden n
    double pagePercents;
    int n = getN(argv[5]); //calcula tam matriz. esta en el nombre del archivo. ej: DH_1_8192.txt -> 8192 tamanio matriz
    Stats stats; //para guardar el resultado de los experimentos.
    if (stoi(argv[4]) == 0) {
        pagePercents = 1;
    }
    std::string K = argv[6];
    pagePercents = stoi(argv[4]) / 100.0;
    int pageSize = sysconf(_SC_PAGE_SIZE); //lo devuelve en bytes.
    int D = ceil(((pageSize * 8.0) / 64.0) * pagePercents);

    std::string datasetName = removeExtension(basename(argv[5]));

    string filename;
    Storage<uint64_t> *storage = 0;

    if (tipoEstructura == 2) {
        filename = BIN_DIR + "adjMatrix_" + datasetName + "_" + K + ".bin";
    } else {
        filename = BIN_DIR + "extK2tree_" + datasetName + "_" + K + ".bin";
    }

    //elegimos el tipo de caché
    if (op == 0) { // NO CACHE
        //filename = "adjMatrix_" + datasetName + ".bin";
        storage = new Storage<uint64_t>(filename, D);
        stats.cacheName = "NO CACHE";
    }
    if (op == 1) { //LFU
        //filename = "adjMatrix_" + datasetName + "LFU" + ".bin";
        storage = new LFUCacheStorage<uint64_t>(filename, D, 50);
        //LFUCacheStorage<uint64_t> storage(filename, D, 50);
        stats.cacheName = "LFU";
    }
    if (op == 2) { //LRU
        //filename = "adjMatrix_" + datasetName + "LRU" + ".bin";
        storage = new LRUCacheStorage<uint64_t>(filename, D, 50);
        stats.cacheName = "LRU";
    }
    storage->resetCacheStat();
    //ejecutemos el experimento.
    buildStructure(argv[5], storage, n, stats, tipoEstructura);

    //usando stat para ver si el archivo ya existe
    //struct stat buffer;
    //get size of dataset in bytes
    struct stat stat_buf;
    stat(filename.c_str(), &stat_buf);

    struct stat dataset;
    stat(argv[5], &dataset);
    stats.dataset = datasetName;
    stats.pagesize = pageSize; // viene en bytes
    stats.pageSizeUsed = D;
    stats.binFileSpace = convertByteToKb(stat_buf.st_size);
    stats.datasetSpace = convertByteToKb(dataset.st_size);
    printStats(&stats);
    storage->resetCacheStat();

    if (storage) {
        delete storage;
    }
}

void experimentoConstruccion2(char *argv[], int op, int tipoEstructura) {
    double pagePercents;
    uint32_t max = 0;
    //1°) lee los puntos (x,y) y convierte a Z
    vector<uint32_t> X;
    vector<uint32_t> Y;
    reads(argv[5], max, X, Y);

    int n = getN(argv[5]); //calcula tam matriz. esta en el nombre del archivo. ej: DH_1_8192.txt -> 8192 tamanio matriz
    std::string K = argv[6];
    if (stoi(argv[4]) == 0) {
        pagePercents = 1;
    }
    pagePercents = stoi(argv[4]) / 100.0;
    int pageSize = sysconf(_SC_PAGE_SIZE); //lo devuelve en bytes.
    int D = ceil(((pageSize * 8.0) / 64.0) * pagePercents);

    std::string datasetName = removeExtension(basename(argv[5]));
    string filename = BIN_DIR + "extK2tree_" + datasetName + "_" + K + ".bin";
    Storage<uint64_t> stor(filename, D);
    ExternalBitvector<uint64_t> pruebaVector(stor);
    stor.setMeta("K2tree_K", K);
    stor.setMeta("K2Tree_MATRIX_SIZE", to_string(n));
    stor.setMeta("K2tree_NODES", to_string(max + 1));
    stor.setMeta("K2tree_EDGES", to_string(X.size()));
    pruebaVector.create();

    int i = 0;
    Page<uint64_t> page(stor.getD());
    page.createBlock(true);

    for (std::string line; std::getline(std::cin, line);) {
        std::size_t pos;
        int value = std::stol(line, &pos);

        if (line[pos] == '\0') {
            if (i % stor.bitsPerPage() == 0 && i != 0) {
                pruebaVector.appendPage(page, stor.bitsPerPage());
                page.createBlock(true); //la limpio..
            }
            if (value == 1) {
                BitBasic::BitSet(page.data(), i % stor.bitsPerPage(), stor.bitsPerWord());
            }
            i++;
        }
    }

    pruebaVector.appendPage(page, i % stor.bitsPerPage());
    pruebaVector.buildRank();
    stor.close();

}

void experimentoQuerys2(char *argv[], int opBuffer, int tipoEstructura, int operation) {
    //Cantidad de bloques de 64 bits que se necesitan para representar la matriz de adyacencia de orden n
    double pagePercents;
    StatsForQuerys stats; //para guardar el resultado de los experimentos.
    int n = getN(argv[5]); //calcula tam matriz. esta en el nombre del archivo. ej: DH_1_8192.txt -> 8192 tamanio matriz
    std::string K = argv[7];
    int pagesBuffer = atoi(argv[8]); //cuantas paginas para el buffer..

    if (stoi(argv[4]) == 0) {
        pagePercents = 1;
    }
    pagePercents = stoi(argv[4]) / 100.0;
    int pageSize = sysconf(_SC_PAGE_SIZE) ; //lo devuelve en bytes.
    int D = ceil(((pageSize * 8.0) / 64.0) * pagePercents); // param D: cantidad de palabras que tiene cada página.

    std::string datasetName = removeExtension(basename(argv[5]));
    string storageFileName;
    Storage<uint64_t> *storage = 0;
    //necesitamos definir cual es el archivo...y donde està.
    if (tipoEstructura == 2) {
        storageFileName = BIN_DIR + "adjMatrix_" + datasetName + "_" + K + ".bin";
    } else {
        storageFileName = BIN_DIR + "extK2tree_" + datasetName + "_" + K + ".bin";
    }

    //definimos el storage, segùn el cache.
    if (opBuffer == 0) { // NO CACHE
        storage = new Storage<uint64_t>(storageFileName, D);
        stats.cacheName = "NO CACHE";
    }
    if (opBuffer == 1) { //LFU
        storage = new LFUCacheStorage<uint64_t>(storageFileName, D, pagesBuffer);
        stats.cacheName = "LFU";
    }
    if (opBuffer == 2) { //LRU
        storage = new LRUCacheStorage<uint64_t>(storageFileName, D, pagesBuffer);
        stats.cacheName = "LRU";
    }
    storage->resetCacheStat();

    //seteamos el struct de los resultados.
    stats.dataset = datasetName + "_" + K;
    stats.pagesize = pageSize * pagePercents ; // en bytes
    //stats.pageSizeUsed = D;
    stats.pageSizeUsed = pagesBuffer;
    stats.binFileSpace = getFileSize(storageFileName);//convertByteToKb(binFile.st_size);
    stats.datasetSpace = getFileSize(argv[5]);//convertByteToKb(dataset.st_size);

    executeQuery(argv[9],argv[6], stats, operation, storage, tipoEstructura, n, atoi(K.c_str()));
    fflush(stdout);

    if (storage) {
        delete storage;
    }
}


int doExperiment(int argc, char *argv[]) {
    char *homedir = getenv("HOME");

    if (argc < 2) {
        printUsage();
        return 0;
    }
    int tipoEstructura = atoi(argv[1]); //que estructura ocupara? k2tree disco o mat-ady? 1 para k2tree, 2 para mat ady
    int operation = atoi(argv[2]); // construccion es 0, 1,2,3,4  para querys, 5 para construir con bitmap pasado por simongog
    int tipoBuffer = atoi(argv[3]); // 0 sin buffer, 1 con LFU, 2 CON LRU

    switch (operation) {
        case 0: { //build
            strcat(homedir, "/RESULTADOS/resultBuild.txt");
            if (checkFileExist(homedir) == 1 && fileIsEmpty(homedir) == 1) {
                printHeader();
                fflush(stdout);
            }
            experimentoConstruccion(argv, tipoBuffer, tipoEstructura);
            fflush(stdout);
            break;
        }
        case 1: {//querys
            strcat(homedir, "/RESULTADOS/resultQuery.txt");
            if (checkFileExist(homedir) == 1 && fileIsEmpty(homedir) == 1) {
                printHeaderConsultas();
                fflush(stdout);
            }
            experimentoQuerys2(argv, tipoBuffer, tipoEstructura, operation);
            fflush(stdout);
            break;
        }
        case 2: {//querys
            strcat(homedir, "/RESULTADOS/resultQuery.txt");
            if (checkFileExist(homedir) == 1 && fileIsEmpty(homedir) == 1) {
                printHeaderConsultas();
                fflush(stdout);
            }
            experimentoQuerys2(argv, tipoBuffer, tipoEstructura, operation);
            fflush(stdout);
            break;
        }
        case 3: { //querys
            strcat(homedir, "/RESULTADOS/resultQuery.txt");
            if (checkFileExist(homedir) == 1 && fileIsEmpty(homedir) == 1) {
                printHeaderConsultas();
                fflush(stdout);
            }
            experimentoQuerys2(argv, tipoBuffer, tipoEstructura, operation);
            fflush(stdout);
            break;
        }
        case 4: {//querys
            strcat(homedir, "/RESULTADOS/resultQuery.txt");
            if (checkFileExist(homedir) == 1 && fileIsEmpty(homedir) == 1) {
                printHeaderConsultas();
                fflush(stdout);
            }
            experimentoQuerys2(argv, tipoBuffer, tipoEstructura, operation);
            fflush(stdout);
            break;
        }
        case 5: { //build con SIMONG GOG bitmap
            experimentoConstruccion2(argv, tipoBuffer, tipoEstructura);
            break;
        }
        default:
            cout << "Tipo de operación elegida no existe..." << endl;
    }

    return -1;
}

#endif //BITVECTOR_EXPERIMENTS_H
