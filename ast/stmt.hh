#pragma once

#include "expr.hh"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <vector>

namespace statements {
	enum class stmt_type {
		builtin_print,
	};

	struct stmt {
		stmt_type T;
		expressions::expr expr;
		const char * readable;

		void print() {
			std::cout << "STMT: " << readable << std::endl;
			std::cout << "\t";
			expr.print();
			std::cout << std::endl;
		}
	};

	struct stmt_container {
		std::vector<stmt> statements;
	};
}
