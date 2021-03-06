/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "minisplitter.h"

#include "stylehelper.h"

#include <QPaintEvent>
#include <QPainter>
#include <QSplitterHandle>
#include <QDebug>

class MiniSplitterHandle : public QSplitterHandle
{
public:
    MiniSplitterHandle(Qt::Orientation orientation, QSplitter *parent)
            : QSplitterHandle(orientation, parent)
    {
        _backgroundColor = QColor(0,0,0,0);
        setMask(QRegion(contentsRect()));
        setAttribute(Qt::WA_MouseNoMask, true);

    }
    void setBackgroundColor(QColor color) {
        _backgroundColor = color;
    }
    ~MiniSplitterHandle();
protected:
    QColor _backgroundColor;
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
};
MiniSplitterHandle::~MiniSplitterHandle()
{
#ifdef DEBUG_DESTRUCTOR
    qDebug()<<"delete MiniSplitterHandle";
#endif
}
void MiniSplitterHandle::resizeEvent(QResizeEvent *event)
{
    if (orientation() == Qt::Horizontal)
        setContentsMargins(2, 0, 2, 0);
    else
        setContentsMargins(0, 2, 0, 2);
    setMask(QRegion(contentsRect()));
    QSplitterHandle::resizeEvent(event);
}

void MiniSplitterHandle::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    painter.fillRect(event->rect(), _backgroundColor);
}

void MiniSplitter::setBackgroundColor(QColor color)
{
    for(int i = 1; i < this->count(); ++i)
    {
        dynamic_cast<MiniSplitterHandle*>(this->handle(i))->setBackgroundColor(color);
    }
}

QSplitterHandle *MiniSplitter::createHandle()
{
    return new MiniSplitterHandle(orientation(), this);
}

MiniSplitter::MiniSplitter(QWidget *parent)
    : QSplitter(parent)
{
    setHandleWidth(2);
    setChildrenCollapsible(false);
    setProperty("minisplitter", true);
}

MiniSplitter::MiniSplitter(Qt::Orientation orientation)
    : QSplitter(orientation)
{
    setHandleWidth(2);
    setChildrenCollapsible(false);
    setProperty("minisplitter", true);
}

