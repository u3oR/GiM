#ifndef NES_CPU6502_H
#define NES_CPU6502_H

#include <stdint.h>
#include <stdbool.h>
#include "Bus.h"

enum CPU6502_FLAGS{
    CARRY_BIT       = 0x01, // carry bit
    ZERO_BIT        = 0x02, // zero bit
    ITDIS_BIT       = 0x04, // interrupt disabled bit
    DECIMAL_BIT     = 0x08, // decimal mode bit (exactually unused)
    BREAK_BIT       = 0x10, // break command bit
    OVERFLOW_BIT    = 0x40, // overflow bit
    NEGATIVE_BIT    = 0x80  // negative bit
};

struct Instruction{
    char name[3];
    uint8_t (*operate)(struct CPU6502 *cpu);
    uint8_t (*addrmode)(struct CPU6502 *cpu);
    uint8_t cycles;
};

struct CPU6502{
    struct Bus *bus;//挂载到总线

    uint8_t a;  // 寄存器A
    uint8_t x;  // 寄存器X
    uint8_t y;  // 寄存器Y
    uint8_t sp; // 栈指针
    uint16_t pc; // 程序计数器
    uint8_t status; // 状态寄存器

    void (*connect_bus)(struct CPU6502 *cpu, struct Bus *bus); // 挂载到总线

    void (*clock)(struct CPU6502 *cpu);    // 时钟
    void (*reset)(struct CPU6502 *cpu);    // 复位
    void (*irq)(struct CPU6502 *cpu);      // 中断
    void (*nmi)(struct CPU6502 *cpu);      // 不可屏蔽中断
// private variables
    const struct Instruction *_lookup; // 指令表
    uint8_t (*fetch)(struct CPU6502 *cpu);
    uint8_t fetched;
    uint16_t addr_abs;// 绝对地址
    uint16_t addr_rel;// 相对地址
    uint8_t opcode; // 操作码
    uint8_t cycles;

// private functions
    void (*_write)(struct CPU6502 *cpu, uint16_t addr, uint8_t data);
    uint8_t (*_read)(struct CPU6502 *cpu, uint16_t addr, bool read_only);
    void (*_set_flag)(struct CPU6502 *cpu, enum CPU6502_FLAGS flag, bool level);
    uint8_t (*_get_flag)(struct CPU6502 *cpu, enum CPU6502_FLAGS flag);
};

void CPU6502_Init(struct CPU6502 *cpu6502);
void CPU6502_DeInit(struct CPU6502 *cpu6502);



#endif //NES_CPU6502_H
