/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the WebP plugins in the Qt ImageFormats module.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWEBPHANDLER_P_H
#define QWEBPHANDLER_P_H

#include <QtGui/qcolor.h>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
#include <QtGui/qcolorspace.h>
#endif
#include <QtGui/qimage.h>
#include <QtGui/qimageiohandler.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qsize.h>

#include "webp/decode.h"
#include "webp/demux.h"

class QWebpHandler : public QImageIOHandler
{
public:
    QWebpHandler();
    ~QWebpHandler();

public:
    bool canRead() const Q_DECL_OVERRIDE;
    bool read(QImage *image) Q_DECL_OVERRIDE;

    static bool canRead(QIODevice *device);

    bool write(const QImage &image) Q_DECL_OVERRIDE;
    QVariant option(ImageOption option) const Q_DECL_OVERRIDE;
    void setOption(ImageOption option, const QVariant &value) Q_DECL_OVERRIDE;
    bool supportsOption(ImageOption option) const Q_DECL_OVERRIDE;

    int imageCount() const Q_DECL_OVERRIDE;
    int currentImageNumber() const Q_DECL_OVERRIDE;
    QRect currentImageRect() const Q_DECL_OVERRIDE;
    int loopCount() const Q_DECL_OVERRIDE;
    int nextImageDelay() const Q_DECL_OVERRIDE;

private:
    bool ensureScanned() const;
    bool ensureDemuxer();

private:
    enum ScanState {
        ScanError = -1,
        ScanNotScanned = 0,
        ScanSuccess = 1,
    };

    int m_quality;
    mutable ScanState m_scanState;
    WebPBitstreamFeatures m_features;
    uint32_t m_formatFlags;
    int m_loop;
    int m_frameCount;
    QColor m_bgColor;
    QByteArray m_rawData;
    WebPData m_webpData;
    WebPDemuxer *m_demuxer;
    WebPIterator m_iter;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    QColorSpace m_colorSpace;
#endif
    QImage *m_composited;   // For animation frames composition
};

#endif // WEBPHANDLER_H
