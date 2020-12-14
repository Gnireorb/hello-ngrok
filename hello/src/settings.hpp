#pragma once
#include <string>

namespace settings
{
    static std::string ip_address = "Unknown IP";
    static char authtoken[ 50 ];
    static int port = 0;
    static int region = 0;
    const char* const regions[]
    {
        "South America",
        "North America",
        "Europe",
        "Asia",
        "Australia"
    };
}
