// src/game/SPEditors/SPCanvasResFilter.cpp
// Object file: SPEditors/SPCanvasResFilter.obj.
//
// Request filters that add an AppResolution header (cCanvasResFilter) and a BuddyMode header
// (cBuddyModeFilter) to outgoing XHTML resource requests.

#include "game/types.h"

// Retail callees and globals that have no canonical header yet (address in the comment).
extern "C" void EASTL_allocator_deallocate(void* p);  // 0x00f47380
extern "C" void* EASTL_allocator_allocate(unsigned int n, const char* name, int flags, unsigned debugFlags,
                                          const char* file, int line);
extern "C" long __cdecl _InterlockedExchange(long volatile*, long);
extern "C" void* __cdecl memmove(void*, const void*, unsigned int);
extern "C" unsigned int __cdecl strlen(const char*);
extern char gEmptyString[];  // 0x01667bac (shared "" for char and wchar_t strings)
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(strlen)

// Virtual slots whose retail meaning is not yet known. Declared only to keep the vtable layout.
#define UNNAMED_VIRTUAL_SLOT(n) virtual void unnamed_slot##n();

// Local stand-ins for the EASTL and EA types this file touches. Each one reproduces only the
// layout and inline code the retail functions use.
namespace eastl {
struct StringAllocator {
  StringAllocator() {}
};

template <typename T>
inline const T& min_alt(const T& a, const T& b) { return b < a ? b : a; }

int Compare(const char* p1, const char* p2, unsigned int n);  // FUN_00606ea0

template <typename I1, typename I2>
I1 search(I1 first1, I1 last1, I2 first2, I2 last2);  // 0x005e8ff0

template <typename T, typename A = StringAllocator>
struct BasicString {
  typedef unsigned int size_type;
  enum { npos = -1 };
  T* mpBegin;
  T* mpEnd;
  T* mpCapacity;
  A mAllocator;

  BasicString() : mpBegin(0), mpEnd(0), mpCapacity(0) {
    mpBegin = (T*)gEmptyString;
    mpEnd = mpBegin;
    mpCapacity = mpBegin + 1;
  }
  BasicString(const T* pBegin, const T* pEnd, const A& allocator)
      : mpBegin(0), mpEnd(0), mpCapacity(0), mAllocator(allocator) {
    RangeInitialize(pBegin, pEnd);
  }
  struct CtorSprintf {};
  BasicString(CtorSprintf, const T* pFormat, ...);  // 0x00473020 (wchar_t), 0x00472f50 (char)
  BasicString& sprintf(const T* pFormat, ...);      // 0x00472fe0 (char)
  // Out of line at 0x0057cb10 elsewhere. This TU inlines it with AllocateSelf expanded.
  BasicString(const BasicString& x) : mpBegin(0), mpEnd(0), mpCapacity(0), mAllocator(x.mAllocator) {
    RangeInitializeInl(x.mpBegin, x.mpEnd);
  }
  ~BasicString() { DeallocateSelf(); }

  void DeallocateSelf() {
    if ((mpCapacity - mpBegin) > 1) DoFree(mpBegin, mpCapacity - mpBegin);
  }
  void DoFree(T* p, size_type) {
    if (p) EASTL_allocator_deallocate(p);
  }
  void AllocateSelf(size_type n);  // 0x00475ab0 (char)
  void RangeInitialize(const T* pBegin, const T* pEnd) {
    const size_type n = (size_type)(pEnd - pBegin);
    AllocateSelf(n + 1);
    mpEnd = CharStringUninitializedCopy(pBegin, pEnd, mpBegin);
    *mpEnd = 0;
  }
  static T* CharStringUninitializedCopy(const T* pSource, const T* pSourceEnd, T* pDestination) {
    memmove(pDestination, pSource, (size_t)(pSourceEnd - pSource) * sizeof(T));
    return pDestination + (pSourceEnd - pSource);
  }
  size_type length() const { return (size_type)(mpEnd - mpBegin); }
  const T* c_str() const { return mpBegin; }
  void clear() {
    if (mpBegin != mpEnd) {
      *mpBegin = 0;
      mpEnd = mpBegin;
    }
  }
  BasicString& assign(const T* pBegin, const T* pEnd);  // 0x00454cb0 (char), 0x00423650 (wchar_t)
  BasicString& operator=(const T* p) { return assign(p, p + strlen(p)); }
  BasicString& operator=(const BasicString& x) {
    if (&x != this) assign(x.mpBegin, x.mpEnd);
    return *this;
  }
  BasicString& replace(size_type position, size_type n, const T* p);  // 0x005f8f80 (wchar_t)
  void Trim();                                                        // 0x00606e60 (wchar_t)

