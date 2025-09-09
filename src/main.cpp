/* defines FILENAME_MAX */
// #define WINDOWS  /* uncomment this line to use it for windows.*/

#include <iostream>
#include "Storage.h"
#include "experiments.h"
#include "testsAdayacencia.h"
#include "testsKtree.h"

using namespace std;

void convertXYtoMatAdyAndPrint(int argc, char *argv[]){
    string filename;
    double pagePercents;
    pagePercents = 100 / 100.0;
    int pageSize = sysconf(_SC_PAGE_SIZE); //lo devuelve en bytes.
    int D = ceil(((pageSize * 8.0) / 64.0) * pagePercents);
    filename = argv[1];
   // LFUCacheStorage<uint64_t> storage ("adjMatrix.bin", D, 50);
    Storage<uint64_t> storage ("adjMatrix.bin", D);

    int n = atoi(argv[2]);
    ExternalAdjacencyMatrix<uint64_t> externalAdjacencyMatrix(storage, n);
    externalAdjacencyMatrix.create(filename.c_str());
    externalAdjacencyMatrix.open();
    size_t lastindex = filename.find_last_of(".");
    string rawname = filename.substr(0, lastindex);
    //vector<pair<uint32_t, uint32_t>> rangePoints = externalAdjacencyMatrix.range2(0,n-1,0,n-1);
   // cout << "tamaño del vector is. "<<rangePoints.size()<<endl;
    externalAdjacencyMatrix.printMatrixClean(rawname);
    externalAdjacencyMatrix.close();

}

void readBitMapFromSimonGog(){
    Storage<uint64_t> stor("TestingGOG.bin", 10);
    ExternalBitvector<uint64_t> pruebaVector(stor);
    pruebaVector.create();
    int i = 0;
    Page<uint64_t> page(stor.getD());
    page.createBlock(true);
    int unos = 0;
    //int ceros = 0;

    for (std::string line; std::getline(std::cin, line);) {
        std::size_t pos;
        int value = std::stol(line, &pos);

        if (line[pos] == '\0') {
            /* The whole line was a valid number */
            if(i % stor.bitsPerPage() == 0 && i!=0){
                pruebaVector.appendPage(page,stor.bitsPerPage());
                page.createBlock(true); //la limpio..
            }
            if(value == 1) {
             //   unos++;
                BitBasic::BitSet(page.data(), i % stor.bitsPerPage(), stor.bitsPerWord());
            }/*else{
                ceros++;
            }*/
            i++;
        }
    }

    pruebaVector.appendPage(page,i % stor.bitsPerPage());
    pruebaVector.buildRank();
    stor.close();
}

int main(int argc, char *argv[]) {
    doExperiment(argc, argv);
    // readBitMapFromSimonGog();
    // testK2treeGOG();
   //  convertXYtoMatAdyAndPrint(argc,argv);
    // testExternalK2treeSinCache();
    // testExternalK2treeLRU();
    // test2ExternalK2treeLFU();
    // testExternalK2treeALL();
    // test2ExternalK2treeLRU();
    // test2BigExternalK2treeSinCache();
    // test2BigExternalK2treeLRU();
    // test2BigExternalK2treeLFU();
    // test_matrizAdyacencia(argv);
    // testMATADYALL(argv);
    // testK2TREEall(argv);
    // testK2TREEall2(argv);
    // test_ExternalBitVector();
}