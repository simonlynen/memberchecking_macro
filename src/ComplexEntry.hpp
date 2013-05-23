#include <cstdlib>
#include <boost/shared_ptr.hpp>
#include "macro.h"

class ComplexEntry {
 private:
  int key_;
  double value_;
  SimpleEntry* pSimple_;
  boost::shared_ptr<SimpleEntry> pSharedSimple_;

  bool operator==(const ComplexEntry& other) const;

 public:

  ComplexEntry(){
    key_ = 0;
    value_ = 0;
    pSimple_ = new SimpleEntry;
    pSharedSimple_.reset(new SimpleEntry);
  }

  ~ComplexEntry() {
    delete pSimple_;
    pSimple_ = NULL;
  }

  ComplexEntry(const ComplexEntry& rhs) {
    key_ = rhs.key_;
    value_ = rhs.value_;
    pSimple_ = new SimpleEntry(*rhs.pSimple_);
    pSharedSimple_.reset(new SimpleEntry(*rhs.pSharedSimple_));
  }

  ComplexEntry operator=(const ComplexEntry& rhs) {
    key_ = rhs.key_;
    value_ = rhs.value_;
    pSimple_ = new SimpleEntry(*rhs.pSimple_);
    pSharedSimple_.reset(new SimpleEntry(*rhs.pSharedSimple_));
    return *this;
  }

  void setRandom() {
    key_ = rand();
    value_ = static_cast<double>(rand()) / RAND_MAX;
    pSimple_->setRandom();
    pSharedSimple_->setRandom();
  }

  bool isBinaryEqual(const ComplexEntry& other) const {
    bool same = true;
    same = same && CHECKMEMBERSSAME(this, other, key_);
    same = same && CHECKMEMBERSSAME(this, other, value_);
    same = same && CHECKMEMBERSSAME(this, other, pSimple_);
    same = same && CHECKMEMBERSSAME(this, other, pSharedSimple_);
    return same;
  }
};
