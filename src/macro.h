#ifndef ASLAM_SERIALIZATION_MACROS_HPP
#define ASLAM_SERIALIZATION_MACROS_HPP

#include <iostream>
#include <boost/static_assert.hpp>
#include <type_traits>
#include <boost/shared_ptr.hpp>
#include <sstream>
#include <ostream>

namespace {
typedef char yes;
typedef char (&no)[2];

struct anyx { template <class T> anyx(const T &); };

no operator << (const anyx &, const anyx &);
no operator >> (const anyx &, const anyx &);


template <class T> yes check(T const&);
no check(no);


template <typename StreamType, typename T>
struct HasOStreamOperator {
  static StreamType & stream;
  static T & x;
  enum{
    value = sizeof(check(stream << x)) == sizeof(yes)
  };
};


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
    if (!lhs || !rhs) {
      return false;
    }
    return *lhs == *rhs;
  }
};

template<typename A>
struct isSame<false, boost::shared_ptr<A> > {
  static bool eval(const boost::shared_ptr<A>& lhs, const boost::shared_ptr<A>& rhs) {
    if (!lhs || !rhs) {
      return false;
    }
    return *lhs == *rhs;
  }
};

//if the object supports it stream to ostream, otherwise put NA
template<bool, typename A>
struct streamIf;

template<typename A>
struct streamIf<true, A> {
  static std::string eval(const A& rhs) {
    std::stringstream ss;
    ss << rhs;
    return ss.str();
  }
};


template<typename A>
struct streamIf<true, A*> {
  static std::string eval(const A* rhs) {
    if(!rhs){
      return "NULL";
    }
    std::stringstream ss;
    ss << *rhs;
    return ss.str();
  }
};

template<typename A>
struct streamIf<false, A> {
  static std::string eval(const A&) {
    return "NA";
  }
};
}

//this defines the default behaviour if no verbosity argument is given

#define ASLAM_SERIALIZATION_CHECKSAME_IMPL(THIS, OTHER, VERBOSE) \
    (isSame<HasIsBinaryEqual<decltype(OTHER)>::value, decltype(OTHER) >::eval(THIS, OTHER)) ? true :\
        (VERBOSE ? (std::cout <<  "*** Validation failed on " << #OTHER << ": "<< \
            streamIf<HasOStreamOperator<std::ostream, decltype(OTHER)>::value, decltype(OTHER) >::eval(THIS) << \
            " other " << streamIf<HasOStreamOperator<std::ostream, decltype(OTHER)>::value >::eval(OTHER) \
            << " at " << __PRETTY_FUNCTION__ << \
            " In: " << __FILE__ << ":" << __LINE__ << std::endl << std::endl) && false : false)

#define ASLAM_SERIALIZATION_CHECKMEMBERSSAME_IMPL(OTHER, MEMBER, VERBOSE) \
    (isSame<HasIsBinaryEqual<decltype(MEMBER)>::value, decltype(MEMBER) >::eval(this->MEMBER, OTHER.MEMBER)) ? true :\
        (VERBOSE ? (std::cout <<  "*** Validation failed on " << #MEMBER << ": "<< \
            streamIf<HasOStreamOperator<std::ostream, decltype(MEMBER)>::value, decltype(MEMBER) >::eval(this->MEMBER) << \
            " other " << streamIf<HasOStreamOperator<std::ostream, decltype(MEMBER)>::value, decltype(MEMBER) >::eval(OTHER.MEMBER) \
            << " at " << __PRETTY_FUNCTION__ << \
            " In: " << __FILE__ << ":" << __LINE__ << std::endl << std::endl) && false : false)

#define ASLAM_SERIALIZATION_CHECKSAME_VERBOSE(THIS, OTHER) ASLAM_SERIALIZATION_CHECKSAME_IMPL(THIS, OTHER, true)
#define ASLAM_SERIALIZATION_CHECKMEMBERSSAME_VERBOSE(OTHER, MEMBER) ASLAM_SERIALIZATION_CHECKMEMBERSSAME_IMPL(OTHER, MEMBER, true)

#define ASLAM_SERIALIZATION_GET_3RD_ARG(arg1, arg2, arg3, ...) arg3
#define ASLAM_SERIALIZATION_GET_4TH_ARG(arg1, arg2, arg3, arg4, ...) arg4

#define ASLAM_SERIALIZATION_MACRO_CHOOSER_MEMBER_SAME(...) \
    ASLAM_SERIALIZATION_GET_4TH_ARG(__VA_ARGS__, ASLAM_SERIALIZATION_CHECKMEMBERSSAME_IMPL,  ASLAM_SERIALIZATION_CHECKMEMBERSSAME_VERBOSE )
#define ASLAM_SERIALIZATION_MACRO_CHOOSER_SAME(...) \
    ASLAM_SERIALIZATION_GET_4TH_ARG(__VA_ARGS__, ASLAM_SERIALIZATION_CHECKSAME_IMPL,  ASLAM_SERIALIZATION_CHECKSAME_VERBOSE )

//this is the visible macro name that the user should use
#define CHECKMEMBERSSAME(...) ASLAM_SERIALIZATION_MACRO_CHOOSER_MEMBER_SAME(__VA_ARGS__)(__VA_ARGS__)
#define CHECKSAME(...) ASLAM_SERIALIZATION_MACRO_CHOOSER_SAME(__VA_ARGS__)(__VA_ARGS__)

#endif //ASLAM_SERIALIZATION_MACROS_HPP
