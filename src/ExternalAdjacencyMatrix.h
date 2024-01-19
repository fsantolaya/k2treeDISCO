//
// Created by Angelo on 07-09-22.
//

#ifndef BITVECTOR_EXTERNALADJACENCYMATRIX_H
#define BITVECTOR_EXTERNALADJACENCYMATRIX_H


#include "ExternalBitvect.h"
#include "Storage.h"
#include <vector>
#include <utils.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace BitBasic;

template<typename uint_t>
class ExternalAdjacencyMatrix {
public:
    ExternalAdjacencyMatrix(Storage<uint_t> &storage, uint_t n);

    virtual ~ExternalAdjacencyMatrix();

    void create(const char *fileName);

    void addEdge(uint_t u, uint_t v);

    void removeEdge(uint_t u, uint_t v);

    bool isEdge(uint_t u, uint_t v);

    void print();

    void printMatrixClean(string fileName);

    void close();

    void open();

    vector<int> reverse(uint_t column);

    vector<int> direct(uint_t row);

    void printDirectNeighbors(uint_t x, uint_t y);

    void printDirectsNeighborsMatrix(uint_t x, uint_t y);

    void printReversesNeighborsMatrix(uint_t x, uint_t y);

    void printReverseNeighbors(uint_t x, uint_t y);

    vector<pair<uint32_t, uint32_t>> range(uint_t ix, uint_t jx, uint_t iy, uint_t jy);
    vector<pair<uint32_t, uint32_t>> range2(uint_t ix, uint_t jx, uint_t iy, uint_t jy);

    void printRange(uint_t ix, uint_t jx, uint_t iy, uint_t jy);

    void printRangeMatrix(uint_t ix, uint_t jx, uint_t iy, uint_t jy);

    long long getEdgeCount();

    void printTL(int num);

    void buildRank();

private:
    unsigned long long n;
    unsigned long long matrix_size;
    unsigned long long edges;
    Storage<uint_t> &storage;
    bool get(uint_t i, uint_t j);
    ExternalBitvector<uint_t> T_L;
};
//holi
template<typename uint_t>
ExternalAdjacencyMatrix<uint_t>::ExternalAdjacencyMatrix(Storage<uint_t> &storage, uint_t n):
storage(storage), n(n), T_L(ExternalBitvector<uint_t>(storage)) {
    matrix_size = n * n;
    edges = 0;
    storage.setMeta("MATRIX_SIZE", to_string(matrix_size));
    storage.setMeta("N", to_string(n));
}

template<typename uint_t>
ExternalAdjacencyMatrix<uint_t>::~ExternalAdjacencyMatrix() {
    storage.setMeta("EDGE_COUNT", to_string(edges));
    storage.setMeta("CacheHit", to_string(storage.getCacheHit()));
    storage.setMeta("CacheMiss", to_string(storage.getCacheMissed()));
    storage.setMeta("HitRate", to_string(storage.hitRatio()));
}

template<typename uint_t>
vector<pair<uint32_t, uint32_t>> ExternalAdjacencyMatrix<uint_t>::range(uint_t ix, uint_t jx, uint_t iy, uint_t jy) {
    vector<pair<uint32_t, uint32_t>> result;
    if (ix > n || jx > n || iy > n || jy > n) {
        cout << "ERROR: THE QUERY POINT IS OUT OF THE MATRIX" << endl;
        return result;
    }
    for (int i = ix; i <= jx; i++) {
        for (int j = iy; j <= jy; j++) {
            if (this->isEdge(i, j)) {
                result.push_back(make_pair(i, j));
            }
        }
    }
    return result;
}

