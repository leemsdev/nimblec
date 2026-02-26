#include "ast.hh"
#include "expr.hh"
#include "stmt.hh"
#include "../token.hh"
#include <optional>

using tokens::token;
using tokens::token_iter;
using tokens::token_type;
using ast::program;

std::optional<token*> try_match_next(token_iter * ti, std::initializer_list<token_type> expected) {
	auto maybe_next = ti->peek();

	if (!maybe_next.has_value()) {
		return std::nullopt;
	}

	token * next = maybe_next.value();	

	for (auto e : expected) {
		if(e == next->type) {
			return next;
		}
	}

	return std::nullopt;
}

void parse_stmt(program * p, token_iter * ti) {
	token * tok = ti->current().value();	

	if (tok->type == token_type::keyword_print) {
		auto maybe_next = try_match_next(ti, {token_type::string});	

		if (!maybe_next.has_value()) {
			p->rep_err("Print statement must be following by a string value", tok->line);
			return;
		}

		// create an expression
		auto e = p->expressions_container.register_literal(
			expressions::expr_literal_type::string,
			maybe_next.value(),
			"literal expr" 
		);

		p->add_stmt(statements::stmt_type::builtin_print, e, "printfn")	;

		// advance past next token cos we already dealt w it
		ti->advance();
	}
}

void parse_token(program * p, token_iter * ti) {
	token * tok = ti->current().value();

	switch(tok->type) {
		case tokens::token_type::keyword_print: parse_stmt(p, ti); break;
		case tokens::token_type::string: p->rep_err("Invalid string literal", tok->line); break;
		case tokens::token_type::ident: break;
		case tokens::token_type::unknown: p->rep_err("Invalid syntax", tok->line); break;
		case tokens::token_type::newline: ti->advance(); break;
		case tokens::token_type::eof: break;
	}

	ti->advance();
}


program ast::parse_tree(token_iter * ti) {
	program p;

	while (!ti->done()) {
		parse_token(&p, ti);
	}

	if (!p.ok()) {
		p.print_errs();
		exit(-1);
	}

	return p;
}

