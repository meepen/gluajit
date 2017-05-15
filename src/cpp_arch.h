extern "C" {
#include "lj_arch.h"
}

void *ll_load(const char *path, int gl = 0);
void *ll_sym(void *lib, const char *sym);

enum EProtection {
    PROTECTION_READWRITE,
    PROTECTION_READEXEC
};
void ll_mprotect(void *addr, size_t size, EProtection prot);

#ifdef _MSC_VER
#define ll_asm(c) __asm { c }
#define ll_jmp(c) __asm { jmp c }
#else
#define ll_asm(c) asm(#c)
#error "ll_jmp"
#endif