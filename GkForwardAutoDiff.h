// A simple implementation of forward-mode AutoDiff using operator
// overloading

#pragma once

// std includes
#include <cmath>

namespace Gkyl {

  /* Hyperreal number: real + infinitesimal */
  class HyperReal {
    public:
      // various ctors
      HyperReal() {
        r_p = 0;
        inf_p[0] = 0;
      }
      HyperReal(double rel) {
        r_p = rel;
        inf_p[0] = 0;
      }
      HyperReal(double rel, double _inf) {
        r_p = rel;
        inf_p[0] = _inf;
      }

      // real part
      double real() const { return r_p; }
      // infinitesimal part
      double inf(unsigned n) const { return inf_p[n]; }
      
    private:
      double r_p; /* real part */
      double inf_p[1]; /* Infinitesimal parts */
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
    template <>
    struct _R<HyperReal> {
        static double g(const HyperReal& r) { return r.real(); }
    };
    /* Fetch infinitesimal part of HyperReal number */
    template <>
    struct _I<HyperReal> {
        static double g(unsigned i, const HyperReal& r) { return r.inf(i); }
    };
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
    };
  }

  // Math functions
  template <typename T> inline T sqrt(const T& x) { return _m<T>::sqrt(x); }
  template <typename T> inline T cos(const T& x) { return _m<T>::cos(x); }
  template <typename T> inline T sin(const T& x) { return _m<T>::sin(x); }
}
