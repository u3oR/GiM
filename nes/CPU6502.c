#include "CPU6502.h"
#include "instructions.h"

// defined in "instructions.h"
extern struct Instruction *instruction_table;


static inline uint8_t CPU_Read(struct CPU6502 *cpu6502, uint16_t addr, bool read_only)
{
    return cpu6502->bus->read(cpu6502->bus, addr, read_only);
}

static inline void CPU_write(struct CPU6502 *cpu6502, uint16_t addr, uint8_t data)
{
    cpu6502->bus->write(cpu6502->bus, addr, data);
}

static inline void CPU_ConnectBus(struct CPU6502 *cpu6502, struct Bus *bus)
{
    cpu6502->bus = bus;
}

static inline void CPU_SetFlag(struct CPU6502 *cpu, enum CPU6502_FLAGS flag, bool level)
{
    if (level){
        cpu->status |= flag;
    } else{
        cpu->status &= ~flag;
    }
}

static inline uint8_t CPU_GetFlag(struct CPU6502 *cpu, enum CPU6502_FLAGS flag)
{
    return ((cpu->status & flag) > 0 ? 1 : 0);
}

// 取指
uint8_t CPU_Fetch(struct CPU6502 *cpu)
{
    // 当取指模式为IMP时，不执行此取操作
    if (cpu->_lookup[cpu->opcode].addrmode != IMP){
        cpu->fetched = cpu->_read(cpu, cpu->addr_abs ,false);
    }
    return cpu->fetched;
}

/**
 * CPU时钟
 * @param cpu
 */
static void CPU_Clock(struct CPU6502 *cpu)
{
    if (cpu->cycles == 0){
        // 读取PC指针的指令
        cpu->opcode = cpu->_read(cpu, cpu->pc, false);
        // PC自增
        cpu->pc += 1;
        // 获取指令对应的指令周期
        cpu->cycles = cpu->_lookup[cpu->opcode].cycles;
        // 执行对应的指令
        cpu->_lookup[cpu->opcode].addrmode(cpu);   // 寻址指令
        cpu->_lookup[cpu->opcode].operate(cpu);    // 操作指令
    }
    cpu->cycles -= 1;
}
// Reset
static void CPU_Reset(struct CPU6502 *cpu)
{
    cpu->status = cpu->status;
}
// Interrupt Request
static void CPU_Irq(struct CPU6502 *cpu)
{
    cpu->status = cpu->status;
}

// Non Maskable Interrupt
static void CPU_Nmi(struct CPU6502 *cpu)
{
    cpu->status = cpu->status;
}



void CPU6502_Init(struct CPU6502 *cpu)
{
    // 初始化寄存器
    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->sp = 0;
    cpu->pc = 0;
    cpu->status = 0;
    //
    cpu->addr_rel = 0;   // 绝对地址
    cpu->addr_abs = 0;   // 相对地址
    cpu->opcode = 0;     // 操作码
    cpu->cycles = 0;     // 指令周期
    cpu->fetched = 0;

    // 初始化函数指针
    cpu->clock   = CPU_Clock;    // 时钟
    cpu->reset   = CPU_Reset;    // 复位
    cpu->irq     = CPU_Irq;      // 中断
    cpu->nmi     = CPU_Nmi;      // 不可屏蔽中断

    cpu->connect_bus = CPU_ConnectBus;
    cpu->_write      = CPU_write;
    cpu->_read       = CPU_Read;
    cpu->_get_flag   = CPU_GetFlag;
    cpu->_set_flag   = CPU_SetFlag;
    cpu->fetch       = CPU_Fetch;
    // 指令表
    cpu->_lookup     = instruction_table;
}

void CPU6502_DeInit(struct CPU6502 *cpu)
{
    cpu->status = cpu->status;
}

