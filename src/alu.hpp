// alu.hpp
#pragma once
#include <cstdint>
#include "cpu.hpp" // містить структуру Flags

// Структура для результату ALU (значення + прапорці)
struct ALUResult {
    std::uint8_t value;
    Flags flags;
};

// Оголошення функцій ALU
ALUResult alu_add(std::uint8_t a, std::uint8_t b);
ALUResult alu_sub(std::uint8_t a, std::uint8_t b);
ALUResult alu_and(std::uint8_t a, std::uint8_t b);
ALUResult alu_or (std::uint8_t a, std::uint8_t b);
ALUResult alu_xor(std::uint8_t a, std::uint8_t b);
ALUResult alu_not(std::uint8_t a);
ALUResult alu_shl(std::uint8_t a, std::uint8_t b);
ALUResult alu_shr(std::uint8_t a, std::uint8_t b);
ALUResult alu_inc(std::uint8_t a);
ALUResult alu_dec(std::uint8_t a);