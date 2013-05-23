#ifndef ASLAM_SERIALIZATION_MACROS_HPP
#define ASLAM_SERIALIZATION_MACROS_HPP

#include <iostream>
#include <boost/static_assert.hpp>
#include <type_traits>
#include <boost/shared_ptr.hpp>
#include <sstream>


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

template<typename T>
class HasOStreamOperator {
  template<typename U, std::ostream& (U::*)(std::ostream&) const> struct CheckMember;
  template<typename U> static char funcMember(CheckMember<U, &U::operator<<> *); //std::ostream& operator<<(std::ostream& lhs);
  template<typename U> static int funcMember(...);
 public:
  enum {
    value = (sizeof(funcMember<T>(0)) == sizeof(char))
  };
};

template<class T>
auto serialize_imp(std::ostream& os, T const& obj, int)
    -> decltype(os << obj, void())
{
  os << obj;
}

template<class T>
auto serialize_imp(std::ostream& os, T const& obj, long)
    -> decltype(obj.stream(os), void())
{
  obj.stream(os);
}

template<class T>
auto serialize(std::ostream& os, const T& obj)
    -> decltype(serialize_imp(os, obj, 0), void())
{
  serialize_imp(os, obj, 0);
}

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

//if the object supports it stream to ostream, otherwise put NA
template<bool, typename A>
struct streamIf;

template<typename A>
struct streamIf<true, A> {
  static std::string eval(const A& rhs){
    std::stringstream ss;
    ss<<rhs;
    return ss.str();
  }
};

template<typename A>
struct streamIf<false, A> {
  static std::string eval(const A&){
    return "NA";
  }
};
}


//this defines the default behaviour if no verbosity argument is given
#define ASLAM_SERIALIZATION_CHECKMEMBERSSAME_VERBOSE(OTHER, MEMBER) ASLAM_SERIALIZATION_CHECKMEMBERSSAME_IMPL(OTHER, MEMBER, true)

#define ASLAM_SERIALIZATION_CHECKMEMBERSSAME_IMPL(OTHER, MEMBER, VERBOSE) \
    (isSame<HasIsBinaryEqual<decltype(MEMBER)>::value, decltype(MEMBER) >::eval(this->MEMBER, OTHER.MEMBER)) ? true :\
        (VERBOSE ? (std::cout <<  "*** Validation failed on " << #MEMBER << ": "<< \
            streamIf<HasOStreamOperator<decltype(MEMBER)>::value, decltype(MEMBER) >::eval(this->MEMBER) << \
              " other " << streamIf<HasOStreamOperator<decltype(MEMBER)>::value, decltype(MEMBER) >::eval(OTHER.MEMBER) \
              << " at " << __PRETTY_FUNCTION__ << \
              " In: " << __FILE__ << ":" << __LINE__ << std::endl<<std::endl) && false : false);


#define ASLAM_SERIALIZATION_GET_4TH_ARG(arg1, arg2, arg3, arg4, ...) arg4
#define ASLAM_SERIALIZATION_MACRO_CHOOSER(...) \
    ASLAM_SERIALIZATION_GET_4TH_ARG(__VA_ARGS__, ASLAM_SERIALIZATION_CHECKMEMBERSSAME_IMPL,  ASLAM_SERIALIZATION_CHECKMEMBERSSAME_VERBOSE )

//this is the visible macro name that the user should use
#define CHECKMEMBERSSAME(...) ASLAM_SERIALIZATION_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)


#endif //ASLAM_SERIALIZATION_MACROS_HPP
