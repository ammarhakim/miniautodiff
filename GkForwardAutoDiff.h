// A simple implementation of AutoDiff using operator overloading

#pragma once

// std includes
#include <cmath>

namespace Gkyl {

  /* Hyperreal number: real + infinitesimal */
  class HyperReal {
    public:
      HyperReal() { r[0] = 0; r[1] = 0; }
      HyperReal(double rel) { r[0] = rel; r[1] = 0; }
      HyperReal(double rel, double inf) { r[0] = rel; r[1] = inf; }
      double sel(unsigned n) const { return r[n]; }
    private:
      double r[2]; /* r[0]: real part. r[1]: infinitesimal part */
  };

  /* Fetch parts of number based on type: provided by specializations */
  template <typename REAL> struct F { };

  template <>
  struct F<double> {
      static double g(unsigned i, const double& r) {
        if (i == 0) return r; else return 0; 
      }
  };

  template <>
  struct F<int> {
      static int g(unsigned i, const int& r) {
        if (i == 0) return r; else return 0;
      }
  };
  
  template <>
  struct F<HyperReal> {
      static double g(unsigned i, const HyperReal& r) { return r.sel(i); }
  };

  /* Basic arithmetic operators */

  // binary +
  template<typename L, typename R>
  inline HyperReal operator+(const L& lv, const R& rv) {
    double x0 = F<L>::g(0,lv), x1 = F<L>::g(1,lv);
    double y0 = F<R>::g(0,rv), y1 = F<R>::g(1,rv);
    
    double rel = x0+y0;
    double inf = x1+y1;
    return HyperReal(rel, inf);
  }

  // binary -  
  template<typename L, typename R>
  inline HyperReal operator-(const L& lv, const R& rv) {
    double x0 = F<L>::g(0,lv), x1 = F<L>::g(1,lv);
    double y0 = F<R>::g(0,rv), y1 = F<R>::g(1,rv);
    
    double rel = x0-y0;
    double inf = x1-y1;
    return HyperReal(rel, inf);
  }

  // binary *  
  template<typename L, typename R>
  inline HyperReal operator*(const L& lv, const R& rv) {
    double x0 = F<L>::g(0,lv), x1 = F<L>::g(1,lv);
    double y0 = F<R>::g(0,rv), y1 = F<R>::g(1,rv);
    
    double rel = x0*y0;
    double inf = x0*y1+x1*y0;

    return HyperReal(rel, inf);
  }

  // binary /
  template<typename L, typename R>
  inline HyperReal operator/(const L& lv, const R& rv) {
    double x0 = F<L>::g(0,lv), x1 = F<L>::g(1,lv);
    double y0 = F<R>::g(0,rv), y1 = F<R>::g(1,rv);
    
    double rel = x0/y0;
    double inf = -(x0*y1-x1*y0)/(y0*y0);
    
    return HyperReal(rel, inf);
  }

  // unary -
  template<typename T>
  inline HyperReal operator-(const T& v) {
    double x0 = F<T>::g(0,v), x1 = F<T>::g(1,v);
    return HyperReal(-x0, -x1);
  }

  // unary +
  template<typename T>
  inline HyperReal operator+(const T& v) {
    return v;
  }  

  /* Functions from std::math library */
  namespace m {

    // this default private struct supplies methods for use with POD
    // types (double and float)
    template <typename T>
    struct _m {
        static T cos(const T& x) { return std::cos(x); }
        static T sin(const T& x) { return std::sin(x); }
    };

    // specialization to HyperReal number
    template <>
    struct _m<HyperReal> {
        static HyperReal cos(const HyperReal& x) {
          double x0 = F<HyperReal>::g(0,x), x1 = F<HyperReal>::g(1,x);
          return HyperReal(std::cos(x0), -x1*std::sin(x0));
        }
        static HyperReal sin(const HyperReal& x) {
          double x0 = F<HyperReal>::g(0,x), x1 = F<HyperReal>::g(1,x);
          return HyperReal(std::sin(x0), x1*std::cos(x0));
        }
    };    

    // Trig functions
    template <typename T> inline T cos(const T& x) { return _m<T>::cos(x); }
    template <typename T> inline T sin(const T& x) { return _m<T>::sin(x); }
  }
}
