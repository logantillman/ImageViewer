/*
   Copyright (C) 2017 Peter S. Zhigalov <peter.zhigalov@gmail.com>

   This file is part of the `ImageViewer' program.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#if !defined(ANIMATIONOBJECT_H_INCLUDED)
#define ANIMATIONOBJECT_H_INCLUDED

#include <QObject>
#include "Utils/ScopedPointer.h"

class QPixmap;
class IAnimationProvider;

class AnimationObject : public QObject
{
    Q_OBJECT

public:
    explicit AnimationObject(QObject *parent = NULL);
    explicit AnimationObject(IAnimationProvider *provider, QObject *parent = NULL);
    ~AnimationObject();

    void setProvider(IAnimationProvider *provider);
    IAnimationProvider *provider() const;

    QPixmap currentPixmap() const;

    bool isValid() const;

signals:
    void updated();

private slots:
    void loadNextFrame();

private:
    Q_DISABLE_COPY(AnimationObject)
    struct Impl;
    QScopedPointer<Impl> m_impl;
};

#endif // ANIMATIONOBJECT_H_INCLUDED