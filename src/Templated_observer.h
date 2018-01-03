#pragma once

//
// Created by Anton Södergren on 2018-01-03.
//

#include <functional>
#include <vector>


namespace T_observer
{
	// Observer with templates and function callbacks

	template<typename... Args>
	class Subject;


	// Takes a function argument with Args arguments. Not meant to be inherited from.
	template<typename... Args>
	class Observer
	{
	public:
		explicit Observer(std::function<void(Args ...)> function);
		~Observer();

	private:
		void on_notify(Args ... args);

		std::function<void(Args ...)> function_;
		std::vector<Subject<Args...>*> subjects_;

		friend class Subject<Args...>;
	};


	// To be used together with Observer objects of the same template type.
	template<typename... Args>
	class Subject
	{
	public:
		~Subject();

		void add_observer(Observer<Args...>& observer);
		void remove_observer(Observer<Args...>& observer);
		void clear();
		void operator()(Args ... args);

	private:
		std::vector<Observer<Args...>*> observers_;
	};


	template<typename... Args>
	Observer<Args...>::Observer(std::function<void(Args ...)> function)
		: function_{function} { }


	// Remove observer from all subjects when it gets destroyed
	template<typename ... Args>
	Observer<Args...>::~Observer()
	{
		while(subjects_.size() > 0)
		{
			subjects_[0]->remove_observer(*this);
		}
	}


	template<typename ... Args>
	void Observer<Args...>::on_notify(Args ... args)
	{
		function_(args...);
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
