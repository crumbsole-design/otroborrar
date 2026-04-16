#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <vector>
#include <optional>
#include <cmath>
#include "transformations.h"

using tf::CommandToken;
using tf::Vector2D;

TEST(FlowExecution, SequentialScaleThenRotate) {
    // ESCALA 2 then ROT_DEG 90 applied to (1,0) => scale to (2,0) then rotate 90 => (0,2)
    std::string input = "ESCALA 2 ROT_DEG 90 ;";
    auto cmds = tf::parseCommandSequence(input);
    double x = 1.0, y = 0.0;
    double scale = 1.0;
    double rot = 0.0;
    for (auto &c : cmds) {
        if (c.key == "ESCALA" && c.hasValue) scale = c.value;
        if (c.key == "ROT_DEG" && c.hasValue) rot = c.value;
    }
    Vector2D v{x,y};
    auto sv = tf::scaleVector(v, scale);
    auto rv = tf::rotateVectorDeg(sv, rot);
    EXPECT_NEAR(rv.x, 0.0, 1e-10);
    EXPECT_NEAR(rv.y, 2.0, 1e-10);
}

TEST(Validation, InvalidCommandDetected) {
    std::string input = "ESCALA 1 FOO 3 ;";
    auto cmds = tf::parseCommandSequence(input);
    bool foundInvalid = false;
    for (auto &c : cmds) {
        if (c.key != "ESCALA" && c.key != "ROT_DEG") {
            foundInvalid = true;
            break;
        }
    }
    EXPECT_TRUE(foundInvalid);
}

TEST(Validation, MissingParameterBeforeSemicolon) {
    // Missing number after ROT_DEG before ';'
    std::string input = "ESCALA 1 ROT_DEG ;";
    auto cmds = tf::parseCommandSequence(input);
    bool missingParam = false;
    for (auto &c : cmds) {
        if ((c.key == "ESCALA" || c.key == "ROT_DEG") && !c.hasValue) {
            missingParam = true;
            break;
        }
    }
    EXPECT_TRUE(missingParam);
}
