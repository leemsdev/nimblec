#pragma once

#include "token.hh"
#include <cstddef>
#include <vector>
#include <string>

namespace scanning {
struct scanner {
	// this is the start index of the current token being scanned
	size_t cursor;

	// this is the end index of the current token being scanned
	size_t offset;

	size_t line;

	std::vector<tokens::token> tokens;
	std::string * source;

	static inline scanner make(std::string * source) {
		return {
			.cursor = 0,
			.offset = 0,
			.line = 0,
			.tokens = std::vector<tokens::token>{},
			.source = source,
		};
	}

	inline bool done() {
		return offset >= source->length();
	}

	inline char current() {
		if (done()) return '\0';

		return source->at(offset);
	}

	inline void rewind() {
		offset--;
	}

	inline void advance() {
		offset++;
	}

	inline void skip() {
		cursor++;
		offset++;
	}

	inline void start_new_token() {
		cursor = offset;
	}
};

void scan(scanner * s);
void scan_token(scanner * s);
}
