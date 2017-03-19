#include "UnitTest++/UnitTest++.h"

#include "helper/pathConverter.hpp"

#include <iostream>

SUITE(PathConverterTest)
{
    TEST(ShouldReturnCorrect83Path)
    {
        std::string filename = "/www/jakis_html.html";
        auto EXPECTED_FILENAME = "/WWW/JAKIS_~1.HTM";

        CHECK_EQUAL(EXPECTED_FILENAME, PathConverter::convertFileNameTo8dot3(filename).getStr());

        CHECK_EQUAL("/AA", PathConverter::convertFileNameTo8dot3("/aa").getStr());
        CHECK_EQUAL("/AA/OLALAL~1", PathConverter::convertFileNameTo8dot3("/aa/olalalalalala").getStr());
        CHECK_EQUAL("/", PathConverter::convertFileNameTo8dot3("/").getStr());
        CHECK_EQUAL("/TESTIN~1/PASHIN~1/AWDAWD~1/ASSA/INDEXA~1.JS",
            PathConverter::convertFileNameTo8dot3("/testingaaa/pashinasiasda/awdawdawda/assa/indexaaaaa.js").getStr());
        CHECK_EQUAL("/TESTIN~1/PASHIN~1/AWDAWD~1/ASSA/INDEXA~1.HTM",
            PathConverter::convertFileNameTo8dot3("/testingaaa/pashinasiasda/awdawdawda/assa/indexaaaaa.html").getStr());
    }
}
