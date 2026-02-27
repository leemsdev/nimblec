#include "codegen.hh"
#include "codegen_utils.hh"
#include <cstdarg>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <string>

void write_print_stmt(codegen::generator *g, const statements::stmt *s)
{
        expressions::expr_literal e = g->p->expressions_container.get_literal(&s->e);

        std::string value = g->src->substr(e.tok->start, e.tok->end);

        switch (e.tok->type)
        {
                case tokens::token_type::string:
                        g->write(codegen_utils::c_printf("\"" + value + "\"", "%s"));
                        break;
                case tokens::token_type::ident:
                {
                        auto maybe_ident = g->try_get_ident_in_scope(value);

                        if (!maybe_ident.has_value())
                        {
                                g->p->rep_err("Undeclared identifier " + value, e.tok->line,
                                              e.tok->start, e.tok->end);
                                return;
                        }

                        codegen::identifier i = maybe_ident.value();

                        switch (i.T)
                        {
                                case codegen::identifier_type::number:
                                        g->write(codegen_utils::c_printf(i.name, "%d"));
                                        break;
                                case codegen::identifier_type::string:
                                        g->write(codegen_utils::c_printf(i.name, "%s"));
                                        break;
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
        expressions::expr_assign e = g->p->expressions_container.get_assign(&s->e);

        std::string ident = g->src->substr(e.ident->start, e.ident->end);
        std::string value = g->src->substr(e.value->start, e.value->end);

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

void write_if_stmt(codegen::generator *g, statements::stmt *s)
{
        printf("trying to get comp expr: \n");
        expressions::expr_compare e = g->p->expressions_container.get_comparison(&s->e);

        printf("got comp expr: \n");

        std::string left = g->src->substr(e.left->start, e.left->end);
        std::string right = g->src->substr(e.right->start, e.right->end);

        g->write("if (");
        g->write(left);
        g->write(" == ");
        g->write(right);
        g->write(")");
}

std::string codegen::gen(const source *source, ast::program *p)
{
        std::cout << "Beginning codegen..." << std::endl;

        generator g = {std::stack<scope>(), std::string(), p, source};

        g.output.append("#include \"stdio.h\"\n");
        g.output.append("int main(void) {\n");

        g.enter_scope();

        for (statements::stmt &s : p->statements_container.statements)
        {
                std::cout << "Generating code for " << std::endl;
                p->print_stmt(&s);
                switch (s.T)
                {
                        case statements::stmt_type::builtin_print:
                                write_print_stmt(&g, &s);
                                break;
                        case statements::stmt_type::assignment:
                                write_assignment(&g, &s);
                                break;
                        case statements::stmt_type::if_stmt:
                                write_if_stmt(&g, &s);
                                break;
                        case statements::stmt_type::open_block_stmt:
                                g.write("{");
                                break;
                        case statements::stmt_type::close_block_stmt:
                                g.write("}");
                                break;
                }

                g.newline();
                std::cout << "========" << std::endl;
        }

        g.write("\nreturn 0;\n}\n");

        if (!g.p->ok())
        {
                p->print_errs();
        }

        return g.output;
}
