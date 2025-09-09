

/*
 * ****************************** English *********************************
 *
 * Header credits.
 * A modified version of Page.h for Carlos Céspedes.
 * Documented by Carlos Céspedes Cárdenas.
 *
 * This header was created to
 *
 * ****************************** Español *********************************
 *
 * Créditos del header.
 * Una versión modificada de Page.h por Carlos Céspedes.
 * Documentado por: Carlos Céspedes Cárdenas.
 *
 * Este header busca
 *
 * */

#ifndef BV_PAGES_H
#define BV_PAGES_H


#include <memory>
#include <iostream>
#include <cassert>

template<typename uint_t>
class Pages {
private:

    //Puntero compartido del espacio de memoria.
    std::shared_ptr<uint_t[]> ptrPages = nullptr;

    //Cantidad de palabras que contiene cada página.
    int D;

    //Cantidad de páginas que contiene el puntero
    int totalPages;

public:

    //Constructor que no pide espacio en memoria
    Pages(int d, int numPages) : ptrPages(nullptr){
        if(d>0 && numPages >0){
            D = d;
            totalPages = numPages;
        }else{
            D = 1;
            totalPages = 1;
        }
    }

    //Constructor que pide espacio en memoria, este se inicializa en 0 si se
    //pasa un true por parámetro.
    Pages(int d, int numPages, bool blank){
        if(d>0 && numPages >0){
            D = d;
            totalPages = numPages;
        }else{
            D = 1;
            totalPages = 1;
        }
        if(blank){
            ptrPages= std::shared_ptr<uint_t[]>(new uint_t[d * numPages]{0});
        }else{
            ptrPages= std::shared_ptr<uint_t[]>(new uint_t[d * numPages]);
        }
    }

    //Constructor "copia"
    Pages(const Pages &other){
        //share ptrPages, not copy
        ptrPages=other.ptrPages;
        D=other.D;
        totalPages=other.totalPages;
    }

    //Palloc (Pages allocation) pide el espacio en la memoria. Se inicializa en
    // 0 si se pasa un true por parámetro. Además, si ya se pidió espacio antes
    // este se libera.
    void Palloc(bool blank=false){
        if(ptrPages != nullptr)
            delete ptrPages;
        if(blank){
            ptrPages= std::shared_ptr<uint_t[]>(new uint_t[D * totalPages]{0});
        }else{
            ptrPages= std::shared_ptr<uint_t[]>(new uint_t[D * totalPages]);
        }
    }

    //Operador de asignación.
    uint_t& operator=(const uint_t& other){
        ptrPages=other.ptrPages;
        D=other.D;
        totalPages=other.totalPages;
    }

    //Operador corchete, retorna puntero a la página indicada
    uint_t* operator[](int page) const{
        assert(0 <= page && page < totalPages);
        return ptrPages.get() + page * D;
    }

    //Operador paréntesis con 2 parámetros. Retorna la palabra word en la
    // página page.
    uint_t operator()(uint_t page, uint_t word){
        return ptrPages[page * D + word];
    }

    bool operator==(const Pages &rhs) const {
        return ptrPages == rhs.ptrPages &&
               D == rhs.D &&
               totalPages=rhs.totalPages;
    }

    bool operator!=(const Pages &rhs) const {
        return !(rhs == *this);
    }

    uint_t* data() {
        return ptrPages.get();
    }

    int getD() const {
        return D;
    }

    int getTotalPages() const {
        return totalPages;
    }
};


#endif
