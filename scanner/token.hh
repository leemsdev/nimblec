#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace tokens
{
        using std::optional;
        using std::string;
        using std::vector;

        enum class token_type
        {
                keyword_print,
                string,
                ident,
                unknown,
                newline,
                colon_eq,
                number,
                eof,
        };

        struct token
        {
                token_type type;
                size_t start;
                size_t end;
                size_t line;

                inline static token make(token_type t, size_t start, size_t end, size_t line)
                {
                        return {
                            .type = t,
                            .start = start,
                            .end = end,
                            .line = line,
                        };
                }
        };

        struct token_iter
        {
                size_t cursor;
                vector<token> *tokens;

                inline static token_iter make(vector<token> *tokens)
                {
                        return {
                            .cursor = 0,
                            .tokens = tokens,
                        };
                }

                void advance()
                {
                        cursor++;
                };

                bool done()
                {
                        return cursor >= tokens->size();
                }

                optional<token *> peek() const
                {
                        if (cursor + 1 > tokens->size())
                        {
                                return std::nullopt;
                        }

                        return &tokens->at(cursor + 1);
                }

                optional<token *> current()
                {
                        if (done())
                        {
                                return std::nullopt;
                        }

                        return &tokens->at(cursor);
                }
        };

        inline static string type_to_str(token_type t)
        {
                using tt = tokens::token_type;

                switch (t)
                {
                        case tt::keyword_print:
                                return "print";
                        case tt::string:
                                return "string";
                        case tt::ident:
                                return "identifier";
                        case tt::unknown:
                                return "unknown";
                        case tt::eof:
                                return "eof";
                        case tt::newline:
                                return "newline";
                        case tt::colon_eq:
                                return "colon_equal";
                        case tt::number:
                                return "number";
                }
        }

        inline static optional<token_type> try_match_keyword(const string *src, token t)
        {
                if (src->substr(t.start, t.end) == "print")
                {
                        return token_type::keyword_print;
                }

                return std::nullopt;
        }

} // namespace tokens
