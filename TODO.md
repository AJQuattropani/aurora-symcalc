# What TODO:

1. Simplification Features:
    - Overhaul current system in a new /internal/ file.
    - Fix no-simplification on case where consteval portions are unable to merge due to a non-const expression separating them in commutative operations.
    - Give resolve-simplification responsibility for building nodes. Remove redundancy for all binary functions.
    - Do final pass to check for self-similar branches.
2. Differentiation Features:
    - Select the variable to partial differentiate w rsp to.
    - Make a copy of the initial function.
    - Apply chain rule recursively with definitions for derivative of each operation.
    - Simplify at the end so that you don't have an utter mess.
2. Implement File IO.
3. Implement graphing.
4. Newton's Method, Value Estimation, Root Finding, Integration
5. Write more scripts to fuzz out bugs.
6. Add an automatic '==' reader that does not require explicit naming of type.

# KNOWN BUGS:



