// src/game/SPMovie/bufferedwriter.cpp
// Object file: SPMovie/bufferedwriter.obj.
//
// Two small write-buffering helpers used by the movie encoder. The classes are local stand-ins until the
// canonical headers declare them; each function keeps its '// @ 0xVA' marker.

#include <string.h>

#include "game/types.h"

namespace rw {
namespace movie {

// Stand-in for the buffer that BufferedWriter::SetBuffer initializes. No canonical header declares it yet.
struct BufferedWriter {
  char  pad00[8];
  void* mpBuffer08;  // +0x08
  int   mSize0c;     // +0x0c
  int   mOffset10;   // +0x10

  void SetBuffer(void* buf, int size);  // 0x00fc8390
};

// Stand-in for the fixed-capacity byte buffer with a read/write cursor.
struct BufferedWriter_s00fc_8450 {
  char           pad00[8];
  unsigned char* mBuffer;   // +0x08
  int            mCapacity; // +0x0c
  int            mPos;      // +0x10
  int            mBase;     // +0x14
  int            mEnd;      // +0x18

  void Flush();                                        // 0x00fc83b0
  void Seek(int offset, int origin);                   // 0x00fc8450
  void Write(const void* data, int count);             // 0x00fc84b0
  void WriteFill(unsigned char value, int count);      // 0x00fc8500
  void Write16(unsigned short value);                  // 0x00fc8550
  void Write32(unsigned int value);                    // 0x00fc8580
};

}  // namespace movie
}  // namespace rw

// Stand-in for the buffered output stream with a flush callback. Field names after mpCallback are guesses.
typedef void (__cdecl* FlushCallback)(int, int, int, int, int);
struct Writer {
  FlushCallback mpCallback;  // +0x00
  int           mField04;    // +0x04
  int           mField08;    // +0x08
  int           mField0c;    // +0x0c
  int           mField10;    // +0x10
  int           mField14;    // +0x14
  int           mField18;    // +0x18
  int           mField1c;    // +0x1c

  void Flush();  // 0x00fc83b0
};

namespace {

inline int MinInt(int a, int b) {
  return a <= b ? a : b;
}

}  // namespace

// @ 0x00fc8390
void rw::movie::BufferedWriter::SetBuffer(void* buf, int size)
{
  mpBuffer08 = buf;
  mSize0c = size;
  mOffset10 = 0;
}

// @ 0x00fc83b0
void Writer::Flush()
{
  FlushCallback cb = mpCallback;
  if (cb == 0)
    return;

  int delta = mField1c;
  if (delta == mField14) {
    if (delta == 0) {
      cb(mField08, mField10, 0, 1, mField04);
      delta = mField10;
      mField1c = mField1c + delta;
    } else if (delta == mField18) {
      cb(mField08, mField10, 0, 2, mField04);
      delta = mField10;
      mField1c = mField1c + delta;
    } else {
      cb(mField08, mField10, 0, 3, mField04);
      delta = mField10;
      mField1c = mField1c + delta;
    }
  } else {
    cb(mField08, mField10, mField14 - delta, 3, mField04);
    delta = mField10;
    mField1c = mField14 + delta;
  }

  if (mField1c > mField18)
    mField18 = mField1c;
  mField14 = mField14 + delta;
  mField10 = 0;
}

// @ 0x00fc8450
void rw::movie::BufferedWriter_s00fc_8450::Seek(int offset, int origin)
{
  int delta = offset;
  if (origin == 1) {
    delta = offset - mBase - mPos;
  } else if (origin == 2) {
    delta = mEnd - mBase - mPos + offset;
  }
  if (delta == 0)
    return;

  int newPos = mPos + delta;
  if (newPos > 0 && newPos < mCapacity && mBase == mEnd) {
    mPos = newPos;
    return;
  }
  Flush();
  mBase += delta;
}

// @ 0x00fc84b0
void rw::movie::BufferedWriter_s00fc_8450::Write(const void* data, int count)
{
  while (count > 0) {
    int n = MinInt(mCapacity - mPos, count);
    memcpy(mBuffer + mPos, data, n);
    mPos += n;
    if (mPos == mCapacity)
      Flush();
    count -= n;
    data = (const char*)data + n;
  }
}

// @ 0x00fc8500
void rw::movie::BufferedWriter_s00fc_8450::WriteFill(unsigned char value, int count)
{
  while (count > 0) {
    int n = MinInt(mCapacity - mPos, count);
    memset(mBuffer + mPos, value, n);
    mPos += n;
    if (mPos == mCapacity)
      Flush();
    count -= n;
  }
}

// @ 0x00fc8550
void rw::movie::BufferedWriter_s00fc_8450::Write16(unsigned short value)
{
  if (mCapacity - mPos < 2)
    Flush();
  *(unsigned short*)(mBuffer + mPos) = value;
  mPos += 2;
}

// @ 0x00fc8580
void rw::movie::BufferedWriter_s00fc_8450::Write32(unsigned int value)
{
  if (mCapacity - mPos < 4)
    Flush();
  *(unsigned int*)(mBuffer + mPos) = value;
  mPos += 4;
}
