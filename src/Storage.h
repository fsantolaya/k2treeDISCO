/**
 *
 * ****************************** English *********************************
 *
 * Header credits.
 * Programmer: Miguel Romero.
 * Documented by Carlos Céspedes Cárdenas.
 *
 * This header was created to
 *
 * ****************************** Español *********************************
 *
 * Créditos del header.
 * Programador: Miguel Romero.
 * Documentado por: Carlos Céspedes Cárdenas.
 *
 * Este header busca
 *
 * */

#ifndef BV_STORAGE_H
#define BV_STORAGE_H

#include <string>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <map>
#include <cstdio>
#include "Page.h"

using namespace std;

namespace fs = std::filesystem;

/**
* Al instanciarse la clase recibe el parámetro por template:
* @tparam uint_t (entero sin signo)
* Que sirve para indicar la cantidad de bits que usa cada palabra.
*/
template<typename uint_t>
class Storage {
private:
    /**
     * La clase tiene 5 atributos privados.
     *
     * fileName: Nombre del archivo en disco
     * metaData: mapa para metadatos (longitud del bitvector, rank, etc).
     * D: Cantidad de palabras que contiene cada página del archivo.
     * pages: Cantidad de páginas que contiene el archivo.
     * file: archivo fileStream con el que se crea y escribe el archivo en disco.
     */
    std::string fileName;
    map<string, string> metaData;
    int D;
    int pages;
    std::fstream file;
    int cacheMissed=0;
    int cacheHit=0;
    Page<uint_t> currentPage;
public:

    void resetCacheStat(){
        cacheMissed=0;
        cacheHit=0;
    }
    int getCacheHit() const;
    void setCacheHit(int cacheHit);
    void incCacheHit(){
        cacheHit++;
    }
    void incCacheMissed(){
        cacheMissed++;
    }
    int getCacheMissed() const;
    void setCacheMissed(int cacheMissed);
    double hitRatio();
    Storage(const std::string fileName, int D);
    virtual ~Storage();
    int getD() const;
    bool open();
    bool create(bool deleteIfExists=true);
    virtual void close();
    virtual void appendPage(Page<uint_t> &page);
    virtual void updatePage(Page<uint_t> &page, uint_t p);
    virtual Page<uint_t> readPage(uint_t p);
    int getPages();
    inline uint_t bitsPerPage();
    inline int bitsPerWord();
    inline uint_t pageSize();
    inline uint_t size();
    void setMeta(string key, string value);
    string getMeta(string key);

private:
    void readMetaData();
    void saveMetaData();
};

/**
 * El constructor recibe los parámetros:
 *
 * @param fileName: nombre del archivo en disco.
 * @param D: cantidad de palabras que tiene cada página.
 */
template<typename uint_t>
Storage<uint_t>::Storage(const std::string fileName, int D):fileName(fileName), D(D) {
    pages = 0;
    metaData["D"] = to_string(D);
    metaData["PAGES"] = to_string(pages);
}

/**
 * El destructor cierra el archivo y salva la metaData.
 */
template<typename uint_t>
Storage<uint_t>::~Storage() {
    if (file.is_open()) {
        file.close();
        saveMetaData();
    }
}

/**
 * Método getD.
 *
 * @return Retorna el tamaño de palabras que tiene cada página.
 */
template<typename uint_t>
int Storage<uint_t>::getD() const {
    return D;
}

/**
 * Método open.
 *
 * @return Si se ha abierto el archivo desde el disco.
 *
 * Este método abre el archivo en disco para entrada, salida en binario. Además lee
 * el metadata del archivo.
 */
template<typename uint_t>
bool Storage<uint_t>::open() {
    if (!file.is_open()) {
        file.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
    }
    readMetaData();
    pages=stoi(getMeta("PAGES"));
    D=stoi(getMeta("D"));
    return file.is_open();
}

/**
 * Método create.
 *
 * @param deleteIfExists:
 * @return retorna si existe el archivo en disco.
 *
 * Verifica si existe un archivo con el nombre de fileName o si se ha pasado un true por
 * parámetro. Cualquiera de de estos casos crea un archivo en blanco (si existe se borra
 * su contenido) y cerrado. Se guarda la metadata y se retorna si existe el archivo.
 */
template<typename uint_t>
bool Storage<uint_t>::create(bool deleteIfExists) {
    if (!(fs::exists(fileName))||deleteIfExists) {
        std::ofstream createFile(fileName);
        assert(createFile.is_open()); // cambiar por excepción
        createFile.close();
    }
    saveMetaData();
    return fs::exists(fileName);
}

/**
 * Método close.
 *
 * Cierra el archivo y salva la metaData.
 */
template<typename uint_t>
void Storage<uint_t>::close() {
    if (file.is_open()) {
        file.close();
        saveMetaData();
    }
}

/**
 * Método appendPage.
 *
 * @param page: Página (objeto con el puntero) a grabar en disco.
 *
 * Escribe la página al final del archivo.
 */
