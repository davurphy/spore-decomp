// src/game/UTFSpore_Win32/EAFileUtil.cpp
// Object file: UTFSpore_Win32/EAFileUtil.obj. EA::IO::File and EA::IO::Directory helpers over the Win32 API.
// Each function keeps its '// @ 0xVA' marker.

#include "game/types.h"

#include <windows.h>
#include <direct.h>
#include <sys/stat.h>
#include <wchar.h>

// Cached temp path, empty until GetTempDirectoryPath first fills it.
extern wchar_t gTempPath[];   // 0x01668f28

// Appends a path separator when missing. Defined at 0x0092fe00 (EAFilePath.cpp).
bool EnsureTrailingPathSeparator(wchar_t* path, int len);

// Win32 constants the bodies below name. Values are the ones the retail code uses.
namespace {
const DWORD kTempBufferChars = 0x100;        // GetTempPathW / GetLongPathNameW buffer
const int kCwdBufferChars = 0xff;            // _wgetcwd buffer
const DWORD kMoveReplaceAndCopy = 3;         // MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED

// SetAttributes mask bits. The names are guesses from the bit values; the bodies match regardless.
const int kMaskWritable = 0x2;               // guess: clears or sets FILE_ATTRIBUTE_READONLY, inverted
const int kMaskUnsupportedA = 0x8;           // returns false
const int kMaskUnsupportedB = 0x10;          // returns false
const int kMaskArchive = 0x20;               // guess: FILE_ATTRIBUTE_ARCHIVE
const int kMaskHidden = 0x40;                // guess: FILE_ATTRIBUTE_HIDDEN
const int kMaskSystem = 0x80;                // guess: FILE_ATTRIBUTE_SYSTEM

// GetTime selector. The names are guesses from the stat fields read.
const int kTimeCreate = 1;                   // st_ctime
const int kTimeModify = 2;                   // st_mtime
const int kTimeAccess = 4;                   // st_atime
}  // namespace

// @ 0x00931fa0
namespace EA {
namespace IO {
namespace File {

bool Exists(wchar_t const * path)
{
    if (!path || !*path)
        return false;
    DWORD a = GetFileAttributesW(path);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

// @ 0x00931fd0
bool Remove(wchar_t const * path)
{
    return DeleteFileW(path) != 0;
}

// @ 0x00931ff0
bool Move(wchar_t const * from, wchar_t const * to, bool overwrite)
{
    if (!overwrite && to && *to) {
        DWORD a = GetFileAttributesW(to);
        if (a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY) == 0)
            return false;
    }
    return MoveFileExW(from, to, kMoveReplaceAndCopy) != 0;
}

// @ 0x00932060
unsigned int GetSize(wchar_t const * path)
{
    WIN32_FIND_DATAW fd;
    DWORD size = INVALID_FILE_SIZE;
    HANDLE h = FindFirstFileW(path, &fd);
    if (h != INVALID_HANDLE_VALUE) {
        size = fd.nFileSizeLow;
        FindClose(h);
    }
    return size;
}

// @ 0x009320a0
bool SetAttributes(wchar_t const * path, int mask, bool set)
{
    DWORD orig = GetFileAttributesW(path);
    DWORD a = orig;
    if (mask & kMaskWritable) {
        if (set)
            a &= ~FILE_ATTRIBUTE_READONLY;
        else
            a |= FILE_ATTRIBUTE_READONLY;
    }
    if (mask & kMaskUnsupportedA)
        return false;
    if (mask & kMaskUnsupportedB)
        return false;
    if (mask & kMaskArchive) {
        if (set)
            a |= FILE_ATTRIBUTE_ARCHIVE;
        else
            a &= ~FILE_ATTRIBUTE_ARCHIVE;
    }
    if (mask & kMaskHidden) {
        if (set)
            a |= FILE_ATTRIBUTE_HIDDEN;
        else
            a &= ~FILE_ATTRIBUTE_HIDDEN;
    }
    if (mask & kMaskSystem) {
        if (set)
            a |= FILE_ATTRIBUTE_SYSTEM;
        else
            a &= ~FILE_ATTRIBUTE_SYSTEM;
    }
    if (a == orig)
        return true;
    return SetFileAttributesW(path, a) != 0;
}

}  // namespace File
}  // namespace IO
}  // namespace EA

// @ 0x00932030
namespace File {
bool Copy(const wchar_t* from, const wchar_t* to, bool overwrite)
{
    return CopyFileW(from, to, !overwrite) != 0;
}

// @ 0x00932130
int64_t GetTime(const wchar_t* path, int type)
{
    struct _stat64i32 st;
    if (_wstat64i32(path, &st) == 0) {
        if (type == kTimeCreate)
            return st.st_ctime;
        if (type == kTimeModify)
            return st.st_mtime;
        if (type == kTimeAccess)
            return st.st_atime;
    }
    return 0;
}
}  // namespace File

// @ 0x00932190
int GetTempDirectoryPath(wchar_t* out)
{
    if (gTempPath[0]) {
        const wchar_t* s = gTempPath;
        wchar_t* d = out;
        do {
        } while ((*d++ = *s++) != 0);
        return (int)wcslen(out);
    }
    DWORD n = GetTempPathW(kTempBufferChars, out);
    if (n != 0) {
        n = GetLongPathNameW(out, out, kTempBufferChars);
        if (EnsureTrailingPathSeparator(out, -1))
            n = n + 1;
        return n;
    }
    return -1;
}

// @ 0x00932220
namespace EA {
namespace IO {
int GetDriveSerialNumber(wchar_t const * path, wchar_t * out)
{
    wchar_t volume[MAX_PATH];
    DWORD serial;
    DWORD flags;
    if (GetVolumePathNameW(path, volume, MAX_PATH) &&
        GetVolumeInformationW(volume, 0, 0, &serial, 0, &flags, 0, 0)) {
        wsprintfW(out, L"%04X-%04X", serial >> 16, serial & 0xffff);
        return 10;
    }
    *out = 0;
    return 0;
}
}  // namespace IO
}  // namespace EA

// @ 0x00932370
namespace EA {
namespace IO {
namespace Directory {
int GetCurrentWorkingDirectory(wchar_t * out)
{
    if (_wgetcwd(out, kCwdBufferChars)) {
        EnsureTrailingPathSeparator(out, -1);
        return (int)wcslen(out);
    }
    return -1;
}
}  // namespace Directory
}  // namespace IO
}  // namespace EA

// @ 0x009322b0
namespace Directory {
bool Exists(const wchar_t* path)
{
    if (!path || !*path)
        return false;
    DWORD a = GetFileAttributesW(path);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY) != 0;
}
}  // namespace Directory

// @ 0x009322e0
bool DirectoryCreate(const wchar_t* path)
{
    wchar_t tmp[260];
    if (*path != 0) {
        unsigned last = path[wcslen(path) - 1];
        if (last == '\\' || last == '/' || last == ':') {
            tmp[wcslen(path)] = 0;
            return CreateDirectoryW(path, 0) != 0;
        }
        return CreateDirectoryW(path, 0) != 0;
    }
    return false;
}
