// cpu.cpp
#include "cpu.hpp"
#include "alu.hpp"
#include <iostream>
#include <iomanip>

void dump_regs(const CPU& cpu) {
    std::cout << "PC: 0x" << std::hex << std::uppercase << cpu.pc << std::dec
              << " | A: " << (int)cpu.a 
              << " | B: " << (int)cpu.b 
              << " | Flags [ Z:" << cpu.f.z 
              << " N:" << cpu.f.n 
              << " C:" << cpu.f.c << " ]"
              << (cpu.halted ? " [HALTED]" : "") << "\n";
}

void step(CPU& cpu) {
    if (cpu.halted) {
        std::cout << "halted\n";
        return;
    }

    Byte op = mem_get(*cpu.mem, cpu.pc);

    // Розбір опкода на групу та індекс (старший і молодший півбайти)
    Byte group = (op >> 4) & 0x0F;
    Byte index = op & 0x0F;

    // Спочатку перевіримо спеціальні поодинокі інструкції (група 0x0_)
    if (group == 0x00) {
        switch (op) {
        case OP_HALT:
            cpu.halted = true;
            cpu.pc += 1;
            break;
        case OP_NOP:
            cpu.pc += 1;
            break;
        default:
            cpu.pc += 1; // невідома інструкція в групі 0
            break;
        }
        return;
    }

    // Група 0x1_ — це всі ALU операції (результат пишеться в регістр A)
    if (group == 0x01) {
        ALUResult res = {0, cpu.f};
        bool executed = true;

        switch (op) {
        case OP_ADD: res = alu_add(cpu.a, cpu.b); break;
        case OP_SUB: res = alu_sub(cpu.a, cpu.b); break;
        case OP_AND: res = alu_and(cpu.a, cpu.b); break;
        case OP_OR:  res = alu_or(cpu.a, cpu.b);  break;
        case OP_XOR: res = alu_xor(cpu.a, cpu.b); break;
        case OP_NOT: res = alu_not(cpu.a);        break;
        case OP_SHL: res = alu_shl(cpu.a, cpu.b); break;
        case OP_SHR: res = alu_shr(cpu.a, cpu.b); break;
        case OP_INC: res = alu_inc(cpu.a);        break;
        case OP_DEC: res = alu_dec(cpu.a);        break;
        default:
            executed = false;
            break;
        }

        if (executed) {
            cpu.a = res.value;
            cpu.f = res.flags;
            cpu.pc += 1; // Розмір ALU інструкцій у цій лабі — 1 байт
            return;
        }
    }

    // Якщо інструкція не розпізнана
    cpu.pc += 1;
}