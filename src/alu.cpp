// alu.cpp
#include "alu.hpp"

// Допоміжна функція для встановлення загальних прапорців Z та N
static Flags compute_zn(std::uint16_t full_res, std::uint8_t res) {
    Flags f;
    f.z = (res == 0);
    f.n = (res & 0x80) != 0; // старший біт (біт 7)
    f.c = (full_res > 0xFF); // перевірка на вихід за 8 бітів
    return f;
}

ALUResult alu_add(std::uint8_t a, std::uint8_t b) {
    std::uint16_t full = (std::uint16_t)a + b;
    std::uint8_t res = (std::uint8_t)full;
    Flags f = compute_zn(full, res);
    return {res, f};
}

ALUResult alu_sub(std::uint8_t a, std::uint8_t b) {
    int full = (int)a - b;
    std::uint8_t res = (std::uint8_t)full;
    Flags f;
    f.z = (res == 0);
    f.n = (res & 0x80) != 0;
    f.c = (a < b); // borrow (перенесення при відніманні)
    return {res, f};
}

ALUResult alu_and(std::uint8_t a, std::uint8_t b) {
    std::uint8_t res = a & b;
    Flags f;
    f.z = (res == 0);
    f.n = (res & 0x80) != 0;
    f.c = false;
    return {res, f};
}

ALUResult alu_or(std::uint8_t a, std::uint8_t b) {
    std::uint8_t res = a | b;
    Flags f;
    f.z = (res == 0);
    f.n = (res & 0x80) != 0;
    f.c = false;
    return {res, f};
}

ALUResult alu_xor(std::uint8_t a, std::uint8_t b) {
    std::uint8_t res = a ^ b;
    Flags f;
    f.z = (res == 0);
    f.n = (res & 0x80) != 0;
    f.c = false;
    return {res, f};
}

ALUResult alu_not(std::uint8_t a) {
    std::uint8_t res = ~a;
    Flags f;
    f.z = (res == 0);
    f.n = (res & 0x80) != 0;
    f.c = false;
    return {res, f};
}

ALUResult alu_shl(std::uint8_t a, std::uint8_t b) {
    // Зсув на кількість бітів з b (або просто на 1, якщо бідний процесор)
    unsigned int shift = b & 0x07;
    std::uint16_t full = (std::uint16_t)a << shift;
    std::uint8_t res = (std::uint8_t)full;
    Flags f;
    f.z = (res == 0);
    f.n = (res & 0x80) != 0;
    // Біт перенесення — останній випущений біт
    f.c = (shift > 0) ? ((a & (1u << (8 - shift))) != 0) : false;
    return {res, f};
}

ALUResult alu_shr(std::uint8_t a, std::uint8_t b) {
    unsigned int shift = b & 0x07;
    std::uint8_t res = a >> shift;
    Flags f;
    f.z = (res == 0);
    f.n = (res & 0x80) != 0;
    f.c = (shift > 0) ? ((a & (1u << (shift - 1))) != 0) : false;
    return {res, f};
}

ALUResult alu_inc(std::uint8_t a) {
    return alu_add(a, 1);
}

ALUResult alu_dec(std::uint8_t a) {
    return alu_sub(a, 1);
}