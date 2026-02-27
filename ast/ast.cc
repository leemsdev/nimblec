#include "ast.hh"
#include "../scanner/token.hh"
#include "expr.hh"
#include "stmt.hh"
#include <cstdlib>
#include <iostream>
#include <optional>
#include <ostream>

using ast::program;
using tokens::token;
using tokens::token_iter;
using tokens::token_type;

std::optional<token *> try_match_next(token_iter *ti, std::initializer_list<token_type> expected)
{
        auto maybe_next = ti->peek();

        if (!maybe_next.has_value())
        {
                return std::nullopt;
        }

        token *next = maybe_next.value();

        for (auto e : expected)
        {
                if (e == next->type)
                {
                        return next;
                }
        }

        return std::nullopt;
}

// grammar (current) = if <ident> <eqeq> <literalval> <then> <stmt>
void parse_if(program *p, token_iter *ti)
{
        token *tok = ti->current().value();
        auto maybe_ident = try_match_next(ti, {tokens::token_type::ident});

        if (!maybe_ident.has_value())
        {
                p->rep_err("invalid if syntax", tok->line, tok->start, tok->end);
                return;
        }

        ti->advance();

        token *ident = ti->current().value();

        auto maybe_eqeq = try_match_next(ti, {tokens::token_type::double_eq});

        if (!maybe_eqeq.has_value())
        {
                p->rep_err("invalid if syntax", tok->line, tok->start, tok->end);
                return;
        }

        ti->advance();

        token *eqeq = ti->current().value();

        auto maybe_literal = try_match_next(ti, {tokens::token_type::number});

        if (!maybe_literal.has_value())
        {
                p->rep_err("invalid if syntax", tok->line, tok->start, tok->end);
                return;
        }

        ti->advance();

        token *literal = ti->current().value();

        p->add_stmt(statements::stmt_type::if_stmt,
                    p->expressions_container.register_comparison(ident, literal));
}

void parse_stmt(program *p, token_iter *ti)
{
        token *tok = ti->current().value();

        if (tok->type == token_type::keyword_print)
        {
                auto maybe_next = try_match_next(ti, {token_type::string, token_type::ident});

                if (!maybe_next.has_value())
                {
                        p->rep_err(
                            "Print statement must be following by a string or identifier value",
                            tok->line, tok->start, tok->end);
                        return;
                }

                // create an expression
                auto e = p->expressions_container.register_literal(
                    expressions::expr_literal_type::string, maybe_next.value());

                p->add_stmt(statements::stmt_type::builtin_print, e);

                // advance past next token cos we already dealt w it
                ti->advance();
        }

        if (tok->type == token_type::ident)
        {
                auto maybe_coloneq = try_match_next(ti, {token_type::colon_eq});

                if (!maybe_coloneq.has_value())
                {
                        p->rep_err("Invalid identifier usage", tok->line, tok->start, tok->end);
                        return;
                }

                // we are an assignment
                ti->advance();

                auto maybe_value = try_match_next(ti, {token_type::number, token_type::string});

                if (!maybe_value.has_value())
                {
                        p->rep_err("Invalid assignment", tok->line, tok->start, tok->end);
                        return;
                }

                auto e = p->expressions_container.register_assign(tok, maybe_value.value());

                p->add_stmt(statements::stmt_type::assignment, e);

                ti->advance();
        }

        ti->advance();
}

void parse_token(program *p, token_iter *ti)
{
        token *tok = ti->current().value();

        std::cout << "Current token: " << tokens::type_to_str(tok->type)
                  << " value: " << p->src->substr(tok->start, tok->end) << std::endl;

        switch (tok->type)
        {
                case tokens::token_type::keyword_print:
                        parse_stmt(p, ti);
                        break;
                case tokens::token_type::string:
                        break;
                case tokens::token_type::ident:
                        parse_stmt(p, ti);
                        break;
                case tokens::token_type::unknown:
                        p->rep_err("Invalid syntax", tok->line, tok->start, tok->end);
                        break;
                case tokens::token_type::newline:
                        break;
                case tokens::token_type::eof:
                        break;
                case tokens::token_type::number:
                        p->rep_err("Invalid number ", tok->line, tok->start, tok->end);
                        break;
                case tokens::token_type::colon_eq:
                        p->rep_err("Invalid syntax ", tok->line, tok->start, tok->end);
                        break;
                case tokens::token_type::keyword_then:
                        p->rep_err("Invalid then ", tok->line, tok->start, tok->end);
                        break;
                case tokens::token_type::keyword_if:
                        parse_if(p, ti);
                        break;
                case tokens::token_type::double_eq:
                        p->rep_err("Invalid == ", tok->line, tok->start, tok->end);
                        break;
                case tokens::token_type::left_brace:
                        p->add_stmt(statements::stmt_type::open_block_stmt, {});
                        break;
                case tokens::token_type::right_brace:
                        p->add_stmt(statements::stmt_type::close_block_stmt, {});
                        break;
                case tokens::token_type::eq:
                        p->rep_err("invalid = syntax", tok->line, tok->start, tok->end);
                        break;
        }
        ti->advance();
}

void ast::parse_tree(program *p, token_iter *ti)
{
        while (!ti->done())
        {
                parse_token(p, ti);
        }

        if (!p->ok())
        {
                p->print_errs();
                exit(1);
        }
}
