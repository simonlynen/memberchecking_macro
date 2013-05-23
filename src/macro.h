#include <iostream>
#include <boost/static_assert.hpp>
#include <type_traits>
#include <boost/shared_ptr.hpp>


namespace {
//this template metaprogramming struct can tell us whether a class has a member
//function isBinaryEqual
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

//these structs are used to choose between isBinaryEqual function call and the
//default operator==
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

//this defines the default behaviour if no verbosity argument is given
#define ASLAM_SERIALIZATION_CHECKMEMBERSSAME_VERBOSE(THIS, OTHER, MEMBER) ASLAM_SERIALIZATION_CHECKMEMBERSSAME_IMPL(THIS, OTHER, MEMBER, true)

#define ASLAM_SERIALIZATION_CHECKMEMBERSSAME_IMPL(THIS, OTHER, MEMBER, VERBOSE) \
    (isSame<HasIsBinaryEqual<decltype(MEMBER)>::value, decltype(MEMBER) >::eval(THIS->MEMBER, OTHER.MEMBER)) ? true :\
        (VERBOSE ? (std::cout <<  "*** Validation failed on " << #MEMBER << ": "<< this->MEMBER << \
              " other " << OTHER.MEMBER << " at " << __PRETTY_FUNCTION__ << \
              " In: " << __FILE__ << ":" << __LINE__ << std::endl<<std::endl) && false : false);


#define ASLAM_SERIALIZATION_GET_5TH_ARG(arg1, arg2, arg3, arg4, arg5, ...) arg5
#define ASLAM_SERIALIZATION_MACRO_CHOOSER(...) \
    ASLAM_SERIALIZATION_GET_5TH_ARG(__VA_ARGS__, ASLAM_SERIALIZATION_CHECKMEMBERSSAME_IMPL,  ASLAM_SERIALIZATION_CHECKMEMBERSSAME_VERBOSE )

//this is the visible macro name that the user should use
#define CHECKMEMBERSSAME(...) ASLAM_SERIALIZATION_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


