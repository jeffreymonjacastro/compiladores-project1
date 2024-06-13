cmake_minimum_required(VERSION 3.28)
project(compiladores_project1)

set(CMAKE_CXX_STANDARD 23)

add_executable(compiladores_project1
        imp_value.cpp
        imp_parser.cpp
        imp_parser.hh
        imp_codegen.cpp
        imp_value.hh
        imp_test.cpp
        imp_interpreter.cpp
        imp.cpp
        imp_printer.hh
        imp_printer.cpp
        imp_compiler.cpp
        env_test.cpp
        environment.hh
        imp_typechecker.hh
        imp_visitor.hh
        imp_typechecker.cpp
        type_visitor.hh
        imp.hh
        imp_value_visitor.hh
        imp_interpreter.hh
        imp_codegen.hh
)
