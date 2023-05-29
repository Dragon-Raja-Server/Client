#pragma once
#include <cassert>

template <typename T>
class Singleton {
public:
	Singleton()
	{
		assert(s_pInstance == nullptr);
		s_pInstance = static_cast<T*>(this);
	}

	virtual ~Singleton()
	{
		assert(s_pInstance != nullptr);
		s_pInstance = nullptr;
	}

	static T& Reference()
	{
		assert(s_pInstance != nullptr);
		return *s_pInstance;
	}

	static T* Pointer()
	{
		assert(s_pInstance != nullptr);
		return s_pInstance;
	}

private:
	static T* s_pInstance;
};

template<typename T> T* Singleton<T>::s_pInstance = nullptr;
