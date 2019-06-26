# BitManCe - Bit Manipulation Routines in C++

BitManCe is a collection of basic C++ functions to help with direct bit manipulation across different C++ data types, mainly floats and ints.

Scenarios where BitManCe can be useful:
- When you want to change individual bits in an integer or long representation.
- When you want to generate random floating point numbers using rand() in C++ with fine tuned control. You can selectively choose to randomize only mantissa, exponent or both easily, which finds use cases in many applications.
- When you want to fiddle with individual bits in float representation like toggling bits at a certain position in mantissa, etc.
- When you want to print the 3 components, namely sign bit, mantissa and exponent, of an IEEE754 standard float number.

The functions should be compatible with both endian representations.
