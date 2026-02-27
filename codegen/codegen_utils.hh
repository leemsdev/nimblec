#pragma once

#include <stdlib.h>
#include <string>
#include <vector>

namespace codegen_utils
{
        struct fn
        {
                std::string fname;
                std::vector<std::string> args;

                static fn make(std::string fname)
                {
                        return {fname, {}};
                }

                fn add_arg(std::string arg)
                {
                        args.push_back(arg);
                        return *this;
                }

                fn add_str_arg(std::string arg)
                {
                        std::string wrapped_a;

                        wrapped_a.push_back('"');
                        wrapped_a.append(arg);
                        wrapped_a.push_back('"');

                        args.push_back(wrapped_a);

                        return *this;
                }

                std::string to_string()
                {
                        auto s = fname + "(";
                        for (std::string &a : args)
                        {
                                s.append(a);
                                s.append(",");
                        }

                        // remove trailing comma
                        s.pop_back();

                        s.append(");");

                        return s;
                }
        };

        inline static std::string c_funccall(std::string fname, std::string arg1)
        {
                return fn::make(fname) //
                    .add_arg(arg1)
                    .to_string();
        }

        inline static std::string c_printf(std::string arg, std::string formatter)
        {

                return fn::make("printf") //
                    .add_str_arg(formatter + "\\n")
                    .add_arg(arg)
                    .to_string();
        }
} // namespace codegen_utils
