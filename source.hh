#pragma once

#include <string>

#include "./scanner/token.hh"

struct source
{
        std::string raw;

        std::string substr_from_token(const tokens::token *t) const
        {
                return raw.substr(t->start, t->end);
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
