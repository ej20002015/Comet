#pragma once
#include "CometPCH.h"

#include <memory>
#include <exception>
#include <sstream>

#define CMT_BIT(x) (1 << x)

/* 
This macro creates a lamda function that calls the passed in method. The lamda captures the 'this' pointer in order to call member functions. Its arguments are
a variadic list of deduced types that are determined by the compiler when the lamda is called. The arguments are simply forwaded onto the appropriate bound function.
In the instance of the dispatcher class, the argument(s) would become a one reference to a Event subclass (e.g. key pressed event). The decltype(auto) return type
tells the compiler to deduce the return type of the lamda at compile time (auto on it's own won't be sufficient because it could be const or/and a reference),
when the bound member function is known and thus the return type is known. When passed to a function, the lamda is sent as a compiler generated struct type. Therefore
templates can be used to take in the lamda (see the 'Dispatcher.dispatch()' method).

The ellipsis after the auto&& argument type declaration indicates that args is a pack of arguments. decltype(args) explicitly gets the types of the arguments. The elipsis
after (args) unpacks the arguments and passes them to the function.
*/
#define CMT_BIND_METHOD(method) [this](auto&&... args) -> decltype(auto) { return this->method(std::forward<decltype(args)>(args)...); }
#define CMT_BIND_FUNCTION(function) [](auto&&... args) -> decltype(auto) { return function(std::forward<decltype(args)>(args)...); }
#define CMT_BIND_METHOD_FROM_OBJ(method, obj) [objPtr = obj](auto&&... args) -> decltype(auto) { return objPtr->method(std::forward<decltype(args)>(args)...); }

#ifdef CMT_DEBUG
	#ifdef CMT_PLATFORM_WINDOWS
		#define CMT_BREAKPOINT() __debugbreak()
	#else 
		#define CMT_BREAKPOINT()
	#endif

	#define CMT_CLIENT_ASSERT_MESSAGE(x, ...) { if(!(x)) { Comet::Log::clientError("Assertion Failed: {0}", __VA_ARGS__); CMT_BREAKPOINT(); } }
	#define CMT_COMET_ASSERT_MESSAGE(x, ...) { if(!(x)) { Comet::Log::cometError("Assertion Failed: {0}", __VA_ARGS__); CMT_BREAKPOINT(); } }
	
	#define CMT_CLIENT_ASSERT(x) { if(!(x)) { CMT_BREAKPOINT(); } }
	#define CMT_COMET_ASSERT(x) { if(!(x)) { CMT_BREAKPOINT(); } }


	#define STRINGIFY2(x) #x
	#define STRINGIFY1(x) STRINGIFY2(x)
	#define LOCATION __FILE__ ":" STRINGIFY1(__LINE__)

	#define CMT_STATIC_ASSERT(x, description) static_assert(x, "Static Assert Failed (" LOCATION "): " description)
#else
	#define CMT_CLIENT_ASSERT_MESSAGE(x, ...)
	#define CMT_COMET_ASSERT_MESSAGE(x, ...)
	#define CMT_CLIENT_ASSERT(x, ...)
	#define CMT_COMET_ASSERT(x, ...)
	#define CMT_STATIC_ASSERT(x, description)
#endif

namespace Comet
{

using byte = uint8_t;

template<typename T>
using Reference = std::shared_ptr<T>;
template<typename T, typename... Args>
constexpr Reference<T> createReference(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T, typename... Args>
constexpr Unique<T> createUnique(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

class CometException : public std::exception
{
public:
	CometException() = default;

	CometException(const CometException& other)
	{
		m_ss << other.m_ss.rdbuf();
		m_str = other.m_str;
	}

	CometException(CometException&& other) noexcept
	{
		m_ss = std::move(other.m_ss);
		m_str = std::move(other.m_str);
	}

	template<typename T>
	CometException& operator<<(T&& output)
	{
		m_ss << std::forward<T>(output);
		m_str = m_ss.str();
		return *this;
	}

	char const* what() const override
	{
		return m_str.c_str();
	}

private:
	std::stringstream m_ss;
	std::string m_str;
};

static constexpr uint32_t DEFAULT_WIDTH = 1280;
static constexpr uint32_t DEFAULT_HEIGHT = 720;

}