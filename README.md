# C++ Observe - A Templated Observer for Modern C++
This is a simple but flexible implementation of the observer pattern.

This implementation favours composition rather than inheritance, which means that a single object can have multiple observers, with different names and functions. It uses std::function to allow callback to any free- or member function, with possibilities for lambdas. Arbitrary arguments can be sent to the observer with variadic templates.

Both the subjects and observers are completely safe to destroy or move without having to unregister the observer, as the destructors and move constructors will take care of that automatically.

# Installation
There are currently two ways to install C++ Observe:
## Via Conan
If you're using Conan for package management, you can add this package to your [requires] list in your conanfile.txt.
You can find the package on Bintray: [insert URL here]
## Manual download
There is only a single header file, so you can just download it directly and include it.
Download cpp_observe.hpp and put it in one of your project's include directories.

# Usage
TODO: Write an example
