#include <iostream>
#include <vector>
#include <map>

enum variant_type { Symbol, Number, List, Proc, Lambda, Cadena };

struct Entorno;

class Variant {
public:
    using proc_type = Variant (*)(const std::vector<Variant>&);

    variant_type type;
    std::string val;
    std::vector<Variant> list;
    proc_type proc;
    Entorno* env;

    Variant(variant_type type = Symbol) : type(type), env(nullptr), proc(nullptr) { }
    Variant(variant_type type, const std::string& val) : type(type), val(val), env(nullptr), proc(nullptr) { }
    Variant(proc_type proc) : type(Proc), proc(proc), env(nullptr) { }

    std::string to_string() const;
    std::string to_json_string() const;
    static Variant from_json_string(const std::string& json);
    static Variant parse_json(const std::map<std::string, Variant>& json_map);
};

std::string Variant::to_string() const {
    switch (type) {
        case Symbol:
        case Number:
        case Cadena:
            return val;
        case List: {
            std::string result = "(";
            for (const auto& item : list) {
                result += item.to_string() + " ";
            }
            if (!list.empty()) {
                result.pop_back(); // Remove the trailing space
            }
            result += ")";
            return result;
        }
        case Proc:
            return "<Procedure>";
        case Lambda:
            return "<Lambda>";
        default:
            return "<Unknown>";
    }
}

std::string Variant::to_json_string() const {
    switch (type) {
        case Symbol:
        case Number:
        case Cadena:
            return "\"" + val + "\"";
        case List: {
            std::string result = "[";
            for (const auto& item : list) {
                result += item.to_json_string() + ",";
            }
            if (!list.empty()) {
                result.pop_back(); // Remove the trailing comma
            }
            result += "]";
            return result;
        }
        case Proc:
            return "\"<Procedure>\"";
        case Lambda:
            return "\"<Lambda>\"";
        default:
            return "\"<Unknown>\"";
    }
}

Variant Variant::from_json_string(const std::string& sjson) {
    // Implementación básica, asume que el JSON es una lista
    std::vector<Variant> result;
    std::string current;
    bool in_string = false;

    for (char c : sjson) {
        if (c == '"') {
            in_string = !in_string;
        } else if (c == ',' && !in_string) {
            result.push_back(Variant(Cadena, current));
            current.clear();
        } else if (c != '[' && c != ']' && c != ' ' && c != '\n' && c != '\t' && c != '\r') {
            current += c;
        }
    }

    if (!current.empty()) {
        result.push_back(Variant(Cadena, current));
    }

    return Variant(List, result);
}

Variant Variant::parse_json(const std::map<std::string, Variant>& json_map) {
    // Implementación básica, asume que el JSON es un mapa
    return Variant(List, json_map.at("value").list);
}

int main() {
    // Ejemplo de uso
    Variant symbol(Symbol, "x");
    Variant number(Number, "42");
    Variant str(Cadena, "Hello, World!");
    Variant procedure([](const std::vector<Variant>& args) -> Variant { return Variant(Number, "123"); });
    Variant list(List, {symbol, number, str, procedure});

    std::cout << "Symbol: " << symbol.to_string() << std::endl;
    std::cout << "Number: " << number.to_string() << std::endl;
    std::cout << "String: " << str.to_string() << std::endl;
    std::cout << "Procedure: " << procedure.to_string() << std::endl;
    std::cout << "List: " << list.to_string() << std::endl;

    // Conversión a cadena JSON
    std::cout << "JSON String: " << list.to_json_string() << std::endl;

    // Conversión desde cadena JSON
    std::string json_input = "[\"x\", \"42\", \"Hello, World!\", \"<Procedure>\"]";
    Variant parsed_variant = Variant::from_json_string(json_input);
    std::cout << "Parsed Variant: " << parsed_variant.to_string() << std::endl;

    return 0;
}
