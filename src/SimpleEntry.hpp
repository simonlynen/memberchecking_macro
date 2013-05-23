#include <cstdlib>
#include "macro.h"


class SimpleEntry {
 private:
  int key_;
  double value_;
 public:
  void setRandom() {
    key_ = rand();
    value_ = static_cast<double>(rand()) / RAND_MAX;
  }
  bool operator==(const SimpleEntry& other) const {
    bool same = true;
    same = same && CHECKMEMBERSSAME(this, other, key_);
    same = same && CHECKMEMBERSSAME(this, other, value_);
    return same;
  }
};
