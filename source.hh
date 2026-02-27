#pragma once

#include <cstddef>
#include <string>

struct source
{
        std::string raw;

        std::string substr(size_t start, size_t end) const
        {
                return raw.substr(start, end);
        }

        size_t len()
        {
                return raw.length();
        };

        char at(size_t loc)
        {
                return raw.at(loc);
        }
};
