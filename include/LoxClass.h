#pragma once
#include <string>

namespace lox {
    class LoxClass {
        std::string name;

    public:
        explicit LoxClass(std::string name) : name(std::move(name)) {
        }

        std::string toString() { return name; }
    };
}
