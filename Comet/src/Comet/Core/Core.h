#pragma once

#define CMT_BIT(x) (x << 1)


/* 
This macro creates a lamda function that calls the passed in method. The lamda captures the 'this' pointer in order to call member functions. Its arguments are
a variadic list of deduced types that are determined by the compiler when the lamda is called. The arguments are simply forwaded onto the appropriate bound function.
In the instance of the dispatcher class, the argument(s) would become a one reference to a Event subclass (e.g. key pressed event). The decltype(auto) return type
tells the compiler to deduce the return type of the lamda at compile time, when the bound member function is known and thus the return type is known. When passed
to a function, the lamda is sent as a compiler generated struct type. Therefore templates can be used to take in the lamda (see the 'Dispatcher.dispatch()' method).
*/
#define CMT_BIND_EVENT_FUNCTION(function) [this](auto&&... args) -> decltype(auto) { return this->function(std::forward<decltype(args)>(args)...); }