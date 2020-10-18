#include <gtest/gtest.h>
#include "parser.hpp"

TEST(semantic_test, check_binary_oper_sign)
{
	Sema *sema = new Sema();
	bool result = sema->check_binary_oper_sign("%");

	EXPECT_EQ(true, result);
}

