# What

This is small, prototype C++ header-only library in order to learn how
to best implement a usable and relatively fast library for both
forward and reverse mode of automatic differentiation.

# Some random notes

The ```HyperReal``` class only works with double precision
numbers. Can template it allow float or half-precision numbers.

The code only computes first derivatives. Should be relatively easy to
extend to compute higher derivatives, at least in forward mode AD.