#pragma once
#include <string>

namespace settings
{
    static std::string ip_address = "unk";
    static char authtoken[ 50 ];
    static int port = 0;
    static int region = 0;
    const char* const regions[]
    {
        "south america",
        "north america",
        "europe",
        "asia",
        "australia"
    };
}
