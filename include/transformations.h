#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <vector>
#include <string>

namespace tf {

std::vector<int> parseVector(const std::string& input);
bool validateVector(const std::vector<int>& v);

struct Transformation {
    enum class Type { Add, Mul, Reverse, Invalid };
    Type type = Type::Invalid;
    int value = 0;
};

Transformation parseTransformation(const std::string& s);

std::vector<int> applyTransformation(const std::vector<int>& v, const Transformation& t);
std::vector<int> applyTransformations(const std::vector<int>& v, const std::vector<Transformation>& ts);

std::string visualizeVector(const std::vector<int>& v);

std::vector<int> runFlow(const std::string& inputVec, const std::vector<std::string>& transformationStrs);

// New parsing and 2D transform helpers
struct CommandToken {
    std::string key;
    double value = 0.0;
    bool hasValue = false;
};

std::vector<CommandToken> parseCommandSequence(const std::string& input);

struct Vector2D {
    double x = 0.0;
    double y = 0.0;
};

Vector2D scaleVector(const Vector2D& v, double factor);
Vector2D rotateVectorDeg(const Vector2D& v, double degrees);

// Output formatting
std::string formatFixed3(double x);
std::string formatVector(const Vector2D& v);
std::string formatSequenceOutput(double scale, const Vector2D& v);

} // namespace tf

#endif // TRANSFORMATIONS_H
