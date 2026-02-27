#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

#include "../source.hh"

namespace tokens
{
        using std::optional;
        using std::string;
        using std::vector;

        enum class token_type
        {
                keyword_print,
                keyword_then,
                keyword_if,
                string,
                ident,
                unknown,
                newline,
                colon_eq,
                double_eq,
                number,
                left_brace,
                right_brace,
                eof,
                eq,
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
                        case tt::keyword_if:
                                return "if";
                        case tt::keyword_then:
                                return "then";
                        case tt::double_eq:
                                return "double_eq";
                        case token_type::left_brace:
                                return "left_brace";
                        case token_type::right_brace:
                                return "right_brace";
                                break;
                        case token_type::eq:
                                return "eq";
                                break;
                }
        }

        inline static optional<token_type> try_match_keyword(source *src, token t)
        {
                if (src->substr(t.start, t.end) == "print")
                {
                        return token_type::keyword_print;
                }

                if (src->substr(t.start, t.end) == "if")
                {
                        return token_type::keyword_if;
                }

                if (src->substr(t.start, t.end) == "then")
                {
                        return token_type::keyword_then;
                }

                return std::nullopt;
        }

} // namespace tokens