  static int compare(const T* pBegin1, const T* pEnd1, const T* pBegin2, const T* pEnd2) {
    const int n1 = (int)(pEnd1 - pBegin1);
    const int n2 = (int)(pEnd2 - pBegin2);
    const int nMin = min_alt(n1, n2);
    const int cmp = Compare(pBegin1, pBegin2, (size_type)nMin);
    return (cmp != 0 ? cmp : (n1 < n2 ? -1 : (n1 > n2 ? 1 : 0)));
  }
  int compare(const T* p) const;
  BasicString substr(size_type position, size_type n) const;
  BasicString left(size_type n) const;
  size_type find(const T* p, size_type position, size_type n) const {
    T* const pBegin = mpBegin;
    T* const pEnd = mpEnd;
    if ((position + n) <= (size_type)(pEnd - pBegin)) {
      const T* const pResult = search(pBegin + position, pEnd, p, p + n);
      if ((pResult != pEnd) || (n == 0)) return (size_type)(pResult - pBegin);
    }
    return (size_type)npos;
  }
  size_type find(const T* p, size_type position = 0) const;
  size_type find(T c, size_type position = 0) const;  // 0x00607580 (char)
  bool empty() const { return (mpBegin == mpEnd); }
  size_type size() const { return (size_type)(mpEnd - mpBegin); }
  T* DoAllocate(size_type n) {
    return (T*)EASTL_allocator_allocate(n * sizeof(T), "Editor", 0, 0, "EASTL/allocator.h", 0xd1);
  }
  void AllocateSelf() {
    mpBegin = (T*)gEmptyString;
    mpEnd = mpBegin;
    mpCapacity = mpBegin + 1;
  }
  void AllocateSelfN(size_type n) {
    if (n > 1) {
      mpBegin = DoAllocate(n);
      mpEnd = mpBegin;
      mpCapacity = mpBegin + n;
    } else {
      AllocateSelf();
    }
  }
  void RangeInitializeInl(const T* pBegin, const T* pEnd) {
    const size_type n = (size_type)(pEnd - pBegin);
    AllocateSelfN(n + 1);
    mpEnd = CharStringUninitializedCopy(pBegin, pEnd, mpBegin);
    *mpEnd = 0;
  }
};
}  // namespace eastl

typedef eastl::BasicString<char> string8;

template <>
__declspec(noinline) int eastl::BasicString<char>::compare(const char* p) const {
  return compare(mpBegin, mpEnd, p, p + strlen(p));
}
template <>
__declspec(noinline) string8 eastl::BasicString<char>::substr(size_type position, size_type n) const {
  return BasicString(mpBegin + position, mpBegin + position + min_alt(n, (size_type)(mpEnd - mpBegin) - position),
                     mAllocator);
}
template <>
__declspec(noinline) string8 eastl::BasicString<char>::left(size_type n) const {
  const size_type nLength = length();
  if (n < nLength) return substr(0, n);
  return *this;
}
template <>
__declspec(noinline) string8::size_type eastl::BasicString<char>::find(char c, size_type position) const {
  if (position < (size_type)(mpEnd - mpBegin)) {
    const char* p = mpBegin + position;
    while (p != mpEnd && *p != c) ++p;
    if (p != mpEnd) return (size_type)(p - mpBegin);
  }
  return (size_type)npos;
}

