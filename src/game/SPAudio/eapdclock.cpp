// src/game/SPAudio/eapdclock.cpp -- EA::Audio::Eapd clock (logical time and the ordered timer list).
// Object file: SPAudio/eapdclock.obj. Each function keeps its '// @ 0xVA' marker.

namespace EA {
namespace Audio {
namespace Eapd {

// Local layout of a clock. No canonical header declares t_clock yet, so it stays here.
struct t_clock {
    double   c_settime;   // +0   time the clock fires (logical seconds)
    int      c_set;       // +8   0 = idle, 1 = in the list, 2 = paused
    void*    c_owner;     // +0xc
    void*    c_method;    // +0x10
    t_clock* c_next;      // +0x14 next clock in s_clocklist
};

const int kClockBytes = 0x18;               // sizeof(t_clock) in the game's allocator
const float kMillisecondsToSeconds = 0.001f;

extern t_clock*     s_clocklist;            // 0x1675410  head of the ordered list of set clocks
extern double       s_time;                 // 0x1675420  logical time
extern const double s_ctime;                // 0x1454410  initial c_settime for a new clock
extern const double s_timescale;            // 0x1454428  guess: scale used by clock_gettimesince; value not confirmed

void* getbytes(int n, const char* tag);     // 0xa49610
void  freebytes(void* p, int n);            // 0xa49650

// @ 0x00a49080
t_clock* clock_new(void* owner, void* fn)
{
    t_clock* c = static_cast<t_clock*>(getbytes(kClockBytes, "clock"));
    c->c_settime = s_ctime;
    c->c_set = 0;
    c->c_owner = owner;
    c->c_method = fn;
    c->c_next = 0;
    return c;
}

// @ 0x00a490b0
void clock_unset(t_clock* c)
{
    if (c->c_set == 1) {
        t_clock* o = s_clocklist;
        if (c == o) {
            s_clocklist = c->c_next;
            c->c_set = 0;
            return;
        }
        while (o->c_next != c)
            o = o->c_next;
        o->c_next = c->c_next;
    }
    c->c_set = 0;
}

// @ 0x00a490f0
// Not byte-exact yet (see nonmatching.txt). Body kept as the slice wrote it.
void clock_set(t_clock* c, double systime)
{
    if (c->c_set == 1)
        clock_unset(c);
    double t = (s_time > systime) ? s_time : systime;
    c->c_settime = t;
    t_clock* cl = s_clocklist;
    if (cl == 0 || t < cl->c_settime) {
        c->c_next = cl;
        s_clocklist = c;
        c->c_set = 1;
    } else {
        t_clock* o = cl;
        t_clock* n = cl->c_next;
        while (n && n->c_settime <= t) {
            o = n;
            n = n->c_next;
        }
        o->c_next = c;
        c->c_set = 1;
        c->c_next = n;
    }
}

// @ 0x00a49170
void clock_pause(t_clock* c)
{
    if (c->c_set == 1) {
        double t = c->c_settime - s_time;
        clock_unset(c);
        c->c_set = 2;
        c->c_settime = t;
    }
}

// @ 0x00a491a0
void clock_unpause(t_clock* c)
{
    if (c->c_set == 2) {
        double t = c->c_settime + s_time;
        c->c_set = 0;
        clock_set(c, t);
    }
}

// @ 0x00a491d0
bool clock_ispaused(t_clock* c)
{
    return c->c_set == 2;
}

// @ 0x00a491e0
void clock_delay(t_clock* c, float delay)
{
    clock_set(c, (double)(delay * kMillisecondsToSeconds) + s_time);
}

// @ 0x00a49210
double clock_getlogicaltime()
{
    return s_time;
}

// @ 0x00a49220
double clock_gettimesince(double systime)
{
    return (s_time - systime) * s_timescale;
}

// @ 0x00a49240
double clock_getlogicaltimeafter(float delay)
{
    return (double)delay * kMillisecondsToSeconds + s_time;
}

// @ 0x00a49260
void clock_free(t_clock* c)
{
    clock_unset(c);
    freebytes(c, kClockBytes);
}

}  // namespace Eapd
}  // namespace Audio
}  // namespace EA
