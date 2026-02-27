#pragma once

#include "../ast/ast.hh"
#include <map>
#include <stack>

#include "../source.hh"

namespace codegen
{
        std::string gen(const source *src, ast::program *p);

        enum class identifier_type
        {
                string,
                number
        };

        struct identifier
        {
                std::string name;
                std::string value;
                identifier_type T;
        };

        // each time we enter a new scope we push one onto the stack
        // giving us easy access to the identifiers and such that are in scope
        // also means we can have identifiers in different scopes w/ the same name
        struct scope
        {
                std::map<std::string, identifier> identifiers;
        };

        struct generator
        {
                std::stack<scope> scopes;
                std::string output;
                ast::program *p;

                const source *src;

                void write(std::string s)
                {
                        output.append(s);
                }

                void write(char c)
                {
                        output.push_back(c);
                }

                void newline()
                {
                        output.append("\n");
                }

                void enter_scope()
                {
                        scopes.push({});
                }

                void exit_scope()
                {
                        scopes.pop();
                }

                std::optional<identifier> try_get_ident_in_scope(std::string name)
                {
                        if (scopes.top().identifiers.contains(name))
                        {
                                return scopes.top().identifiers[name];
                        }

                        return std::nullopt;
                }

                void add_ident(std::string name, std::string value, identifier_type T)
                {
                        scopes.top().identifiers[name] = {name, value, T};
                }
        };
} // namespace codegen
