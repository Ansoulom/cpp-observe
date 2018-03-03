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
	SECTION("Adding an already added observer will make it execute twice")
	{
		subject.add_observer(observer);

		times_executed = 0;
		subject();
		REQUIRE(times_executed == 2);
	}
}


TEST_CASE("Operator += works")
{
	auto times_executed = 0;
	auto subject = observe::subject<>{};
	auto observer = observe::observer<>{ [&times_executed]() { ++times_executed; } };
	subject += observer;
	subject();

	REQUIRE(times_executed == 1);

	SECTION("Operator -= works")
	{
		subject -= observer;
		subject();
		REQUIRE(times_executed == 1);
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

		SECTION("Removing an observer that has not be added will not do anything")
		{
			auto observer_4 = observe::observer<>{ [&sum]() {sum += 4; } };
			REQUIRE_NOTHROW(subject.remove_observer(observer_4));

			sum = 0;
			REQUIRE_NOTHROW(subject());
			REQUIRE(sum == 4);
		}
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
	auto test_result = "Has not changed"s;
	auto times_executed = 0;
	auto subject = observe::subject<>{};
	SECTION("Observer move assignment operator")
	{
		auto observer_1 = observe::observer<>{};
		{
			auto observer_2 = observe::observer<>{ [&test_result]() {test_result = "Moved"s; } };
			subject.add_observer(observer_2);
			observer_1 = std::move(observer_2);
		}
		subject();

		REQUIRE(test_result == "Moved"s);
	}
	SECTION("Observer move constructor")
	{
		auto observer_1 = observe::observer<>{ [&times_executed]() {++times_executed; } };
		subject.add_observer(observer_1);
		auto observer_2 = std::move(observer_1);

		subject();
		REQUIRE(times_executed == 1);

		observer_1 = {};
		subject();
		REQUIRE(times_executed == 2);
	}
	SECTION("Observer copy constructor and copy assignment operator")
	{
		auto observer_1 = observe::observer<>{ [&times_executed]() {++times_executed; } };
		subject.add_observer(observer_1);
		auto observer_2 = observer_1;

		subject();
		REQUIRE(times_executed == 1);

		subject.add_observer(observer_2);
		times_executed = 0;
		subject();
		REQUIRE(times_executed == 2);

		observer_1 = {};
		times_executed = 0;
		subject();
		REQUIRE(times_executed == 1);
	}
	SECTION("Subject move constructor")
	{
		auto observer_1 = observe::observer<>{ [&times_executed]() {++times_executed; } };
		subject.add_observer(observer_1);
		auto subject_2 = std::move(subject);

		subject();
		REQUIRE(times_executed == 0);

		subject_2();
		REQUIRE(times_executed == 1);

		SECTION("Subject move assignment operator")
		{
			subject = std::move(subject_2);

			times_executed = 0;
			subject();
			REQUIRE(times_executed == 1);

			subject_2();
			REQUIRE(times_executed == 1);
		}
	}
	SECTION("Subject copy operations")
	{
		auto observer_1 = observe::observer<>{ [&times_executed]() {++times_executed; } };
		subject.add_observer(observer_1);
		auto subject_2 = subject;

		subject();
		REQUIRE(times_executed == 1);

		subject_2();
		REQUIRE(times_executed == 2);

		SECTION("Subject copy assignment operator")
		{
			auto observer_2 = observe::observer<>{ [&times_executed]() {times_executed += 2; } };
			subject.add_observer(observer_2);

			times_executed = 0;
			subject();
			REQUIRE(times_executed == 3);
			subject_2();
			REQUIRE(times_executed == 4);

			subject_2 = subject;
			subject_2();
			REQUIRE(times_executed == 7);
		}
	}
}
