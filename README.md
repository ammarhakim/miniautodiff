# What

This is small, prototype C++ header-only library in order to learn how
to best implement a usable and relatively fast library for both
forward and reverse mode of automatic differentiation.

# Building tests

Note that there is really nothing to build to use the library. The
full library will be supplied as a single header file. However, to run
tests you do need to build some code. We use the [waf](https://waf.io)
build system. You need to configure the code once:
```
./waf configure
```

Then build the code:
```
./waf build
```

The test cases are in the ```build/Unit`` direction and you can run
them to check if the code is working properly.

# Some random notes

The ```HyperReal``` class only works with double precision
numbers. Can template it allow float or half-precision numbers.

The code only computes first derivatives. Should be relatively easy to
extend to compute higher derivatives, at least in forward mode AD.