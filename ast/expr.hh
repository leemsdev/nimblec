#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <vector>

#include "../token.hh"

namespace expressions {
enum class expr_type {
	literal,
	unary,
	binary,
};

enum class expr_literal_type {
	string,
};

struct expr_literal {
	expr_literal_type type;
	tokens::token * tok;
};

struct expr {
	expr_type type;
	size_t location;
	const char * readable;

	void print() {
		printf("%s", readable);
	}
};

struct expressions_container {
	std::vector<expr_literal> expressions_literal;

	expr register_literal(expr_literal_type type, tokens::token * tok, const char * readable) {
		expr_literal lit = {type, tok};
		size_t next_loc = expressions_literal.size();

		expressions_literal.push_back(lit);

		expr e = { .type = expr_type::literal, .location = next_loc, .readable = readable };

		return e;
	}

	expr_literal get_literal(expr e) {
		size_t i = e.location;

		return expressions_literal.at(i);
	}
};
}

