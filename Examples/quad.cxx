#include <GkForwardAutoDiff.h>
#include <iostream>
#include <cmath>

static const double gkyl_gauss_ordinates_3[] =
{ -0.7745966692414833770359, 0, 0.7745966692414833770359 };

static const double gkyl_gauss_weights_3[] =
{ 0.5555555555555555555556, 0.888888888888888888889, 0.555555555555555555556 };

// For use in computing \int G(x,y) dy
template <typename T>
T
Gfunc(T x, T y) {
  return y*y + x;
}

// Computes F(x) = \int_a^b G(x,y) dy. Also computes dF(x)/dx
template <typename T>
T
quad(T x, T a, T b) {
  T sum(0.0);
  for (int i=0; i<3; ++i) {
    T eta = gkyl_gauss_ordinates_3[i];
    sum += gkyl_gauss_weights_3[i]*Gfunc(x, a + 0.5*(b-a)*(1+eta));
  }
  return 0.5*(b-a)*sum;
}

int main(void) {

  do {
    Gkyl::HyperDouble x(1.0, 1.0);
    Gkyl::HyperDouble res = quad(x, x, 2*x*x+x);
    std::cout << "quad " << res.real() << " diff " << res.inf() << std::endl;

    std::cout << "quad err " << res.real() - 32.0 / 3.0 << " diff err "
              << res.inf() - 50.0 << std::endl;
  } while (0);

  do {
    double x = 1.0;
    double res = quad(x, x, 2*x*x+x);
    std::cout << "(real) quad " << res << std::endl;

  } while (0);  
    
  return 0;
}
