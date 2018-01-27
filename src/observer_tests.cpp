#include <catch.hpp>
#include <observe/cpp_observe.hpp>
#include <string>


using namespace std::string_literals;


TEST_CASE("Callback works", "[observer]")
{
	auto times_executed = 0;
	auto subject = observe::subject<>{};
	auto observer = observe::observer<>{ [&times_executed]() { ++times_executed; } };
	subject.add_observer(observer);
	subject();

	REQUIRE(times_executed == 1);

	SECTION("Can set a new function for the observer")
	{
		auto new_function = false;
		observer.set_function([&new_function]() {new_function = true; });
		subject();

		REQUIRE(times_executed == 1);
		REQUIRE(new_function);
	}
}


TEST_CASE("Correct argument is sent", "[observer]")
{
	auto test_result = ""s;
	auto subject = observe::subject<std::string>{};
	auto observer = observe::observer<std::string>{ [&test_result](std::string arg) {test_result = arg; } };
	subject.add_observer(observer);
	subject("Correct value");

	REQUIRE(test_result == "Correct value");
}


TEST_CASE("Can use multiple observers for a subject", "[observer]")
{
	auto sum = 0;
	auto subject = observe::subject<>{};
	auto observer_1 = observe::observer<>{ [&sum]() {sum += 1; } };
	auto observer_2 = observe::observer<>{ [&sum]() {sum += 2; } };
	auto observer_3 = observe::observer<>{ [&sum]() {sum += 3; } };
	subject.add_observer(observer_1);
	subject.add_observer(observer_2);
	subject.add_observer(observer_3);
	subject();

	REQUIRE(sum == 6);

	SECTION("Can remove observers")
	{
		subject.remove_observer(observer_2);

		sum = 0;
		subject();
		
		REQUIRE(sum == 4);
	}
	SECTION("Subjects can be cleared")
	{
		subject.clear();

		sum = 0;
		subject();

		REQUIRE(sum == 0);
	}
	SECTION("Observers remove themselves when destroyed")
	{
		{
			auto observer_4 = observe::observer<>{ [&sum]() {sum += 4; } };
			subject.add_observer(observer_4);

			sum = 0;
			subject();

			REQUIRE(sum == 10);
		}
		sum = 0;
		subject();

		REQUIRE(sum == 6);
	}
}


TEST_CASE("Move and copy operations work")
{
	SECTION("Observer move operation")
	{
		auto test_result = "Has not changed";
		auto subject = observe::subject<>{};
		auto observer_1 = observe::observer<>{};
		{
			auto observer_2 = observe::observer<>{ [&test_result]() {test_result = "Moved"; } };
			subject.add_observer(observer_2);
			observer_1 = std::move(observer_2);
		}
		subject();

		REQUIRE(test_result == "Moved");
	}
}


// TODO: Test observer move constructor
// TODO: Test subject move constructor
// TODO: Test subject move assignment operator
// TODO: Test observer copy constructor
// TODO: Test observer copy assignment operator
// TODO: Test subject copy constructor
// TODO: Test subject copy assignment operator
