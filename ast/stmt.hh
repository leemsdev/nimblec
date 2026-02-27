#pragma once

#include "expr.hh"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <vector>

namespace statements
{
        enum class stmt_type
        {
                builtin_print,
                assignment,
                if_stmt,
                open_block_stmt,
                close_block_stmt,
        };

        struct stmt
        {
                stmt_type T;
                expressions::expr e;

                std::string type_to_str()
                {
                        switch (T)
                        {

                                case stmt_type::builtin_print:
                                        return "builtin_print_fn";
                                case stmt_type::assignment:
                                        return "assignment";
                                case stmt_type::if_stmt:
                                        return "if";
                                case stmt_type::open_block_stmt:
                                        return "open block";
                                case stmt_type::close_block_stmt:
                                        return "close block";
                        }
                }

                void print()
                {
                        std::cout << "stmt: " << type_to_str() << std::endl;
                }
        };

        struct stmt_container
        {
                // top-level statements. all are written.
                std::vector<stmt> statements;

                void add(stmt s)
                {
                        statements.push_back(s);
                }
        };
} // namespace statements
