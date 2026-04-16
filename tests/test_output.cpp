#include <gtest/gtest.h>
#include "transformations.h"

TEST(OutputFormatting, FixedThreeDecimals) {
    double x = 1.5;
    EXPECT_EQ(tf::formatFixed3(x), "1.500");
    double y = -0.123456;
    EXPECT_EQ(tf::formatFixed3(y), "-0.123");
}

TEST(OutputFormatting, SequenceFormat) {
    tf::Vector2D v{1.5, -2.3456};
    double scale = 2.0;
    std::string out = tf::formatSequenceOutput(scale, v);
    EXPECT_EQ(out, "Escala a = 2.000, v = (1.500, -2.346)");
}