namespace EA {
class IMessageServer;

namespace Messaging {
// Local stand-in for EA::Messaging::IHandler. The canonical header declares its ctor and dtor
// out of line, which would change the base-class code in cBuddyModeFilter.
class IHandler {
 public:
  virtual ~IHandler() {}
  virtual bool HandleMessage(uint32_t messageID, void* pMessage) = 0;
};

IMessageServer* GetServer();  // 0x00883860
}  // namespace Messaging

class IMessageServer {
 public:
  UNNAMED_VIRTUAL_SLOT(0) UNNAMED_VIRTUAL_SLOT(1) UNNAMED_VIRTUAL_SLOT(2) UNNAMED_VIRTUAL_SLOT(3) UNNAMED_VIRTUAL_SLOT(4)
  virtual void PostMSG(uint32_t messageID, void* p1, void* p2);  // +0x14
  UNNAMED_VIRTUAL_SLOT(6) UNNAMED_VIRTUAL_SLOT(7) UNNAMED_VIRTUAL_SLOT(8)
  virtual void AddListener(Messaging::IHandler* pHandler, uint32_t messageID);  // +0x24
  UNNAMED_VIRTUAL_SLOT(10)
  virtual void RemoveListener(Messaging::IHandler* pHandler, uint32_t messageID, int priority);  // +0x2c
};
}  // namespace EA

namespace SP {
class IConfigManager {
 public:
  UNNAMED_VIRTUAL_SLOT(0) UNNAMED_VIRTUAL_SLOT(1) UNNAMED_VIRTUAL_SLOT(2) UNNAMED_VIRTUAL_SLOT(3)
  UNNAMED_VIRTUAL_SLOT(4) UNNAMED_VIRTUAL_SLOT(5) UNNAMED_VIRTUAL_SLOT(6) UNNAMED_VIRTUAL_SLOT(7)
  UNNAMED_VIRTUAL_SLOT(8) UNNAMED_VIRTUAL_SLOT(9) UNNAMED_VIRTUAL_SLOT(10) UNNAMED_VIRTUAL_SLOT(11)
  virtual int GetInt(uint32_t id);  // +0x30
  UNNAMED_VIRTUAL_SLOT(13) UNNAMED_VIRTUAL_SLOT(14) UNNAMED_VIRTUAL_SLOT(15) UNNAMED_VIRTUAL_SLOT(16)
  UNNAMED_VIRTUAL_SLOT(17) UNNAMED_VIRTUAL_SLOT(18)
  virtual void GetUserName(int a, string8& out);  // +0x4c
};

IConfigManager* ConfigManager();  // 0x0067dd30
}  // namespace SP

namespace EA {
namespace XHTML {
namespace Resource {
struct HeaderMap {
  void Set(const char* name, const char* value);                // 0x00944d00
  void AddHeaderEntry(const char* name, const string8& value);  // 0x00944cb0
};

struct URLInfo {
  int GetScheme();  // 0x0094baa0
};

struct Request {
  uint32_t pad[12];
  URLInfo mURL;  // +0x30
  uint32_t pad34[176];
  HeaderMap mHeaders;  // +0x2f4
};
}  // namespace Resource
}  // namespace XHTML

namespace Thread {
template <typename T>
struct AtomicInt {
  AtomicInt() { _InterlockedExchange((volatile long*)&mValue, 0); }
  volatile T mValue;
};
}  // namespace Thread

namespace COM {
template <typename T>
class RefCountTemplate {
 public:
  virtual ~RefCountTemplate() {}
  T mRefCount;
};
}  // namespace COM

namespace Internet {
class RequestResponseFilter : public COM::RefCountTemplate<Thread::AtomicInt<int> > {
 public:
  virtual void Filter(XHTML::Resource::Request* pRequest) = 0;
  void operator delete(void* p) { EASTL_allocator_deallocate(p); }
};
}  // namespace Internet
}  // namespace EA

