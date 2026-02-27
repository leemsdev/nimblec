#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "./ast/ast.hh"
#include "./scanner/scanner.hh"
#include "./scanner/token.hh"
#include "codegen/codegen.hh"

#define LOG true

using scanning::scanner;
using std::string;

void log(std::string log)
{
        if (LOG)
        {
                std::cout << "[LOG] " << log << std::endl;
        }
}

void write_and_run(string contents)
{
        system("rm -rf ./out/");
        system("mkdir ./out");

        std::ofstream srcfile("./out/main.c");

        srcfile << contents;

        srcfile.close();

        system("clang ./out/main.c -o ./out/a.out");
        system("./out/a.out");
}

void run(string src)
{
        log(src);

        source source = {src};

        scanner s = scanner::make(&source);

        log("made scanner");

        scanning::scan(&s);

        s.print_tokens();

        log("scanned");

        tokens::token_iter ti = tokens::token_iter::make(&s.tokens);

        ast::program p;
        p.src = &source;

        ast::parse_tree(&p, &ti);

        log("parsed tree");

        p.print_stmts();

        std::string out = codegen::gen(&source, &p);

        log("generated code");

        write_and_run(out);
}

std::string load_src(std::string path)
{
        std::ifstream file(path, std::ios::binary);

        if (!file)
        {
                throw std::runtime_error("Failed to open file: " + path);
        }

        return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

int main(int argc, char *argv[])
{

        if (argc > 1)
        {
                string src = load_src(argv[1]);
                run(src);
                return EXIT_SUCCESS;
        }

        for (;;)
        {
                std::cout << "NimbleC Repl 0.0.1" << std::endl;
                std::cout << "> ";

                string inp;

                std::getline(std::cin, inp);

                run(inp);
        }

        return 1;
}
