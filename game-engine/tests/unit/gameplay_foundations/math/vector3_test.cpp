// Unit tests for Vector3 using Google Test

#include <gtest/gtest.h>

#include "gameplay_foundations/math/vector3.hpp"

TEST(Vector3Test, DefaultConstructorInitializesToZero) {
    nathan::Vector3 v;

    nathan::Vector3 expected_result{0.0f, 0.0f, 0.0f};
    EXPECT_EQ(v, expected_result);
}

TEST(Vector3Test, PlusEqualsAddsComponentsInPlace) {
    nathan::Vector3 a{1.0f, 2.0f, 3.0f};
    nathan::Vector3 b{10.0f, 20.0f, 30.0f};

    a += b;

    nathan::Vector3 expected_result{11.0f, 22.0f, 33.0f};
    EXPECT_EQ(a, expected_result);
}

TEST(Vector3Test, OperatorPlusReturnsSum) {
    nathan::Vector3 a{1.0f, 2.0f, 3.0f};
    nathan::Vector3 b{4.0f, 5.0f, 6.0f};

    nathan::Vector3 result = a + b;

    nathan::Vector3 expected_result{5.0f, 7.0f, 9.0f};
    EXPECT_EQ(result, expected_result);
}

TEST(Vector3Test, OperatorMinusReturnsDifference) {
    nathan::Vector3 a{5.0f, 7.0f, 9.0f};
    nathan::Vector3 b{1.0f, 2.0f, 3.0f};

    nathan::Vector3 result = a - b;

    nathan::Vector3 expected_result{4.0f, 5.0f, 6.0f};
    EXPECT_EQ(result, expected_result);
}

TEST(Vector3Test, OperatorMultipliesByScalar) {
    nathan::Vector3 v{1.0f, -2.0f, 3.0f};

    nathan::Vector3 result = v * 2.0f;

    nathan::Vector3 expected_result{2.0f, -4.0f, 6.0f};
    EXPECT_EQ(result, expected_result);
}

TEST(Vector3Test, OperatorMultipliesByZeroScalarYieldsZeroVector) {
    nathan::Vector3 v{1.0f, -2.0f, 3.0f};

    nathan::Vector3 result = v * 0.0f;

    nathan::Vector3 expected_result{0.0f, 0.0f, 0.0f};
    EXPECT_EQ(result, expected_result);
}

TEST(Vector3Test, ChainedAdditionAndScalarMultiplicationProduceExpectedResult) {
    nathan::Vector3 a{1.0f, 1.0f, 1.0f};
    nathan::Vector3 b{2.0f, 3.0f, 4.0f};

    nathan::Vector3 result = (a + b) * 2.0f;

    nathan::Vector3 expected_result{6.0f, 8.0f, 10.0f};
    EXPECT_EQ(result, expected_result);
}

TEST(Vector3Test, OperatorEqualsReturnsTrueForEqualVectors) {
    nathan::Vector3 a{1.0f, 2.0f, 3.0f};
    nathan::Vector3 b{1.0f, 2.0f, 3.0f};

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(Vector3Test, OperatorEqualsReturnsFalseForDifferentVectors) {
    nathan::Vector3 a{1.0f, 2.0f, 3.0f};
    nathan::Vector3 b{4.0f, 5.0f, 6.0f};

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(Vector3Test, OperatorEqualsReturnsTrueForApproximatelyEqualVectors) {
    nathan::Vector3 a{1.0f, 2.0f, 3.0f};
    nathan::Vector3 b{1.0f + 1e-6f, 2.0f - 1e-6f, 3.0f + 5e-6f};

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(Vector3Test, OperatorOutputStreamsVectorInExpectedFormat) {
    nathan::Vector3 v{1.0f, 2.0f, 3.0f};

    std::ostringstream oss;
    oss << v;

    std::string expected_output = "(1, 2, 3)";
    EXPECT_EQ(oss.str(), expected_output);
}
