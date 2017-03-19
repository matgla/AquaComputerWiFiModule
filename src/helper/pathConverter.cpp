#include "pathConverter.hpp"

#include <algorithm>

String PathConverter::convertFileNameTo8dot3(const String &filename)
{
    String delimiter = "/";
    String s = filename;

    size_t pos = 0;
    String filename83;
    while ((pos = s.indexOf(delimiter)) != -1) {
        String part = s.substring(0, pos);
        if (part.length() > 8)
        {
            part.remove(6, part.length());
            part += "~1";
        }

        filename83 += part;
        filename83 += "/";
        s.remove(0, pos + delimiter.length());
    }
    auto extensionPos = s.indexOf(".");
    String name;
    String ext;
    if (extensionPos != -1)
    {
        name = s.substring(0, extensionPos);
        ext = s.substring(extensionPos, s.length());
    } else
    {
        name = s;
    }
    if (name.length() > 8)
    {
        name.remove(6, name.length());
        name += "~1";
    }
    if(ext.length() > 4)
    {
        ext.remove(4, ext.length());
    }
    filename83 += name;
    filename83 += ext;

    filename83.toUpperCase();
    return filename83;
}
