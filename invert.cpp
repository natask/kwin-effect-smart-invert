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

#include "invert.h"

// KConfigSkeleton
#include "invertconfig.h"

#include <QAction>
#include <QFile>
#include <kwinglutils.h>
#include <kwinglplatform.h>
#include <KGlobalAccel>
#include <KLocalizedString>
#include <QStandardPaths>

#include <QMatrix4x4>

namespace KWin
{

  InvertEffect::InvertEffect()
    :   m_inited(false),
        m_valid(true),
        m_shader(nullptr),
        m_allWindows(true)
  {
    initConfig<InvertConfig>();

    QAction* a = new QAction(this);
    a->setObjectName(QStringLiteral("SmartInvert"));
    a->setText(i18n("Toggle Smart Invert Effect"));
    KGlobalAccel::self()->setDefaultShortcut(a, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_R);
    KGlobalAccel::self()->setShortcut(a, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_R);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_R, a);
    connect(a, &QAction::triggered, this, &InvertEffect::toggleScreenInversion);

    QAction* b = new QAction(this);
    b->setObjectName(QStringLiteral("SmartInvertWindow"));
    b->setText(i18n("Toggle Smart Invert Effect on Window"));
    KGlobalAccel::self()->setDefaultShortcut(b, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_R);
    KGlobalAccel::self()->setShortcut(b, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_R);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_R, b);
    connect(b, &QAction::triggered, this, &InvertEffect::toggleWindow);

    reconfigure(ReconfigureAll);
    connect(effects, &EffectsHandler::windowClosed, this, &InvertEffect::slotWindowClosed);
  }

  InvertEffect::~InvertEffect()
  {
    delete m_shader;
  }

  bool InvertEffect::supported()
  {
    return effects->compositingType() == OpenGLCompositing;
  }

  void InvertEffect::reconfigure(ReconfigureFlags)
  {
    InvertConfig::self()->read();
    m_blocklist = InvertConfig::blocklist().toLower().split(",");
  }

  bool InvertEffect::loadData()
  {
    m_inited = true;

    QString shadersDir(QStringLiteral("kwin/shaders/1.10/"));
#ifdef KWIN_HAVE_OPENGLES
    const qint64 coreVersionNumber = kVersionNumber(3, 0);
#else
    const qint64 version = KWin::kVersionNumber(1, 40);
#endif
    if (KWin::GLPlatform::instance()->glslVersion() >= version)
      shadersDir = QStringLiteral("kwin/shaders/1.40/");

    const QString fragmentshader = QStandardPaths::locate(QStandardPaths::GenericDataLocation, shadersDir + QStringLiteral("invert.frag"));


    QFile file(fragmentshader);
    if (file.open(QFile::ReadOnly))
    {
      QByteArray frag = file.readAll();
      m_shader = KWin::ShaderManager::instance()->generateCustomShader(KWin::ShaderTrait::MapTexture, QByteArray(), frag);
      file.close();

      if (!m_shader->isValid()) {
        //qCCritical(KWINEFFECTS) << "The shader failed to load!";
        return false;
      }
      return true;
    }
    else {
      deleteLater();
      return false;
    }
  }

  /**
   * Returns shader from package kwin/shaders/1.40/.
   * **/
  GLShader* InvertEffect::readShader()
  {
    auto shader = m_shader;
    QString shadersDir(QStringLiteral("kwin/shaders/1.10/"));
#ifdef KWIN_HAVE_OPENGLES
    const qint64 coreVersionNumber = kVersionNumber(3, 0);
#else
    const qint64 version = KWin::kVersionNumber(1, 40);
#endif
    if (KWin::GLPlatform::instance()->glslVersion() >= version)
      shadersDir = QStringLiteral("kwin/shaders/1.40/");

    const QString fragmentshader = QStandardPaths::locate(QStandardPaths::GenericDataLocation, shadersDir + QStringLiteral("invert.frag"));


    QFile file(fragmentshader);
    if (file.open(QFile::ReadOnly))
    {
      QByteArray frag = file.readAll();
      shader = KWin::ShaderManager::instance()->generateCustomShader(KWin::ShaderTrait::MapTexture, QByteArray(), frag);
      file.close();}
    return shader;
  }

  bool InvertEffect::isWindowInBlocklist(EffectWindow * w) {
    auto windowClass = w->windowClass().toLower();
    for (auto&& blockedWindow: m_blocklist){
      if(windowClass.contains(blockedWindow)){
        return true;
      }
    }
    return false;
  }

  void InvertEffect::drawWindow(EffectWindow* w, int mask, const QRegion &region, WindowPaintData& data)
  {
    // Load if we haven't already
    if (m_valid && !m_inited)
      m_valid = loadData();

    bool useShader = m_valid && (m_allWindows != m_windows.contains(w)) && !isWindowInBlocklist(w);
    auto shader = m_windows_shader.value(w, m_shader);

    if (useShader) {
      ShaderManager *shaderManager = ShaderManager::instance();
      shaderManager->pushShader(shader);

      data.shader = shader;
    }

    effects->drawWindow(w, mask, region, data);

    if (useShader) {
      ShaderManager::instance()->popShader();
    }
  }

  void InvertEffect::slotWindowClosed(EffectWindow* w)
  {
    m_windows_shader.remove(w);
    m_windows.removeOne(w);
  }

  void InvertEffect::toggleScreenInversion()
  {
    m_valid = loadData(); //hotswap
    m_windows_shader.clear();
    m_allWindows = !m_allWindows;
    effects->addRepaintFull();
  }

  void InvertEffect::toggleWindow()
  {
    if (!effects->activeWindow()) {
      return;
    }
    m_windows_shader.insert(effects->activeWindow(), readShader());
    if (!m_windows.contains(effects->activeWindow()))
      m_windows.append(effects->activeWindow());
    else
      m_windows.removeOne(effects->activeWindow());
    effects->activeWindow()->addRepaintFull();
  }

  bool InvertEffect::isActive() const
  {
    return m_valid && (m_allWindows || !m_windows.isEmpty());
  }

  bool InvertEffect::provides(Feature f)
  {
    return f == ScreenInversion;
  }

} // namespace
