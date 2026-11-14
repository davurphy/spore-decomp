// src/game/SPEditors/SPEditorPageControls.cpp
// SP::cSPEditorPageControls: the page-number control under the palette category UI (prev/next page buttons and "n/m" text).
// Object file: SPEditors/SPEditorPageControls.obj.

#include "game/types.h"
#include "EA/Messaging/IHandler.h"
#include "EA/Messaging/Server.h"
#include "EA/UTFWin/IWinProc.h"
#include "EA/UTFWin/IWindow.h"
#include "EA/ResourceMan/Key.h"
#include "SP/cSPEditorPageControls.h"
#include "SP/cSPPaletteCategoryUI.h"
#include "cSPUILayout.h"

// Unnamed retail callees and globals (address in the comment).
extern "C" void EASTL_allocator_deallocate(void* p);  // 0x00f47380
void* operator new(unsigned int size, const char* name, int flags, unsigned debugFlags, const char* file, int line);
namespace eastl {
extern wchar_t gEmptyString[2];
}  // namespace eastl

// Window IDs and message IDs as the retail code compares them.
namespace {
const int kWinRootID = -1;                      // EA::UTFWin::IWindow root of the layout
const uint32_t kWinPageNumberTextID = 0x734afa63;
const uint32_t kLayoutKeyInstance = 0xb0039d21;
const uint32_t kLayoutKeyType = 0x510a95b;
const uint32_t kLayoutKeyGroup = 0x40464100;
const uint32_t kLayoutParentID = 0x5b598fa;
const uint32_t kMsgPageChanged = 0x287259f6;    // UI message type sent by the page buttons
const uint32_t kMsgCyclePageNext = 0x92df6fd6;  // button parameter: next page
const uint32_t kMsgCyclePagePrev = 0x92df6fd7;  // button parameter: previous page
const uint32_t kWindowFlagVisible = 1;          // IWindow::SetFlag flag for visibility
}  // namespace

// Local stand-ins for the canonical classes (the canonical headers do not yet match the retail layout).
// TODO(rewrite): replace these with the canonical classes once the headers match retail.
namespace eastl {
class wstring {
 public:
  wchar_t* mpBegin;
  wchar_t* mpEnd;
  wchar_t* mpCapacity;
  int mAllocator;
  wstring() {
    mpEnd = mpBegin = gEmptyString;
    mpCapacity = mpBegin + 1;
  }
  wstring(const wchar_t* p) : mpBegin(0), mpEnd(0), mpCapacity(0) { RangeInitialize(p); }
  ~wstring() {
    if ((mpCapacity - mpBegin) > 1 && mpBegin)
      EASTL_allocator_deallocate(mpBegin);
  }
  void RangeInitialize(const wchar_t* p);
  wstring& trim();
  void sprintf(const wchar_t* fmt, ...);
  wstring& assign(const wchar_t* first, const wchar_t* last);  // WString_Assign (0x00423650)
  const wchar_t* c_str() const { return mpBegin; }
  bool empty() const { return mpEnd == mpBegin; }
};
}  // namespace eastl

namespace EA {
template <typename T>
class AutoRefCount_s005b_fb90 {
 public:
  T* mpObject;
  AutoRefCount_s005b_fb90() : mpObject(0) {}
  ~AutoRefCount_s005b_fb90() { if (mpObject) mpObject->Release(); }
  AutoRefCount_s005b_fb90& operator=(T* pObject) {
    if (pObject != mpObject) {
      T* const pTemp = mpObject;
      if (pObject) pObject->AddRef();
      mpObject = pObject;
      if (pTemp) pTemp->Release();
    }
    return *this;
  }
  T* operator->() const { return mpObject; }
  operator T*() const { return mpObject; }
};
}  // namespace EA

namespace EA {
template <typename T>
class RefCountVTemplate_s005b_fb90 {
 public:
  RefCountVTemplate_s005b_fb90() : mRefCount(0) {}
  virtual ~RefCountVTemplate_s005b_fb90() {}
  virtual int AddRef();
  virtual int Release();
  T mRefCount;
};
}  // namespace EA

namespace EA {
namespace ResourceMan {
struct Key_s005b_fb90 {
  unsigned int mInstance, mType, mGroup;
  Key_s005b_fb90(unsigned int i, unsigned int t, unsigned int g) : mInstance(i), mType(t), mGroup(g) {}
};
}  // namespace ResourceMan
}  // namespace EA

namespace EA {
namespace UTFWin {
class IWinProc_s005b_fb90;
}  // namespace UTFWin
}  // namespace EA

