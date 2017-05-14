void hijack_CreateManagementState(void);
void hijack_StateCreation(lua_State *state);

#define LIBOPENDEF(_) _(ALL) _(FFI) _(BASE) _(GMODBASE)

#define ENUMFN(t) LIBTYPE_##t,
typedef enum {
    LIBOPENDEF(ENUMFN)
    LIBTYPE_COUNT
} LibOpenType;
#undef ENUMFN
#define ENUMSTR(t) #t,
static const char *libtype_strings[] = {
    LIBOPENDEF(ENUMSTR)
    0
};
#undef ENUMSTR

int hijack_LibOpen(lua_State *state, LibOpenType type);