#include "ipv4.h"
#include <cstdlib>
#include <iostream>
#include <string>

static unsigned long checks = 0;
void check(const std::string& input, bool expected, unsigned long address = 0,
           int port = -1) {
    unsigned long actualAddress = 123;
    int actualPort = 456;
    const bool found = extractIPv4(input, actualAddress, actualPort);
    ++checks;
    if (found != expected || actualAddress != address || actualPort != port) {
        std::cerr << "Failed case " << checks << ": " << input.substr(0, 100)
                  << "\nGot " << found << ' ' << actualAddress << ' ' << actualPort
                  << "; expected " << expected << ' ' << address << ' ' << port << '\n';
        std::exit(1);
    }
}

int main() {
    check("connecting to 192.168.1.1 now", true, 3232235777UL);
    check("server=10.0.0.255:8080end", true, 167772415UL, 8080);
    check("192a168.1.1.1", true, 2818638081UL);
    check("Connection from 192.168.1.1 refused", true, 3232235777UL);
    check("0.0.0.0", true, 0);
    check("255.255.255.255:65535", true, 4294967295UL, 65535);
    check("[1.2.3.4:0]", true, 16909060UL, 0);
    check("-1.2.3.4", true, 16909060UL); // '-' is garbage under this grammar.
    check("1.2.3.4:12x34", true, 16909060UL, 12);
    check("1.2.3.4 5.6.7.8", true, 16909060UL);
    check("1.2.3.4:99999 then 5.6.7.8:80", true, 84281096UL, 80);
    check("1.2.3.4. then 5.6.7.8", true, 84281096UL);
    const char* invalid[] = {
        "", "no number here", "12.34.56", "1.2.3", "1.2.3.4.5",
        "192.168.1.1.", ".1.2.3.4", ":1.2.3.4", "1.2.3.4:",
        "1.2.3.4::80", "1.2.3.4:80:90", "1.2.3.4:80.", "1.2:3.4",
        "1..2.3.4", "1.2..4", "1.2.3.", "....", "::::", "12345",
        "256.2.3.4", "1.256.3.4", "1.2.256.4", "1.2.3.256",
        "192.168.01.1", "01.2.3.4", "1.02.3.4", "1.2.3.04",
        "00.0.0.0", "000.0.0.0", "0000.0.0.0", "1000.2.3.4",
        "1.2.3.4:00", "1.2.3.4:080", "1.2.3.4:00000",
        "1.2.3.4:65536", "1.2.3.4:99999", "1.2.3.4:100000",
        "1.2.3.4:000000", "1.2.3.4: 80", "1.2.3.4:-80"
    };
    for (const char* input : invalid) check(input, false);
    // Independently known numeric values across every possible octet position.
    for (unsigned int n = 0; n <= 255; ++n) {
        const std::string s = std::to_string(n);
        check(s + ".0.0.0", true, static_cast<unsigned long>(n) * 16777216UL);
        check("0." + s + ".0.0", true, static_cast<unsigned long>(n) * 65536UL);
        check("0.0." + s + ".0", true, static_cast<unsigned long>(n) * 256UL);
        check("0.0.0." + s, true, n);
    }
    for (int port = 0; port <= 65535; ++port) {
        check("1.2.3.4:" + std::to_string(port), true, 16909060UL, port);
    }
    check(std::string(100000, '9') + ".1.2.3", false);
    check("1.2.3.4:" + std::string(100000, '9'), false);
    check(std::string(100000, '9') + "x1.2.3.4", true, 16909060UL);
    check(std::string("noise\0", 6) + "1.2.3.4", true, 16909060UL);
    check(std::string(1, static_cast<char>(0xff)) + "1.2.3.4", true, 16909060UL);
    std::cout << "Passed " << checks << " parser checks.\n";
}
