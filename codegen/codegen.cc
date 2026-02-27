#include "codegen.hh"
#include <cstdlib>
#include <iostream>
#include <stack>
#include <string>

void write_print_stmt(codegen::generator *g, const statements::stmt *s)
{
        expressions::expr_literal e = g->p->expressions_container.get_literal(s->e);

        std::string value = g->src->substr_from_token(e.tok);

        g->write("printf(\"");

        switch (e.tok->type)
        {
                std::cout << "current codegen token: " << g->src->substr_from_token(e.tok) << std::endl;

                case tokens::token_type::string:
                {
                        for (char c : value)
                        {
                                g->write(c);
                        }

                        g->write("\\n\");");

                        break;
                }
                case tokens::token_type::ident:
                {
                        auto maybe_ident = g->try_get_ident_in_scope(value);

                        if (!maybe_ident.has_value())
                        {
                                g->p->rep_err("Undeclared identifier " + value, e.tok->line, e.tok->start, e.tok->end);
                                return;
                        }

                        codegen::identifier i = maybe_ident.value();

                        switch (i.T)
                        {
                                case codegen::identifier_type::number:
                                {
                                        g->write("%d\\n\", ");
                                        g->write(i.name);
                                        g->write(");");
                                        break;
                                }
                                case codegen::identifier_type::string:
                                {
                                        g->write("%s\\n\", ");
                                        g->write(i.name);
                                        g->write(");");
                                        break;
                                }
                                default:
                                        break;
                        }

                        break;
                }

                default:
                {
                        g->p->rep_err("Invalid syntax", e.tok->line, e.tok->start, e.tok->end);
                }
        }

        g->newline();
}

void write_assignment(codegen::generator *g, statements::stmt *s)
{
        expressions::expr_assign e = g->p->expressions_container.get_assign(s->e);

        std::string ident = g->src->substr_from_token(e.ident);
        std::string value = g->src->substr_from_token(e.value);

        if (e.value->type == tokens::token_type::number)
        {
                g->write("int ");
                g->write(ident);
                g->write(" = ");
                g->write(value);
                g->write(";");
                g->newline();

                g->add_ident(ident, value, codegen::identifier_type::number);
                return;
        }

        if (e.value->type == tokens::token_type::string)
        {
                g->write("char *");
                g->write(ident);
                g->write(" = \"");
                g->write(value);
                g->write("\";");
                g->newline();

                g->add_ident(ident, value, codegen::identifier_type::string);
                return;
        }
}

std::string codegen::gen(const source *source, ast::program *p)
{
        generator g = {std::stack<scope>(), std::string(), p, source};

        g.output.append("#include \"stdio.h\"\n");
        g.output.append("int main(void) {\n");

        g.enter_scope();

        std::cout << "Beginning codegen..." << std::endl;

        for (statements::stmt &s : p->statements_container.statements)
        {
                switch (s.T)
                {
                        case statements::stmt_type::builtin_print:
                                write_print_stmt(&g, &s);
                                break;
                        case statements::stmt_type::assignment:
                                write_assignment(&g, &s);
                                break;
                }

                g.newline();
        }

        g.write("\nreturn 0;\n}\n");

        if (!g.p->ok())
        {
                p->print_errs();
                exit(1);
        }

        return g.output;
}
