#pragma once

#define CMT_BIT(x) (x << 1)

#define CMT_BIND_EVENT_FUNCTION(function) [this](auto&&... args) -> decltype(auto) { return this->function(std::forward<decltype(args)>(args)...); }