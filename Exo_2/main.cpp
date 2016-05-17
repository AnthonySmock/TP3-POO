#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

template<class T> class observer_ptr;

template<typename T> 
class observable_ptr
{
	std::unique_ptr<T> membre;
	std::vector<std::reference_wrapper<observer_ptr<T>>> observers;
	
public:
	observable_ptr(std::unique_ptr<T> &&t) : membre(std::move(t))
	{
		
	}
	
	T* get()
	{
		return membre.get();
	}
	
	observable_ptr<T>& operator= (std::unique_ptr<T> && t)
	{
		membre = std::move(t);
		notify();
		return *this;
	}
	
	
	
	T& operator* ()
	{
		return *membre;
	}
	
	void notify()
	{
		std::cout << "notify" << std::endl;
		for(auto o : observers)
		{
			o.get().update();
		}
	}
	
	void add_observer(observer_ptr<T>& o)
	{
		observers.push_back(o);
		std::cout << "add_observer" << std::endl;
	}
	
	void remove_observer(observer_ptr<T>& o)
	{
		observers.erase(std::find_if(observers.begin(),observers.end(),[&](auto & e)->bool{ return &o == &e.get(); }));
		std::cout << "remove_observer" << std::endl;
	}

	void release()
	{
		membre.release();
		notify();
	}

};

template<typename T> 
std::ostream& operator<<(std::ostream & os, observable_ptr<T> &t)
{
	os << *t;
	return os;
}

template<typename T> 
std::ostream& operator<<(std::ostream & os, observer_ptr<T> &t)
{
	os << *t;
	return os;
}
	
template<typename T> 
class observer_ptr
{
	T* membre;
	//hopp::view_ptr<observable_ptr<T>> p;
	observable_ptr<T> *p;
	
public:
	observer_ptr(observable_ptr<T> &o)
	{
		p = &o;
		p->add_observer(*this);
		membre = p->get();
	}
	
	T& operator* ()
	{
		return *membre;
	}
	
	observable_ptr<T>& operator= (observable_ptr<T> &o)
	{
		
		p->remove_observer(*this);
		p = &o;
		p->add_observer(*this);
		membre = p->get();
		return o;
	}
	
	T* get()
	{
		return membre;
	}
	
	void update()
	{
		membre = p->get();
	}


	~observer_ptr()
	{
		
		p->remove_observer(*this);
		p = NULL;
		membre = NULL;
	}

};

int main()
 {
	observable_ptr<int> p0 = std::make_unique<int>(7);

	{
		observer_ptr<int> v0(p0); // add_observer
		observer_ptr<int> v1(p0); // add_observer

		std::cout << "p0 = " << p0 << std::endl; // p0 = 7
		std::cout << "v0 = " << v0 << std::endl; // v0 = 7
		std::cout << "v1 = " << v1 << std::endl; // v1 = 7
		*v1 = 42;
		std::cout << "p0 = " << p0 << std::endl; // p0 = 42
		std::cout << "v0 = " << v0 << std::endl; // v0 = 42
		std::cout << "v1 = " << v1 << std::endl; // v1 = 42
	}
	{
		observable_ptr<std::string> p0 = std::make_unique<std::string>("An observable_ptr of string");
		observable_ptr<std::string> p1 = std::make_unique<std::string>("A second one");
		observer_ptr<std::string> v0(p0); // add_observer
		observer_ptr<std::string> v1(p0); // add_observer

		std::cout << "p0 = " << p0 << std::endl; // p0 = An observable_ptr of string
		std::cout << "p1 = " << p1 << std::endl; // p1 = A second one
		std::cout << "v0 = " << v0 << std::endl; // v0 = An observable_ptr of string
		std::cout << "v1 = " << v1 << std::endl; // v1 = An observable_ptr of string

		v0 = p1; // remove_observer
			// add_observer
		std::cout << "v0 = " << v0 << std::endl; // v0 = A second one
		std::cout << "v1 = " << v1 << std::endl; // v1 = An observable_ptr of string

		p0.release(); // observer_ptr<T>::update(): p changed, new value = (null)
		// remove_observer
		// remove_observer
	}
}
