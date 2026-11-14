// src/game/UTFSpore_Win32/EAGimexGlue.cpp
// Glue between the Gimex image library and the game's allocator and stream objects.

#include "game/types.h"

namespace EA { namespace Gimex { class Importer; } }

void* operator_new(int size, const char* name, int a, int b, int c, int d);  // 0xf473a0
void  operator_delete__(void* p);                                            // 0xf47380

__declspec(noinline) bool __stdcall gfree(void* p);

// Head of the list of registered importer formats (0x165039c).
// Each importer's first word is its link to the next one.
void* g_ImporterFormats;

// Stream objects are called through their vtable; these are the slots Gimex uses.
// The slot meanings are inferred from how the glue calls them.
enum StreamSlot
{
    kSlotLength = 7,
    kSlotTell   = 9,
    kSlotSeek   = 10,
    kSlotRead   = 12,
    kSlotWrite  = 14,
};

// @ 0x0087d4a0
void* __stdcall galloc(int size)
{
    return operator_new(size, "UTF/Gimex", 0, 0, 0, 0);
}

// @ 0x0087d4c0
bool __stdcall gfree(void* p)
{
    operator_delete__(p);
    return true;
}

// @ 0x0087d4e0
int __stdcall gread(void* f, void* buf, int len)
{
    if (len < 0) return 0;
    return ((int(__thiscall*)(void*, void*, int))((void**)*(void**)f)[kSlotRead])(f, buf, len);
}

// @ 0x0087d510
unsigned __stdcall gwrite(void* f, void* buf, unsigned len)
{
    if ((int)len < 0) return 0;
    bool ok = ((bool(__thiscall*)(void*, void*, unsigned))((void**)*(void**)f)[kSlotWrite])(f, buf, len);
    return ok ? len : 0;
}

// @ 0x0087d540
void __stdcall gseek(void* f, int offset, int /*origin*/)
{
    // The origin argument is ignored; the stream always seeks from the start (mode 0).
    ((void(__thiscall*)(void*, int, int))((void**)*(void**)f)[kSlotSeek])(f, offset, 0);
}

// @ 0x0087d560
unsigned long long __stdcall glen(void* f)
{
    return ((unsigned int(__thiscall*)(void*))((void**)*(void**)f)[kSlotLength])(f);
}

// @ 0x0087d570
long long __stdcall gtell(void* f)
{
    return ((int(__thiscall*)(void*, int))((void**)*(void**)f)[kSlotTell])(f, 0);
}

// @ 0x0087d590
namespace EA {
namespace Gimex {
void AddImportFormat(Importer* p)
{
    *(void**)p = g_ImporterFormats;
    g_ImporterFormats = p;
}
}
}  // namespace EA::Gimex
