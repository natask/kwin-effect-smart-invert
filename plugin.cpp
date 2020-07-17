#include "invert.h"

KWIN_EFFECT_FACTORY_ENABLED(InvertEffectFactory,
                            KWin::InvertEffect,
                            "smartInvert.json",
                            return false;)

#include "plugin.moc"
