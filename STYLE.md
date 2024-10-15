# Style Guide
General code style rules.

## General Practices
- Platform specific code should prevented from compiling through the use of conditional compilation with the preprocessor (do not exclude the file from build with cmake when on the wrong platform)
  - Exception to this rule: Objective-C and Objective-C++ files should only be compiled on MacOS.
- Avoid magic numbers
  - Exception to this rule: math like `1/n` or `n/2` or `powf(n, 2)` where the number does not have a specific meaning do not count as magic numbers.
  - This applies to other constants as well (try to make commonly used values constants, preferably `constexpr` if allowed).
- Functions which require complex parameters (either a complex set of default values or more than 6 arguments) should use a struct designed to contain the parameters.
- Overloads should make sense (don't have overloads which do completely different actions. A common function that delegates a call to different objects based on the input types is valid, but a function called `move` which moves something on one overload and not on another would not be allowed).

## C++
- Preprocessor macros which are not externally useful but defined in a header should be undefined before the end of the header
- Never use `using namespace` in file scope of a header.
- Never use `using namespace std` in file scope of source or header files.
- Never use `namespace new=old` in file scope of a header.
- Never use `namespace new=old` or `using namespace` in a header within scope of another namespace **unless** the namespace being used is a child namespace of the current. 
- Avoid defining macros which change the settings of libraries like `glm` in a header (instead, define these in the cmake file for Neuron Stem)
- Always use the appropriate `MODULENAME_API` preprocessor macro to ensure members are exported properly when building shared libraries on Windows.
- Avoid `goto` unless there is **absolutely no other option**, and even then consider restructuring your code instead.
- Do not call `exit`, instead throw an exception and allow the system to gracefully handle or exit your error.
- Do not publicly expose global variables.
- Make sure that your code is formatted using the `.clang-format` file present in the base directory of this project (eventually this will be a commit action).
- Use concepts over `std::enable_if` when possible and sensible.
- Getters should be pure functions, and also inline methods.
- Setters may have side effects, but if they do not they should be inline.
- Getters and other functions where the return value is part of the purpose should be marked `[[nodiscard]]`.
- Unused arguments in functions should be marked `[[maybe_unused]]`.
- Integer-backed arguments should be passed by value.
- Non integer-backed arguments should be passed by const reference (unless that value is A: a temporary, in which case it should be passed as such `T&&`, or B: being copied into a field, in which case it should be passed by value and moved for better compiler optimization chances).
- Single argument constructors should be marked as `explicit`.
- Non-final classes should contain a virtual destructor.
- Any class which is not supposed to be extended from should be marked `final`.
- Any class which can require objects to be used in a `shared_ptr` form should implement `std::enable_shared_from_this`, provide a static function which creates an object as a shared_ptr, and the constructor should be made private.
- Function implementations should mark non-mutated arguments as const 

## CMake
- When adding preprocessor defines without a value, use `-DMY_PREPROCESSOR_DEFINE`.
- When adding preprocessor defines with a value, use `MY_PREPROCESSOR_DEFINE=value`

## Naming Conventions
- Classes should be named in UpperCamelCase.
- Functions should be named in snake_case.
- Constants (including enum constants) should be named in SCREAMING_SNAKE_CASE.
- Concepts should be named in snake_case.
- Namespaces should be named in snake_case.
- Source and header files should be named in snake_case.
- Public and protected class methods should be named in snake_case.
- Private class methods should be named in snake_case with a leading underscore.
- Public class members should be named in snake_case.
- Private and protected class members should be named in snake_case prefixed with m_.
- Private and protected static class members should be named in snake_case prefixed with s_.
- Getters can be written similarly to how Java records generate them (i.e. to get the `m_handle` field of a class, a getter might be called `handle()`).
- Setters should have the prefix `set_`