template<typename uint_t>
vector<pair<uint32_t, uint32_t>> ExternalAdjacencyMatrix<uint_t>::range2(uint_t ix, uint_t jx, uint_t iy, uint_t jy) {
    vector<pair<uint32_t, uint32_t>> result;
    if (ix > n || jx > n || iy > n || jy > n) {
        cout << "ERROR: THE QUERY POINT IS OUT OF THE MATRIX" << endl;
        return result;
    }
    /*  iy          jy
     *   0  1  2  3  4  5 ix
     *   6  7  8  9 10 11
     *  12 13 14 15 16 17
     *  18 19 20 20 21 22 jx
     *
     */
    vector<int> rango;
    int j;
   for (int i = ix; i <= jx; i++) {
        //rango en bitmap:[i*m+jx,i*m+jy]
    //   cout << "rango is:"<< i*m+iy<<","<<i*m+jy <<endl;
       rango = this->T_L.range(i*n+iy,i*n+jy);
        for (const auto &z: rango) {
            j = z % n;
            result.push_back(make_pair(i, j));
        }
    }
    return result;
}

/**
 * Get the direct neighbors of a node, i.e. the nodes that are in the same row as the node(i,j)
 * in the delimited matrix
 * @param i the node
 * @param j the column offset
 * @return a vector of  the form (column offset), -1 if the query point not exist.
 */
template<typename uint_t>
vector<int> ExternalAdjacencyMatrix<uint_t>::direct(uint_t row) {
    vector<int> neighbors;
    for (int k = 0; k < n; ++k) {
        if (this->isEdge(row, k)) {
            neighbors.push_back(k);
        }
    }
    return neighbors;
}

/**
 * Get the reverse neighbors of a node, i.e. the nodes that are in the same column as the node(i,j)
 * in the delimited matrix and not include the node itself (i,j)
 * @param i the node
 * @param j the column offset
 * @return a vector of pairs of the form ( fil offset), -1 if the query point not exist.
 */
template<typename uint_t>
vector<int> ExternalAdjacencyMatrix<uint_t>::reverse(uint_t column) {
    vector<int> neighbors;
    for (int k = 0; k < n; ++k) {
        if (this->isEdge(k, column)) {
            neighbors.push_back(k);
        }
    }
    return neighbors;
}

template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::buildRank() {
    T_L.buildRank();
}

template<typename uint_t>
long long ExternalAdjacencyMatrix<uint_t>::getEdgeCount(){
    return this->edges;
}

template<typename uint_t>
bool ExternalAdjacencyMatrix<uint_t>::get(uint_t i, uint_t j) {
    return T_L.access(i * n + j);
}

/**
 * This method create the storage file and initialize the bitvector
 * the storage file will be created with blank pages
 * @tparam uint_t
 * @param fileName
 */
template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::create(const char *fileName) {
    T_L.create(matrix_size); //pide espacio en disco.. n paginas.
    ifstream input;
    input.open(fileName);
    if (!input.is_open()) {
        cout << "ERROR: THE FILE " << fileName << " DOES NOT EXIST" << endl;
        exit(1);
    }
    uint32_t a, b;
    while (input >> a >> b) {
        this->edges++; //que pasa con los puntos repetidos??... ojo con esto..
        addEdge(a, b);
        //   printRange(0,0,n-1,n-1); //for debug...
    }
    storage.setMeta("EDGE_COUNT", to_string(this->edges));
    input.close();
    close();
}

template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::addEdge(uint_t u, uint_t v) {
    T_L.bitSet(u * n + v);
}

template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::removeEdge(uint_t u, uint_t v) {
    T_L.bitClean(u * n + v);
}

template<typename uint_t>
bool ExternalAdjacencyMatrix<uint_t>::isEdge(uint_t u, uint_t v) {
    return get(u, v);
}

template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::close() {
    storage.setMeta("EDGE_COUNT", to_string(edges));
    storage.setMeta("CacheHit", to_string(storage.getCacheHit()));
    storage.setMeta("CacheMiss", to_string(storage.getCacheMissed()));
    storage.setMeta("HitRate", to_string(storage.hitRatio()));
    storage.close();
}

template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::open() {
    T_L.open();
    matrix_size = stoull(storage.getMeta("MATRIX_SIZE"));
    edges = stoull(storage.getMeta("EDGE_COUNT"));
    n = stoull(storage.getMeta("N"));
}

/**************************************************************
 * ************************************************************
 * ************************************************************
 * ************************************************************
 * ************************************************************
 * ************************************************************
 * ************************************************************
 * ************************************************************
 */

