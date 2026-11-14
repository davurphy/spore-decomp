// src/game/SPGonzago/_at_00d40000.cpp
// Object file: SPGonzago/_at_00d40000.obj.

#include "game/types.h"

// Local stand-ins for classes that have no canonical header yet. Each one names the retail address it describes.
// Replace them with the real class header once it exists.

void operator delete(void* p);                                              // 0x00f47380
void* operator new(uint32_t size, const char* name, int, int, int, int);   // 0x00f473a0
int GetCurrentGameMode();                                                   // 0x00b5b800
void* GetAbilityUser();                                                     // 0x00d1e610 (unnamed retail callee)
void UseAbility(void* user, int index);                                     // 0x00d1e930
void ActivateAbilityTarget(int index);                                      // 0x00d1fe00

// Game modes compared in InitButtonTable and SelectAbility.
enum
{
    kGameModeCreatureEditor = 0x1654c01,   // creature editor / creature game
    kGameModeCreatureGame   = 0x1654c10,
    kGameModeTribeStage     = 0x1654c02,   // tribe/civ stage
};

struct ButtonEntry
{
    int32_t mID;          // +0x00 (-1 = unused slot)
    uint8_t mFlag;        // +0x04
    uint8_t pad05[3];     // +0x05
};

struct ButtonCategory
{
    ButtonEntry mEntries[5];   // +0x00 .. +0x27
};

struct ButtonTable
{
    union
    {
        ButtonEntry mAllEntries[0x19];    // +0x000 (flat view used by the filler loop)
        ButtonCategory mCategories[5];    // +0x000 (category view used by the lookups)
    };
    uint32_t mFieldC8;                    // +0x0c8
    uint32_t mCategoryCount;              // +0x0cc (ids per category range)
    uint32_t mBase0;                      // +0x0d0 (first id of categories 0/1)
    uint32_t mBase1;                      // +0x0d4 (first id of categories 3/4)
};

extern ButtonTable* gButtonTable;         // 0x0169e3ec

struct WString
{
    wchar_t* mpBegin;        // +0
    wchar_t* mpEnd;          // +4
    wchar_t* mpCapacity;     // +8

    WString& assign(const wchar_t* s);                            // 0x005c3d90 (ret 4)
    WString& Assign(const wchar_t* first, const wchar_t* last);   // 0x00423650 (ret 8)
};

class cSpeciesProfile
{
public:
    void GetUiName(WString* out);            // 0x004da330 (ret 4)
};

class cCivTokenTranslator
{
public:
    cCivTokenTranslator();                                           // 0x00b32470
    virtual bool TranslateToken(const wchar_t* token, WString* out); // 0x00b324f0 (ret 8)
    uint32_t pad04[2];                                               // +0x04 .. +0x0b
};

class cCreatureTokenTranslator : public cCivTokenTranslator
{
public:
    cCreatureTokenTranslator();                                      // 0x00d486f0

    uint32_t mValueC;                // +0x0c
    uint32_t mValue10;               // +0x10
    uint32_t mValue14;               // +0x14
    uint32_t mValue18;               // +0x18
    cSpeciesProfile* mpProfile;      // +0x1c
    int32_t mValue20;                // +0x20
    int32_t mValue24;                // +0x24
    const wchar_t* mpText;           // +0x28 (null = no token)
    float mValue2c;                  // +0x2c

    bool TranslateToken(const wchar_t* token, WString* out);         // 0x00d48750
};

// @ 0x00d486f0
cCreatureTokenTranslator::cCreatureTokenTranslator()
: mValueC(0), mValue10(0), mValue14(0), mpProfile(0), mValue20(0), mpText(0), mValue2c(0.0f)
{
}

// @ 0x00d48a00
void InitButtonTable()
{
    gButtonTable = (ButtonTable*)operator new(0xd8, "Simulator", 0, 0, 0, 0);
    switch (GetCurrentGameMode())
    {
    case kGameModeCreatureEditor:
    case kGameModeCreatureGame:
        gButtonTable->mFieldC8 = 5;
        gButtonTable->mCategoryCount = 5;
        gButtonTable->mBase0 = 0x1012010;
        gButtonTable->mBase1 = 0x1012020;
        break;

    case kGameModeTribeStage:
        gButtonTable->mFieldC8 = 5;
        gButtonTable->mCategoryCount = 3;
        gButtonTable->mBase0 = 0x1012060;
        gButtonTable->mBase1 = 0x1012050;
        break;
    }

    ButtonTable* t = gButtonTable;
    for (int i = 0; i < 0x19; ++i)
    {
        t->mAllEntries[i].mID = -1;
        t->mAllEntries[i].mFlag = 0;
    }
}

// @ 0x00d48ae0
void FreeButtonTable()
{
    if (gButtonTable != 0)
        operator delete(gButtonTable);
    gButtonTable = 0;
}

// @ 0x00d48b00
void SelectAbility(int index)
{
    void* user = GetAbilityUser();
    if (user == 0)
        return;

    // Slot 0xb0 of the user's vtable returns the number of abilities it offers.
    const int kAbilityCountSlotOffset = 0xb0;
    int count = ((int (__thiscall*)(void*))(*(void***)user)[kAbilityCountSlotOffset / 4])(user);
    if (index < count)
    {
        UseAbility(user, index);
        return;
    }
    if (GetCurrentGameMode() == kGameModeTribeStage)
        ActivateAbilityTarget(index - count);
}
