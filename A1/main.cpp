#include "ipv4.h"
#include <iostream>
#include <string>

int main() {
    std::string line;
    while (true) {
        std::cout << "Enter a string (or 'END' to quit): ";
        if (!std::getline(std::cin, line)) {
            break;
        }
        if (line == "END") {
            break;
        }
        unsigned long address = 0;
        int port = -1;
        if (!extractIPv4(line, address, port)) {
            std::cout << "Invalid input: no valid IPv4 address found\n";
            continue;
        }
        std::cout << "Extracted IPv4 address: "
                  << ((address >> 24U) & 255UL) << '.'
                  << ((address >> 16U) & 255UL) << '.'
                  << ((address >> 8U) & 255UL) << '.'
                  << (address & 255UL)
                  << " (decimal value: " << address << ", port: ";
        if (port == -1) {
            std::cout << "none";
        } else {
            std::cout << port;
        }
        std::cout << ")\n";
    }
    std::cout << "Program terminated.\n";
    return 0;
}
