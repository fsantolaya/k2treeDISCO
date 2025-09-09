/**
 *
 * ****************************** English *********************************
 *
 * Header credits.
 * Programmer: Miguel Romero.
 * Based on:
 * https://github.com/fclaude/libcds2/blob/master/include/libcds2/libcds.h
 * Documented by Carlos Céspedes Cárdenas.
 *
 * This header was created to group in one file the basic operations of a
 * bitvector alocated in main memory.
 * The scope BitBasic is created to contain this operations.
 *
 *
 * ****************************** Español *********************************
 *
 * Créditos del header.
 * Programador: Miguel Romero.
 * Basado en:
 * https://github.com/fclaude/libcds2/blob/master/include/libcds2/libcds.h
 * Documentado por: Carlos Céspedes Cárdenas.
 *
 * Este header busca agrupar en un archivo las operaciones básicas que posee
 * un bitvector que se aloja en memoria principal.
 * Se crea el ámbito BitBasic el cual contiene estas operaciones.
 *
 * */


#ifndef BV_BITBASIC_H
#define BV_BITBASIC_H

#include <bit>
#include <cmath>
#include <vector>
#include <bitset>
#include <iostream>
#include <cstdint>


namespace BitBasic {
    /**
    * Cada método recibe el parámetro template:
    * @tparam uint_t (entero sin signo)
    * Que sirve para indicar la cantidad de bits que usa cada palabra.
    */

    /**
     * El método access recibe los siguientes parámetros.
     *
     * @param e: arreglo donde está alojado el bitvector.
     * @param p: posición del bit de quien se quiere conocer su valor (0 o 1).
     * @param W: cantidad de bits que ocupa cada palabra en el arreglo e.
     * @return el valor del bit posicionado en p.
     *
     * Para entender como obtiene este valor, primero hay que notar que
     * el bit posicionado en p está ubicado en la palabra p/W del arreglo.
     * Después hay que notar que, de los W bits que hay en la palabra p/W,
     * el que ocupa la posición p en el arreglo, ocupa la posición p%W en
     * la palabra de izquierda a derecha contando del 0.
     * Así ocupando el operador bitwise & con los parámetros:
     * 1) la palabra e[p/W]
     * 2) la palabra de W bits que tiene un 1 en el bit p%w y 0 en los
     *    demás (que es la posición (W-1)-(p%W) de derecha a izquierda).
     * Podemos obtener 2 resultados:
     * 1) 0 si hay un 0 en el bit p%W de la palabra e[p/W].
     * 2) Algo distinto de 0 si hay un 1 en el bit p%W (el segundo
     *    parámetro descrito arriba específicamente).
     * */

    template<typename uint_t>
    inline bool access(const uint_t *e, const uint_t p, const int W) {
        return 0 != (e[p / W] & ((uint_t) 1 << ((W - 1) - (p % W))));
    }

    /**
     * El método BitSet recibe los siguientes parámetros.
     *
     * @param e: arreglo donde está alojado el bitvector.
     * @param p: posición del bit de quien se quiere asignar valor de 1.
     * @param W: cantidad de bits que ocupa cada palabra en el arreglo e.
     *
     * Y asigna el valor de 1 al bit posicionado en p (prende el bit).
     * Para entender como prende el bit, primero hay que notar que
     * el bit posicionado en p está ubicado en la palabra p/W del arreglo.
     * Después hay que notar que, de los W bits que hay en la palabra p/W,
     * el que ocupa la posición p en el arreglo, ocupa la posición p%W en
     * la palabra de izquierda a derecha contando del 0.
     * Así ocupando el operador bitwise | con los parámetros:
     * 1) la palabra e[p/W]
     * 2) la palabra de W bits que tiene un 1 en el bit p%w y 0 en los
     *    demás (que es la posición (W-1)-(p%W) de derecha a izquierda).
     * Prendemos el bit que queríamos.
     * */

    template<typename uint_t>
    inline void BitSet(uint_t *e, const uint_t p, const int W) {
        e[p / W] |= ((uint_t) 1 << ((W - 1) - (p % W)));
    }

    /**
     * El método BitClear recibe los siguientes parámetros.
     *
     * @param e: arreglo donde está alojado el bitvector.
     * @param p: posición del bit de quien se quiere asignar el valor de 0.
     * @param W: cantidad de bits que ocupa cada palabra en el arreglo e.
     *
     * Y asigna el valor de 0 al bit posicionado en p (apaga el bit).
     * Para entender como apaga el bit, primero hay que notar que
     * el bit posicionado en p está ubicado en la palabra p/W del arreglo.
     * Después hay que notar que, de los W bits que hay en la palabra p/W,
     * el que ocupa la posición p en el arreglo, ocupa la posición p%W en
     * la palabra de izquierda a derecha contando del 0.
     * Así ocupando el operador bitwise | con los parámetros:
     * 1) la palabra e[p/W]
     * 2) la palabra de W bits que tiene un 0 en el bit p%w y 1 en los
     *    demás (que es la posición (W-1)-(p%W) de derecha a izquierda).
     * Apagamos el bit que queríamos y los demás quedan como estaban.
     * */

    template<typename uint_t>
    inline void BitClear(uint_t *e, const uint_t p, const int W) {
        e[p / W] &= ~((uint_t) 1 << ((W - 1) - (p % W)));
    }

