#include "transformations.h"
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

// small helper
static std::string trimString(const std::string &s) {
    auto l = s.find_first_not_of(" \t\n\r");
    if (l == std::string::npos) return std::string();
    auto r = s.find_last_not_of(" \t\n\r");
    return s.substr(l, r - l + 1);
}

namespace tf {

std::vector<int> parseVector(const std::string& input) {
    std::string s = trimString(input);

    // remove surrounding brackets if present
    if (!s.empty() && s.front() == '[' && s.back() == ']') {
        s = s.substr(1, s.size() - 2);
    }

    // replace commas with spaces to tokenize
    for (char &c : s) if (c == ',') c = ' ';

    std::istringstream iss(s);
    std::string tok;
    std::vector<int> out;
    while (iss >> tok) {
        try {
            size_t pos = 0;
            int val = std::stoi(tok, &pos);
            if (pos != tok.size()) throw std::invalid_argument("invalid token");
            out.push_back(val);
        } catch (const std::exception&) {
            throw std::invalid_argument("Invalid vector input");
        }
    }
    return out;
}

bool validateVector(const std::vector<int>& v) {
    // Basic validation: vector must not be empty and values reasonable (example constraint)
    if (v.empty()) return false;
    for (int x : v) {
        // example bounds to avoid overflow in transforms
        if (x < -1000000 || x > 1000000) return false;
    }
    return true;
}

Transformation parseTransformation(const std::string& s) {
    // trim
    std::string t = trimString(s);

    // tokenize by space
    std::istringstream iss(t);
    std::string cmd;
    if (!(iss >> cmd)) throw std::invalid_argument("Empty transformation");
    // to lower
    for (auto &c : cmd) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    if (cmd == "add") {
        std::string arg; if (!(iss >> arg)) throw std::invalid_argument("add requires a value");
        try { size_t pos; int v = std::stoi(arg, &pos); if (pos != arg.size()) throw std::invalid_argument("bad"); return Transformation{Transformation::Type::Add, v}; }
        catch (...) { throw std::invalid_argument("Invalid add value"); }
    }
    if (cmd == "mul") {
        std::string arg; if (!(iss >> arg)) throw std::invalid_argument("mul requires a value");
        try { size_t pos; int v = std::stoi(arg, &pos); if (pos != arg.size()) throw std::invalid_argument("bad"); return Transformation{Transformation::Type::Mul, v}; }
        catch (...) { throw std::invalid_argument("Invalid mul value"); }
    }
    if (cmd == "reverse") {
        return Transformation{Transformation::Type::Reverse, 0};
    }

    throw std::invalid_argument("Unknown transformation");
}

std::vector<int> applyTransformation(const std::vector<int>& v, const Transformation& t) {
    std::vector<int> out = v;
    switch (t.type) {
        case Transformation::Type::Add:
            for (auto &x : out) x += t.value;
            return out;
        case Transformation::Type::Mul:
            for (auto &x : out) x *= t.value;
            return out;
        case Transformation::Type::Reverse:
            std::reverse(out.begin(), out.end());
            return out;
        default:
            throw std::invalid_argument("Invalid transformation application");
    }
}

std::vector<int> applyTransformations(const std::vector<int>& v, const std::vector<Transformation>& ts) {
    std::vector<int> cur = v;
    for (const auto& t : ts) cur = applyTransformation(cur, t);
    return cur;
}

std::string visualizeVector(const std::vector<int>& v) {
    // stub: simple bracketless serialization
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) oss << ", ";
        oss << v[i];
    }
    oss << "]";
    return oss.str();
}

std::vector<int> runFlow(const std::string& inputVec, const std::vector<std::string>& transformationStrs) {
    auto v = parseVector(inputVec);
    if (!validateVector(v)) throw std::invalid_argument("Invalid input vector");
    std::vector<Transformation> ts;
    for (const auto &s : transformationStrs) ts.push_back(parseTransformation(s));
    return applyTransformations(v, ts);
}

} // namespace tf

// New implementations
namespace tf {

std::vector<CommandToken> parseCommandSequence(const std::string& input) {
    std::vector<CommandToken> out;
    std::istringstream ss(input);
    std::string token;
    while (ss >> token) {
        if (token == ";") break;
        std::string key = token;
        bool keyEndedWithSemi = false;
        if (!key.empty() && key.back() == ';') {
            key.pop_back();
            keyEndedWithSemi = true;
        }
        double val = 0.0;
        bool hasVal = false;
        if (ss >> token) {
            std::string numTok = token;
            bool endedWithSemi = false;
            if (!numTok.empty() && numTok.back() == ';') {
                numTok.pop_back();
                endedWithSemi = true;
            }
            try {
                val = std::stod(numTok);
                hasVal = true;
            } catch (...) {
                hasVal = false;
            }
            if (endedWithSemi) {
                out.push_back(CommandToken{key, val, hasVal});
                break;
            }
        }
        out.push_back(CommandToken{key, val, hasVal});
        if (keyEndedWithSemi) break;
    }
    return out;
}

Vector2D scaleVector(const Vector2D& v, double factor) {
    return Vector2D{v.x * factor, v.y * factor};
}

Vector2D rotateVectorDeg(const Vector2D& v, double degrees) {
    double rad = degrees * M_PI / 180.0;
    double c = std::cos(rad);
    double s = std::sin(rad);
    return Vector2D{v.x * c - v.y * s, v.x * s + v.y * c};
}

} // namespace tf

namespace tf {

std::string formatFixed3(double x) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3) << x;
    return oss.str();
}

std::string formatVector(const Vector2D& v) {
    return "(" + formatFixed3(v.x) + ", " + formatFixed3(v.y) + ")";
}

std::string formatSequenceOutput(double scale, const Vector2D& v) {
    return std::string("Escala a=") + formatFixed3(scale) + " v=" + formatVector(v);
}

} // namespace tf
