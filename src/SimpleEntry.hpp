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
    same = same && SM_CHECKMEMBERSSAME(other, key_);
    same = same && SM_CHECKMEMBERSSAME(other, value_);
    return same;
  }
  friend std::ostream& operator<<(std::ostream &os, const SimpleEntry& lhs);
};

std::ostream& operator<<(std::ostream &os, const SimpleEntry& lhs)
{
  os << lhs.key_ << " " << lhs.value_;
  return os;
}
