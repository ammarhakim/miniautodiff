#include <GkForwardAutoDiff.h>
#include <iostream>
#include <cmath>

template <typename T>
T func(const T& x) {
  return x*x*Gkyl::cos(x)-0.5;
}

double newtonIter(double x0) {
  Gkyl::HyperDouble y = func(Gkyl::HyperDouble(x0, 1.0));
  return x0 - y.real()/y.inf();
}

int
main(void) {

  int maxCount = 100, count = 0;
  double xlast = -1, xnext = newtonIter(xlast);
  while (std::fabs(xnext-xlast) > 1e-10) {
    if (count++ > maxCount) break;
    xlast = xnext;
    xnext = newtonIter(xlast);
  }
  std::cout << "Root " << xnext << " Iterations " << count << std::endl;
  std::cout << " Check " << func(xnext) << std::endl;

  return 0;
}
