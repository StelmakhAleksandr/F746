#include <cstdlib>
#include <map>

inline std::map<void*, size_t> allocations;

// extern "C" caddr_t _sbrk(int incr)
// {
//     printf("_sbrk called! %d\r\n",
//         incr);
// }

extern "C" void lv_mem_init()
{
}

extern "C" void* lv_malloc_core(size_t size)
{
    return std::malloc(size);
}

extern "C" void lv_free_core(void* ptr)
{
    return std::free(ptr);
}

extern "C" void* lv_realloc_core(void* ptr, size_t new_size)
{
    return std::realloc(ptr, new_size);
}