namespace EA {
namespace UTFWin {
class IWindow_s005b_fb90 {
 public:
  virtual int AddRef();
  virtual int Release();
  virtual void pv2();
  virtual void* Cast(uint32_t typeID);                  // +0xc
  virtual void pv4();
  virtual void pv5();
  virtual void pv6();
  virtual uint32_t GetControlID();                      // +0x1c
  virtual void pv8();
  virtual void pv9();
  virtual void pv10();
  virtual void pv11();
  virtual void pv12();
  virtual void pv13();
  virtual void pv14();
  virtual const wchar_t* GetCaption();                  // +0x3c
  virtual void pv16();
  virtual void pv17();
  virtual void pv18();
  virtual void pv19();
  virtual void pv20();
  virtual void pv21();
  virtual void pv22();
  virtual void pv23();
  virtual void pv24();
  virtual void pv25();
  virtual void pv26();
  virtual void pv27();
  virtual void pv28();
  virtual void pv29();
  virtual void pv30();
  virtual void SetFlag(int flag, bool value);           // +0x7c
  virtual void SetCaption(const wchar_t* text);         // +0x80
  virtual void pv33();
  virtual void pv34();
  virtual void pv35();
  virtual void pv36();
  virtual void pv37();
  virtual void pv38();
  virtual void pv39();
  virtual void pv40();
  virtual void pv41();
  virtual IWindow_s005b_fb90* GetDrawable();            // +0xa8
  virtual void pv43();
  virtual void pv44();
  virtual void pv45();
  virtual void pv46();
  virtual void pv47();
  virtual void pv48();
  virtual void pv49();
  virtual void pv50();
  virtual void pv51();
  virtual void pv52();
  virtual void pv53();
  virtual void pv54();
  virtual void pv55();
  virtual void pv56();
  virtual void pv57();
  virtual void pv58();
  virtual void pv59();
  virtual void pv60();
  virtual void pv61();
  virtual void pv62();
  virtual void pv63();
  virtual void pv64();
  virtual void AddWinProc(IWinProc_s005b_fb90* proc);   // +0x104
  virtual void RemoveWinProc(IWinProc_s005b_fb90* proc);  // +0x108
};
}  // namespace UTFWin
}  // namespace EA

namespace EA {
namespace UTFWin {
class IWinProc_s005b_fb90 {
 public:
  ~IWinProc_s005b_fb90() {}
  virtual int AddRef();
  virtual int Release();
  virtual void pv2();
  virtual void pv3();
  virtual void pv4();
  virtual bool HandleUIMessage(IWindow_s005b_fb90* window, const void* message);  // +0x14
  virtual void pv6();
};
}  // namespace UTFWin
}  // namespace EA

namespace EA {
namespace Messaging {
class IHandler_s005b_fb90 {
 public:
  IHandler_s005b_fb90() {}
  ~IHandler_s005b_fb90() {}
  virtual bool HandleMessage(uint32_t messageID, void* message);
};
}  // namespace Messaging
}  // namespace EA

using EA::UTFWin::IWindow_s005b_fb90;

class cSPUILayout_s005b_fb90 {
 public:
  virtual void pv0();
  virtual int AddRef();
  virtual int Release();
  cSPUILayout_s005b_fb90();
  IWindow_s005b_fb90* FindWindowByID(uint32_t id, bool recursive);  // 0x008105b0 (cSPUILayout::FindWindowByID)
  bool Init(const EA::ResourceMan::Key_s005b_fb90& key, bool b, uint32_t id);  // 0x008120d0 (cSPUILayout::Init)
  void SetParentWin(IWindow_s005b_fb90* parent, bool b, uint32_t id);  // 0x008121b0 (cSPUILayout::SetParentWin)
  void Shutdown(bool b);  // 0x00811ad0 (cSPUILayout::Shutdown)
  char pad[0x18 - 4];
};

namespace SP {
struct UIMessage {
  uint32_t pad0;
  IWindow_s005b_fb90* mpSource;  // +0x4
  int mType;                     // +0x8
  int mParam1;                   // +0xc
  int mParam2;                   // +0x10
  int mParam3;                   // +0x14
};

class cSPPaletteCategoryUI_s005b_fb90 {
 public:
  virtual void pv0();
  virtual void pv1();
  virtual int AddRef();
  virtual int Release();
  void CyclePage(int direction);  // 0x005c3c20 (SP::cSPPaletteCategoryUI::CyclePage)
  bool IsPageVisible(int page);   // FUN_005c29c0
  char pad04[0x88 - 4];
  struct Page { uint32_t a, b; }* mPagesBegin;  // +0x88
  Page* mPagesEnd;                              // +0x8c
  char pad90[0xa0 - 0x90];
  int mCurrentPage;                             // +0xa0
};

class cSPEditorPageControls_s005b_fb90 : public EA::UTFWin::IWinProc_s005b_fb90, public EA::Messaging::IHandler_s005b_fb90, public EA::RefCountVTemplate_s005b_fb90<int> {
 public:
  EA::AutoRefCount_s005b_fb90<cSPUILayout_s005b_fb90> mLayout;               // +0x10
  EA::AutoRefCount_s005b_fb90<cSPPaletteCategoryUI_s005b_fb90> mCategoryUI;  // +0x14
  EA::AutoRefCount_s005b_fb90<IWindow_s005b_fb90> mWinPageNumberText;        // +0x18
  EA::AutoRefCount_s005b_fb90<IWindow_s005b_fb90> mWinRoot;                  // +0x1c
  int mCurrentPage;                                                          // +0x20
  int mPageCount;                                                            // +0x24

