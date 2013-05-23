#include <cstdlib>
#include <boost/shared_ptr.hpp>
#include "gtest/gtest.h"

#include "macro.h"
#include "SimpleEntry.hpp"
#include "ComplexEntry.hpp"


TEST(General, TestClassesComparisonWorks) {
  ComplexEntry e1, e2;
  e1.setRandom();
  e2.setRandom();

  ASSERT_TRUE(e1.isBinaryEqual(e1));
  ASSERT_FALSE(e1.isBinaryEqual(e2));
  ASSERT_FALSE(e2.isBinaryEqual(e1));
}


TEST(General, TestClassesMacroWorks) {
  ComplexEntry e1, e2;
  e1.setRandom();
  e2.setRandom();

  ASSERT_FALSE(SM_CHECKSAME(e1, e2));
  ASSERT_FALSE(SM_CHECKSAME(e2, e1));

  e2 = e1;

  ASSERT_TRUE(SM_CHECKSAME(e1, e2));
  ASSERT_TRUE(SM_CHECKSAME(e2, e1));

  ComplexEntry e3(e1);

  ASSERT_TRUE(SM_CHECKSAME(e1, e3));
  ASSERT_TRUE(SM_CHECKSAME(e3, e1));

}

TEST(General, TestClassesCopyCtorAssignWorks) {
  ComplexEntry e1, e2;
  e1.setRandom();
  e2.setRandom();

  ASSERT_TRUE(e1.isBinaryEqual(e1));
  ASSERT_FALSE(e1.isBinaryEqual(e2));
  ASSERT_FALSE(e2.isBinaryEqual(e1));

  e2 = e1;
  ASSERT_TRUE(e2.isBinaryEqual(e1));
  ASSERT_TRUE(e1.isBinaryEqual(e2));

  ComplexEntry e3(e1);
  ASSERT_TRUE(e3.isBinaryEqual(e1));
  ASSERT_TRUE(e1.isBinaryEqual(e3));

}

TEST(General, TestClassHasMethodDeduction) {
  ASSERT_EQ(HasIsBinaryEqual<ComplexEntry>::value, 1);
  ASSERT_EQ(HasIsBinaryEqual<SimpleEntry>::value, 0);
}

TEST(General, TestClassHasStreamOperator) {
  ComplexEntry e1;
  e1.setRandom();
  streamIf<HasOStreamOperator<std::ostream, decltype(e1)>::value, decltype(e1) >::eval(e1);

  ASSERT_EQ((HasOStreamOperator<std::ostream, SimpleEntry>::value), 1);
  ASSERT_EQ((HasOStreamOperator<std::ostream, ComplexEntry>::value), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
