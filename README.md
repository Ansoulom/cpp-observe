# C++ Observe - A Templated Observer for Modern C++
This is a simple but flexible implementation of the observer pattern.

This implementation favours composition rather than inheritance, which means that a single object can have multiple observers, with different names and functions. It uses std::function to allow callback to any free- or member function, with possibilities for lambdas. Arbitrary arguments can be sent to the observer with variadic templates.

Both the subjects and observers are completely safe to destroy or move without having to unregister the observer, as the destructors and move constructors will take care of that automatically.

The library version is currently 0.1.0, as I would like more feedback and polishing before releasing a 1.0.0 version. It's probably safe to use in your application anyway, but not recommended yet.

# Installation
There are currently two ways to install C++ Observe:

## Via Conan
If you're using Conan for package management, you can add this package to your [requires] list in your conanfile.txt.
You can find the package on Bintray: https://bintray.com/ansou/conan/cpp-observe:ansou

## Manual download
There is only a single header file, so you can just download it directly and include it.
Download cpp_observe.hpp and put it in one of your project's include directories.

# Usage
There are two template classes in this library: An observer class and a subject class. You don't need to inherit from either of these, but you can still do it if you want your class to have the same interface as a subject or an observer.

## Example 1: Basic usage
```cpp
#include <observe/observe.hpp>
#include <iostream>

void print_num(int num)
{
    std::cout << num << std::endl;
}

int main()
{
    auto observer_1 = observe::observer<int>{ [](int i) {
        std::cout << "First observer called, result is: " << 1 + i << std::endl; } };
    auto observer_2 = observe::observer<int>{ print_num };

    auto subject = observe::subject<int>{};
    subject.add_observer(observer_1);
    subject.add_observer(observer_2);

    subject(5); // Will execute the functions associated with all added observers
}
```

## Example 2: As member variables
```cpp
#include <observe/observe.hpp>
#include <iostream>
#include <string>
#include <string_view>

class Foo
{
public:
    void add_on_text(observe::observer<std::string_view>& observer)
    {
        on_text.add_observer(observer);
    }

    void remove_on_text(observe::observer<std::string_view>& observer)
    {
        on_text.remove_observer(observer);
    }

    void text_function(std::string_view text)
    {
        // Do something with text

        on_text(text); // Notify observers that the text event has occured
    }

private:
    observe::subject<std::string_view> on_text{};
};


class Bar
{
public:
    explicit Bar(Foo& foo)
    {
        foo.add_on_text(text_listener);
    }

    // We do not need to remove the observer from foo in our destructor,
    // as the observer handles that automatically when destroyed

    void on_text(std::string_view text)
    {
        some_text = text;
        std::cout << text << std::endl;
    }

private:
        // Binds this object's on_foo function to the observer
        // A lambda could be used instead if you prefer that over std::bind
    observe::observer<std::string_view> text_listener{
        std::bind(&Bar::on_text, *this, std::placeholders::_1)};

        // Some variable that might be used for something else (like GUI maybe)
    std::string some_text{};
};
```

## Example 3: Exposing a subject directly
An alternative to having add_on_[event name] and remove_on_[event name] functions is to expose your subject variable directly.
```cpp
#include <observe/cpp_observe.hpp>
#include <string_view>
#include <iostream>

class Foo
{
public:
    observe::subject<std::string_view>& on_text()
    {
        return on_text_;
    }

private:
    observe::subject<std::string_view> on_text_{};
};

int main()
{
    auto foo = Foo{};
    auto observer = observe::observer<std::string_view>{
        [](std::string_view text){ std::cout << text << std::endl; }};
        
        // Add to the subject directly via foo
    foo.on_text().add_observer(observer);
        // Remove also works, don't have to add another member function for it
    foo.on_text().remove_observer(observer);
}
```
Note that this also exposes other functions/operators than add_observer and remove_observer, which may be undesirable.
I am actually planning to create an extra interface class that only contains these two functions, so that you can safely expose that instead of the whole subject object.

## Example 4: Destroyed and moved events
An interesting use case for this library is to fire events when an object is moved or destroyed, or maybe copied.
```cpp
#include <observe/cpp_observe.hpp>

class Moved_and_destroyed_observable
{
public:
    ~Moved_and_destroyed_observable()
    {
        on_destroyed_(*this);
    }

    Moved_and_destroyed_observable(Moved_and_destroyed_observable&& other) noexcept :
        on_destroyed_{std::move(other.on_destroyed_)}, on_moved_{std::move(other.on_moved_)}
    {
        on_moved_(other, *this); // Notifies observer which old object has moved to which new object
    }

    Moved_and_destroyed_observable& operator=(Moved_and_destroyed_observable&& other) noexcept
    {
            // When a subject is moved, the new one will take over the list of added observers
        on_destroyed_ = std::move(other.on_destroyed_);
        on_moved_ = std::move(other.on_moved_);

        on_moved_(other, *this);

        return *this;
    }

    observe::subject<Moved_and_destroyed_observable&>& on_destroyed()
    {
        return on_destroyed_;
    }

    observe::subject<Moved_and_destroyed_observable&, Moved_and_destroyed_observable&>& on_moved()
    {
        return on_moved_;
    }

private:
    observe::subject<Moved_and_destroyed_observable&> on_destroyed_{};
    observe::subject<Moved_and_destroyed_observable&, Moved_and_destroyed_observable&> on_moved_{};
};
```
I am planning on utilizing moved and destroyed events in another library I'm writing, for a "safe" pointer type that continue to point to an object after it has moved, and point to nullptr if the object has been destroyed. That will also require me to construct Move_observable and Destroy_observable concepts/requirements that other templates could use.

# Thanks
Thank you for checking out this small, but (hopefully) neat library!
Any kind of feedback would be very much appreciated!
