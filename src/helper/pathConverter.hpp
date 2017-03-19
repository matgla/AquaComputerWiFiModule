#ifndef PATH_CONVERTER_HPP
#define PATH_CONVERTER_HPP

#ifdef UNIT_TEST
#include "stringAdapter.hpp"
#else
#include <Arduino.h>
#endif  // UNIT_TEST

class PathConverter
{
public:
    static String convertFileNameTo8dot3(const String& filename);
};



#endif /* end of include guard: PATH_CONVERTER_HPP */
