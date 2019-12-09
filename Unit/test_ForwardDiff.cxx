#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <GkForwardAutoDiff.h>
#include <cmath>
#include <vector>

// these set of test ensure that we can call the Gkyl::m math
// functions with regular POD types (double and float)
TEST_CASE("Tests for calling spec funcs with PODs", "[pod-func]") {
  double x = 5.0;
  double z;

  // f(x) = x*cos(x)
  z = x*Gkyl::cos(x);
  REQUIRE( z == Approx(5.0*std::cos(5.0)) );

  // f(x) = x*sin(x)
  z = x*Gkyl::sin(x);
  REQUIRE( z == Approx(5.0*std::sin(5.0)) );

  // f(x) = cos(x)^2 + sin(x)^2
  z = Gkyl::sin(x)*Gkyl::sin(x) + Gkyl::cos(x)*Gkyl::cos(x);
  REQUIRE( z == Approx(1.0) );

  // f(x) = cos(x*sin(x))
  z = Gkyl::cos(x*Gkyl::sin(x));
  REQUIRE( z == Approx(std::cos(5*std::sin(5))) );

  // f(x) = sqrt(x)
  z = Gkyl::sqrt(x);
  REQUIRE( z == Approx(std::sqrt(5)) );
}

TEST_CASE("Basic first derivative tests", "[simple-first-diff]") {
  Gkyl::HyperReal x(5.0, 1.0);
  Gkyl::HyperReal z;

  // f(x) = 3
  z = 3;
  REQUIRE( z.real() == 3 );
  REQUIRE( z.inf(0) == 0.0 );

  // f(x) = -x+3
  z = -x+3;
  REQUIRE( z.real() == -5.0+3 );
  REQUIRE( z.inf(0) == -1.0 );

  // f(x) = 3-x
  z = 3-x;
  REQUIRE( z.real() == -5.0+3 );
  REQUIRE( z.inf(0) == -1.0 );  

  // f(x) = 2*x^2
  z = 2*x*x;
  REQUIRE( z.real() == 2*25.0 );
  REQUIRE( z.inf(0) == 20.0 );

  // f(x) = x/(1+x)
  z = x/(1+x);
  REQUIRE( z.real() == Approx(5./(1+5)) );
  REQUIRE( z.inf(0) == Approx(1./(5*5+2*5+1)) );

  // f(x) = x*cos(x)
  z = x*Gkyl::cos(x);
  REQUIRE( z.real() == Approx(5.0*std::cos(5.0)) );
  REQUIRE( z.inf(0) == Approx(std::cos(5.0)-5.0*std::sin(5.0)) );

  // f(x) = x*sin(x)
  z = x*Gkyl::sin(x);
  REQUIRE( z.real() == Approx(5.0*std::sin(5.0)) );
  REQUIRE( z.inf(0) == Approx(std::sin(5.0)+5.0*std::cos(5.0)) );

  // f(x) = cos(x)^2 + sin(x)^2
  z = Gkyl::sin(x)*Gkyl::sin(x) + Gkyl::cos(x)*Gkyl::cos(x);
  REQUIRE( z.real() == Approx(1.0) );
  REQUIRE( z.inf(0) == Approx(0.0) );

  // f(x) = cos(x*sin(x))
  z = Gkyl::cos(x*Gkyl::sin(x));
  REQUIRE( z.real() == Approx(std::cos(5*std::sin(5))) );
  REQUIRE( z.inf(0) == Approx(-0.4578343032148585) );

  // f(x) = cos(w); w = x*sin(x)
  Gkyl::HyperReal w = x*Gkyl::sin(x);
  z = Gkyl::cos(w);
  REQUIRE( z.real() == Approx(std::cos(5*std::sin(5))) );
  REQUIRE( z.inf(0) == Approx(-0.4578343032148585) );

  // f(x) = sqrt(x)
  z = Gkyl::sqrt(x);
  REQUIRE( z.real() == Approx(std::sqrt(5)) );
  REQUIRE( z.inf(0) == Approx(0.223606797749979) );
}
