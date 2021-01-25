#pragma once
#include "CometPCH.h"

#include <memory>

#define CMT_BIT(x) (x << 1)


/* 
This macro creates a lamda function that calls the passed in method. The lamda captures the 'this' pointer in order to call member functions. Its arguments are
a variadic list of deduced types that are determined by the compiler when the lamda is called. The arguments are simply forwaded onto the appropriate bound function.
In the instance of the dispatcher class, the argument(s) would become a one reference to a Event subclass (e.g. key pressed event). The decltype(auto) return type
tells the compiler to deduce the return type of the lamda at compile time, when the bound member function is known and thus the return type is known. When passed
to a function, the lamda is sent as a compiler generated struct type. Therefore templates can be used to take in the lamda (see the 'Dispatcher.dispatch()' method).

The ellipsis after the auto&& argument type declaration indicates that args is a pack of arguments. decltype(args) explicitly gets the types of the arguments. The elipsis
after (args) unpacks the arguments and passes them to the function.
*/
#define CMT_BIND_FUNCTION(function) [this](auto&&... args) -> decltype(auto) { return this->function(std::forward<decltype(args)>(args)...); }

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

}