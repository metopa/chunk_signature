#include <gtest/gtest.h>

#include "../src/program_config.h"

TEST(sizeLiteralToNumTest, Basic) {
	ASSERT_EQ(sizeLiteralToNum(""), 1);
	ASSERT_EQ(sizeLiteralToNum("k"), 1024);
	ASSERT_EQ(sizeLiteralToNum("m"), 1024 * 1024);
	ASSERT_EQ(sizeLiteralToNum("g"), 1024 * 1024 * 1024);
	ASSERT_THROW(sizeLiteralToNum("x"), std::runtime_error);
}

TEST(sizeLiteralToNumTest, HandlesB) {
	ASSERT_EQ(sizeLiteralToNum("b"), 1);
	ASSERT_EQ(sizeLiteralToNum("kb"), 1024);
	ASSERT_EQ(sizeLiteralToNum("mb"), 1024 * 1024);
	ASSERT_EQ(sizeLiteralToNum("gb"), 1024 * 1024 * 1024);
	ASSERT_THROW(sizeLiteralToNum("xb"), std::runtime_error);
}

TEST(sizeLiteralToNumTest, HandlesUppercase) {
	ASSERT_EQ(sizeLiteralToNum("B"), 1);
	ASSERT_EQ(sizeLiteralToNum("Kb"), 1024);
	ASSERT_EQ(sizeLiteralToNum("kB"), 1024);
	ASSERT_EQ(sizeLiteralToNum("MB"), 1024 * 1024);
	ASSERT_EQ(sizeLiteralToNum("GB"), 1024 * 1024 * 1024);
	ASSERT_THROW(sizeLiteralToNum("X"), std::runtime_error);
}


TEST(exactSizeStringToNum, Basic) {
	ASSERT_EQ(exactSizeStringToNum("000"), 0);
	ASSERT_EQ(exactSizeStringToNum("1200"), 1200);
	ASSERT_EQ(exactSizeStringToNum("   1200 "), 1200);
}

TEST(exactSizeStringToNum, HandlesLiteral) {
	ASSERT_EQ(exactSizeStringToNum("10b"), 10);
	ASSERT_EQ(exactSizeStringToNum("10 b"), 10);
	ASSERT_EQ(exactSizeStringToNum("10 b "), 10);
	ASSERT_EQ(exactSizeStringToNum("10kb"), 10240);
	ASSERT_EQ(exactSizeStringToNum("10 kb"), 10240);
	ASSERT_EQ(exactSizeStringToNum("10 kb "), 10240);
	ASSERT_EQ(exactSizeStringToNum("10 KB "), 10240);
}

TEST(exactSizeStringToNum, FloatInput) {
	ASSERT_THROW(exactSizeStringToNum("00."), std::runtime_error);
	ASSERT_THROW(exactSizeStringToNum("3.14"), std::runtime_error);
	ASSERT_THROW(exactSizeStringToNum("2.KB"), std::runtime_error);
}

TEST(floatSizeStringToNum, Basic) {
	ASSERT_EQ(floatSizeStringToNum("000"), 0);
	ASSERT_EQ(floatSizeStringToNum("1200"), 1200);
	ASSERT_EQ(floatSizeStringToNum("   1200 "), 1200);
	ASSERT_EQ(floatSizeStringToNum("00.0"), 0);
	ASSERT_EQ(floatSizeStringToNum("1.200"), 1);
	ASSERT_EQ(floatSizeStringToNum("   .1200 "), 0);
}

TEST(floatSizeStringToNum, HandlesLiteral) {
	ASSERT_EQ(floatSizeStringToNum("10.3b"), 10);
	ASSERT_EQ(floatSizeStringToNum("10.2 b"), 10);
	ASSERT_EQ(floatSizeStringToNum("10.7 b "), 11);
	ASSERT_EQ(floatSizeStringToNum("0.5kb"), 512);
	ASSERT_EQ(floatSizeStringToNum("0.5 kb"), 512);
	ASSERT_EQ(floatSizeStringToNum("0.5 kb "), 512);
	ASSERT_EQ(floatSizeStringToNum("0.5 KB "), 512);
}

