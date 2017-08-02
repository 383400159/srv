#ifndef __SINGLETON_H__
#define __SINGLETON_H__


class Noncopyable
{
protected:
	Noncopyable() {}
	~Noncopyable() {}
private:  // emphasize the following members are private
	Noncopyable( const Noncopyable& );
	const Noncopyable& operator=( const Noncopyable& );
};

template<class T>
class Singleton : public Noncopyable{
protected:
	Singleton(){

	}
	~Singleton(){

	}

public:
	static inline T& get_instance(){
		static T t;
		return t;
	}

	static inline T* get_instance_ptr(){
		return &get_instance();
	}
};


class TestSingleton : public Singleton<TestSingleton>{

};

#endif