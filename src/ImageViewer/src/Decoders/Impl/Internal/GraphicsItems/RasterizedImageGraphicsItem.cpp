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

#include "RasterizedImageGraphicsItem.h"

#include <algorithm>
#include <cassert>

#include <QObject>
#include <QPainter>
#include <QThread>

#include "Utils/ScopedPointer.h"
#include "Utils/SharedPointer.h"

#include "AbstractScalingManager.h"
#include "AbstractScalingWorker.h"
#include "AutoUpdatedScalingWorkerHandler.h"
#include "GraphicsItemUtils.h"

// ====================================================================================================

RasterizedImageGraphicsItem::IRasterizedImageProvider::~IRasterizedImageProvider()
{}

typedef RasterizedImageGraphicsItem::IRasterizedImageProvider IProvider;

// ====================================================================================================

namespace {

class RasterizerWorker : public AbstractScalingWorker
{
public:
    void setProvider(const QSharedPointer<IProvider> &provider)
    {
        m_provider = provider;
    }

    bool generateScaledImage(const bool checkAbortedState)
    {
#define CHECK_ABORT_STATE if(checkAbortedState && isAborted()) return false
        CHECK_ABORT_STATE;
        if(!m_provider)
            return false;
        CHECK_ABORT_STATE;
        const qreal newScaleFactor = m_scaleFactor;
        CHECK_ABORT_STATE;
        QImage image = m_provider->image(newScaleFactor);
        if(image.isNull())
            return false;
        CHECK_ABORT_STATE;
        lockScaledImage();
        m_scaledData.reset(new ScaledImageData(image, newScaleFactor));
        unlockScaledImage();
        CHECK_ABORT_STATE;
        return true;
#undef CHECK_ABORT_STATE
    }

private:
    bool scaleImpl()
    {
        return generateScaledImage(true);
    }

    QSharedPointer<IProvider> m_provider;
};

} // namespace

// ====================================================================================================

namespace {

class RasterizerManager : public AbstractScalingManager
{
public:
    RasterizerManager(RasterizerWorker *worker, AutoUpdatedScalingWorkerHandler* handler, QThread *thread)
        : AbstractScalingManager(worker, handler, thread)
    {}

    void setProvider(const QSharedPointer<IProvider> &provider)
    {
        static_cast<RasterizerWorker*>(m_scalingWorker)->setProvider(provider);
    }

    void execTaskSynchronously(const qreal scaleFactor)
    {
        abortTask();
        m_scalingWorker->setScaleFactor(scaleFactor);
        static_cast<RasterizerWorker*>(m_scalingWorker)->generateScaledImage(false);
    }
};

RasterizerManager *createRasterizerManager(RasterizedImageGraphicsItem *item)
{
    RasterizerWorker *worker = new RasterizerWorker();
    QThread *thread = new QThread();
    AutoUpdatedScalingWorkerHandler *handler = new AutoUpdatedScalingWorkerHandler(item, worker, thread);
    return new RasterizerManager(worker, handler, thread);
}

} // namespace

// ====================================================================================================

struct RasterizedImageGraphicsItem::Impl
{
    QSharedPointer<IProvider> provider;
    Qt::TransformationMode transformationMode;
    QScopedPointer<RasterizerManager> rasterizerManager;

    Impl(RasterizedImageGraphicsItem *rasterizedImageGraphicsItem)
        : transformationMode(Qt::FastTransformation)
        , rasterizerManager(createRasterizerManager(rasterizedImageGraphicsItem))
    {}
};

// ====================================================================================================

RasterizedImageGraphicsItem::RasterizedImageGraphicsItem(QGraphicsItem *parentItem)
    : QGraphicsItem(parentItem)
    , m_impl(new Impl(this))
{}

RasterizedImageGraphicsItem::RasterizedImageGraphicsItem(QSharedPointer<IRasterizedImageProvider> provider, QGraphicsItem *parentItem)
    : QGraphicsItem(parentItem)
    , m_impl(new Impl(this))
{
    setProvider(provider);
}

RasterizedImageGraphicsItem::~RasterizedImageGraphicsItem()
{}

QSharedPointer<RasterizedImageGraphicsItem::IRasterizedImageProvider> RasterizedImageGraphicsItem::provider() const
{
    return m_impl->provider;
}

void RasterizedImageGraphicsItem::setProvider(QSharedPointer<IRasterizedImageProvider> provider)
{
    m_impl->provider = provider;
    m_impl->rasterizerManager->setProvider(provider);
    update();
}

Qt::TransformationMode RasterizedImageGraphicsItem::transformationMode() const
{
    return m_impl->transformationMode;
}

void RasterizedImageGraphicsItem::setTransformationMode(Qt::TransformationMode mode)
{
    m_impl->transformationMode = mode;
    update();
}

QRectF RasterizedImageGraphicsItem::boundingRect() const
{
    if(!m_impl->provider)
        return QRectF();
    return m_impl->provider->boundingRect();
}

void RasterizedImageGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(!m_impl->provider || !m_impl->provider->isValid())
        return;

    const qreal deviceScaleFactor = GraphicsItemUtils::GetDeviceScaleFactor(painter);
    const qreal maxScaleFactor = m_impl->provider->maxScaleFactor();
    const qreal minScaleFactor = m_impl->provider->minScaleFactor();
    const qreal newScaleFactor = std::max(std::min(deviceScaleFactor, maxScaleFactor), minScaleFactor);

    if(!m_impl->rasterizerManager->hasScaledData())
        m_impl->rasterizerManager->execTaskSynchronously(newScaleFactor);

    if(!m_impl->rasterizerManager->hasScaledData())
        return;

    painter->setRenderHint(QPainter::SmoothPixmapTransform, m_impl->transformationMode == Qt::SmoothTransformation);
    m_impl->rasterizerManager->beginScaledImageProcessing();
    const qreal actualScaleFactor = m_impl->rasterizerManager->getScaledScaleFactor();
    GraphicsItemUtils::DrawScaledImage(painter,m_impl->rasterizerManager->getScaledImage(), boundingRect(), actualScaleFactor);
    m_impl->rasterizerManager->endScaledImageProcessing();

    if(!GraphicsItemUtils::IsFuzzyEqualScaleFactors(newScaleFactor, actualScaleFactor))
        m_impl->rasterizerManager->startTask(newScaleFactor);
}

// ====================================================================================================