/**
* Print all matrix .
*/
template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::print() {
    //Display the 1 in the matrix as dots and the 0
    // Se es desplegada la matriz en consola(fila/columna)

    cout << "BitMap: " << endl;
    T_L.print();

    cout << "i\t";
    for (int i = 0; i < n; i++) {
        cout << "\t" << BOLD << YELLOW << UNDERLINE << i << RESET;
    }
    cout << endl;
    for (int i = 0; i < n; i++) {
        cout << "\t" << BOLD << YELLOW << i << "|" << RESET;
        for (int j = 0; j < n; j++) {
            if (get(i, j)) {
                cout << "\t" << GREEN << "1" << RESET;
            } else {
                cout << "\t0";
            }
        }
        cout << endl;
    }
}

template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::printTL(int num) {
    T_L.print(num);
}

template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::printMatrixClean(string fileName) {
    // Create and open a text file
    string finalName=fileName+".map";
    ofstream MyFile(finalName);
    //int contadorunos=0;
    //int contadorceros=0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (get(i, j)) {
                MyFile << "1 ";
               // contadorunos++;
            } else {
               // contadorceros++;
                MyFile << "0 ";
            }
        }
    }
    //MyFile<<"unos: "<<contadorunos<<endl;
    //MyFile<<"ceros: "<<contadorceros<<endl;
    // Close the file
    MyFile.close();
}

template<typename uint_t>
/**
 * print adyacency matrix, only with direct neighbors from query point
 * @param x, int value fil
 * @param y, int value col
 * */
void ExternalAdjacencyMatrix<uint_t>::printDirectsNeighborsMatrix(uint_t x, uint_t y) {

    vector<int> neighbors = direct(x, y);
    if (neighbors.empty()) { //exite el punto que estan preguntando???
        printf("no existen vecinos directos para este query point [%d,%d]\n", x, y);
        return;
    }

    this->printDirectNeighbors(x, y);

    cout << endl;
    int index = 0;
    cout << "i\t";
    for (int i = 0; i < n; i++) {
        cout << "\t" << BOLD << YELLOW << UNDERLINE << i << RESET;
    }
    cout << endl;
    for (int i = 0; i < n; i++) {
        cout << "\t" << BOLD << YELLOW << i << "|" << RESET;
        for (int j = 0; j < n; j++) {
            if (get(i, j)) {
                if (i == x && neighbors[index] == j && neighbors.size() != index) {
                    cout << "\t" << BOLD << RED << "1" << RESET;
                    index++;
                } else {
                    if (i == x && j == y) { //es el query point el que voy a mostrar?
                        cout << "\t" << GREEN << BOLD << UNO << RESET;
                    } else {
                        cout << "\t" << GREEN << "1" << RESET;
                    }
                }
            } else {
                cout << "\t0";
            }
        }
        cout << endl;
    }
    cout << BOLD << "SIMBOLOGY" << RESET << endl;
    cout << UNO << " MEANS: THE QUERY POINT" << endl;
    cout << "RED ONES: THE ONE'S OF QUERY RESULT" << endl;
    cout << "GREEN ONES: THE ONE'S OF THE MATRIX" << endl;
}

/**
 * print adyacency matrix, only with reverse neighbors from query point
 * @param x, int value fil
 * @param y, int value col
 * */
template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::printReversesNeighborsMatrix(uint_t x, uint_t y) {

    vector<int> neighbors = reverse(x, y);

    if (neighbors.empty()) { //exite el punto que estan preguntando???
        printf("no existen vecinos directos para este query point [%d,%d]\n", x, y);
        return;
    }
    this->printReverseNeighbors(x, y);
    cout << endl;
    int index = 0;
    cout << "i\t";
    for (int i = 0; i < n; i++) {
        cout << "\t" << BOLD << YELLOW << UNDERLINE << i << RESET;
    }
    cout << endl;
    for (int i = 0; i < n; i++) {
        cout << "\t" << BOLD << YELLOW << i << "|" << RESET;
        for (int j = 0; j < n; j++) {
            if (get(i, j)) {
                if (j == x && neighbors[index] == i && neighbors.size() != index) {
                    cout << "\t" << BOLD << RED << "1" << RESET;
                    index++;
                } else {
                    if (i == x && j == y) { //es el query point el que voy a mostrar?
                        cout << "\t" << GREEN << BOLD << UNO << RESET;
                    } else {
                        cout << "\t" << GREEN << "1" << RESET;
                    }
                }
            } else {
                cout << "\t0";
            }
        }
        cout << endl;
    }
    cout << BOLD << "SIMBOLOGY" << RESET << endl;
    cout << UNO << " MEANS: THE QUERY POINT" << endl;
    cout << "RED ONES: THE ONE'S OF QUERY RESULT" << endl;
    cout << "GREEN ONES: THE ONE'S OF THE MATRIX" << endl;
}

