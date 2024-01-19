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
 * La clase ExternalBitvector entrega el manejo de un bitvector en disco
 * mediante un objeto Storage
 *
 * */

#ifndef BV_EXTERNALBITVECT_H
#define BV_EXTERNALBITVECT_H

#include <string>
#include <filesystem>
#include "BitBasic.h"
#include "Storage.h"

using namespace std;

/**
* Al instanciarse la clase recibe el parámetro por template:
* @tparam uint_t (entero sin signo)
* Que sirve para indicar la cantidad de bits que usa cada palabra.
*/
template<typename uint_t>
class ExternalBitvector {
    /**
     * La clase tiene 3 atributos privados
     *
     * lenght: Cantidad total de bits del bitvector
     * ones: Cantidad de bits en uno del bitvector
     * theStorage: Objeto que permite administrar el archivo que almacena el bitvector
     *             en disco.
     */
private:
    uint64_t length = 0;//in bits
    uint64_t ones = 0; //in bits
    Storage<uint_t> &theStorage;
public:
    ExternalBitvector(Storage<uint_t> &theStorage);
    ExternalBitvector(const ExternalBitvector<uint_t> &otro);

    ~ExternalBitvector();
    void create(const uint64_t len);
    void create();
    void open();
    void appendPage(Page<uint_t> buffer, uint_t n);
    void resize(const uint_t new_length);
    int operator[](const uint_t b) const;
    int access(const uint_t b) const;
    vector<int> range(const uint_t i, const uint_t j);
    void bitSet(const uint_t b);
    void bitClean(const uint_t b);
    uint_t rank(const uint_t b) const;
    void buildRank();
    uint64_t getLength();
    void print(int with = 128);

private:
    void apendBlank(uint_t N);
    inline tuple<Page<uint_t>, uint_t, uint_t> getBufferPageAndPos(const uint_t b) const;
};

/**
 * El constructor.
 *
 * @param storage: Objeto que administra el archivo que guarda el nombre del bitvector.
 *
 * Inicializa el tamaño del Bitvector (length) y la cantidad de unos (para rank).
 * Después actualiza estos valores en la metadata del Storage.
 */
template<typename uint_t>
ExternalBitvector<uint_t>::ExternalBitvector(Storage<uint_t> &storage):theStorage(storage) {
    length = 0;
    ones = 0;
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_LENGTH", to_string((unsigned long long )length));
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_ONES", to_string((unsigned long long )ones));
}

/**
 * El destructor actualiza el tamaño y la cantidad de unos en metadata y cierra el archivo.
 */
template<typename uint_t>
ExternalBitvector<uint_t>::~ExternalBitvector() {
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_LENGTH", to_string((unsigned long long )length));
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_ONES", to_string((unsigned long long )ones));
    theStorage.close();
}

/**
 * El método create.
 *
 * @param len: longitud del bitvector en bits
 *
 * Crea el archivo en disco, lo abre y aloja en memoria la cantidad de páginas necesarias
 * para poder almacenar un bitvector del tamaño entregado por parámetro.
 */
template<typename uint_t>
void ExternalBitvector<uint_t>::create(const uint64_t len) {
    assert(len>0);
    theStorage.create();
    open();
    resize(len);
    assert((len / theStorage.bitsPerPage() <
            theStorage.getPages()));// "No se crea correctamente el 'ExternalBitVector'"
}

/**
 * El método create sin parámetros solo crea y abre el archivo, el tamaño del bitvector
 * se mantiene en 0.
 */
template<typename uint_t>
void ExternalBitvector<uint_t>::create() {
    theStorage.create();
    open();
    length = 0;
    ones = 0;
}

/**
 * El método open abre el archivo y obtiene los metadatos del tamaño y la cantidad de
 * unos (si es que existen estos metadatos)
 */
template<typename uint_t>
void ExternalBitvector<uint_t>::open() {
    theStorage.open();
    length = stoull(theStorage.getMeta("EXTERNAL_BIT_VECTOR_LENGTH"));
    ones = stoull(theStorage.getMeta("EXTERNAL_BIT_VECTOR_ONES"));
}

/**
 * El método appendPage.
 *
 * @param buffer: Página que contiene el contenido que se quiere adjuntar
 * @param n: Cantidad de bits a adjuntar de la página (para aumentar length)
 *
 * Adjunta la página al final de todas las del archivo
 */
template<typename uint_t>
void ExternalBitvector<uint_t>::appendPage(Page<uint_t> buffer, uint_t n) {
  //  cout << "e - n is :" << n<< endl;
   // cout << "e -ones: "<<ones <<" ";
   // cout << "e -length:"<< length<<endl;
    //append buffer
    uint_t bitsPerPage = theStorage.bitsPerPage();
    uint_t bitsPerWord = theStorage.bitsPerWord();
    assert(n<=bitsPerPage);
    buffer[theStorage.getD() - 1] = ones;
    theStorage.appendPage(buffer);
    ones+=BitBasic::Rank(buffer.data(), n - 1, bitsPerWord);

    length+=n;
    //quizas solo debería guardarse en el storage y rescatarlo con funciones getter y setter
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_LENGTH", to_string((unsigned long long int)this->length));
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_ONES", to_string((unsigned long long int)this->ones));
 //   cout << "ones: "<<ones <<" ";
  //  cout << "length:"<< length<<endl;
}

