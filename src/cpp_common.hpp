
#define MODULE_NAME "garrysmod/bin/real_lua_shared.dll"
#define INITIALIZER(name, def) \
struct init##name { \
    init##name() def \
}; \
static init##name initialier_##name;
