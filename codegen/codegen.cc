#include "codegen.hh"

void write_print_stmt(std::string source, ast::program * p , std::string * out, const statements::stmt * s) {
	expressions::expr_literal e = p->expressions_container.get_literal(s->expr);

	std::string value = source.substr(e.tok->start, e.tok->end);

	out->append("printf(\"");

	for (char c : value) {
		out->push_back(c);
	}

	out->append("\\n\");");
}

std::string codegen::gen(const std::string source, ast::program * p) {	
	std::string output;

	output.append("#include \"stdio.h\"\n");
	output.append("int main(void) {\n");

	for (statements::stmt &s : p->statements_container.statements) {
		switch(s.T) {
                case statements::stmt_type::builtin_print: write_print_stmt(source, p, &output, &s);
        }
	}

	output.append("\nreturn 0;\n}\n");

	return output;
}
