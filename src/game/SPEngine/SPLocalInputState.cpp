// SP::cLocalInputState: keyboard and mouse state of the local player
// (SPEngine/SPLocalInputState.obj). Each function keeps its '// @ 0xVA' marker.

#include "SP/cLocalInputState.h"
#include <string.h>

namespace {
// Virtual-key codes the modifier mask is built from.
enum {
  kKeyShift = 0x10,     // VK_SHIFT
  kKeyControl = 0x11,   // VK_CONTROL
  kKeyAlt = 0x12,       // VK_MENU
  kKeyWindows = 0x5b,   // VK_LWIN
  kKeyCommand = 0xe1,   // VK_OEM 0xe1; role guessed from mModifiers bit 8
  kLastKey = 0xff,      // mKeys holds one bit per key 0..kLastKey
};

// Mouse button ids start here; mMouseButtons is indexed from it.
enum { kMouseButtonFirst = 1000 };

// Bits of cLocalInputState::mModifiers.
enum {
  kModShift = 0x01,
  kModControl = 0x02,
  kModAlt = 0x04,
  kModCommand = 0x08,
  kModWindows = 0x10,
};
}  // namespace

// dev only in retail (always inlined)
inline bool SP::cLocalInputState::IsKeyDown(unsigned int key)
{
  return (mKeys[key / 32] & (1u << (key % 32))) != 0;
}

// @ 0x00697980
void SP::cLocalInputState::Reset()
{
  memset(mKeys, 0, sizeof(mKeys));
  mModifiers = 0;
  memset(mMouseButtons, 0, sizeof(mMouseButtons));
  mDragButton = 0;
  mMouseStartX = 0.0f;
  mMouseStartY = 0.0f;
  mMouseDownModifiers = 0;
}

// @ 0x006979c0
void SP::cLocalInputState::UpdateModifiers()
{
  mModifiers = 0;
  if (IsKeyDown(kKeyShift)) mModifiers = kModShift;
  if (IsKeyDown(kKeyControl)) mModifiers |= kModControl;
  if (IsKeyDown(kKeyAlt)) mModifiers |= kModAlt;
  if (IsKeyDown(kKeyCommand)) mModifiers |= kModCommand;
  if (IsKeyDown(kKeyWindows)) mModifiers |= kModWindows;
}

// @ 0x00697a10
void SP::cLocalInputState::OnFocusChange(bool hasFocus)
{
  if (!hasFocus) Reset();
}

// @ 0x00697a50
void SP::cLocalInputState::OnKeyDown(unsigned int key, unsigned int /*modifiers*/)
{
  if (key <= kLastKey) {
    mKeys[key / 32] |= 1u << (key % 32);
    UpdateModifiers();
  }
}

// @ 0x00697a80
void SP::cLocalInputState::OnKeyUp(unsigned int key, unsigned int /*modifiers*/)
{
  if (key <= kLastKey) {
    mKeys[key / 32] &= ~(1u << (key % 32));
    UpdateModifiers();
  }
}

// @ 0x00697ab0
void SP::cLocalInputState::OnMouseDown(unsigned int button, float x, float y, unsigned int modifiers)
{
  mMouseButtons[button - kMouseButtonFirst] = true;
  mMouseX = x;
  mMouseY = y;
  if (mDragButton == 0) {
    mDragButton = button;
    mMouseDownModifiers = modifiers;
    mMouseStartX = x;
    mMouseStartY = y;
  }
}

// @ 0x00697af0
void SP::cLocalInputState::OnMouseUp(unsigned int button, float x, float y, unsigned int /*modifiers*/)
{
  mMouseButtons[button - kMouseButtonFirst] = false;
  mMouseX = x;
  mMouseY = y;
  if (mDragButton == button) mDragButton = 0;
}
