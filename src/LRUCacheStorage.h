

/*
 * ****************************** English *********************************
 *
 * Header credits.
 * Programer: Miguel Romero.
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
 * */

#ifndef BV_LRUCACHESTORAGE_H
#define BV_LRUCACHESTORAGE_H

#include <list>
#include <unordered_map>
#include "Storage.h"
#include "Pages.h"
#include "utils.h"

template<typename uint_t>
class LRUCacheStorage : public Storage<uint_t> {
private:
    // store page of cache
    list <Page<uint_t>> pageList;
    // store references of page in pageList of key in cache
    unordered_map<uint_t, typename list<Page<uint_t>>::iterator> mapa;
    int cacheSize; // maximum capacity of cache
public:
    LRUCacheStorage(const string &fileName, int d, int csize) : Storage<uint_t>(fileName, d), cacheSize(csize) {}

    virtual void appendPage(Page<uint_t> &buffer) override;

    virtual void updatePage(Page<uint_t> &page, uint_t p) override;

    virtual Page<uint_t> readPage(uint_t p) override;

    void close() override;

private:
    inline void removeCacheEntry();

    inline Page<uint_t> getCacheEntry(uint_t p);

    inline void updateCacheEntry(Page<uint_t> &page);

    inline void addNewCacheEntry(Page<uint_t> &page);

    inline bool cacheContains(uint_t p);

    inline bool cacheIsFull();
};

template<typename uint_t>
Page<uint_t> LRUCacheStorage<uint_t>::readPage(uint_t p) {
    if (cacheContains(p)) {
        Storage<uint_t>::incCacheHit();
        return getCacheEntry(p);
    } else {
        //ir a buscar página al disco
        Page<uint_t> out(Storage<uint_t>::readPage(p)); //internamente incrementa cacheMissed
        addNewCacheEntry(out);
        return out;
    }
}

template<typename uint_t>
void LRUCacheStorage<uint_t>::appendPage(Page<uint_t> &page) {
    //los cacheMissed los cuenta la superclase cuando se accede efectivamente al disco
    Storage<uint_t>::appendPage(page);
 //   addNewCacheEntry(page);
}

template<typename uint_t>
void LRUCacheStorage<uint_t>::updatePage(Page<uint_t> &page, uint_t p) {
    if (cacheContains(p)) {
        Storage<uint_t>::incCacheHit(); //la pille en mi caché!
        updateCacheEntry(page);
    } else { //no la pille en el cache, entonces la tengo que meter en el cache
        // los cacheMissed los cuenta la superclase cuando se accede efectivamente al disco
        //Storage<uint_t>::updatePage(page, p);
        page.setUpdated();
        addNewCacheEntry(page);
    }
}
template<typename uint_t>
inline void LRUCacheStorage<uint_t>::updateCacheEntry(Page<uint_t> &page) {// present in cache
    //en la llamada anterior valida que p exista en el cache.
    page.setUpdated();
    pageList.erase(mapa[page.getId()]);
    pageList.push_front(page);
    // update reference
    mapa[page.getId()] = pageList.begin();
}

template<typename uint_t>
inline void LRUCacheStorage<uint_t>::addNewCacheEntry(Page<uint_t> &page) {
    if (pageList.size() == cacheSize) {
        removeCacheEntry();
    }
    pageList.push_front(page);
    mapa[page.getId()] = pageList.begin();
}

template<typename uint_t>
inline void LRUCacheStorage<uint_t>::removeCacheEntry() {
    // delete least recently used element
    //en la llamada anterior valida que p exista en el cache.
    Page<uint_t> last = pageList.back();
    // Pops the last element
    pageList.pop_back();
    // Erase the last
    mapa.erase(last.getId());
    //if changed, update on disk!
    if (last.isUpdated()) {
        Storage<uint_t>::updatePage(last, last.getId());
    }
}

template<typename uint_t>
inline Page<uint_t> LRUCacheStorage<uint_t>::getCacheEntry(uint_t p) {
    // then move to front and return buffer.
    //en la llamada anterior valida que p exista en el cache.
    Page<uint_t> page = *(mapa[p]);
    pageList.erase(mapa[p]);
    pageList.push_front(page);
    // update reference
    mapa[page.getId()] = pageList.begin();
    return page;
}




template<typename uint_t>
inline bool LRUCacheStorage<uint_t>::cacheIsFull() {
    return pageList.size() >= cacheSize;
}

template<typename uint_t>
inline bool LRUCacheStorage<uint_t>::cacheContains(uint_t p) { return mapa.find(p) != mapa.end();}//mapa.contains(p); }

template<typename uint_t>
void LRUCacheStorage<uint_t>::close() {
    //save the cache changed block
    for (Page<uint_t> &page: pageList) {
        if (page.isUpdated()) Storage<uint_t>::updatePage(page, page.getId());
    }
    pageList.clear();
    mapa.clear();
    Storage<uint_t>::close();
}

#endif