namespace SP {
namespace Pollen {
struct Rect {
  int left, top, right, bottom;
};

class ICanvas {
 public:
  UNNAMED_VIRTUAL_SLOT(0) UNNAMED_VIRTUAL_SLOT(1) UNNAMED_VIRTUAL_SLOT(2) UNNAMED_VIRTUAL_SLOT(3)
  UNNAMED_VIRTUAL_SLOT(4) UNNAMED_VIRTUAL_SLOT(5) UNNAMED_VIRTUAL_SLOT(6) UNNAMED_VIRTUAL_SLOT(7)
  UNNAMED_VIRTUAL_SLOT(8) UNNAMED_VIRTUAL_SLOT(9) UNNAMED_VIRTUAL_SLOT(10) UNNAMED_VIRTUAL_SLOT(11)
  UNNAMED_VIRTUAL_SLOT(12) UNNAMED_VIRTUAL_SLOT(13) UNNAMED_VIRTUAL_SLOT(14)
  virtual void GetClientRect(Rect& r);  // +0x3c
};

ICanvas* Canvas();  // 0x0067dcf0

class cCanvasResFilter : public EA::Internet::RequestResponseFilter {
 public:
  cCanvasResFilter();
  virtual ~cCanvasResFilter() {}
  virtual void Filter(EA::XHTML::Resource::Request* pRequest);
  bool UpdateHeaderString();
  bool Init();
  bool mbInited;          // +0x8
  string8 mHeaderString;  // +0xc
};

class cBuddyModeFilter : public EA::Internet::RequestResponseFilter, public EA::Messaging::IHandler {
 public:
  cBuddyModeFilter();
  virtual ~cBuddyModeFilter() {}
  virtual void Filter(EA::XHTML::Resource::Request* pRequest);
  virtual bool HandleMessage(uint32_t messageID, void* pMessage);
  bool UpdateHeaderString();
  bool Init();
  bool mbInited;          // +0xc
  string8 mHeaderString;  // +0x10
};
}  // namespace Pollen
}  // namespace SP

namespace SP {
namespace Pollen {

// @ 0x00609460
void cCanvasResFilter::Filter(EA::XHTML::Resource::Request* pRequest) {
  pRequest->mHeaders.AddHeaderEntry("AppResolution", mHeaderString);
}

// @ 0x00609480
void cBuddyModeFilter::Filter(EA::XHTML::Resource::Request* pRequest) {
  pRequest->mHeaders.AddHeaderEntry("BuddyMode", mHeaderString);
}

// @ 0x006094a0
cCanvasResFilter::cCanvasResFilter() : mbInited(false) {}

// @ 0x006094e0
cBuddyModeFilter::cBuddyModeFilter() : mbInited(false) {}

// @ 0x006095d0
bool cBuddyModeFilter::UpdateHeaderString() {
  if (ConfigManager()) {
    if (ConfigManager()->GetInt(0x5de7b4a) == 1)
      mHeaderString = "true";
    else
      mHeaderString = "false";
    return true;
  }
  return false;
}

// @ 0x00609630
bool cBuddyModeFilter::Init() {
  if (!mbInited) {
    UpdateHeaderString();
    EA::IMessageServer* pServer = EA::Messaging::GetServer();
    if (pServer) {
      pServer->AddListener(this, 0x5de7b4a);
      pServer->AddListener(this, 0x238de9c);
      mbInited = true;
    }
  }
  return mbInited;
}

// @ 0x00609690
bool cBuddyModeFilter::HandleMessage(uint32_t messageID, void* pMessage) {
  switch (messageID) {
    case 0x238de9c: {
      EA::IMessageServer* pServer = EA::Messaging::GetServer();
      if (pServer) {
        pServer->RemoveListener(static_cast<EA::Messaging::IHandler*>(this), 0x5de7b4a, -9999);
        pServer->RemoveListener(static_cast<EA::Messaging::IHandler*>(this), 0x238de9c, -9999);
        mbInited = false;
      }
      break;
    }
    case 0x5de7b4a:
      UpdateHeaderString();
      return true;
  }
  return true;
}

// @ 0x00609710
bool cCanvasResFilter::UpdateHeaderString() {
  ICanvas* pCanvas = Canvas();
  if (pCanvas) {
    Rect r;
    pCanvas->GetClientRect(r);
    mHeaderString.sprintf("%dx%d", r.right - r.left, r.bottom - r.top);
    return true;
  }
  return false;
}

}  // namespace Pollen
}  // namespace SP
