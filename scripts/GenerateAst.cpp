#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <unordered_set>
//
// Created by sheshan on 3/11/2025.
//

std::unordered_set<std::string> sharedFields{"Expr", "Stmt"};

struct Field {
    std::string type;
    std::string name;
};

std::string extractClassName(const std::string &definition) {
    std::istringstream stream(definition);
    std::string className;
    stream >> className;
    return className;
}

std::vector<Field> extractFields(const std::string &definition) {
    std::vector<Field> fields;
    const size_t colonPos = definition.find(':');

    if (colonPos == std::string::npos) return fields;

    const std::string fieldsPart = definition.substr(colonPos + 1);
    std::istringstream stream(fieldsPart);

    std::string token;
    std::string type;
    bool insideTemplate = false;

    while (stream >> token) {
        if (token.find('<') != std::string::npos) {
            insideTemplate = true;
            type += token;
        } else if (insideTemplate) {
            std::string name = token;
            if (!name.empty() && name.back() == ',') {
                name.pop_back();
            }
            fields.push_back({type, name});
            type.clear();
            insideTemplate = false;
        } else {
            if (type.empty()) {
                type = token;
            } else {
                std::string name = token;
                if (!name.empty() && name.back() == ',') {
                    name.pop_back();
                }
                fields.push_back({type, name});
                type.clear();
            }
        }
    }

    return fields;
}

void defineType(std::ofstream &writer, const std::string &baseName, const std::string &className,
                const std::vector<Field> &fields) {
    writer << "class " << className << " final : public " << baseName << " {\n";

    // Fields with shared_ptr
    for (const auto &[type, name]: fields) {
        std::string fieldType = type;
        if (sharedFields.contains(fieldType)) {
            fieldType = "std::shared_ptr<" + fieldType + ">";
        }
        writer << "    " << fieldType << " " << name << ";\n";
    }

    writer << "\npublic:\n";

    // Constructor
    writer << "    " << (fields.size() == 1 ? "explicit " : "") << className << "(";
    for (size_t i = 0; i < fields.size(); ++i) {
        std::string fieldType = fields[i].type;
        if (sharedFields.contains(fieldType)) {
            fieldType = "std::shared_ptr<" + fieldType + ">";
        }
        writer << fieldType << " " << fields[i].name;
        if (i != fields.size() - 1) {
            writer << ", ";
        }
    }
    writer << ")\n        : ";

    // Constructor initialization
    for (size_t i = 0; i < fields.size(); ++i) {
        const auto &name = fields[i].name;
        writer << name << "(std::move(" << name << "))";
        if (i != fields.size() - 1) {
            writer << ", ";
        } else {
            writer << " {}\n\n";
        }
    }

    // Accept method
    writer << "    TokenLiteral accept(" << baseName << "Visitor& visitor) const override {\n";
    writer << "        return visitor.visit" << className << baseName << "(*this);\n";
    writer << "    }\n\n";

    for (const auto &[type, name]: fields) {
        if (std::string fieldType = type; sharedFields.contains(fieldType)) {
            writer << "[[nodiscard]] const std::shared_ptr<" << fieldType << ">& get"
                    << std::string(1, static_cast<char>(std::toupper(name[0]))) << name.substr(1)
                    << "() const { return " << name << "; }\n";
        } else {
            writer << "[[nodiscard]] const " << fieldType << "& get"
                    << std::string(1, static_cast<char>(std::toupper(name[0]))) << name.substr(1)
                    << "() const { return " << name << "; }\n";
        }
    }

    writer << "};\n\n";
}

std::string getLowerCased(const std::string &str) {
    std::string result = str;
    std::ranges::transform(result, result.begin(), [](const unsigned char c) { return std::tolower(c); });
    return result;
}

void defineVisitor(std::ofstream &writer, const std::string &baseName, const std::vector<std::string> &types) {
    const auto lowerCasedBaseName = getLowerCased(baseName);
    writer << "class " << baseName << "Visitor {\n" << "public:\n";
    writer << "virtual ~" << baseName << "Visitor() = default;\n";

    for (const auto &type: types) {
        auto name = extractClassName(type);
        writer << "virtual TokenLiteral visit" << name << baseName << "(const " << name << "& " << lowerCasedBaseName <<
                ") = 0;\n";
    }
    writer << "};\n";

    writer << "class " << baseName << " {\n" << "public:\n";
    writer << "virtual ~" << baseName << "() = default;\n";
    writer << "virtual TokenLiteral accept(" << baseName << "Visitor& visitor) const = 0;\n";

    writer << "};\n";
}


void defineAst(const std::string &outputDir, const std::string &basename, const std::vector<std::string> &types) {
    const std::string path = outputDir + "/" + basename + ".h";
    std::ofstream writer{path};
    writer << "#pragma once\n#include <memory>\n#include <variant>\n#include \"../include/Token.h\"\n";
    writer << "namespace lox {" << "\n";

    for (const auto &type: types) {
        std::string className = extractClassName(type);
        writer << "class " << className << ";\n";
    }
    defineVisitor(writer, basename, types);

    for (const auto &type: types) {
        std::string className = extractClassName(type);
        auto fields = extractFields(type);
        defineType(writer, basename, className, fields);
    }

    writer << "}";
}

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: generate_ast <output_directory>";
        exit(64);
    }
    const std::string outputDir{argv[1]};
    defineAst(outputDir, "Expr", {
                  "Assign : Token name, Expr value",
                  "Binary   : Expr left, Token op, Expr right",
                  "Call     : Expr calle, Token paren, std::vector<std::shared_ptr<Expr>> arguments",
                  "Grouping : Expr expression",
                  "Literal  : TokenLiteral value",
                  "Logical  : Expr left, Token op, Expr right",
                  "Unary    : Token op, Expr right",
                  "Variable : Token name"
              });
    defineAst(outputDir, "Stmt", {
                  "Block        : std::vector<std::shared_ptr<Stmt>> statements",
                  "Expression   : Expr expression",
                  "Function     : Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body",
                  "If           : Expr condition, Stmt thenBranch, Stmt elseBranch",
                  "Print        : Expr expression",
                  "Var          : Token name, Expr initializer",
                  "While        : Expr condition, Stmt body"
              });
}
