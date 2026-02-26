#include "scanner.hh"
#include <cctype>
#include <iostream>

#include "token.hh"

using scanning::scanner;

void save_token(scanner * s, tokens::token_type type) {
	tokens::token t = tokens::token::make(type, s->cursor, s->offset - s->cursor, s->line);

	s->tokens.push_back(t);
}

void parse_string(scanner * s) {
	// we want to skip the very first " character
	s->skip();

	while (s->current() != '"') {
		s->advance();
	}

	save_token(s, tokens::token_type::string);

	// advance past the second ", cos we don't want it
	s->advance();
}

void parse_ident(scanner * s) {
	while (std::isalnum(s->current())) {
		s->advance();
	}

	save_token(s, tokens::token_type::ident);	

	// check if value of last identifier matches a keyword
	tokens::token t = s->tokens.back();

	auto maybe_keyword = tokens::try_match_keyword(s->source, t);

	if (maybe_keyword.has_value()) {
		s->tokens.pop_back();

		save_token(s, maybe_keyword.value());
	}
}

void parse_newline(scanner * s) {
	save_token(s, tokens::token_type::newline);
	s->line++;
}

void scanning::scan_token(scanner * s) {
	s->start_new_token();

	switch (s->current()) {
		case '"': parse_string(s); break;	
		case ' ':	s->skip(); break;
		case '\0': case EOF: s->skip(); break;
		case '\n': parse_newline(s); break;
		default: parse_ident(s); break;
	}
}

void scanning::scan(scanner * s) {
	while (!s->done()) {
		scan_token(s);
	}
}
