#ifndef CUSTOMHOUSECONFIG_H
#define CUSTOMHOUSECONFIG_H

#include <charconv>
#include <string>
#include <system_error>

namespace CustomHouseConfig {

/**
 * Parse a complete base-10 integer without throwing.
 * The destination is left unchanged when parsing fails.
 */
inline bool parseInteger(const std::string& text, int& destination) noexcept {
    if(text.empty()) {
        return false;
    }

    int parsedValue = 0;
    const char* const begin = text.data();
    const char* const end = begin + text.size();
    const auto result = std::from_chars(begin, end, parsedValue, 10);
    if(result.ec != std::errc{} || result.ptr != end) {
        return false;
    }

    destination = parsedValue;
    return true;
}

} // namespace CustomHouseConfig

#endif // CUSTOMHOUSECONFIG_H
