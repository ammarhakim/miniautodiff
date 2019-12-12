// A simple implementation of forward-mode AutoDiff using operator
// overloading

#pragma once

// std includes
#include <cmath>

namespace Gkyl {

  /* Hyperreal number: real + infinitesimal. RT is the type of the
   * real-part and AT the type of the adoint (derivative) part */
  template <typename RT, typename AT=RT>
  class HyperReal {
    public:
      // various ctors
      HyperReal()
        : r_p(0), inf_p(0) {
      }
      HyperReal(const RT& rel)
        : r_p(rel), inf_p(0) {
      }
      HyperReal(double rel, double _inf)
        : r_p(rel), inf_p(_inf) {
      }

      // real part
      RT real() const { return r_p; }
      // infinitesimal part
      AT inf() const { return inf_p; }
      
    private:
      RT r_p; /* real part */
      AT inf_p; /* Infinitesimal parts */
  };

  namespace {
    /* Fetch real part of POD number */
    template <typename T>
    struct _R {
        static T g(T r) { return r; }
    };
    /* Fetch infinitesimal part of POD number */
    template <typename T>
    struct _I {
        static T g(unsigned i, T r) { return 0; }
    };    

    /* Fetch real part of HyperReal number */
    template <typename RT, typename AT>
    struct _R<HyperReal<RT, AT> > {
        static RT g(const HyperReal<RT, AT>& r) { return r.real(); }
    };
    /* Fetch infinitesimal part of HyperReal number */
    template <typename RT, typename AT>
    struct _I<HyperReal<RT, AT> > {
        static AT g(unsigned i, const HyperReal<RT, AT>& r) { return r.inf(); }
    };
  }

  namespace {
    // sign of value
    template <typename T>
    int sgn(T val) { return (T(0) < val) - (val < T(0)); }
  }

  /* Basic arithmetic operators */

  // binary +
  template<typename L, typename R>
  inline HyperReal operator+(const L& lv, const R& rv) {
    double x0 = _R<L>::g(lv), x1 = _I<L>::g(0,lv);
    double y0 = _R<R>::g(rv), y1 = _I<R>::g(0,rv);
    
    double rel = x0+y0;
    double inf = x1+y1;
    return HyperReal(rel, inf);
  }

  // binary -  
  template<typename L, typename R>
  inline HyperReal operator-(const L& lv, const R& rv) {
    double x0 = _R<L>::g(lv), x1 = _I<L>::g(0,lv);
    double y0 = _R<R>::g(rv), y1 = _I<R>::g(0,rv);
    
    double rel = x0-y0;
    double inf = x1-y1;
    return HyperReal(rel, inf);
  }

  // binary *  
  template<typename L, typename R>
  inline HyperReal operator*(const L& lv, const R& rv) {
    double x0 = _R<L>::g(lv), x1 = _I<L>::g(0,lv);
    double y0 = _R<R>::g(rv), y1 = _I<R>::g(0,rv);
    
    double rel = x0*y0;
    double inf = x0*y1+x1*y0;

    return HyperReal(rel, inf);
  }

  // binary /
  template<typename L, typename R>
  inline HyperReal operator/(const L& lv, const R& rv) {
    double x0 = _R<L>::g(lv), x1 = _I<L>::g(0,lv);
    double y0 = _R<R>::g(rv), y1 = _I<R>::g(0,rv);
    
    double rel = x0/y0;
    double inf = -(x0*y1-x1*y0)/(y0*y0);
    
    return HyperReal(rel, inf);
  }

  // unary -
  template<typename T>
  inline HyperReal operator-(const T& x) {
    double x0 = _R<T>::g(x), x1 = _I<T>::g(0,x);
    return HyperReal(-x0, -x1);
  }

  // unary +
  template<typename T>
  inline HyperReal operator+(const T& v) {
    return v;
  }

  /* Derivatives of functions from std::math library */