/**
 * El método resize.
 *
 * @param new_length: Nuevo tamaño del que se va conformar el bitvector.
 *
 * Si el nuevo tamaño es superior al que se tiene se agregan más páginas.
 */
template<typename uint_t>
void ExternalBitvector<uint_t>::resize(uint_t new_length) {
    if (new_length == length) return;
    //si el nuevo valor es menor que length, borrar bloques de la cola, sino agregar.
    if (new_length > length) {
        //se agregan páginas
        //pageSize son los bits utiles por página - de datos. un word entero para rank
        uint_t pageSize = theStorage.pageSize()*8 - theStorage.bitsPerWord();//bits used for bitvector.
        //cout << "page size is: " << pageSize << "\n";
        uint_t pageNumber = (new_length - length + pageSize - 1) / pageSize;
        //cout << "page number is: "<< pageNumber << "\n";

      //  exit(1);
        apendBlank(pageNumber);

    } else {
        static_assert(true, "Falta implementar la eliminación de páginas cuando 'resize' achica el bitvector");
    }
    length = new_length;
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_LENGTH", to_string((unsigned long long int)this->length));
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_ONES", to_string((unsigned long long int)this->ones));
}

/**
 * El operador corchete.
 *
 * @param b: posición del bitvector de la cual se quiere conocer su valor.
 * @return el valor de la posición b del bitvector.
 */
template<typename uint_t>
int ExternalBitvector<uint_t>::operator[](const uint_t b) const {
    return access(b);
}

/**
 * El método access.
 *
 * @param b: posición del bitvector del cual se quiere conocer su valor
 * @return el valor de la posición b del bitvector.
 *
 * Se realiza con el método access del ámbito BitBasic (ver BitBasic.h).
 */
template<typename uint_t>
int ExternalBitvector<uint_t>::access(const uint_t b) const {
    Page<uint_t> buffer(theStorage.getD()); uint64_t page,pos;
    tie(buffer, page,pos)= getBufferPageAndPos(b);
    return (BitBasic::access(buffer.data(), pos, theStorage.bitsPerWord()))?1:0;
}

template<typename uint_t>
vector<int> ExternalBitvector<uint_t>::range(const uint_t i, const uint_t j){
//cout<< "ENTRANDO A RANGE DE EXTERNAL BIT VECTOR "<<endl;
    assert(i >= 0 && i < length);
    assert(j >= 0 && j < length);
    vector<int> result;

    uint_t bitsPerPage = theStorage.bitsPerPage();
    Page<uint_t> page(theStorage.getD());

    uint_t numPageIni = i / bitsPerPage;
    uint_t bit=i;
    uint_t numPageFin = j / bitsPerPage;
   // uint64_t posFin = j % bitsPerPage;32576
  // cout<< "i is: "<<i <<endl;
   // cout<< "j is: "<<j<<endl;

    //cout <<"numpageIni is: "<<numPageIni<<endl;
//cout << "numpageFin is:"<<numPageFin<<endl;
//cout << "bits per page is: "<< bitsPerPage <<endl;
    for(int numPage=numPageIni;numPage<=numPageFin;numPage++){
  //      cout <<"entre a la pagina:"<< numPage<<endl;
        page = theStorage.readPage(numPage);
    //    cout <<"Num page is: "<<numPage;
      //  cout <<"bit es:"<< bit<<" bit inside page:"<<bit %bitsPerPage<<endl;
        for(uint_t k=bit%bitsPerPage;k < bitsPerPage && bit<=j;bit++,k++){
        //    cout <<"bit es:"<< bit<<" bit inside page:"<<bit %bitsPerPage<<endl;

            if(BitBasic::access(page.data(), k , theStorage.bitsPerWord())){
       //         cout << "bit is: "<<bit;
                result.push_back(bit);
            }
      //      cout <<endl;
        }
    }
    //cout<< "fin A RANGE DE EXTERNAL BIT VECTOR "<<endl;

    return result;
}
/**
 * El método bitSet.
 *
 * @param b: posición del bitvector al que se quiere asignar el valor de 1.
 *
 * Se utiliza el método BitSet del ámbito BitBasic (ver BitBasic.h).
 */
template<typename uint_t>
void ExternalBitvector<uint_t>::bitSet(const uint_t b) {
    Page<uint_t> page(theStorage.getD());
    uint64_t numPage,pos;
    tie(page, numPage, pos) = getBufferPageAndPos(b);

    BitBasic::BitSet(page.data(), pos, theStorage.bitsPerWord());
    theStorage.updatePage(page, numPage);
    ones++; //puesto por fdo
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_ONES", to_string((unsigned long long )ones));  //puesto por fdo
}

/**
 * El método bitClean
 *
 * @param b: posición del bitvector al que se quiere asignar el valor de 0.
 *
 * Se utiliza el método BitClear del ámbito BitBasic (ver BitBasic.h).
 */
