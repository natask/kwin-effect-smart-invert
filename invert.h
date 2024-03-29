/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2007 Rivo Laks <rivolaks@hot.ee>
Copyright (C) 2008 Lucas Murray <lmurray@undefinedfire.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#ifndef KWIN_INVERT_H
#define KWIN_INVERT_H

#include <kwineffects.h>

namespace KWin
{

class GLShader;

/**
 * Inverts desktop's colors
 */
class InvertEffect
    : public Effect
{
    Q_OBJECT
    Q_PROPERTY(QStringList blocklist READ blocklist);
public:
    InvertEffect();
    ~InvertEffect() override;
    void reconfigure(ReconfigureFlags) override;

    void drawWindow(EffectWindow* w, int mask, const QRegion &region, WindowPaintData& data) override;
    bool isActive() const override;
    bool provides(Feature) override;
    bool isWindowInBlocklist(EffectWindow * w);

    int requestedEffectChainPosition() const override;

    static bool supported();

    QStringList blocklist() const;
public Q_SLOTS:
    void toggleScreenInversion();
    void toggleWindow();
    void slotWindowClosed(KWin::EffectWindow *w);

protected:
    GLShader* readShader();
    bool loadData();

private:
    bool m_inited;
    bool m_valid;
    GLShader* m_shader;
    bool m_allWindows;
    QStringList m_blocklist;
    QList<EffectWindow*> m_windows;
    QMap<EffectWindow*,GLShader*> m_windows_shader;
};

inline QStringList InvertEffect::blocklist() const
{
        return m_blocklist;
}

inline int InvertEffect::requestedEffectChainPosition() const
{
    return 99;
}

} // namespace

#endif
