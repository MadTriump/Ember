#include "memory.hpp"
// Функція mem_get читає один байт із пам'яті за вказаною адресою
Byte mem_get(const Memory& mem, std::size_t addr) {
    // Перевіряємо, чи адреса не виходить за межі 4096 байтів
    if (addr >= MEM_SIZE) {
        return 0; // Якщо адреса неправильна, повертаємо 0
    }
    // Якщо адреса правильна, повертаємо значення з пам'яті
    return mem.data[addr];
}

// Функція mem_set записує один байт у пам'ять за вказаною адресою
bool mem_set(Memory& mem, std::size_t addr, Byte value) {
    // Якщо адреса поза межами пам'яті (>= 4096), відхиляємо запис
    if (addr >= MEM_SIZE) {
        return false; // Повідомляємо про помилку
    }
    mem.data[addr] = value; // Записуємо нове значення у комірку
    return true;            // Успішно записали
}
