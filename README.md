# Templated Observer for C++
This is a simple but flexible implementation of the observer pattern.

Similarly to delegates in C#, this implementation favours composition rather than inheritance and uses std::function to callback to any free- or member function. Arbitrary arguments can be sent to the observer with variadic templates.

Both the subjects and observers are completely safe to destroy without having to unregister the observer, as the destructors will take care of that automatically.

# Installation
There is only a single header file, so all you need to do is download it and include it.

# Usage
TODO: Write an example
