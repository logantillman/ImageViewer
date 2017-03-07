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

#include "ExifUtils.h"
#include <QImage>
#include <QTransform>
#include <QDebug>

#if defined (HAS_THIRDPARTY_QTEXTENDED)
#include "qexifimageheader.h"
#endif

namespace ExifUtils {

quint16 GetExifOrientation(const QString &filePath)
{
#if defined (HAS_THIRDPARTY_QTEXTENDED)
    QExifImageHeader exifHeader;
    if(exifHeader.loadFromJpeg(filePath) && exifHeader.contains(QExifImageHeader::Orientation))
    {
        quint16 orientation = exifHeader.value(QExifImageHeader::Orientation).toShort();
        qDebug() << "EXIF header detected";
        qDebug() << "EXIF orientation =" << orientation;
        return orientation;
    }
#else
    Q_UNUSED(filePath);
#endif
    return 1;
}

// https://bugreports.qt.io/browse/QTBUG-37946
// https://codereview.qt-project.org/#/c/110668/2
// https://github.com/qt/qtbase/blob/v5.4.0/src/gui/image/qjpeghandler.cpp
void ApplyExifOrientation(QImage *image, quint16 exifOrientation)
{
    // This is not an optimized implementation, but easiest to maintain
    QTransform transform;

    switch (exifOrientation) {
        case 1: // normal
            break;
        case 2: // mirror horizontal
            *image = image->mirrored(true, false);
            break;
        case 3: // rotate 180
            transform.rotate(180);
            *image = image->transformed(transform);
            break;
        case 4: // mirror vertical
            *image = image->mirrored(false, true);
            break;
        case 5: // mirror horizontal and rotate 270 CCW
            *image = image->mirrored(true, false);
            transform.rotate(270);
            *image = image->transformed(transform);
            break;
        case 6: // rotate 90 CW
            transform.rotate(90);
            *image = image->transformed(transform);
            break;
        case 7: // mirror horizontal and rotate 90 CW
            *image = image->mirrored(true, false);
            transform.rotate(90);
            *image = image->transformed(transform);
            break;
        case 8: // rotate 270 CW
            transform.rotate(-90);
            *image = image->transformed(transform);
            break;
        default:
            qWarning("This should never happen");
    }
    exifOrientation = 1;
}

} // namespace ExifUtils