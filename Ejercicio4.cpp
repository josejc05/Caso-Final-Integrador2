#include <iostream>
#include <vector>
#include <map>
#include <sstream>

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
private:
    static Variant parse_json_internal(const std::string& json);
};

std::string Variant::to_string() const {
    // Implementación de to_string
}

std::string Variant::to_json_string() const {
    // Implementación de to_json_string
}

Variant Variant::from_json_string(const std::string& sjson) {
    return parse_json_internal(sjson);
}

Variant Variant::parse_json(const std::map<std::string, Variant>& json_map) {
    // Implementación de parse_json
}

Variant Variant::parse_json_internal(const std::string& json) {
    std::istringstream stream(json);
    char ch;
    stream >> std::ws >> ch;

    if (ch == '{') {
        // Es un objeto JSON
        std::map<std::string, Variant> json_map;
        while (stream >> std::ws >> ch && ch != '}') {
            std::string key;
            if (ch == '"') {
                std::getline(stream, key, '"');
                stream >> ch; // Consume the ':'
                json_map[key] = parse_json_internal(stream.str());
            }
        }
        return Variant::parse_json(json_map);
    } else if (ch == '[') {
        // Es una lista JSON
        std::vector<Variant> json_list;
        while (stream >> std::ws >> ch && ch != ']') {
            stream.unget();
            json_list.push_back(parse_json_internal(stream.str()));
        }
        return Variant(List, json_list);
    } else if (ch == '"') {
        // Es una cadena JSON
        std::string value;
        std::getline(stream, value, '"');
        return Variant(Cadena, value);
    } else {
        // Es un valor simple (número, símbolo, etc.)
        stream.unget();
        std::string value;
        stream >> value;
        return Variant(Cadena, value);
    }
}

int main() {
    // Código principal
    // ...
    return 0;
}