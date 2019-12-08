#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <GkForwardAutoDiff.h>

TEST_CASE("Basic first derivative tests", "[]") {
  Gkyl::HyperReal x(5.0, 1.0);
  Gkyl::HyperReal z;

  // f(x) = x
  z = x;
  REQUIRE( z.sel(0) == 5.0 );
  REQUIRE( z.sel(1) == 1.0 );

  // f(x) = 2*x*x
  z = 2*x*x;
  REQUIRE( z.sel(0) == 2*25.0 );
  REQUIRE( z.sel(1) == 20.0 );

  // f(x) = x/(1+x)
  z = x/(1+x);
  REQUIRE( z.sel(0) == Approx(5./(1+5)) );
  REQUIRE( z.sel(1) == Approx(1./(5*5+2*5+1)) );
  
}