    /**
     * El método Rank recibe los siguientes parámetros.
     *
     * @param e: arreglo donde está alojado el bitvector.
     * @param p: posición de un bit en el arreglo e.
     * @param W: cantidad de bits que ocupa cada palabra en el arreglo e.
     * @return la cantidad de bits en 1 (prendidos) que hay en el
     *         arreglo hasta la posición p.
     *
     * Para entender como cuenta cuantos bits están prendidos, primero hay
     * que notar que el bit posicionado en p está ubicado en la palabra
     * p/W del arreglo.
     * Después hay que notar que, de los W bits que hay en la palabra p/W,
     * el que ocupa la posición p en el arreglo, ocupa la posición p%W en
     * la palabra de izquierda a derecha contando del 0.
     * Y por último que la palabra e[p / W] >> ((W - 1) - (p % W)) desecha
     * los (W - 1) - (p % W) bits posteriores al bit p%W en la palabra.
     * Así sumando la cantidad de bits prendidos que hay en cada palabra
     * hasta la anterior a p/W (esto se hace con popcount que retorna la
     * cantidad de bits prendidos en una palabra), con la cantidad de bits
     * prendidos en la palabra (e[p / W] >> (W - 1 - (p % W))), obtenemos
     * la cantidad de bits prendidos hasta p.
     * */
    template<typename uint_t>
    inline uint_t Rank(uint_t *e, const uint_t p, const int W) {
        uint_t sum = 0;
        for (auto i = 0; i < (p / W); i++) {
            sum += popcount(e[i]);
        }
        sum += popcount(e[p / W] >> ((W - 1) - (p % W)));
        return sum;
    }

    uint64_t MASK_11 = 0b11;

    inline uint64_t getGroup(uint64_t x, int height) {
        return x >> (2 * height + 2);
    }

    //Hijo del nodo viendo el bitvector como un k2-tree
    inline uint8_t getChild(uint64_t x, int height) {
        return (uint8_t) ((x >> (2 * height)) & MASK_11);
    }
//A array, p position, chunk is bitarray to set
//      i=       0         1         2         3
//ejemplo: [0000,0000|0000,0000|0000,0000|0000,0000]
//    P=     0    1     2   3    4    5    6    7
//set chunk=[0010], in p=3
//     i=       0         1         2         3
//         [0000,0000|0000,0000|0000,0000|0000,0000]
//   p=      0    1     2   3    4    5    6    7
//               OR [0000,0010]
//set chunk=[0010], in p=4
//     i=       0         1         2         3
//         [0000,0000|0000,0000|0000,0000|0000,0000]
//     p=    0    1     2   3    4    5    6    7
//                         OR [0010,0000]

    inline void setChunk(uint8_t *A, uint32_t p, uint8_t chunk) {
        int i = p / 2;
        uint8_t byteOri = A[i];
        if ((p & 1) == 0) {//0 (even) is hi bits position chunk
            A[i] |= (chunk << 4);
        } else {
            A[i] |= chunk;
        }
        cout << "Guardando Chunk " << (p+1) <<"° (" << bitset<4>(chunk)
             << ") en el byte " << i << " del buffer. "
             << " valor antes: " << bitset<8>(byteOri)
             << " valor ahora: " << bitset<8>(A[i]) << endl;
    }


//z-order, from web:http://www-graphics.stanford.edu/~seander/bithacks.html#InterleaveBMN
// Interleave lower 16 bits of x and y, so the bits of x
// are in the even positions and bits from y in the odd;
// x and y must initially be less than 65536.
//return z, z gets the resulting 32-bit Morton Number.

    inline uint32_t zorder(uint32_t x, uint32_t y) {
        const static uint32_t B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
        const static uint32_t S[] = {1, 2, 4, 8};

        uint32_t z = 0; // z gets the resulting 32-bit Morton Number.
        // x and y must initially be less than 65536.

        x = (x | (x << S[3])) & B[3];
        x = (x | (x << S[2])) & B[2];
        x = (x | (x << S[1])) & B[1];
        x = (x | (x << S[0])) & B[0];

        y = (y | (y << S[3])) & B[3];
        y = (y | (y << S[2])) & B[2];
        y = (y | (y << S[1])) & B[1];
        y = (y | (y << S[0])) & B[0];

        z = x | (y << 1);

        return z;
    }


    uint64_t zorder64(uint32_t x, uint32_t y) {
        uint64_t z_low = zorder(x, y);
        uint64_t z_hi = zorder(x >> 16, y >> 16);
        return (z_hi << 32) | z_low;
    }

    //retorna 2 uint64_t
    //out[0] es low, out[1] es hi.
    vector<uint64_t> zorder128(uint64_t x, uint64_t y) {
        vector<uint64_t> out;
        uint32_t _x = (uint32_t) x;
        uint32_t _y = (uint32_t) y;
        out[0] = zorder64(_x, _y);
        _x = (uint32_t) (x >> 32);
        _y = (uint32_t) (y >> 32);
        out[1] = zorder64(_x, _y);
        return out;
    }

    //se invierte el orden del par (x,y) en los parámetros, para hacerlo compatible con el model
    //del k2tree de (fila, columna), dado que x es la columna y la y es la fila
    vector<uint64_t> generateZOrder(vector<uint32_t> &Y, vector<uint32_t> &X) {
        vector<uint64_t> output;
        for (size_t i = 0; i < X.size(); i++) {
            output.push_back(zorder64(X[i], Y[i]));
        }
        return output;
    }
}
#endif