#include <iostream>
#include <boost/static_assert.hpp>
#include <type_traits>
#include <boost/shared_ptr.hpp>

class ComplexEntry;

namespace {

template<typename T>
class HasIsBinaryEqual {
  template<typename U, bool (U::*)(const T&) const> struct Check;
  template<typename U> static char func(Check<U, &U::isBinaryEqual> *);
  template<typename U> static int func(...);
 public:
  enum {
    value = (sizeof(func<T>(0)) == sizeof(char))
  };
};

template<bool, typename A>
struct isSame;

template<typename A>
struct isSame<true, A> {
  static bool eval(const A& lhs, const A& rhs) {
    return lhs.isBinaryEqual(rhs);
  }
};

template<typename A>
struct isSame<false, A> {
  static bool eval(const A& lhs, const A& rhs) {
    return lhs == rhs;
  }
};

template<typename A>
struct isSame<false, A*> {
  static bool eval(A const * lhs, A const * rhs) {
    if(!lhs || !rhs){
      return false;
    }
    return *lhs == *rhs;
  }
};

template<typename A>
struct isSame<false, boost::shared_ptr<A> > {
  static bool eval(const boost::shared_ptr<A>& lhs, const boost::shared_ptr<A>& rhs) {
    if(!lhs || !rhs){
      return false;
    }
    return *lhs == *rhs;
  }
};
}

#define CHECKMEMBERSSAME(THIS, OTHER, MEMBER) \
    (isSame<HasIsBinaryEqual<decltype(MEMBER)>::value, decltype(MEMBER) >::eval(THIS->MEMBER, OTHER.MEMBER)) ? true :\
        !(std::cout <<  "Validation failed on " << #MEMBER << ": "<< this->MEMBER << \
              " other " << OTHER.MEMBER << " at " << __PRETTY_FUNCTION__ << \
              " In: " << __FILE__ << ":" << __LINE__ << std::endl);
