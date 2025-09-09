//
// Created by fernando on 23-11-22.
//
#ifndef BITVECTOR_UTILS_H
#define BITVECTOR_UTILS_H

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#define GetCurrentDir getcwd
#endif

using namespace std;
#define NO_BUFFER 0
#define LRU_BUFFER 1
#define LFU_BUFFER 2

#include <string>
#include <dirent.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
typedef struct {
    int id;
    string dataset;
    string cacheName;
    int points;
    double density;
    double binFileSpace;
    double pagesize;
    double pageSizeUsed;
    double datasetSpace;
    double buildTime;
    unsigned long  peakMemory;
    float cacheHit;
    float cacheMiss;
    float hitRatio;
    string algoritmo;
    long int pageFaultsWithoutIO;
    long int pageFaultWithIO;
    long int swaped;
    long int readFromDisk;
    long int writeToDisk;
} Stats;
typedef struct {
    int id;
    double binFileSpace;
    double datasetSpace;
    string dataset;
    string queryName;
    string cacheName;
    double pageSizeUsed;
    double pagesize;
    double queryTime;
    unsigned long  peakMemory;
    double cacheHit;
    double cacheMiss;
    double hitRatio;
    string algoritmo;
    long int pageFaultsWithoutIO;
    long int pageFaultWithIO;
    long int swaped;
    long int readFromDisk;
    long int writeToDisk;
} StatsForQuerys;

//codificacion para LINUX!!
const std::string GREEN = "\x1b[32m";
const std::string RED = "\x1b[31m";
const std::string YELLOW = "\x1b[33m";
const std::string RESET = "\x1b[0m";

const std::string BOLD = "\x1b[1m";
const std::string UNDERLINE = "\x1b[4m";
const std::string UNO = "①";
const std::string CERO = "⓪";
std::string tipo_estructura = "K-2 Tree (k=2)";
std::string tipo_estructura2 = "Adjacency Matrix";
std::string tipo_estructura_num = "1";
std::string tipo_estructura_num2 = "2";
std::string tipo_operacion = "construccion";
std::string tipo_operacion2 = "consulta(checkLink)";
std::string tipo_operacion3 = "consulta(direct)";
std::string tipo_operacion4 = "consulta(reverse)";
std::string tipo_operacion5 = "consulta(range)";
std::string tipo_operacion_num = "0";
std::string tipo_operacion_num2 = "1";
std::string tipo_operacion_num3 = "2";
std::string tipo_operacion_num4 = "3";
std::string tipo_operacion_num5 = "4";
std::string tipo_cache = "No Cache";
std::string tipo_cache2 = "LFU";
std::string tipo_cache3 = "LRU";
std::string tipo_cache_num = "0";
std::string tipo_cache_num2 = "1";
std::string tipo_cache_num3 = "2";
const int CONVERSION_VALUE = 1024;

std::string get_last_word(const std::string& s) {
    auto index = s.find_last_of(' ');
    return s.substr(++index);
}
int getN(string path) {

    //extract from the file name the n value having the format
    //DH_100M or DH_100K
    string filename = path.substr(path.find_last_of("/\\") + 1);
    //delete the extension .txt
    filename = filename.substr(0, filename.find_last_of("."));
    //delete any prefix
    filename = filename.substr(filename.find_last_of("_") + 1);
    //get the number

    //return the number itself if it is not K or M at the end
    //after de characater x in the string is the number
    int x = filename.find_last_of("x");
    return stoi(filename.substr(x + 1));
}
int checkFileExist(char file[]){
    struct stat st;
    return !stat(file, &st);
}
unsigned long long getFileSize(string file) {
    //calculamos el peso de los archivos indice creados.
    struct stat st;
    int status;
    unsigned long long size = 0;
    status = stat(file.c_str(), &st);
    if (status == 0) {
        size = st.st_size;
    }
    return size/1024; //in kb
}
int fileIsEmpty(char file[]){
    FILE *fp = fopen( file,"r" );
    int c = getc(fp);
    if (c == EOF)
        return 1;
    ungetc(c, fp);
    return 0;

}
int findQuantityOfDatasets(int argc, char *argv[]) {
    //Using the argv[5](folder) to find the quantity of datasets
    DIR *dir;
    struct dirent *ent;
    int quantityOfDatasets = 0;
    if ((dir = opendir(argv[5])) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                quantityOfDatasets++;
            }
        }
        closedir(dir);
        return quantityOfDatasets;
    } else {
        perror("No se pudo abrir el directorio");
        exit(EXIT_FAILURE);
    }

}
char *recoverPath(int argc, char *argv[], int i) {
    DIR *dir;
    struct dirent *ent;
    int quantityOfDatasets = 0;
    if ((dir = opendir(argv[5])) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                if (quantityOfDatasets == i) {
                    char *path = (char *) malloc(sizeof(char) * 100);
                    strcpy(path, argv[5]);
                   // strcat(path, "/");
                    strcat(path, ent->d_name);
                    return path;
                }
                quantityOfDatasets++;
            }
        }
        closedir(dir);
    } else {
        perror("No se pudo abrir el directorio");
        exit(EXIT_FAILURE);
    }
    return NULL;
}
std::string GetCurrentWorkingDir(void) {
    char buff[FILENAME_MAX];
    GetCurrentDir(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}
double convertByteToKb(double bytes){
    return bytes / CONVERSION_VALUE;
}
string removeExtension(string fullname){
    size_t lastindex = fullname.find_last_of(".");
    string rawname = fullname.substr(0, lastindex);
    return rawname;
}
string printPoint(int x, int y){
    std::stringstream sstm;
    sstm << "("<<x<<","<<y<<")";
    return sstm.str();
}
#endif //BITVECTOR_UTILS_H
