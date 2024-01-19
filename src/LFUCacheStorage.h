/**
 *
 * ****************************** English *********************************
 *
 * Least Frequently Used (LFU) Cache Implementation from paper:
 * An O(1) algorithm for implementing the LFU cache eviction scheme
 * reference: https://doi.org/10.48550/arXiv.2110.11602
 *
 * Header credits.
 * Programer: Miguel Romero.
 * Documented by Carlos Céspedes Cárdenas.
 *
 * This header was created to
 *
 * ****************************** Español *********************************
 *
 * Implementación de cache Least Frequently Used (LFU). (O(log n))
 * referencia: https://www.enjoyalgorithms.com/blog/least-frequently-used-cache
 *
 * Créditos del header.
 * Programador: Miguel Romero.
 * Documentado por: Carlos Céspedes Cárdenas.
 *
 * Este header busca
 *
 * */

#ifndef BV_LFUCACHESTORAGE_H
#define BV_LFUCACHESTORAGE_H

#include <list>
#include <unordered_map>
#include "Storage.h"


/**
* Al instanciarse la clase recibe el parámetro por template:
* @tparam uint_t (entero sin signo)
* Que sirve para indicar la cantidad de bits que usa cada palabra.
*/
template<typename uint_t>
class LFUCacheStorage : public Storage<uint_t> {
private:
    //la lista secundaria: puntero al padre y página
    // store page of cache

    //Lista monotonica creciente por frecuencia de listas de páginas
    //la primera lista es de las páginas menos frecuentes, la última
    //es la más frecuente.
    //Dentro de cada lista se almacenan de la más recientemente usada
    //a la ultima usada en la misma frecuencia (se incerta al inicio).
    //la política de reemplazo es eliminar de la lista de páginas
    // menos frecuente (la primera de las listas), la última página.
    //hay dos nodos, el de la lista principal (lista de listas) y
    //la lista de páginas.
    struct NodeListPage;
    //nodo de la lista principal
    typedef struct NodoListaFreq{
        uint_t freq=1;
        list<NodeListPage> lista;
    } NodoListaFreq;

    //nodo de la lista interna
    typedef struct NodeListPage{
        typename list<NodoListaFreq>::iterator padre;
        Page<uint_t> pagina;
    }NodeListPage;

    //lista de lista de páginas agrupadas por frecuencia
    list<NodoListaFreq> frecList;

    //permite acceder a una página en O(1) a partir de su id
    //el mapa [oid]->puntero_al_nodo_lista_que_tien_la_página
    unordered_map<uint_t, typename list<NodeListPage>::iterator> mapa;

    /**
     * La clase tiene 3 atributos privados.
     *
     * frecuencyMap:
     * mapa:
     * cacheSize:
     */

    int cacheSize; // maximum capacity of cache

public:
    LFUCacheStorage(const string &fileName, int d, int capacity) : Storage<uint_t>(fileName, d), cacheSize(capacity) {}

    virtual void appendPage(Page<uint_t> &page) override;

    virtual void updatePage(Page<uint_t> &page, uint_t p) override;

    virtual Page<uint_t> readPage(uint_t pagePos) override;

    void close() override;

private:
    inline void removeCacheEntry();

    inline Page<uint_t> getCacheEntry(uint_t pagePos);

    inline void updateCacheEntry(Page<uint_t> &page, uint_t pagePos);

    inline void addNewCacheEntry(Page<uint_t> &page, uint_t pagePos);

    inline bool cacheContains(uint_t pagePos);

    inline bool cacheIsFull();

    void promovePage(Page<uint_t> &page, uint_t pagePos, typename list<NodoListaFreq>::iterator padre);
};

template<typename uint_t>
void LFUCacheStorage<uint_t>::appendPage(Page<uint_t> &page) {
    //los cacheMissed los cuenta la superclase cuando se accede efectivamente al disco
    int cantPages = Storage<uint_t>::getPages(); //la pagina que voy a guardar, lleva este ID.
    Storage<uint_t>::appendPage(page);
    //addNewCacheEntry(page, cantPages);
}

template<typename uint_t>
void LFUCacheStorage<uint_t>::updatePage(Page<uint_t> &page, uint_t p) {
    if (cacheContains(p)) {
       // page.setUpdated();
        Storage<uint_t>::incCacheHit();
        updateCacheEntry(page, p);
    } else {
       // page.setUpdated();
        //los cacheMissed los cuenta la superclase cuando se accede efectivamente al disco
        addNewCacheEntry(page, p);
    }
}

template<typename uint_t>
Page<uint_t> LFUCacheStorage<uint_t>::readPage(uint_t pagePos) {
    if (cacheContains(pagePos)) {
        Storage<uint_t>::incCacheHit();
        return getCacheEntry(pagePos);
    } else {
        Page<uint_t> out(Storage<uint_t>::readPage(pagePos));//internamente incrementa cacheMissed
        addNewCacheEntry(out, pagePos);
        return out;
    }
}

//Police for LFU cache
template<typename uint_t>
inline Page<uint_t> LFUCacheStorage<uint_t>::getCacheEntry(uint_t pagePos) {
    // assert(p is present in cache)
    // then move to the next list of frecuency and return page.
   // Page<uint_t> page(this->getD());
    auto nodePage = mapa[pagePos]; //obtengo el nodo de la pagina
    Page<uint_t> page=nodePage->pagina;
    typename list<NodoListaFreq>::iterator padre=nodePage->padre;
    //saco la página del frequency List para promoverla

    (*padre).lista.erase(nodePage);
    promovePage(page, pagePos, padre);

    return page;
}

