// test_session_registry.cpp
#include "../domain/session_registry.hpp"

#include <cassert>

int main()
{
	domain::SessionRegistry r;
	assert(r.add("a"));
	assert(!r.add("a"));
	assert(r.contains("a"));
	assert(r.remove("a"));
	assert(!r.contains("a"));
	return 0;
}
