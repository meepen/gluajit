#include "cpp_arch.h"

#if LJ_TARGET_WINDOWS
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

static DWORD protections[] =
 {
    PAGE_READWRITE,
    PAGE_EXECUTE_READ
};
void ll_mprotect(void *addr, size_t size, EProtection prot)
{
    DWORD lpflOldProtect;
    VirtualProtect(addr, size, protections[prot], &lpflOldProtect);
}

#elif LJ_TARGET_POSIX

#include <sys/mman.h>

static int protections[] = {
    PROT_READ | PROT_WRITE,
    PROT_READ | PROT_EXEC
};

void ll_mprotect(void *addr, size_t size, EProtection prot)
{
    mprotect(addr, size, protections[prot]);
}
#endif


#if LJ_TARGET_DLOPEN
#include <dlfcn.h>

void *ll_load(const char *path, int gl)
{
  void *lib = dlopen(path, RTLD_NOW | (gl ? RTLD_GLOBAL : RTLD_LOCAL));
  return lib;
}

void *ll_sym(void *lib, const char *sym)
{
  void *f = (void *)dlsym(lib, sym);
  return f;
}

#elif LJ_TARGET_WINDOWS


void *ll_load(const char *path, int gl)
{
  void *lib = (void *)LoadLibraryA(path);
  return lib;
}

void *ll_sym(void *lib, const char *sym)
{
  void *f = (void *)GetProcAddress((HINSTANCE)lib, sym);
  return f;
}

#else
#error "what os is this?"
#endif