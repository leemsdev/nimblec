#pragma once

#include <iostream>
#include <vector>
#include "../token.hh"
#include "expr.hh"
#include "stmt.hh"

namespace ast {
struct program_err {
	size_t line;
	std::string msg;
};

struct program {
	statements::stmt_container statements_container;
	expressions::expressions_container expressions_container;

	std::vector<program_err> errs;

	void rep_err(const char *msg, size_t line) {
		errs.push_back({
			.line = line,
			.msg = std::string{msg}
		});
	}

	bool ok() {
		return errs.empty();
	}

	void print_errs() {
		std::cout << "Program failed to compile" << std::endl;
		for (program_err &e: errs) {
			std::cout << "\t[line " << e.line << "] " << e.msg << std::endl;
		}
	}

	void add_stmt(statements::stmt_type t, expressions::expr e, const char * readable) {
			statements_container.statements.push_back({
				.T =t,
				.expr = e,
				.readable = readable
			});
	}
};

program parse_tree(tokens::token_iter * ti);
}
