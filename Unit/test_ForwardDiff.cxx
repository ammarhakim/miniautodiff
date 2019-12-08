#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <GkForwardAutoDiff.h>
#include <cmath>

TEST_CASE("Basic first derivative tests", "[]") {
  Gkyl::HyperReal x(5.0, 1.0);
  Gkyl::HyperReal z;

  // f(x) = -x+3
  z = -x+3;
  REQUIRE( z.sel(0) == -5.0+3 );
  REQUIRE( z.sel(1) == -1.0 );

  // f(x) = 2*x^2
  z = 2*x*x;
  REQUIRE( z.sel(0) == 2*25.0 );
  REQUIRE( z.sel(1) == 20.0 );

  // f(x) = x/(1+x)
  z = x/(1+x);
  REQUIRE( z.sel(0) == Approx(5./(1+5)) );
  REQUIRE( z.sel(1) == Approx(1./(5*5+2*5+1)) );

  // f(x) = x*cos(x)
  z = x*Gkyl::m::cos(x);
  REQUIRE( z.sel(0) == Approx(5.0*std::cos(5.0)) );
  REQUIRE( z.sel(1) == Approx(std::cos(5.0)-5.0*std::sin(5.0)) );

  // f(x) = x*sin(x)
  z = x*Gkyl::m::sin(x);
  REQUIRE( z.sel(0) == Approx(5.0*std::sin(5.0)) );
  REQUIRE( z.sel(1) == Approx(std::sin(5.0)+5.0*std::cos(5.0)) );

  // f(x) = cos(x)^2 + sin(x)^2
  z = Gkyl::m::sin(x)*Gkyl::m::sin(x) + Gkyl::m::cos(x)*Gkyl::m::cos(x);
  REQUIRE( z.sel(0) == Approx(1.0) );
  REQUIRE( z.sel(1) == Approx(0.0) );
}
