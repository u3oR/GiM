#ifndef NES_BUS_H
#define NES_BUS_H

#include <stdint.h>
#include <stdbool.h>

#include "CPU6502.h"

struct Bus{
    struct CPU6502 *cpu;

    uint8_t *ram;
// 读写函数
    uint8_t (*read)(struct Bus *bus, uint16_t addr, bool read_only);
    void (*write)(struct Bus *bus, uint16_t addr, uint8_t data);
};

void Bus_Init(struct Bus *bus);
void Bus_DeInit(struct Bus *bus);

#endif //NES_BUS_H