  namespace {
    // this default private struct supplies methods for use with POD
    // types (double and float)
    template <typename T>
    struct _m {
        static T sqrt(const T& x) { return std::sqrt(x); }        
        static T cos(const T& x) { return std::cos(x); }
        static T sin(const T& x) { return std::sin(x); }
        static T tan(const T& x) { return std::tan(x); }
        static T asin(const T& x) { return std::asin(x); }
        static T acos(const T& x) { return std::acos(x); }
        static T atan(const T& x) { return std::atan(x); }
        static T sinh(const T& x) { return std::sinh(x); }
        static T cosh(const T& x) { return std::cosh(x); }
        static T tanh(const T& x) { return std::tanh(x); }
        static T exp(const T& x) { return std::exp(x); }
        static T log(const T& x) { return std::log(x); }
        static T abs(const T& x) { return std::abs(x); }
        static T floor(const T& x) { return std::floor(x); }
        static T ceil(const T& x) { return std::ceil(x); }
    };
    
    // specialization to HyperReal number
    template <>
    struct _m<HyperReal> {
        
        static HyperReal sqrt(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          double y0 = std::sqrt(x0);
          return HyperReal(y0, 0.5*x1/y0);
        }
        
        static HyperReal cos(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          return HyperReal(std::cos(x0), -x1*std::sin(x0));
        }
        
        static HyperReal sin(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          return HyperReal(std::sin(x0), x1*std::cos(x0));
        }

        static HyperReal tan(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          double tx0 = std::tan(x0);
          return HyperReal(tx0, x1*(1+tx0*tx0));
        }

        static HyperReal asin(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          return HyperReal(std::asin(x0), x1/std::sqrt(1-x0*x0));
        }

        static HyperReal acos(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          return HyperReal(std::acos(x0), -x1/std::sqrt(1-x0*x0));
        }

        static HyperReal atan(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          return HyperReal(std::atan(x0), x1/(1+x0*x0));
        }

        static HyperReal sinh(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          return HyperReal(std::sinh(x0), x1*std::cosh(x0));
        }

        static HyperReal cosh(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          return HyperReal(std::cosh(x0), x1*std::sinh(x0));
        }

        static HyperReal tanh(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          double tx0 = std::tanh(x0);
          return HyperReal(tx0, x1*(1-tx0*tx0));
        }

        static HyperReal exp(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          double ex0 = std::exp(x0);
          return HyperReal(ex0, x1*ex0);
        }

        static HyperReal log(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          return HyperReal(std::log(x0), x1/x0);
        }

        static HyperReal abs(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          return HyperReal(std::abs(x0), x1*sgn(x0));
        }

        static HyperReal floor(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          return HyperReal(std::floor(x0), 0.0);
        }

        static HyperReal ceil(const HyperReal& x) {
          double x0 = _R<HyperReal>::g(x), x1 = _I<HyperReal>::g(0,x);
          return HyperReal(std::ceil(x0), 0.0);
        }
    };
  }

  // Math functions
  template <typename T> inline T sqrt(const T& x) { return _m<T>::sqrt(x); }
  template <typename T> inline T cos(const T& x) { return _m<T>::cos(x); }
  template <typename T> inline T sin(const T& x) { return _m<T>::sin(x); }
  template <typename T> inline T tan(const T& x) { return _m<T>::tan(x); }
  template <typename T> inline T asin(const T& x) { return _m<T>::asin(x); }
  template <typename T> inline T acos(const T& x) { return _m<T>::acos(x); }
  template <typename T> inline T atan(const T& x) { return _m<T>::atan(x); }
  template <typename T> inline T sinh(const T& x) { return _m<T>::sinh(x); }
  template <typename T> inline T cosh(const T& x) { return _m<T>::cosh(x); }
  template <typename T> inline T tanh(const T& x) { return _m<T>::tanh(x); }
  template <typename T> inline T exp(const T& x) { return _m<T>::exp(x); }
  template <typename T> inline T log(const T& x) { return _m<T>::log(x); }
  template <typename T> inline T abs(const T& x) { return _m<T>::abs(x); }
  template <typename T> inline T floor(const T& x) { return _m<T>::floor(x); }
  template <typename T> inline T ceil(const T& x) { return _m<T>::ceil(x); }
}
