
#include "macro.h"
#include <cstdlib>
#include <boost/shared_ptr.hpp>
#include "gtest/gtest.h"


class SimpleEntry{
 private:
  int key_;
  double value_;
 public:
  void setRandom(){
    key_ = rand();
    value_ = static_cast<double>(rand()) / RAND_MAX;
  }
  bool operator==(const SimpleEntry& other) const {
    std::cout<<__PRETTY_FUNCTION__<<std::endl;
    bool same = true;
    same = same && CHECKMEMBERSSAME(this, other, key_);
    same = same && CHECKMEMBERSSAME(this, other, value_);
    return same;
  }
};

class ComplexEntry{
 private:
  int key_;
  double value_;
  SimpleEntry* pSimple_;
  boost::shared_ptr<SimpleEntry> pSharedSimple_;

  bool operator==(const ComplexEntry& other) const;

 public:

  ComplexEntry():pSimple_(NULL){
    key_ = 0;
    value_ = 0;
    pSimple_ = new SimpleEntry;
    pSharedSimple_.reset(new SimpleEntry);
  }

  ~ComplexEntry(){
    delete pSimple_;
    pSimple_ = NULL;
  }

  ComplexEntry operator=(const ComplexEntry& rhs){
    key_ = rhs.key_;
    value_ = rhs.value_;
    pSimple_ = new SimpleEntry(*rhs.pSimple_);
    pSharedSimple_.reset(new SimpleEntry(*rhs.pSharedSimple_.get()));
    return *this;
  }

  void setRandom(){
    key_ = rand();
    value_ = static_cast<double>(rand()) / RAND_MAX;
    pSimple_->setRandom();
    pSharedSimple_->setRandom();
  }

  bool isBinaryEqual(const ComplexEntry& other) const {
    std::cout<<__PRETTY_FUNCTION__<<std::endl;
    bool same = true;
    same = same && CHECKMEMBERSSAME(this, other, key_);
    same = same && CHECKMEMBERSSAME(this, other, value_);
    same = same && CHECKMEMBERSSAME(this, other, pSimple_);
    same = same && CHECKMEMBERSSAME(this, other, pSharedSimple_);
    return same;
  }
};

TEST(General, TestClassesComparisonWorks) {
  ComplexEntry e1, e2;
  e1.setRandom();
  e2.setRandom();

  ASSERT_TRUE(e1.isBinaryEqual(e1));
  ASSERT_FALSE(e1.isBinaryEqual(e2));
  ASSERT_FALSE(e2.isBinaryEqual(e1));
}

TEST(General, TestClassesCopyCtorAssignWorks) {
  ComplexEntry e1, e2;
  e1.setRandom();
  e2.setRandom();

  ASSERT_TRUE(e1.isBinaryEqual(e1));
  ASSERT_FALSE(e1.isBinaryEqual(e2));
  ASSERT_FALSE(e2.isBinaryEqual(e1));

  e2 = e1;
  ComplexEntry e3(e1);
  ASSERT_TRUE(e2.isBinaryEqual(e1));
  ASSERT_TRUE(e1.isBinaryEqual(e2));

  ASSERT_TRUE(e3.isBinaryEqual(e1));
  ASSERT_TRUE(e1.isBinaryEqual(e3));

}

TEST(General, TestClassHasMethodDeduction) {
  ASSERT_EQ(HasIsBinaryEqual<ComplexEntry>::value, 1);
  ASSERT_EQ(HasIsBinaryEqual<SimpleEntry>::value, 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
