#include <iostream>
#include <observe/cpp_observe.hpp>

int main()
{
	auto subject = observe::subject<>{};
	auto observer = observe::observer<>{[](){std::cout << "Hello C++ Observe!" << std::endl;}};
	subject.add_observer(observer);
	subject();
}
