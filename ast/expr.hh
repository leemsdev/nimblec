#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

#include "../scanner/token.hh"

namespace expressions
{
        enum class expr_type
        {
                literal,
                unary,
                binary,
                assign,
        };

        enum class expr_literal_type
        {
                string,
                number,
        };

        enum class expr_assignment_type
        {
                number,
        };

        struct expr_literal
        {
                expr_literal_type type;
                tokens::token *tok;
        };

        struct expr_assign
        {
                tokens::token *ident;
                tokens::token *value;
        };

        struct expr
        {
                expr_type type;
                size_t location;
                const char *readable;
        };

        struct expressions_container
        {
                std::vector<expr_literal> expressions_literal;
                std::vector<expr_assign> expressions_assign;

                expr register_literal(expr_literal_type type, tokens::token *tok, const char *readable)
                {
                        expr_literal lit = {type, tok};
                        size_t next_loc = expressions_literal.size();

                        expressions_literal.push_back(lit);

                        return {.type = expr_type::literal, .location = next_loc, .readable = readable};
                }

                expr_literal get_literal(expr e)
                {
                        size_t i = e.location;

                        return expressions_literal.at(i);
                }

                expr register_assign(tokens::token *ident, tokens::token *value)
                {
                        expr_assign e = {ident, value};
                        size_t next_loc = expressions_assign.size();

                        expressions_assign.push_back(e);

                        return {expr_type::assign, next_loc, "assignment"};
                }

                expr_assign get_assign(expr e)
                {
                        size_t i = e.location;

                        return expressions_assign.at(i);
                }

                void print_expr(expr *e)
                {
                }
        };
} // namespace expressions
