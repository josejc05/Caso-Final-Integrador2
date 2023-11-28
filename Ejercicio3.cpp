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
    // Implementación de from_json_string
}

Variant Variant::parse_json(const std::map<std::string, Variant>& json_map) {
    // Implementación de parse_json
}

int main() {
    // Código principal
    // ...
    return 0;
}
