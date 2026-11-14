// Typed accessors that read one property out of a cPropertyList.
// Each returns false when the list is null, the property is missing, or its type does not match.

#include "game/types.h"
#include "SP/cPropertyList.h"
#include "EA/Variant.h"
#include "cSPVector2.h"
#include "cSPVector3.h"
#include "cSPVector4.h"
#include "EA/ResourceMan/Key.h"
#include "SP/cString.h"
#include "cSPColorRGB.h"
#include "cSPColorRGBA.h"

namespace SP {
// Retail helper that copies a text value into a cString (not yet decompiled).
void cStringSet(const void* src, cString* out);
}

// Variant type ids (names are guesses from usage).
enum {
    kTypeString8 = 0x12,
    kTypeString16 = 0x13,
    kTypeKey = 0x20,
    kTypeText = 0x22,
    kTypeVector2 = 0x30,
    kTypeVector3 = 0x31,
    kTypeColorRGB = 0x32,
    kTypeVector4 = 0x33,
    kTypeColorRGBA = 0x34,
};
// Variant flag bit meaning "holds an array" (name is a guess).
const unsigned int kFlagArray = 0x10;

// @ 0x006a0920
namespace SP {
bool GetPropertyAsVector2Array(cPropertyList const * list, unsigned int id, int * count, cSPVector2 ** data)
{
    EA::Variant const * p;
    if (list && list->GetProperty(id, &p) && p->mTypeId == 0x30 && (p->mFlags & kFlagArray)) {
        *count = (p->mFlags & 0x30) ? p->mPointer.mElementCount : 1;
        *data = (p->mFlags & 0x30) ? (cSPVector2*)p->mPointer.mpData : (p->mTypeId ? (cSPVector2*)p : 0);
        return true;
    }
    return false;
}
}  // namespace SP


// @ 0x006a10c0
namespace SP {
bool GetPropertyAsVector2(cPropertyList const * list, unsigned int id, cSPVector2 * dst)
{
    EA::Variant const * p;
    if (list && list->GetProperty(id, &p) && p->mTypeId == kTypeVector2) {
        *dst = *p->GetVector2();
        return true;
    }
    return false;
}
}  // namespace SP


// @ 0x006a1110
namespace SP {
bool GetPropertyAsVector3(cPropertyList const * list, unsigned int id, cSPVector3 * dst)
{
    EA::Variant const * p;
    if (list && list->GetProperty(id, &p) && p->mTypeId == kTypeVector3) {
        *dst = *p->GetVector3();
        return true;
    }
    return false;
}
}  // namespace SP


// @ 0x006a1160
namespace SP {
bool GetPropertyAsVector4(cPropertyList const * list, unsigned int id, cSPVector4 * dst)
{
    EA::Variant const * p;
    if (list && list->GetProperty(id, &p) && p->mTypeId == kTypeVector4) {
        *dst = *p->GetVector4();
        return true;
    }
    return false;
}
}  // namespace SP


// @ 0x006a11b0
namespace SP {
bool GetPropertyAsColorRGB(cPropertyList const * list, unsigned int key, cSPColorRGB * out)
{
  if (!list) return false;
  EA::Variant const * prop;
  if (!list->GetProperty(key, &prop)) return false;
  if (prop->mTypeId == kTypeColorRGB) {
    *out = *prop->GetColorRGB();
    return true;
  }
  return false;
}
}  // namespace SP

// @ 0x006a1200
namespace SP {
bool GetPropertyAsColorRGBA(cPropertyList const * list, unsigned int key, cSPColorRGBA * out)
{
  if (!list) return false;
  EA::Variant const * prop;
  if (!list->GetProperty(key, &prop)) return false;
  if (prop->mTypeId == kTypeColorRGBA) {
    *out = *prop->GetColorRGBA();
    return true;
  }
  return false;
}
}  // namespace SP