template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::printDirectNeighbors(uint_t x, uint_t y) {

    vector<int> neighbors = this->direct(x, y);//Se obtienen los vecinos directos
    if (neighbors.empty()) { //exite el punto que estan preguntando???
        printf("no existen vecinos directos para este query point [%d,%d]\n", x, y);
        return;
    }
    cout << BOLD << "DIRECT NEIGHBORS OF (" << x << "," << y << ") ARE: " << RESET;
    for (int &neighbor: neighbors) {
        cout << RED << "(" << x << "," << neighbor << ") " << RESET;
    }
    cout << endl;
}

template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::printReverseNeighbors(uint_t x, uint_t y) {
    vector<int> neighbors = reverse(x, y);

    if (neighbors.empty()) { //exite el punto que estan preguntando???
        printf("no existen vecinos reversos para este query point [%d,%d]\n", x, y);
        return;
    }
    cout << BOLD << "REVERSE NEIGHBORS OF (" << x << "," << y << ") ARE: " << RESET;
    for (int &neighbor: neighbors) {
        cout << RED << "(" << x << "," << neighbor << ") " << RESET;
    }
}

template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::printRange(uint_t ix, uint_t jx, uint_t iy, uint_t jy) {

    vector<int> rangeMatrix = this->range(ix, jx, iy, jy);
    if (rangeMatrix.empty()) {
        cout << "NO EDGES IN THE RANGE OR THE QUERY POINT IS OUT OF THE MATRIX" << endl;
        return;
    }
    cout << BOLD << "RANGE OF (" << ix << "," << jx << ")x(" << iy << "," << jy << ") ARE: " << RESET;
    for (int i = 0; i < rangeMatrix.size(); i += 2) {
        cout << RED << "(" << rangeMatrix[i] << "," << rangeMatrix[i + 1] << ") " << RESET;
    }
    cout << endl;
}

template<typename uint_t>
void ExternalAdjacencyMatrix<uint_t>::printRangeMatrix(uint_t ix, uint_t jx, uint_t iy, uint_t jy) {
    vector<int> rangeMatrix = this->range(ix, jx, iy, jy);
    if (rangeMatrix.empty()) {
        cout << "NO EDGES IN THE RANGE OR THE QUERY POINT IS OUT OF THE MATRIX" << endl;
        return;
    }
    printRange(ix, jx, iy, jy);
    cout << endl;
    int index = 0;
    cout << "i\t";
    for (int i = 0; i < n; i++) {
        cout << "\t" << BOLD << YELLOW << UNDERLINE << i << RESET;
    }
    cout << endl;
    /*
     * Print the matrix and the range colored with yellow
     *
     */
    for (int i = 0; i < n; i++) {
        cout << "\t" << BOLD << YELLOW << i << "|" << RESET;
        for (int j = 0; j < n; j++) {
            if (get(i, j)) {
                if (i == rangeMatrix[index] && rangeMatrix[index + 1] == j) {
                    if ((i == ix && j == jx) || (i == iy && j == jy)) {
                        cout << "\t" << BOLD << RED << UNO << RESET;
                    } else {
                        cout << "\t" << BOLD << RED << "1" << RESET;
                    }
                    index += 2;
                } else {
                    cout << "\t" << GREEN << "1" << RESET;
                }
            } else {
                if ((i == ix && j == jx) || (i == iy && j == jy)) {
                    cout << "\t" << BOLD << RED << "⓪" << RESET;
                } else {
                    cout << "\t0";
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}



#endif //BITVECTOR_EXTERNALADJACENCYMATRIX_H