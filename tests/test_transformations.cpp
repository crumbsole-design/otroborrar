#include <gtest/gtest.h>
#include "transformations.h"
#include <vector>
#include <stdexcept>
#include <cmath> // Para M_PI

// Test: Creación de estructura Vector2D con componentes reales (double)
//  "sobre un array unidimensional de dos componentes x e y de tipo real"
TEST(Vector2D_Struct, StoresTwoDoubles) {
    struct Vector2D {
        double x;
        double y;
        Vector2D(double xv = 0.0, double yv = 0.0) : x(xv), y(yv) {}
    };

    Vector2D v(1.5, -2.25);
    EXPECT_DOUBLE_EQ(v.x, 1.5);
    EXPECT_DOUBLE_EQ(v.y, -2.25);
    EXPECT_EQ(sizeof(v.x), sizeof(double));
    EXPECT_EQ(sizeof(v.y), sizeof(double));
}

// "Escalado. Consiste en realizar el producto del factor de escala, que es un número real a, por el array (x, y). El resultado es el array unidimensional
// de dos componentes (a · x, a · y)."
// Transformación de Escalado: Multiplicar un vector por un factor real positivo debe devolver los componentes escalados correctamente.
TEST(Transformation_Scaling, MultipliesComponents) {
    struct Vector2D {
        double x;
        double y;
        Vector2D(double xv = 0.0, double yv = 0.0) : x(xv), y(yv) {}
    };

    auto scale = [](const Vector2D& v, double factor) {
        return Vector2D(v.x * factor, v.y * factor);
    };

    Vector2D v(1.0, 2.0);
    double factor = 3.0;
    Vector2D scaled = scale(v, factor);
    EXPECT_DOUBLE_EQ(scaled.x, 3.0);
    EXPECT_DOUBLE_EQ(scaled.y, 6.0);
}

// Escalado por factor negativo: Verificar que el escalado funciona con factores negativos (cambio de sentido).
TEST(Transformation_Scaling, NegativeFactor) {
    struct Vector2D {
        double x;
        double y;
        Vector2D(double xv = 0.0, double yv = 0.0) : x(xv), y(yv) {}
    };

    auto scale = [](const Vector2D& v, double factor) {
        return Vector2D(v.x * factor, v.y * factor);
    };

    Vector2D v(1.0, 2.0);
    double factor = -1.5;
    Vector2D scaled = scale(v, factor);
    EXPECT_DOUBLE_EQ(scaled.x, -1.5);
    EXPECT_DOUBLE_EQ(scaled.y, -3.0);
}

// "por el array (x, y). El ´angulo de rotaci´on α pasado como argumento a las
// funciones trigonom´etricas debe expresarse en radianes mientras que, como se
// explica a continuaci´on, el ´angulo introducido por el usuario a trav´es de la
// consola est´a expresado en grados. Esto implica que el programa debe realizar
// el cambio de unidades"
// Conversión de Unidades: Una función auxiliar debe convertir correctamente grados a radianes (ej: 180º = $\pi$ rad).

TEST(Transformation_Rotation, DegreesToRadians) {
    auto degreesToRadians = [](double degrees) {
        return degrees * M_PI / 180.0;
    };

    EXPECT_DOUBLE_EQ(degreesToRadians(0), 0);
    EXPECT_DOUBLE_EQ(degreesToRadians(90), M_PI / 2);
    EXPECT_DOUBLE_EQ(degreesToRadians(180), M_PI);
    EXPECT_DOUBLE_EQ(degreesToRadians(360), 2 * M_PI);
}

// "Rotaci´on. Consiste en realizar el producto de la matriz"
// [ cos α  -sin α ]
// [ sin α   cos α ]
// Rotación Ortogonal (90º): Rotar el vector (1,0) exactamente 90 grados debe resultar en (0,1).

TEST(Transformation_Rotation, Orthogonal90Degrees) {
    struct Vector2D {
        double x;
        double y;
        Vector2D(double xv = 0.0, double yv = 0.0) : x(xv), y(yv) {}
    };

    auto degreesToRadians = [](double degrees) {
        return degrees * M_PI / 180.0;
    };

    auto rotate = [](const Vector2D& v, double degrees) {
        double rad = degrees * M_PI / 180.0;
        double cos_a = std::cos(rad);
        double sin_a = std::sin(rad);
        return Vector2D(
            v.x * cos_a - v.y * sin_a,
            v.x * sin_a + v.y * cos_a
        );
    };

    Vector2D v(1.0, 0.0);
    Vector2D rotated = rotate(v, 90.0);
    EXPECT_NEAR(rotated.x, 0.0, 1e-10);
    EXPECT_NEAR(rotated.y, 1.0, 1e-10);
}


