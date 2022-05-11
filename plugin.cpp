#include "invert.h"
KWIN_EFFECT_FACTORY_ENABLED(KWin::InvertEffect,
                            "metadata.json",
                            return KWin::InvertEffect::supported();)
#include "plugin.moc"
