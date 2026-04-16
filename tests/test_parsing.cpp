#include <gtest/gtest.h>
#include <string>
#include <optional>
#include <cctype>
#include <sstream>
#include <vector>
#include "transformations.h"


// "especificar el factor
// de escala a continuaci´on de la palabra ESCALA, separando ambos por uno o m´as
// espacios en blanco, y especificar el ´angulo de rotaci´on a continuaci´on de la palabra
// ROT_DEG, separando ambos por uno o m´as espacios en blanco"

// Using real parser from library: tf::parseCommandSequence and tf::CommandToken

TEST(Parsing_Command, ExtractEscala) {
    auto cmds = tf::parseCommandSequence("ESCALA 3 ;");
    ASSERT_FALSE(cmds.empty());
    EXPECT_EQ(cmds[0].key, "ESCALA");
    EXPECT_TRUE(cmds[0].hasValue);
    EXPECT_DOUBLE_EQ(cmds[0].value, 3.0);
}

TEST(Parsing_Command, ExtractRotDeg) {
    auto cmds = tf::parseCommandSequence("ROT_DEG 90 ;");
    ASSERT_FALSE(cmds.empty());
    EXPECT_EQ(cmds[0].key, "ROT_DEG");
    EXPECT_TRUE(cmds[0].hasValue);
    EXPECT_DOUBLE_EQ(cmds[0].value, 90.0);
}
// "Es posible describir
// varias transformaciones sucesivas, para lo cual deben escribirse una a continuaci´on
// de otra, separadas por uno o m´as espacios en blanco. El final de la secuencia de
// transformaciones se indica mediante el caracter punto y coma (;)."
TEST(Parsing_Command, DetectEndOfSequence) {
    auto cmds1 = tf::parseCommandSequence("; ");
    EXPECT_TRUE(cmds1.empty());

    auto cmds2 = tf::parseCommandSequence("ESCALA 5; ");
    ASSERT_FALSE(cmds2.empty());
    EXPECT_EQ(cmds2[0].key, "ESCALA");
    EXPECT_TRUE(cmds2[0].hasValue);
    EXPECT_DOUBLE_EQ(cmds2[0].value, 5.0);
}

TEST(Parsing_Command, SupportWhitespace) {
    auto cmds = tf::parseCommandSequence("   ROT_DEG    30.5   ;");
    ASSERT_FALSE(cmds.empty());
    EXPECT_EQ(cmds[0].key, "ROT_DEG");
    EXPECT_TRUE(cmds[0].hasValue);
    EXPECT_DOUBLE_EQ(cmds[0].value, 30.5);
}


// "Por ejemplo, la siguiente l´ınea de texto introducida por el usuario a trav´es de la consola
// ESCALA 1.5 ROT_DEG 30.5 ROT_DEG -10 ESCALA -0.5;
// o indistintamente (incluyendo m´as espacios en blanco):
// ESCALA 1.5 ROT_DEG 30.5 ROT_DEG -10 ESCALA -0.5 ;"

TEST(Parsing_Sequence, SemicolonSpacingEquivalence) {
    std::string a = "ESCALA 1.5 ROT_DEG 30.5 ROT_DEG -10 ESCALA -0.5;";
    std::string b = "ESCALA 1.5 ROT_DEG 30.5 ROT_DEG -10 ESCALA -0.5 ;";
    auto pa = tf::parseCommandSequence(a);
    auto pb = tf::parseCommandSequence(b);
    ASSERT_EQ(pa.size(), pb.size());
    for (size_t i = 0; i < pa.size(); ++i) {
        EXPECT_EQ(pa[i].key, pb[i].key);
        EXPECT_EQ(pa[i].hasValue, pb[i].hasValue);
        if (pa[i].hasValue && pb[i].hasValue) {
            EXPECT_DOUBLE_EQ(pa[i].value, pb[i].value);
        }
    }
}