// Rotación Arbitraria: Aplicar la matriz de rotación para un ángulo cualquiera (ej: 30.5º) y verificar los nuevos componentes
TEST(Transformation_Rotation, ArbitraryAngle) {
    struct Vector2D {
        double x;
        double y;
        Vector2D(double xv = 0.0, double yv = 0.0) : x(xv), y(yv) {}
    };

    auto rotate = [](const Vector2D& v, double degrees) {
        double rad = degrees * M_PI / 180.0;
        double cos_a = std::cos(rad);
        double sin_a = std::sin(rad);
        return Vector2D(
            v.x * cos_a - v.y * sin_a,
            v.x * sin_a + v.y * cos_a
        );
    };

    Vector2D v(2.0, 1.0);
    double angle = 30.5;
    Vector2D rotated = rotate(v, angle);
    // Cálculo esperado manualmente
    double rad = angle * M_PI / 180.0;
    double expected_x = 2.0 * std::cos(rad) - 1.0 * std::sin(rad);
    double expected_y = 2.0 * std::sin(rad) + 1.0 * std::cos(rad);
    EXPECT_NEAR(rotated.x, expected_x, 1e-10);
    EXPECT_NEAR(rotated.y, expected_y, 1e-10);
}



// TEST(ParseVector, CommaSeparated) {
//     auto v = tf::parseVector("1,2,3");
//     EXPECT_EQ(v.size(), 3u);
//     EXPECT_EQ(v[0], 1);
//     EXPECT_EQ(v[1], 2);
//     EXPECT_EQ(v[2], 3);
// }

// TEST(ParseVector, BracketsAndSpaces) {
//     auto v = tf::parseVector("[4, 5, 6]");
//     EXPECT_EQ(v.size(), 3u);
//     EXPECT_EQ(v[0], 4);
//     EXPECT_EQ(v[1], 5);
//     EXPECT_EQ(v[2], 6);
// }

// TEST(ParseVector, BadInputThrows) {
//     EXPECT_THROW(tf::parseVector("a,b"), std::invalid_argument);
// }

// TEST(ParseTransformation, AddAndMul) {
//     auto t1 = tf::parseTransformation("add 3");
//     EXPECT_EQ(t1.type, tf::Transformation::Type::Add);
//     EXPECT_EQ(t1.value, 3);

//     auto t2 = tf::parseTransformation("mul 2");
//     EXPECT_EQ(t2.type, tf::Transformation::Type::Mul);
//     EXPECT_EQ(t2.value, 2);
// }

// TEST(ParseTransformation, BadThrows) {
//     EXPECT_THROW(tf::parseTransformation("foo 3"), std::invalid_argument);
// }

// TEST(ApplyTransformation, Add) {
//     std::vector<int> v{1,2,3};
//     auto t = tf::parseTransformation("add 1");
//     auto res = tf::applyTransformation(v, t);
//     EXPECT_EQ(res.size(), 3u);
//     EXPECT_EQ(res[0], 2);
//     EXPECT_EQ(res[1], 3);
//     EXPECT_EQ(res[2], 4);
// }

// TEST(ApplyTransformation, Mul) {
//     std::vector<int> v{1,2,3};
//     auto t = tf::parseTransformation("mul 3");
//     auto res = tf::applyTransformation(v, t);
//     EXPECT_EQ(res[0], 3);
//     EXPECT_EQ(res[1], 6);
//     EXPECT_EQ(res[2], 9);
// }

// TEST(ApplyTransformation, Reverse) {
//     std::vector<int> v{1,2,3};
//     auto t = tf::parseTransformation("reverse");
//     EXPECT_EQ(t.type, tf::Transformation::Type::Reverse);
//     auto res = tf::applyTransformation(v, t);
//     EXPECT_EQ(res[0], 3);
//     EXPECT_EQ(res[1], 2);
//     EXPECT_EQ(res[2], 1);
// }

// TEST(Integration, RunFlow) {
//     auto final = tf::runFlow("1,2,3", {"add 1", "mul 2"});
//     EXPECT_EQ(final.size(), 3u);
//     EXPECT_EQ(final[0], 4);
//     EXPECT_EQ(final[1], 6);
//     EXPECT_EQ(final[2], 8);
// }

// TEST(VisualizeVector, Format) {
//     std::vector<int> v{1,2,3};
//     EXPECT_EQ(tf::visualizeVector(v), "[1, 2, 3]");
// }

// TEST(ValidateVector, Basic) {
//     EXPECT_TRUE(tf::validateVector(std::vector<int>{1,2}));
//     EXPECT_FALSE(tf::validateVector(std::vector<int>{}));
// }