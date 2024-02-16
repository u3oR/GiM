#ifndef NES_INSTRUCTIONS_H
#define NES_INSTRUCTIONS_H

#include <stdint.h>
#include <CPU6502.h>

#define table_size 256


// ***************************************************
// 寻址模式
//uint8_t IMP();	uint8_t IMM();
//uint8_t ZP0();	uint8_t ZPX();
//uint8_t ZPY();	uint8_t REL();
//uint8_t ABS();	uint8_t ABX();
//uint8_t ABY();	uint8_t IND();
//uint8_t IZX();	uint8_t IZY();
// ***************************************************

#define read(addr) cpu->_read(cpu, addr, false)

inline uint8_t IMP(struct CPU6502 *cpu){
    cpu->fetched = cpu->a;
    return 0;
}
inline uint8_t IMM(struct CPU6502 *cpu){
    cpu->addr_abs = cpu->pc++;
    return 0;
}

inline uint8_t ZP0(struct CPU6502 *cpu){
    cpu->addr_abs = read(cpu->pc++) & 0x00FF;
    return 0;
}
inline uint8_t ZPX(struct CPU6502 *cpu){
    cpu->addr_abs = ( read(cpu->pc++) + cpu->x ) & 0x00FF;
    return 0;
}
inline uint8_t ZPY(struct CPU6502 *cpu){
    cpu->addr_abs = ( read(cpu->pc++) + cpu->y ) & 0x00FF;
    return 0;
}
// 绝对地址
inline uint8_t ABS(struct CPU6502 *cpu){
    uint16_t lo = read(cpu->pc++);
    uint16_t hi = read(cpu->pc++);
    cpu->addr_abs = (hi << 8 | lo);
    return ((cpu->addr_abs & 0xFF00) != (hi << 8)) ? 1 : 0;
}
inline uint8_t ABX(struct CPU6502 *cpu){
    uint16_t lo = read(cpu->pc++);
    uint16_t hi = read(cpu->pc++);
    cpu->addr_abs = (hi << 8 | lo) + cpu->x;
    return ((cpu->addr_abs & 0xFF00) != (hi << 8)) ? 1 : 0;
}
inline uint8_t ABY(struct CPU6502 *cpu){
    uint16_t lo = read(cpu->pc++);
    uint16_t hi = read(cpu->pc++);
    cpu->addr_abs = (hi << 8 | lo) + cpu->y;
    return ((cpu->addr_abs & 0xFF00) != (hi << 8)) ? 1 : 0;
}
// 间接寻址
inline uint8_t IND(struct CPU6502 *cpu)
{
    uint16_t p_lo = read(cpu->pc++);
    uint16_t p_hi = read(cpu->pc++);
    uint16_t p = (p_hi << 8) | p_lo;
    if (p_lo == 0x00FF){
        cpu->addr_abs = (read(p & 0xFF00) << 8 | read(p));
    } else{
        cpu->addr_abs = (read(p + 0x0001) << 8 | read(p));
    }
    return 0;
}

inline uint8_t IZX(struct CPU6502 *cpu)
{
    uint16_t t = read(cpu->pc++);
    uint16_t lo = read((t + (uint16_t)cpu->x + 0) & 0x00FF);
    uint16_t hi = read((t + (uint16_t)cpu->x + 1) & 0x00FF);
    cpu->addr_abs = ((hi << 8) | lo);
    return 0;
}
inline uint8_t IZY(struct CPU6502 *cpu)
{
    uint16_t t = read(cpu->pc++);
    uint16_t lo = read((t + 0) & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);
    cpu->addr_abs = ((hi << 8) | lo) + cpu->y;
    return ((cpu->addr_abs & 0xFF00) != (hi << 8)) ? 1 : 0;
}
// 相对寻址
inline uint8_t REL(struct CPU6502 *cpu)
{
    cpu->addr_rel = read(cpu->pc++);
    if (cpu->addr_rel & 0x80){
        cpu->addr_rel |= 0xFF00;
    }
    return 0;
}

#undef read
// ***************************************************
// 操作码
// uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
// uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
// uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
// uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
// uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
// uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
// uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
// uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
// uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
// uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
// uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
// uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
// uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
// uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();
// ***************************************************

#define set_flag(BIT, level)    cpu->_set_flag(cpu, BIT, level)
#define get_flag(BIT)           cpu->_get_flag(cpu,BIT)

inline uint8_t AND(struct CPU6502 *cpu)
{
    // 取指令
    cpu->fetch(cpu);
    // 与运算
    cpu->a &= cpu->fetched;
    // 如果与运算结果为0,则设置相应的寄存器
    set_flag(ZERO_BIT, cpu->a == 0);
    set_flag(NEGATIVE_BIT, cpu->a & 0x80);

    return 1;
}
inline uint8_t BCS(struct CPU6502 *cpu)
{
    if (get_flag(CARRY_BIT) == 1){
        cpu->cycles += 1;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;
        if ((cpu->addr_abs & 0xFF00) != (cpu->pc & 0xFF00)){
            cpu->cycles += 1;
        }
        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

inline uint8_t ORA(struct CPU6502 *cpu){

}

#undef set_flag
#undef get_flag

#endif //NES_INSTRUCTIONS_H
