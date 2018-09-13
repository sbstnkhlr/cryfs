#if defined(_MSC_VER)

#include "memory.h"
#include <Windows.h>
#include <stdexcept>
#include <cpp-utils/logging/logging.h>

using namespace cpputils::logging;

namespace cpputils {

void* UnswappableAllocator::allocate(size_t size) {
    void* data = DefaultAllocator().allocate(size);
	const BOOL result = ::VirtualLock(data, size);
    if (!result) {
        throw std::runtime_error("Error calling VirtualLock. Errno: " + std::to_string(GetLastError()));
    }
    return data;
}

void UnswappableAllocator::free(void* data, size_t size) {
	const BOOL result = ::VirtualUnlock(data, size);
    if (!result) {
		// TODO This can happen because a different RAII object already unlocked the page.
		//      Pages are quite large (4KB) and collisions likely, causing pages to be
		//      unlocked too early. We should look for a different mechanism to lock pages.
        LOG(WARN, "Error calling VirtualUnlock. Errno: {}", GetLastError());
    }

    // overwrite the memory with zeroes before we free it
    std::memset(data, 0, size);

    DefaultAllocator().free(data, size);
}

}

#endif
