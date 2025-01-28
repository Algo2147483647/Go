#include "zobristHash.h"

uint64_t Go::randuint64() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;
    return dis(gen);
}

void Go::zobrist_init() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < BOARD_SIZE * BOARD_SIZE; j++) {
            zobristTable[i][j] = randuint64();
        }
    }
};

uint64_t& Go::zobrist_hash(uint64_t& hashValue, int p, Color color) {
    hashValue ^= zobristTable[color + 1][p];
    return hashValue;
}