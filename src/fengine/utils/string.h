#ifndef FRAMES_STRING_H
#define FRAMES_STRING_H

#include <iomanip>
#include <string>

namespace frames {

template <typename T>
std::string to_string_wp(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out << std::fixed << std::setprecision(n) << a_value;
    return out.str();
}

} // namespace frames

#endif // FRAMES_STRING_H