template<typename uint_t>
inline void LFUCacheStorage<uint_t>::addNewCacheEntry(Page<uint_t> &page, uint_t pagePos) {
    if (cacheIsFull()) removeCacheEntry();
    page.startCountFrec();
    //toda página nueva que entra al caché entra con frecuencia 1
    //debería ir en el begin del frecList.
    //si el begin no tiene frecuencia 1 se crea un nuevo nodo al inicio
    //para guardar las páginas de frecuencia 1.
    auto p=frecList.begin();
    if(frecList.size()==0||p->freq!=1){
        NodoListaFreq nuevo;
        nuevo.freq=1;
        frecList.push_front(nuevo);
        p=frecList.begin();
    }
    NodeListPage nuevo;
    nuevo.padre=p;
    nuevo.pagina=page;
    p->lista.push_front(nuevo);
    mapa[pagePos] = p->lista.begin();
    //if changed, update on disk!
   // if (page.isUpdated()) Storage<uint_t>::updatePage(page, page.getId());
}

template<typename uint_t>
inline void LFUCacheStorage<uint_t>::removeCacheEntry() {
    // delete least frecuently used.
    // If exists more tan one, remove least recently used element from this list
    //that is the back().
    Page<uint_t> last(this->getD());
    //siempre la menos frecuente está al inicio
    auto &LFU_List=frecList.begin()->lista;

    //obtengo la ultima página (least recently used page)
    last = LFU_List.back().pagina;
    // Pops the last element
    LFU_List.pop_back(); 
    // Erase the last
    mapa.erase(last.getId());
    if((long long int)LFU_List.size() <= 0){ //esta vacia
       frecList.pop_front();
    }
    //if changed, update on disk!
    if (last.isUpdated())Storage<uint_t>::updatePage(last, last.getId());
}

template<typename uint_t>
inline void LFUCacheStorage<uint_t>::updateCacheEntry(Page<uint_t> &page, uint_t pagePos) {// present in cache
    //page is previously gets from cache. Page isn't a new entry.
    auto nodePage=mapa[pagePos];
    auto padre=nodePage->padre;
    page.setUpdated();

    (*padre).lista.erase(nodePage);

    promovePage(page, pagePos, padre);

}

template<typename uint_t>
void LFUCacheStorage<uint_t>::promovePage(Page<uint_t> &page, uint_t pagePos, typename list<NodoListaFreq>::iterator padre) {
    page.increaceFrec();
    //guardo la página en la siguiente frecuencia natural
    auto next=padre; //aquí se crea next con auto, como una copia del padre
    ++next;//con ++ se avanza el puntero al nodo siguiente, no mueve el padre.
    if(next==frecList.end()){
   // if(next->lista.size() == 0){
        //padre es el útimo nodo, no hay next
        NodoListaFreq nuevo; //se crea el next
        nuevo.freq=padre->freq+1; //cuando se mueve o actualiza una página incrementa en 1 su frecuencia
        frecList.push_back(nuevo);//como el padre es el último, se inserta nuevo al final
        next= prev(frecList.end(),1);//obtengo un iterador al último y actualizo next para que sea el siguiente al padre
    }else if(next->freq > (padre->freq+1)){ //otro caso especial
        //next no corresponde con la siguiente frecuencia natural
        //            padre
        //              |
        //ejemplo [1]->[2]->[7]
        //se inserta un nuevo nodo en next
        NodoListaFreq nuevo;//se supone que aquí debe crear el nuevo nodo con una lista vacía
        nuevo.freq=padre->freq+1;//se actualiza la frecuencia del nodo con el siguiente natural del padre
        next= frecList.insert(next, nuevo);//aquí se supone que insert retorna un iterador al nuevo nodo
        //            padre  next
        //              |    |
        //ejemplo [1]->[2]->[3]->[7]

    }
    //ahora next apunta al nodo donde está la lista para insertar la página
    NodeListPage nuevo;
    nuevo.pagina=page; //aquí puede haber un problema page es Page<uint_t> & y nuevo.pagina es Page<uint_t>, sin &!!!
    nuevo.padre=next;
    next->lista.push_front(nuevo);
    // update reference
    mapa[pagePos] = next->lista.begin();

    //si al mover la página, la lisa donde estaba queda vacía se elimina
    if((long long int)(*padre).lista.size() <= 0){ //esta vacia
        frecList.erase(padre);
    }
}

template<typename uint_t>
inline bool LFUCacheStorage<uint_t>::cacheIsFull() {
    return this->mapa.size() == this->cacheSize;
}

template<typename uint_t>
void LFUCacheStorage<uint_t>::close() {
    //save the cache changed block
    for(auto &nodeFrec: frecList) {
        for (auto &nodePage: nodeFrec.lista) {
            if (nodePage.pagina.isUpdated()) {
                Storage<uint_t>::updatePage(nodePage.pagina, nodePage.pagina.getId());
            }
        }
        nodeFrec.lista.clear();
    }
    frecList.clear();
    mapa.clear();
    Storage<uint_t>::close();
}

template<typename uint_t>
inline bool LFUCacheStorage<uint_t>::cacheContains(uint_t pagePos) { return mapa.find(pagePos) != mapa.end(); }

#endif