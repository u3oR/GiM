#include "Bus.h"
#include "string.h"

#define ram_size (64 * 1024)
static uint8_t nes_ram[ram_size];

static uint8_t Bus_Read(struct Bus *Bus, uint16_t addr, bool read_only) {
    if (addr>= 0x0000 && addr <= 0xFFFF){
        return Bus->ram[addr];
    }
    return 0x00;
}

static void Bus_Write(struct Bus *Bus, uint16_t addr, uint8_t data) {
    if (addr>= 0x0000 && addr <= 0xFFFF){
        Bus->ram[addr] = data;
    }
}


void Bus_Init(struct Bus *bus)
{
//    初始化总线
    bus->read = Bus_Read;
    bus->write = Bus_Write;
//    初始化CPU
    CPU6502_Init(bus->cpu);
//    挂载总线
    bus->cpu->connect_bus(bus->cpu, bus);
//    挂载内存
    bus->ram = nes_ram;
//    清空内存
    memset(bus->ram, 0, ram_size);
}
