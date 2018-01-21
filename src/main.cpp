//
// Created by Anton Södergren on 2018-01-03.
//


#include <string>
#include <functional>
#include <iostream>
#include "Templated_observer.h"


void test1()
{
	std::cout << "test1" << std::endl;
}


void test2(std::string text)
{
	std::cout << text << std::endl;
}


int main()
{
	auto sub1 = T_observer::Subject<>{};

	auto obs1 = T_observer::Observer<>{test1};

	// clang-tidy modernize-avoid-bind adds an extra semicolon
	auto obs2 = T_observer::Observer<>{[]() { test2("obs2"); }};
	auto obs3 = T_observer::Observer<>{[]() { test2("obs3"); }};

	sub1.add_observer(obs1);
	sub1.add_observer(obs2);
	sub1.add_observer(obs3);

	sub1();


	auto sub2 = T_observer::Subject<int>{};

	auto obs2_1 = T_observer::Observer<int>{[](int i) { test2(std::string{"obs1 "} + std::to_string(i)); }};
	auto obs2_2 = T_observer::Observer<int>{[](int i) { test2(std::string{"obs2 "} + std::to_string(i)); }};

	sub2.add_observer(obs2_1);
	sub2.add_observer(obs2_2);

	sub2(5);

	obs2_1 = obs2_2;

	sub2(7);

	// Used to make the console pause
	getchar();
}
