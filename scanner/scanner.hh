#pragma once

#include "token.hh"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "../source.hh"

namespace scanning
{
        struct scanner
        {
                // this is the start index of the current token being scanned
                size_t cursor;

                // this is the end index of the current token being scanned
                size_t offset;

                size_t line;

                std::vector<tokens::token> tokens;
                source *src;

                static inline scanner make(source *src)
                {
                        return {.cursor = 0, .offset = 0, .line = 0, .tokens = std::vector<tokens::token>{}, .src = src};
                }

                inline bool done()
                {
                        return offset >= src->len();
                }

                inline char current()
                {
                        if (done())
                                return '\0';

                        return src->at(offset);
                }

                inline void rewind()
                {
                        offset--;
                }

                inline void advance()
                {
                        offset++;
                }

                inline void skip()
                {
                        cursor++;
                        offset++;
                }

                inline char next()
                {
                        advance();

                        if (done())
                        {
                                return '\0';
                        }

                        char c = current();

                        rewind();

                        return c;
                }

                inline void start_new_token()
                {
                        cursor = offset;
                }

                void print_tokens()
                {
                        for (tokens::token &t : tokens)
                        {
                                std::string token_to_src = src->substr_from_token(&t);

                                std::cout << "Token " << tokens::type_to_str(t.type) << " - " << token_to_src << std::endl;
                        }
                }
        };

        void scan(scanner *s);
        void scan_token(scanner *s);
} // namespace scanning
