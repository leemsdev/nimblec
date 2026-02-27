#pragma once

#include <cassert>
#include <cstdarg>
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
                branch,
                comparison,
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

        struct expr_compare
        {
                tokens::token *left;
                tokens::token *right;
        };

        struct expr
        {
                expr_type type;
                size_t location;
        };

        struct expressions_container
        {
                std::vector<expr_literal> expressions_literal;
                std::vector<expr_assign> expressions_assign;
                std::vector<expr_compare> expressions_compare;

                expr register_literal(expr_literal_type type, tokens::token *tok)
                {
                        expr_literal lit = {type, tok};
                        size_t next_loc = expressions_literal.size();

                        expressions_literal.push_back(lit);

                        return {.type = expr_type::literal, .location = next_loc};
                }

                expr_literal get_literal(const expr *e)
                {
                        size_t i = e->location;

                        return expressions_literal.at(i);
                }

                expr register_assign(tokens::token *ident, tokens::token *value)
                {
                        expr_assign e = {ident, value};
                        size_t next_loc = expressions_assign.size();

                        expressions_assign.push_back(e);

                        return {
                            expr_type::assign,
                            next_loc,
                        };
                }

                expr register_comparison(tokens::token *left, tokens::token *right)
                {
                        expr_compare e = {left, right};
                        size_t next_loc = expressions_compare.size();

                        expressions_compare.push_back(e);

                        return {expr_type::comparison, next_loc};
                }

                expr_assign get_assign(expr *e)
                {
                        size_t i = e->location;

                        return expressions_assign.at(i);
                }

                expr_compare get_comparison(expr *e)
                {
                        size_t i = e->location;

                        return expressions_compare.at(i);
                }

                std::string literal_tostr(const source *s, expr *e)
                {
                        expressions::expr_literal el = get_literal(e);

                        return "[expr lit] - " + s->substr(el.tok->start, el.tok->end);
                }

                std::string assign_tostr(const source *s, expr *e)
                {
                        expressions::expr_assign ea = get_assign(e);

                        size_t ident_start = ea.ident->start;
                        size_t ident_end = ea.ident->end;
                        size_t value_start = ea.value->start;
                        size_t value_end = ea.value->end;

                        std::string ident = s->substr(ident_start, ident_end);
                        std::string value = s->substr(value_start, value_end);

                        return "[expr assign] - " + ident + " = " + value;
                }
        };
} // namespace expressions
