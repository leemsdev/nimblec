#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace tokens
{
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
                std::vector<token> *tokens;

                inline static token_iter make(std::vector<token> *tokens)
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

                std::optional<token *> peek() const
                {
                        if (cursor + 1 > tokens->size())
                        {
                                return std::nullopt;
                        }

                        return &tokens->at(cursor + 1);
                }

                std::optional<tokens::token *> current()
                {
                        if (done())
                        {
                                return std::nullopt;
                        }

                        return &tokens->at(cursor);
                }
        };

        inline static std::string type_to_str(token_type t)
        {
                switch (t)
                {
                        case token_type::keyword_print:
                                return "print";
                        case token_type::string:
                                return "string";
                        case token_type::ident:
                                return "identifier";
                        case token_type::unknown:
                                return "unknown";
                        case token_type::eof:
                                return "eof";
                        case token_type::newline:
                                return "newline";
                        case token_type::colon_eq:
                                return "colon_equal";
                        case token_type::number:
                                return "number";
                }
        }

        inline static std::optional<tokens::token_type> try_match_keyword(const std::string *src, token t)
        {
                if (src->substr(t.start, t.end) == "print")
                {
                        return token_type::keyword_print;
                }

                return std::nullopt;
        }

} // namespace tokens
