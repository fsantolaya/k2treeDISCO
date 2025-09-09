/**
 *
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
 * Este header busca
 *
 * */

#ifndef BV_PAGE_H
#define BV_PAGE_H


#include <memory>
using namespace std;

/**
* Al instanciarse la clase recibe el parámetro por template:
* @tparam uint_t (entero sin signo)
* Que sirve para indicar la cantidad de bits que usa cada palabra.
*/
template<typename uint_t> class Page {
/**
 * La clase tiene 4 atributos privados.
 *
 * block: puntero que contiene los datos de la página.
 * D: cantidad de palabras que tiene la página.
 * update: bandera que indica si la página se ha actualizado.
 * num: número de la página (en el archivo).
 * freq: la frecuencia de consulta de la página.
 */
private:
    shared_ptr<uint_t[]> block= nullptr;
    int D=8;
    bool updated = false;
    int num=0;
    int freq=0;

/**
 * Métodos de la clase:
 *
 * Page(uint_t d)
 * Page(const Page &other)
 * void createBlock(bool blank=false)
 * void setChunk(uint32_t p, uint8_t chunk)
 * uint_t& operator=(const uint_t& other)
 * uint_t& operator[](uint_t b)
 * bool operator==(const Page &rhs)
 * bool operator!=(const Page &rhs)
 * uint_t * data()
 * uint_t getD()
 * bool isUpdated()
 * uint_t getId()
 * void setId(uint_t id)
 * uint_t getFrec()
 * void increaceFrec()
 * void startCountFrec()
 */
public:

    /**
     * Método Constructor.
     * @param d: Cantidad de palabras que tiene la página.
     *
     * Al atributo block se le asigna memoria solo con el método createBlock
     */
    explicit Page(uint_t d) : D(d), block(nullptr){ }
    Page(): D(8), updated(false), num(0), freq(1) { }
    /**
     * Método Constructor copia.
     *
     * @param other: Página que se quiere copiar.
     *
     * El operador asignación (=) esta sobrecargado en la clase shared_ptr. De forma que
     * no se copia el puntero almacenado, si no que se comparte.
     */
    Page(const Page &other){
        //share ptrPages, not copy
        block=other.block;
        D=other.D;
        updated=other.updated;
        num=other.num;
    }

    /**
     * Método createBlock.
     *
     * @param blank: Si se quiere la página en blanco (llena de ceros).
     *
     * Este método pide el espacio en memoria para la variable block
     */
    void createBlock(bool blank=false){
        if(blank){
            block= shared_ptr<uint_t[]>(new uint_t[D]{0});
        }else{
            block= shared_ptr<uint_t[]>(new uint_t[D]);
        }
    }

    //Se usa en External K2Tree
    void setChunk(uint32_t p, uint8_t chunk){
        uint_t * bitArray=block.get();
        //solo chunk útiles para datos (se excluye el word para rank)
        uint32_t chunkPerWord=sizeof(uint_t)*2;
        //incluye el word para rank
        uint32_t bitsPerWord=sizeof(uint_t)*8;
        int i = p / chunkPerWord;
        int j = p % chunkPerWord;
        uint_t mask = chunk;
        bitArray[i]|=mask << bitsPerWord-4-(4*j);
       // cout<<"unt_t:"<< i<<" chunk:"<<j<<" "<<endl;
    }

    /**
     * Método sobrecarga de asignación.
     *
     * @param other: página que se quiere asignar al objeto actual.
     *
     * @return referencia al objeto actual.
     *
     * Se comparte el block (pues la asignación en shared_ptr esta sobrecargada) y se
     * copian los demás atributos.
     */
    Page<uint_t>& operator=(const Page<uint> other){
        block=other.block;
        D=other.D;
        updated=other.updated;
        num=other.num;
        return *this;
    }

    /**
     * Método sobrecarga de asignación con referencia como parámetro.
     *
     * @param other: referencia a otra página.
     * @return referencia al objeto actual.
     *
     * Se comparte el block (pues la asignación en shared_ptr esta sobrecargada) y se
     * copian los demás atributos.
     *
     * Esto permite cosas como a = (b = c).
     */
    Page<uint_t>& operator=(const Page<uint_t>& other){
        block=other.block;
        D=other.D;
        updated=other.updated;
        num=other.num;
        return *this;
    }

    /**
     * Método sobrecarga del operador []
     *
     * @param b: posición de la palabra a la que se quiere acceder.
     * @return palabra de la página a la que se quiere acceder.
     */
    uint_t& operator[](uint_t b) const{
        return block[b];
    }

    /**
     * Método sobrecarga del operador ==
     *
     * @param rhs referencia de la página que se quiere comparar.
     * @return verdadero si comparten el atributo block y si los demás atributos almacenan
     *         los mismos valores. Falso en cualquier otro caso.
     */
    bool operator==(const Page &rhs) const {
        return block == rhs.block &&
               D == rhs.D &&
               updated == rhs.updated && num=rhs.num;
    }

    /**
     * Método sobrecarga del operador !=
     *
     * @param rhs referencia de la página que se quiere comparar.
     * @return lo contrario al resultado del operador ==.
     */
    bool operator!=(const Page &rhs) const {
        return !(rhs == *this);
    }

    /**
     * Método data.
     *
     * @return puntero que almacena, el objeto shared_ptr, block.
     */
    uint_t * data() {
        return block.get();
    }

    /**
     * Método getD.
     *
     * @return cantidad de palabras que tiene la página.
     */
    uint_t getD() const {
        return D;
    }

    /**
     * Método isUpdated.
     *
     * @return verdadero si la página se ha actualizado. Falso en caso contrario.
     */
    bool isUpdated(){
        return updated;
    }

    /**
     * Método setUpdated.
     *
     * Prende la flag updated para indicar que se ha actualizado la página.
     */
    void setUpdated(){
        updated=true;
    }

    /**
     * Método getId.
     *
     * @return número de la página (en el archivo).
     */
    uint_t getId() const {
        return num;
    }

    /**
   * Método isEmpty.
   *
   * @return true si la pagina  NO esta inicializada.
   */
    bool isEmpty() const {
        return block == nullptr;
    }

    /**
     * Método setId.
     *
     * @param id: número de la página (en el archivo)
     *
     * Se le asigna el número de página que ocupa en el archivo.
     * Se utiliza cuando se ha creado recién el objeto.
     */
    void setId(uint_t id) {
        num = id;
    }

    /**
     * Método getFrec.
     *
     * @return La frecuencia con la que se ha consultado la página.
     *
     */
    uint_t getFrec() {
        return freq;
    }

    /**
     * Método increaceFrec.
     *
     * Se utiliza cuando se consulta la página, para aumentar su frecuencia de consulta.
     */
    void increaceFrec(){
        freq++;
    }

    /**
     * Método startCountFrec.
     *
     * Se utiliza cuando se consulta por primera vez la página.
     */
    void startCountFrec(){
        freq=1;
    }
};


#endif
