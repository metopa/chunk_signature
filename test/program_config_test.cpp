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


TEST(sizeStringToNum, Basic) {
	ASSERT_EQ(sizeStringToNum("000"), 0);
	ASSERT_EQ(sizeStringToNum("1200"), 1200);
	ASSERT_EQ(sizeStringToNum("   1200 "), 1200);
}

TEST(sizeStringToNum, HandlesLiteral) {
	ASSERT_EQ(sizeStringToNum("10b"), 10);
	ASSERT_EQ(sizeStringToNum("10 b"), 10);
	ASSERT_EQ(sizeStringToNum("10 b "), 10);
	ASSERT_EQ(sizeStringToNum("10kb"), 10240);
	ASSERT_EQ(sizeStringToNum("10 kb"), 10240);
	ASSERT_EQ(sizeStringToNum("10 kb "), 10240);
	ASSERT_EQ(sizeStringToNum("10 KB "), 10240);
}


TEST(sizeStringToNum, HandlesFloatInput) {
	ASSERT_EQ(sizeStringToNum("00.0"), 0);
	ASSERT_EQ(sizeStringToNum("1.200"), 1);
	ASSERT_THROW(sizeStringToNum("   .1200 "), std::runtime_error);
}

TEST(sizeStringToNum, HandlesFloatInputWithLiteral) {
	ASSERT_EQ(sizeStringToNum("10.3b"), 10);
	ASSERT_EQ(sizeStringToNum("10.2 b"), 10);
	ASSERT_EQ(sizeStringToNum("10.7 b "), 11);
	ASSERT_EQ(sizeStringToNum("0.5kb"), 512);
	ASSERT_EQ(sizeStringToNum("0.5 kb"), 512);
	ASSERT_EQ(sizeStringToNum("0.5 kb "), 512);
	ASSERT_EQ(sizeStringToNum("0.5 KB "), 512);
}

