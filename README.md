# C++ Observe - A Templated Observer for Modern C++
This is a simple but flexible implementation of the observer pattern.

This implementation favours composition rather than inheritance, which means that a single object can have multiple observers, with different names and functions. It uses std::function to allow callback to any free- or member function, with possibilities for lambdas. Arbitrary arguments can be sent to the observer with variadic templates.

Both the subjects and observers are completely safe to destroy or move without having to unregister the observer, as the destructors and move constructors will take care of that automatically.

# Installation
There is only a single header file, so all you need to do is download it and include it.

# Usage
TODO: Write an example
