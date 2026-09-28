#include "dump.hpp"

#include <iomanip>
#include <iostream>

// Скільки байтів друкується в одному рядку
const std::size_t BYTES_PER_LINE = 16;

// Перевіряє, чи є байт друкованим символом
static bool is_printable(Byte b) { return b >= 0x20 && b <= 0x7E; }

void dump(const Memory& mem) {
  
    for (std::size_t row = 0; row < MEM_SIZE; row += BYTES_PER_LINE) {
        
        // Стовпчик адреси: 0000, 0010, 0020, ...
        std::cout << std::hex << std::setfill('0') << std::setw(4) << row << "  ";

        // Стовпчик з байтами в hex: 16 байтів
        for (std::size_t col = 0; col < BYTES_PER_LINE; ++col) {
            Byte b = mem.data[row + col];
            std::cout << std::setw(2) << static_cast<int>(b) << ' ';
        }

        std::cout << " |";

        // Стовпчик з ASCII-символами
        for (std::size_t col = 0; col < BYTES_PER_LINE; ++col) {
            Byte b = mem.data[row + col];
            // M2: Якщо байт друкований, виводимо сам символ b, інакше крапку
            if (is_printable(b)) {
                std::cout << static_cast<char>(b);
            } else {
                std::cout << '.';
            }
        }

        std::cout << "|\n";
    }
    // Повертаємо потік у стандартний десятичний режим
    std::cout << std::dec << std::setfill(' ');
}

void show_byte(Byte b) {
   // 1. Десятичне значення
    std::cout << std::dec << static_cast<int>(b) << "  ";

    // 2. Шістнадцяткове значення (Hex) з префіксом 0x
    std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(b) << "  ";

    // 3. Двійкове значення (Binary) з префіксом 0b
    std::cout << "0b";
    for (int i = 7; i >= 0; --i) {
        std::cout << ((b >> i) & 1);
    }
    std::cout << "  ";

    // 4. Символ у лапках
    std::cout << '\'';
    if (is_printable(b)) {
        std::cout << static_cast<char>(b);
    } else {
        std::cout << '.';
    }
    std::cout << "'\n";

    // Відновлення стандартного налаштування форматування
    std::cout << std::dec << std::setfill(' ');
    
}
