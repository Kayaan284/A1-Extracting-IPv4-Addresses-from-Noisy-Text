#include "ipv4.h"

namespace {
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isTokenChar(char c) {
    return isDigit(c) || c == '.' || c == ':';
}

// Read one bounded decimal field without library numeric conversions.
bool readNumber(const std::string& str, std::size_t& pos, std::size_t end,
                unsigned int maxDigits, unsigned int maxValue,
                unsigned int& value) {
    const std::size_t start = pos;
    value = 0;
    while (pos < end && isDigit(str[pos])) {
        // Check before multiplying; even arbitrarily long input cannot overflow.
        if (pos - start >= maxDigits) {
            return false;
        }
        value = value * 10U + static_cast<unsigned int>(str[pos] - '0');
        if (value > maxValue) {
            return false;
        }
        ++pos;
    }
    return pos > start && !(pos - start > 1 && str[start] == '0');
}

bool parseToken(const std::string& str, std::size_t start, std::size_t end,
                unsigned long& address, int& port) {
    std::size_t pos = start;
    address = 0;
    port = -1;
    for (unsigned int index = 0; index < 4; ++index) {
        unsigned int octet = 0;
        if (!readNumber(str, pos, end, 3, 255, octet)) {
            return false;
        }
        address = (address << 8U) | static_cast<unsigned long>(octet);
        if (index < 3) {
            if (pos == end || str[pos] != '.') {
                return false;
            }
            ++pos;
        }
    }
    if (pos < end) {
        if (str[pos] != ':') {
            return false;
        }
        ++pos;
        unsigned int parsedPort = 0;
        if (!readNumber(str, pos, end, 5, 65535, parsedPort)) {
            return false;
        }
        port = static_cast<int>(parsedPort);
    }
    return pos == end;
}
} // namespace

bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort) {
    outAddress = 0;
    outPort = -1;
    std::size_t pos = 0;
    while (pos < str.size()) {
        if (!isTokenChar(str[pos])) {
            ++pos;
            continue;
        }
        const std::size_t start = pos;
        while (pos < str.size() && isTokenChar(str[pos])) {
            ++pos;
        }
        // Parse the entire maximal run, never a suffix of a rejected run.
        unsigned long address = 0;
        int port = -1;
        if (parseToken(str, start, pos, address, port)) {
            outAddress = address;
            outPort = port;
            return true;
        }
    }
    return false;
}
