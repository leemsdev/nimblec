#pragma once

#include "../scanner/token.hh"
#include "expr.hh"
#include "stmt.hh"
#include <iostream>
#include <vector>

#include "../source.hh"

namespace ast
{
        struct program_err
        {
                size_t line;
                std::string msg;
                size_t start;
                size_t end;
        };

        struct program
        {
                statements::stmt_container statements_container;
                expressions::expressions_container expressions_container;

                std::vector<program_err> errs;

                const source *src;

                void rep_err(std::string msg, size_t line, size_t start, size_t end)
                {
                        errs.push_back({
                            .line = line,
                            .msg = msg,
                            .start = start,
                            .end = end,
                        });
                }

                bool ok()
                {
                        return errs.empty();
                }

                void print_errs()
                {
                        std::cout << "Program failed to compile" << std::endl;
                        for (program_err &e : errs)
                        {
                                std::cout << "\t[line " << e.line << " (" << e.start << "," << e.end << ")] " << e.msg << std::endl;
                        }
                }

                void print_stmts()
                {
                        std::cout << "Printing statements: " << std::endl;

                        for (statements::stmt &s : statements_container.statements)
                        {
                                s.print();

                                switch (s.e.type)
                                {
                                        case expressions::expr_type::literal:
                                        {
                                                expressions::expr_literal e = expressions_container.get_literal(s.e);

                                                std::cout << "\tLiteral expr: " << src->substr_from_token(e.tok) << std::endl;
                                                break;
                                        }
                                        case expressions::expr_type::unary:
                                                break;
                                        case expressions::expr_type::binary:
                                                break;
                                        case expressions::expr_type::assign:
                                        {
                                                expressions::expr_assign e = expressions_container.get_assign(s.e);

                                                std::cout << "\tAssignment: " << src->substr_from_token(e.ident) << " = " << src->substr_from_token(e.value)
                                                          << std::endl;
                                                break;
                                        }
                                }
                        }
                }

                void add_stmt(statements::stmt_type t, expressions::expr e)
                {
                        statements_container.add({.T = t, .e = e});
                }
        };

        void parse_tree(program *p, tokens::token_iter *ti);
} // namespace ast
