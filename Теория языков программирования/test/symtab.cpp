#include <gtest/gtest.h>
#include "parser.hpp"

TEST(SymTab_test, check_multi_declaration)
{
	SymbolTable *symbol_table = new SymbolTable();
	vector <string> vec = {"$str1_size", "$str2_size"};
	vector <string> expected = {"$str1_size", "$str2_size"};
	vector <string> result = symbol_table->check_multi_declaration(vec);

	EXPECT_EQ(expected, result);
}

TEST(semantic_test, compare_levels)
{
	SymbolTable *symbol_table = new SymbolTable();
	bool result = symbol_table->compare_levels("2a", "3a");

	EXPECT_EQ(true, result);
}