// @ 0x006a1250
namespace SP {
bool GetPropertyAsKey(cPropertyList const * list, unsigned int key, EA::ResourceMan::Key * out)
{
  if (!list) return false;
  EA::Variant const * prop;
  if (!list->GetProperty(key, &prop)) return false;
  if (prop->mTypeId == kTypeKey) {
    *out = *prop->GetKey();
    return true;
  }
  return false;
}
}  // namespace SP

// @ 0x006a12a0
namespace SP {
bool GetPropertyAsKeyInstance(cPropertyList const * list, unsigned int key, unsigned int * out)
{
  if (!list) return false;
  EA::Variant const * prop;
  if (!list->GetProperty(key, &prop)) return false;
  if (prop->mTypeId == kTypeKey) {
    *out = prop->GetKey()->mInstance;
    return true;
  }
  return false;
}
}  // namespace SP

// @ 0x006a12e0
namespace SP {
bool GetPropertyAsKeyGroup(cPropertyList const * list, unsigned int key, unsigned int * out)
{
  if (!list) return false;
  EA::Variant const * prop;
  if (!list->GetProperty(key, &prop)) return false;
  if (prop->mTypeId == kTypeKey) {
    *out = prop->GetKey()->mGroup;
    return true;
  }
  return false;
}
}  // namespace SP

// @ 0x006a1320
namespace SP {
bool GetPropertyAsKeyType(cPropertyList const * list, unsigned int key, unsigned int * out)
{
  if (!list) return false;
  EA::Variant const * prop;
  if (!list->GetProperty(key, &prop)) return false;
  if (prop->mTypeId == kTypeKey) {
    *out = prop->GetKey()->mType;
    return true;
  }
  return false;
}
}  // namespace SP

// @ 0x006a1360
namespace SP {
bool GetPropertyAsText(cPropertyList const * list, unsigned int key, cString * out)
{
  if (!list) return false;
  EA::Variant const * prop;
  if (!list->GetProperty(key, &prop)) return false;
  if (prop->mTypeId == kTypeText) {
    cStringSet(prop->GetText(), out);
    return true;
  }
  return false;
}
}  // namespace SP

// @ 0x006a13b0
namespace SP {
bool GetPropertyAsString8(cPropertyList const * list, unsigned int key, eastl::basic_string<char, eastl::allocator> * out)
{
  if (!list) return false;
  EA::Variant const * prop;
  if (!list->GetProperty(key, &prop)) return false;
  if (prop->mTypeId == kTypeString8) {
    eastl::basic_string<char,eastl::allocator>* src = prop->GetString8();
    if (src != out) {
      out->assign(src->mpBegin, src->mpEnd);
    }
    return true;
  }
  return false;
}
}  // namespace SP

// @ 0x006a1400
namespace SP {
bool GetPropertyAsString16(cPropertyList const * list, unsigned int key, eastl::basic_string<wchar_t, eastl::allocator> * out)
{
  if (!list) return false;
  EA::Variant const * prop;
  if (!list->GetProperty(key, &prop)) return false;
  if (prop->mTypeId == kTypeString16) {
    eastl::basic_string<wchar_t,eastl::allocator>* src = prop->GetString16();
    if (src != out) {
      out->assign(src->mpBegin, src->mpEnd);
    }
    return true;
  }
  return false;
}
}  // namespace SP

// @ 0x006a1450
namespace SP {
bool GetPropertyAsChar8Ptr(cPropertyList const * list, unsigned int key, char const ** out)
{
  if (!list) return false;
  EA::Variant const * prop;
  if (!list->GetProperty(key, &prop)) return false;
  if (prop->mTypeId == kTypeString8) {
    *out = prop->GetString8()->mpBegin;
    return true;
  }
  return false;
}
}  // namespace SP

// @ 0x006a1490
namespace SP {
bool GetPropertyAsChar16Ptr(cPropertyList const * list, unsigned int key, wchar_t const ** out)
{
  if (!list) return false;
  EA::Variant const * prop;
  if (!list->GetProperty(key, &prop)) return false;
  if (prop->mTypeId == kTypeString16) {
    *out = prop->GetString16()->mpBegin;
    return true;
  }
  return false;
}
}  // namespace SP
