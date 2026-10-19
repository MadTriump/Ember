// main.cpp — виправлена версія для Lab 2
#include <iostream>
#include <sstream>
#include <string>

#include "dump.hpp"
#include "memory.hpp"
#include "cpu.hpp" // Підключаємо процесор для CPU, dump_regs, step

// Turn a word into a number. Accepts decimal (65) and hex (0x41).
// Returns false if the word is not a number at all.
static bool parse_number(const std::string& word, long& out) {
    try {
        std::size_t used = 0;
        // base 0 means: look at the prefix. "0x41" is hex, "65" is decimal.
        out = std::stol(word, &used, 0);
        return used == word.size();  // reject things like "12abc"
    } catch (...) {
        return false;
    }
}

static void print_help() {
    std::cout << "commands:\n"
              << "  dump              print all " << MEM_SIZE << " bytes\n"
              << "  get <addr>        show one byte four ways\n"
              << "  set <addr> <val>  write one byte (dec or 0x hex)\n"
              << "  reg <a|b> <val>   set register a or b (0..255)\n"
              << "  regs              show cpu registers and flags\n"
              << "  step              execute one instruction\n"
              << "  help              this list\n"
              << "  quit              leave\n";
}

int main() {
    Memory mem;  // 4096 bytes, on the stack, zeroed by the {} in memory.hpp
    
    CPU cpu;
    cpu.mem = &mem;   // «ось твоя пам'ять»

    std::cout << "ember 0.1 - 4096 bytes of memory you can see. Type `help`.\n";

    std::string line;
    while (true) {
        std::cout << "ember> ";

        // getline reads one whole line. It returns false at end of input
        if (!std::getline(std::cin, line)) {
            std::cout << '\n';
            break;
        }

        // Split the line into words
        std::istringstream words(line);
        std::string cmd;
        words >> cmd;

        if (cmd.empty()) {
            continue;  // the user just pressed Enter
        } else if (cmd == "quit" || cmd == "exit") {
            break;
        } else if (cmd == "help") {
            print_help();
        } else if (cmd == "dump") {
            dump(mem);
        } else if (cmd == "get") {
            std::string a;
            long addr = 0;
            if (!(words >> a) || !parse_number(a, addr)) {
                std::cout << "usage: get <addr>\n";
            } else if (addr < 0) {
                std::cout << "address must not be negative\n";
            } else {
                show_byte(mem_get(mem, static_cast<std::size_t>(addr)));
            }
        } else if (cmd == "set") {
            std::string a, v;
            long addr = 0, value = 0;
            if (!(words >> a) || !(words >> v) || !parse_number(a, addr) ||
                !parse_number(v, value)) {
                std::cout << "usage: set <addr> <value>\n";
            } else if (addr < 0) {
                std::cout << "address must not be negative\n";
            } else if (value < 0 || value > 255) {
                // A cell holds ONE byte. 256 does not fit.
                std::cout << "a byte is 0..255, got " << value << '\n';
            } else if (!mem_set(mem, static_cast<std::size_t>(addr),
                                static_cast<Byte>(value))) {
                std::cout << "address " << addr << " is outside 0.." << MEM_SIZE - 1
                          << '\n';
            }
        } else if (cmd == "reg") {
            std::string r, v;
            long value = 0;
            if (!(words >> r) || !(words >> v) || !parse_number(v, value) || value < 0 ||
                value > 255) {
                std::cout << "usage: reg <a|b> <0..255>\n";
            } else if (r == "a") {
                cpu.a = (Byte)value;
            } else if (r == "b") {
                cpu.b = (Byte)value;
            } else {
                std::cout << "unknown register: " << r << '\n';
            }
        } else if (cmd == "regs") {
            dump_regs(cpu);
        } else if (cmd == "step") {
            step(cpu);
            dump_regs(cpu);
        } else {
            std::cout << "unknown command: " << cmd << " (try `help`)\n";
        }
    }

    return 0;  // 0 means "success" to the operating system
}