template<typename uint_t>
void Storage<uint_t>::appendPage(Page<uint_t> &page) {
    assert (file.is_open());
    file.seekp(0, ios::end);
    const char* data = reinterpret_cast<const char *> (page.data());
    file.write(data, pageSize());
    page.setId(pages);
    pages++;
    cacheMissed++;
}

/**
 * Método updatePage.
 *
 * @param page: Página con los datos a actualizar.
 * @param p: número de página del archivo a actualizar.
 *
 * Sobrescribe la pagina p con los datos de page.
 */
template<typename uint_t>
void Storage<uint_t>::updatePage(Page<uint_t> &page, uint_t p) {
    assert (p < pages);
    assert(file.is_open());
    file.seekp(p * pageSize());
    file.write(reinterpret_cast<const char *> (page.data()), pageSize());
    page.setId(p);
    //page.setUpdated();
    cacheMissed++;
}

/**
 * Método readPage.
 *
 * @param p: número de página a leer.
 * @return página pedida.
 */
template<typename uint_t>
Page<uint_t> Storage<uint_t>::readPage(uint_t p) {

    if(!currentPage.isEmpty() && currentPage.getId() == p){
        return currentPage;
    }
    assert (p < pages);
    assert(file.is_open());
    Page<uint_t> page(Storage<uint_t>::getD());
    page.createBlock();
    file.seekg(p * pageSize());
    file.read(reinterpret_cast<char *> (page.data()), pageSize());
    page.setId(p);
    cacheMissed++;
  //  cout <<" fui a leer la pagina : "<< p  <<" Cache miss is : "<< cacheMissed<<endl;
    currentPage  = page;
    return page;
}

/**
 * Método getPages.
 *
 * @return cantidad de páginas que tiene el archivo.
 */
template<typename uint_t>
int Storage<uint_t>::getPages() {
    return pages;
}

/**
 * Método bitsPerPage.
 *
 * @return cantidad de bits que se usan para el bitvector.
 *
 * Como se usan D-1 palabras para el bitvector y 1 para rank acumulado, se retorna D-1
 * multiplicado la cantidad de bits que tiene cada palabra.
 */
template<typename uint_t>
inline uint_t Storage<uint_t>::bitsPerPage() {
    return (D - 1) * bitsPerWord();
}

/**
 * Método bitsPerWord.
 *
 * @return cantidad de bits que tiene cada palabra.
 */
template<typename uint_t>
inline int Storage<uint_t>::bitsPerWord(){
    return sizeof(uint_t) * 8;
}

/**
 * Método pageSize.
 *
 * @return cantidad de bytes que puede almacenar cada página.
 */
template<typename uint_t>
inline uint_t Storage<uint_t>::pageSize() {
    return D * sizeof(uint_t);
}

/**
 * Método size.
 *
 * @return cantidad de bytes que contiene el archivo.
 */
template<typename uint_t>
inline uint_t Storage<uint_t>::size() {
    return pageSize() * pages;
}

/**
 * Método setMeta.
 *
 * @param key: nombre del meta-dato.
 * @param value: valor del meta-dato.
 *
 * Guarda el meta-dato mapeando key a value.
 */
template<typename uint_t>
void Storage<uint_t>::setMeta(string key, string value) {
    metaData[key] = value;
}

/**
 * Método getMeta.
 *
 * @param key: nombre del meta-dato.
 * @return valor asignado al meta-dato key.
 */
template<typename uint_t>
string Storage<uint_t>::getMeta(string key) {
    return metaData[key];
}

/**
 * Método readMetaData.
 *
 * Rescata en el atributo metaData, todos los meta-datos almacenados en disco.
 */
template<typename uint_t>
void Storage<uint_t>::readMetaData() {
    string name = fileName;
    name += "_METADATA";
    ifstream metafile(name.data());
    string key, value;
    while (metafile >> key >> value) {
        metaData[key] = value;
    }
    metafile.close();
}

/**
 * Método saveMetaData.
 *
 * Guarda en un archivo de disco (distinto al de los propios datos) los meta-datos
 * relacionados al archivo que se esta usando.
 */
template<typename uint_t>
void Storage<uint_t>::saveMetaData() {
    metaData["PAGES"] = to_string(pages);
    string name = fileName;
    name += "_METADATA";
    ofstream metafile(name.data());
    string key, value;
    for (auto item: metaData) {
        metafile << item.first << " " << item.second << endl;
    }
    metafile.close();
}

template<typename uint_t>
int Storage<uint_t>::getCacheMissed() const {
    return cacheMissed;
}

template<typename uint_t>
void Storage<uint_t>::setCacheMissed(int cacheMissed) {
    Storage::cacheMissed = cacheMissed;
}

template<typename uint_t>
int Storage<uint_t>::getCacheHit() const {
    return cacheHit;
}

template<typename uint_t>
void Storage<uint_t>::setCacheHit(int cacheHit) {
    Storage::cacheHit = cacheHit;
}
template<typename uint_t>
double Storage<uint_t>::hitRatio(){
    return (double)cacheHit/((double)cacheHit+(double)cacheMissed);
}
#endif //BV_STORAGE_H
