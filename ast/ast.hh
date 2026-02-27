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
                                std::cout << "\t[line " << e.line << " (" << e.start << "," << e.end
                                          << ")] " << e.msg << std::endl;
                        }
                }

                void print_expr(expressions::expr *e)
                {
                        switch (e->type)
                        {
                                case expressions::expr_type::literal:
                                {
                                        std::cout << expressions_container.literal_tostr(src, e);
                                        break;
                                }
                                case expressions::expr_type::unary:
                                        break;
                                case expressions::expr_type::binary:
                                        break;
                                case expressions::expr_type::assign:
                                {
                                        std::cout << expressions_container.assign_tostr(src, e);
                                        break;
                                }
                                case expressions::expr_type::branch:
                                        std::cout << "branch" << std::endl;
                                        break;
                                case expressions::expr_type::comparison:
                                        std::cout << "comparison" << std::endl;
                                        break;
                        }
                }

                void print_stmt(statements::stmt *s)
                {
                        s->print();
                        if (s->T != statements::stmt_type::open_block_stmt &&
                            s->T != statements::stmt_type::close_block_stmt)
                        {

                                print_expr(&s->e);
                        }
                        std::cout << std::endl;
                }

                void print_stmts()
                {
                        std::cout << "Printing statements: " << std::endl;

                        for (statements::stmt &s : statements_container.statements)
                        {
                                print_stmt(&s);
                        }
                }

                void add_stmt(statements::stmt_type t, expressions::expr e)
                {
                        statements_container.add({.T = t, .e = e});
                }
        };

        void parse_tree(program *p, tokens::token_iter *ti);
} // namespace ast