  cSPEditorPageControls_s005b_fb90();
  ~cSPEditorPageControls_s005b_fb90();
  virtual int AddRef();
  virtual int Release();
  void Init(IWindow_s005b_fb90* parent, cSPPaletteCategoryUI_s005b_fb90* categoryUI);  // 0x005c0840 (SP::cSPEditorPageControls::Init)
  void Shutdown();                                                                    // 0x005c0990 (SP::cSPEditorPageControls::Shutdown)
  bool HandleUIMessage(IWindow_s005b_fb90* window, const void* message);
  void Update(float deltaTime);  // 0x005c0a60 (SP::cSPEditorPageControls::Update)
};
}  // namespace SP

// @ 0x005c0770
namespace SP {
cSPEditorPageControls_s005b_fb90::cSPEditorPageControls_s005b_fb90()
: mCurrentPage(-1), mPageCount(-1) {}
}  // namespace SP

// @ 0x005c07d0
namespace SP {
cSPEditorPageControls_s005b_fb90::~cSPEditorPageControls_s005b_fb90()
{}
}  // namespace SP

// @ 0x005c0840
namespace SP {
void cSPEditorPageControls_s005b_fb90::Init(IWindow_s005b_fb90* parent, cSPPaletteCategoryUI_s005b_fb90* categoryUI)
{
  if (parent && categoryUI) {
    mLayout = new ("Editor", 0, 0, 0, 0) cSPUILayout_s005b_fb90();
    mLayout->Init(EA::ResourceMan::Key_s005b_fb90(kLayoutKeyInstance, kLayoutKeyType, kLayoutKeyGroup), true, kLayoutParentID);
    mLayout->SetParentWin(parent, true, kLayoutParentID);
    mWinRoot = mLayout->FindWindowByID(kWinRootID, true);
    if (mWinRoot)
      mWinRoot->AddWinProc(this);
    mCategoryUI = categoryUI;
    mWinPageNumberText = mLayout->FindWindowByID(kWinPageNumberTextID, true);
  }
}
}  // namespace SP

// @ 0x005c0990
namespace SP {
void cSPEditorPageControls_s005b_fb90::Shutdown()
{
  if (mWinRoot) {
    mWinRoot->RemoveWinProc(this);
    mWinRoot = 0;
  }
  if (mLayout) {
    mLayout->Shutdown(true);
    mLayout = 0;
  }
  mCategoryUI = 0;
  mWinPageNumberText = 0;
}
}  // namespace SP

// @ 0x005c0a00
namespace SP {
bool cSPEditorPageControls_s005b_fb90::HandleUIMessage(IWindow_s005b_fb90* window, const void* message)
{
  const UIMessage* msg = (const UIMessage*)message;
  if (msg->mType == (int)kMsgPageChanged) {
    switch ((uint32_t)msg->mParam1) {
      case kMsgCyclePageNext:
        mCategoryUI->CyclePage(1);
        break;
      case kMsgCyclePagePrev:
        mCategoryUI->CyclePage(-1);
        break;
    }
  }
  return false;
}
}  // namespace SP

// @ 0x005c0a60
namespace SP {
void cSPEditorPageControls_s005b_fb90::Update(float deltaTime)
{
  if (mCategoryUI && mWinPageNumberText) {
    int numPages = (int)(mCategoryUI->mPagesEnd - mCategoryUI->mPagesBegin);
    int visiblePages = 0;
    int currentPage = 0;
    for (int i = 0; i < numPages; i++) {
      if (mCategoryUI->IsPageVisible(i)) {
        if (i == mCategoryUI->mCurrentPage)
          currentPage = visiblePages;
        visiblePages++;
      }
    }
    if (visiblePages < 2) {
      if (mWinRoot)
        mWinRoot->SetFlag(kWindowFlagVisible, false);
      return;
    }
    if (mWinRoot)
      mWinRoot->SetFlag(kWindowFlagVisible, true);
    if (mCurrentPage != currentPage || mPageCount != visiblePages) {
      eastl::wstring text;
      text.sprintf(L"%i/%i", currentPage + 1, visiblePages);
      mWinPageNumberText->SetCaption(text.c_str());
      mCurrentPage = currentPage;
      mPageCount = visiblePages;
    }
  }
}
}  // namespace SP
