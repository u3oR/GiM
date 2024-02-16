#include "instructions.h"

static struct Instruction instruction_table[table_size] = {
        {"IMP", IMP, IMP, 7},
        {"ORA", ORA, IZX, 6},
};
