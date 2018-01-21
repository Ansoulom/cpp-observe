#pragma once

//
// Created by Anton Södergren on 2018-01-03.
//

#include <functional>
#include <vector>
#include <algorithm>


namespace Observe
{
	// Observer with templates and function callbacks

	template<typename... Args>
	class Subject;


	// Takes a function argument with Args arguments. Not meant to be inherited from.
	template<typename... Args>
	class Observer
	{
	public:
		Observer();
		explicit Observer(std::function<void(Args ...)> function);
		~Observer();

		Observer(const Observer& other);
		Observer& operator=(const Observer& other);

		Observer(Observer&& other) noexcept;
		Observer& operator=(Observer&& other) noexcept;

		void set_function(std::function<void(Args ...)> function);

	private:
		void on_notify(Args ... args);
		void clear();

		std::function<void(Args ...)> function_{};
		std::vector<Subject<Args...>*> subjects_{};

		friend class Subject<Args...>;
	};


	// To be used together with Observer objects of the same template type.
	template<typename... Args>
	class Subject
	{
	public:
		Subject() = default;
		~Subject();

		Subject(const Subject& other);
		Subject& operator=(const Subject& other);

		Subject(Subject&& other) noexcept;
		Subject& operator=(Subject&& other) noexcept;

		void add_observer(Observer<Args...>& observer);
		void remove_observer(Observer<Args...>& observer);
		void clear();
		void operator()(Args ... args);

	private:
		std::vector<Observer<Args...>*> observers_{};
	};


	template<typename ... Args>
	Observer<Args...>::Observer() {}


	template<typename... Args>
	Observer<Args...>::Observer(std::function<void(Args ...)> function)
		: function_{function} { }


	// Remove observer from all subjects when it gets destroyed
	template<typename ... Args>
	Observer<Args...>::~Observer()
	{
		clear();
	}


	template<typename ... Args>
	Observer<Args...>::Observer(const Observer& other)
		: function_{other.function_} {}


	template<typename ... Args>
	Observer<Args...>& Observer<Args...>::operator=(const Observer& other)
	{
		// Should the list be cleared, remain the same or be copied?
		clear();
		function_ = other.function_;

		return *this;
	}


	template<typename ... Args>
	Observer<Args...>::Observer(Observer&& other) noexcept
		: function_{move(other.function_)}
	{
		for(auto subject : other.subjects_)
		{
			subject->add_observer(*this);
		}
		other.clear();
	}


	template<typename ... Args>
	Observer<Args...>& Observer<Args...>::operator=(Observer&& other) noexcept
	{
		function_ = move(other.function_);

		clear();
		for(auto subject : other.subjects_)
		{
			subject->add_observer(*this);
		}
		other.clear();

		return *this;
	}


	template<typename ... Args>
	void Observer<Args...>::set_function(std::function<void(Args ...)> function)
	{
		function_ = function;
	}


	template<typename ... Args>
	void Observer<Args...>::on_notify(Args ... args)
	{
		function_(args...);
	}


	template<typename ... Args>
	void Observer<Args...>::clear()
	{
		while(subjects_.size() > 0)
		{
			subjects_[0]->remove_observer(*this);
		}
	}


	template<typename ... Args>
	Subject<Args...>::~Subject()
	{
		for(auto observer : observers_)
		{
			observer->subjects_.erase(
				std::remove(observer->subjects_.begin(), observer->subjects_.end(), this),
				observer->subjects_.end());
		}
	}


	template<typename ... Args>
	Subject<Args...>::Subject(const Subject& other)
	{
		for(auto observer : other.observers_)
		{
			add_observer(observer);
		}
	}


	template<typename ... Args>
	Subject<Args...>& Subject<Args...>::operator=(const Subject& other)
	{
		clear();
		for(auto observer : other.observers_)
		{
			add_observer(observer);
		}

		return *this;
	}


	template<typename ... Args>
	Subject<Args...>::Subject(Subject&& other) noexcept
	{
		for(auto observer : other.observers_)
		{
			add_observer(observer);
		}
		other.clear();
	}


	template<typename ... Args>
	Subject<Args...>& Subject<Args...>::operator=(Subject&& other) noexcept
	{
		clear();
		for(auto observer : other.observers_)
		{
			add_observer(observer);
		}
		other.clear();

		return *this;
	}


	template<typename ... Args>
	void Subject<Args...>::add_observer(Observer<Args...>& observer)
	{
		observers_.push_back(&observer);
		observer.subjects_.push_back(this);
	}


	template<typename ... Args>
	void Subject<Args...>::remove_observer(Observer<Args...>& observer)
	{
		observers_.erase(std::remove(observers_.begin(), observers_.end(), &observer), observers_.end());
		observer.subjects_.erase(
			std::remove(observer.subjects_.begin(), observer.subjects_.end(), this),
			observer.subjects_.end());
	}


	template<typename ... Args>
	void Subject<Args...>::clear()
	{
		for(auto observer : observers_)
		{
			observer->subjects_.erase(
				std::remove(observer->subjects_.begin(), observer->subjects_.end(), this),
				observer->subjects_.end());
		}
		observers_.clear();
	}


	template<typename ... Args>
	void Subject<Args...>::operator()(Args ... args)
	{
		for(auto observer : observers_)
		{
			observer->on_notify(args...);
		}
	}
}
