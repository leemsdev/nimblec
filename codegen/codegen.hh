#pragma once

#include "../ast/ast.hh"

namespace codegen {
	std::string gen(std::string source, ast::program * p);
}
