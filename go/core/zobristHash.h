#ifndef GO_ZOBRISTHASH_H
#define GO_ZOBRISTHASH_H

#include <random>
#include "config.h"
using namespace std;

namespace Go {
    static array<array<uint64_t, BOARD_SIZE * BOARD_SIZE>, 3> zobristTable;

    uint64_t randuint64();
    void zobrist_init();
    uint64_t& zobrist_hash(uint64_t& hashValue, int p, Color color);
}
#endif