#ifndef IPV4_H
#define IPV4_H

#include <string>

// Returns the first valid whole token. Failure resets both outputs.
bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort);

#endif