template<typename uint_t>
void ExternalBitvector<uint_t>::bitClean(const uint_t b) {
    Page<uint_t> buffer(theStorage.getD()); uint_t page,pos;
    tie(buffer, page,pos)= getBufferPageAndPos(b);

    BitBasic::BitClear(buffer.data(), pos, theStorage.bitsPerWord());
    theStorage.updatePage(buffer, page);
}

/**
 * El método rank.
 *
 * @param b: posición del bitvector hasta donde se quiere conocer el rank
 * @return: rank del bitvector hasta la posición b.
 *
 * Se utiliza el método Rank del ámbito BitBasic (ver BitBasic.h).
 */
template<typename uint_t>
uint_t ExternalBitvector<uint_t>::rank(const uint_t b) const {
    Page<uint_t> buffer(theStorage.getD()); uint_t page,pos;
    tie(buffer, page,pos)= getBufferPageAndPos(b);

    uint_t prevRank = buffer[theStorage.getD() - 1];
    return prevRank + BitBasic::Rank(buffer.data(), pos, theStorage.bitsPerWord());
}

/**
 * El método buildRank
 *
 * Se realiza Rank en cada página y se utiliza la ultima palabra de cada página para
 * guardar el acumulado.
 * También se guarda el acumulado total en el atributo ones y se almacena en metadata.
 */
//AGREGAR UN PRINT QUE MUESTRE EL AVANCE
template<typename uint_t>
void ExternalBitvector<uint_t>::buildRank(){
    uint_t paginas = theStorage.getPages();
    uint_t bitsPerPage = theStorage.bitsPerPage();
    uint_t bitsPerWord = theStorage.bitsPerWord();
    Page<uint_t> buffer(theStorage.getD());
    uint_t prevRank = 0;
    uint_t ori = 0;
    for (uint_t i = 0; i < paginas; i++){
        buffer = theStorage.readPage(i);
        buffer[theStorage.getD() - 1] = prevRank;
        prevRank += BitBasic::Rank(buffer.data(), bitsPerPage - 1, bitsPerWord);
        theStorage.updatePage(buffer, i);
    }
    ones=prevRank;
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_ONES", to_string((unsigned long long )ones));
}

/**
 * El método getLength
 *
 * @return se retorna la cantidad de bits que maneja el bitvector.
 */
template<typename  uint_t>
uint64_t ExternalBitvector<uint_t>::getLength() {
    return length;
}

/**
 * El método print.
 *
 * @param with: cantidad de caracteres a imprimir por linea.
 *
 * Se imprime el bitvector, con un espacio en blanco cada 4 bits (caracteres 0 o 1) y un
 * salto de linea cada "with" caracteres (1, 0 o espacio).
 */
template<typename uint_t>
void ExternalBitvector<uint_t>::print(int with) {
    //print ExternalBitVector
    cout << "(EXTERNAL_BIT_VECTOR_LENGTH " << length << ")" << endl;
    cout << "(EXTERNAL_BIT_VECTOR_ONES " << ones <<  ") only if builrank() was run!" << endl;
    for (int i = 4; i <= with; i += 4) {
        cout << fixed << setw(4) << setfill(' ') << std::right << i << " ";
    }
    cout << endl;
    for (uint_t i = 0; i < length; i++) {
        cout << access(i);
        if ((i + 1) % 4 == 0)cout << " ";
        if (((i + 1) % with) == 0) {
            cout << "  " << (i + 1) / with << endl;
        }
    }
    cout << endl;
}

/**
 * Método apendBlank
 *
 * @param N: Cantidad de páginas en blanco a insertar en el archivo.
 *
 * Inserta N páginas en blanco en el archivo del bitvector
 */
template<typename uint_t>
void ExternalBitvector<uint_t>::apendBlank(const uint_t N) {
    Page<uint_t> buffer(theStorage.getD());
    buffer.createBlock(true);//true para crear el buffer limpio
    for (uint_t i = 0; i < N; i++) {
        theStorage.appendPage(buffer);
    }
    length+=N;
    theStorage.setMeta("EXTERNAL_BIT_VECTOR_LENGTH", to_string((unsigned long long )this->length));
}

/**
 * Método getBufferPageAndPos
 * @tparam
 * @param b: Entrada del bitvector de la cual se quiere obtener los datos.
 * @return: El número de página y la posición en la página del bit b en el bitvector, junto
 *          a el objeto Page que almacena la dirección de memoria donde esta la página del bit b.
 */
template<typename uint_t>
inline tuple<Page<uint_t>, uint_t, uint_t> ExternalBitvector<uint_t>::getBufferPageAndPos(const uint_t b) const {
    assert(b >= 0 && b < length);
    uint_t bitsPerPage = theStorage.bitsPerPage();
    uint64_t posPage = b / bitsPerPage;
    uint64_t pos = b % bitsPerPage;
    //cout <<" buscando el bit  : "<< b ;
    Page<uint_t> page(theStorage.readPage(posPage));
    return make_tuple(page, posPage, pos);
}


#endif
