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
                                        break;
                        }
                }

                void print()
                {
                        std::cout << "stmt: " << type_to_str() << std::endl;
                }
        };

        struct stmt_container
        {
                std::vector<stmt> statements;

                void add(stmt s)
                {
                        statements.push_back(s);
                }
        };
} // namespace statements
