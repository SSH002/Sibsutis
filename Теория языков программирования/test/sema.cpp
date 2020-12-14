#include <gtest/gtest.h>
#include "parser.hpp"

TEST(semantic_test, check_opers_data_types)
{
	Sema *sema = new Sema();
	map <string, string> type_id;

	type_id.emplace("$str1_size", "int");
	type_id.emplace("$str2_size", "int");
	vector <string> operans = {"$str1_size", "$str2_size"};
	vector <string> expected;

	vector <string> result = sema->check_opers_data_types(operans, type_id, "=");
	EXPECT_EQ(expected, result);
}

TEST(semantic_test, check_binary_oper_sign)
{
	Sema *sema = new Sema();
	bool result = sema->check_binary_oper_sign("%");

	EXPECT_EQ(true, result);
}

