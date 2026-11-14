// src/game/SPBake/SPEditorAccessor.cpp -- editor tuning accessors (slice s00401000).
// Each function keeps its '// @ 0xVA' marker.

// Setting globals read by the accessors below. No canonical header declares them yet,
// so these names are guesses taken from the slice.
int g_Setting3;
int g_Setting4;
int g_Setting5;
int g_Setting6;
int g_Setting7;

// @ 0x00401030
int GetSetting3()
{
    return g_Setting3;
}

// @ 0x00401040
int GetSetting4()
{
    return g_Setting4;
}

// @ 0x00401050
int GetSetting5()
{
    return g_Setting5;
}

// @ 0x00401060
int GetSetting6()
{
    return g_Setting6;
}

// @ 0x00401070
int GetSetting7()
{
    return g_Setting7;
}
