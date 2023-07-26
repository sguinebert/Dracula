#include "CustomGrip.h"


CustomGrip::CustomGrip(QWidget *parent, Qt::Edge position, bool disable_color)
    : QWidget(parent), parentWidget(parent), gripPosition(position) {
    setupUI();

    if (position == Qt::TopEdge) {
        gripTopLeft = new QSizeGrip(topLeft);
        gripTopRight = new QSizeGrip(topRight);
        m_top->setStyleSheet("background-color: rgb(85, 255, 255);");

        gripTopLeft->setCursor(Qt::SizeFDiagCursor);
        gripTopRight->setCursor(Qt::SizeBDiagCursor);

        gripTopRight->installEventFilter(this);
        //connect(m_top, &QWidget::mouseMoveEvent, this, &CustomGrip::resizeTop);
    } else if (position == Qt::BottomEdge) {
        gripBottomLeft = new QSizeGrip(bottomLeft);
        gripBottomRight = new QSizeGrip(bottomRight);
        m_bottom->setStyleSheet("background-color: rgb(85, 170, 0);");

        gripBottomLeft->setCursor(Qt::SizeBDiagCursor);
        gripBottomRight->setCursor(Qt::SizeFDiagCursor);

        gripBottomRight->installEventFilter(this);
        //connect(m_bottom, &QWidget::mouseMoveEvent, this, &CustomGrip::resizeBottom);
    } else if (position == Qt::LeftEdge) {
        gripLeft = new QSizeGrip(leftGrip);
        leftGrip->setStyleSheet("background-color: rgb(255, 121, 198);");

        gripLeft->setCursor(Qt::SizeHorCursor);

        gripLeft->installEventFilter(this);
        //connect(leftGrip, &QWidget::mouseMoveEvent, this, &CustomGrip::resizeLeft);
    } else if (position == Qt::RightEdge) {
        gripRight = new QSizeGrip(rightGrip);
        rightGrip->setStyleSheet("background-color: rgb(255, 0, 127);");

        gripRight->setCursor(Qt::SizeHorCursor);

        gripRight->installEventFilter(this);
        //connect(rightGrip, &QWidget::mouseMoveEvent, this, &CustomGrip::resizeRight);
    }

    if (disable_color) {
        if (position == Qt::TopEdge) {
            topLeft->setStyleSheet("background: transparent;");
            topRight->setStyleSheet("background: transparent;");
            m_top->setStyleSheet("background: transparent;");
        } else if (position == Qt::BottomEdge) {
            bottomLeft->setStyleSheet("background: transparent;");
            bottomRight->setStyleSheet("background: transparent;");
            m_bottom->setStyleSheet("background: transparent;");
        } else if (position == Qt::LeftEdge) {
            leftGrip->setStyleSheet("background: transparent;");
        } else if (position == Qt::RightEdge) {
            rightGrip->setStyleSheet("background: transparent;");
        }
    }
}

void CustomGrip::setupUI() {
    gripContainer = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(gripContainer);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    if (gripPosition == Qt::TopEdge) {
        containerTop = new QFrame(this);
        containerTop->setObjectName("container_top");
        containerTop->setFrameShape(QFrame::NoFrame);
        containerTop->setFrameShadow(QFrame::Raised);
        containerTop->setGeometry(0, 0, parentWidget->width(), 10);
        containerTop->setMinimumSize(QSize(0, 10));
        containerTop->setMaximumSize(QSize(16777215, 10));
        layout->addWidget(containerTop);

        topLeft = new QFrame(containerTop);
        topLeft->setObjectName("top_left");
        topLeft->setFrameShape(QFrame::NoFrame);
        topLeft->setFrameShadow(QFrame::Raised);
        topLeft->setGeometry(0, 0, 10, 10);
        topLeft->setMinimumSize(QSize(10, 10));
        topLeft->setMaximumSize(QSize(10, 10));
        topLeft->setCursor(Qt::SizeFDiagCursor);
        topLeft->setStyleSheet("background-color: rgb(33, 37, 43);");

        m_top = new QFrame(containerTop);
        m_top->setObjectName("m_top");
        m_top->setFrameShape(QFrame::NoFrame);
        m_top->setFrameShadow(QFrame::Raised);
        m_top->setGeometry(10, 0, parentWidget->width() - 20, 10);
        m_top->setCursor(Qt::SizeVerCursor);
        m_top->setStyleSheet("background-color: rgb(85, 255, 255);");

        topRight = new QFrame(containerTop);
        topRight->setObjectName("top_right");
        topRight->setFrameShape(QFrame::NoFrame);
        topRight->setFrameShadow(QFrame::Raised);
        topRight->setGeometry(parentWidget->width() - 10, 0, 10, 10);
        topRight->setMinimumSize(QSize(10, 10));
        topRight->setMaximumSize(QSize(10, 10));
        topRight->setCursor(Qt::SizeBDiagCursor);
        topRight->setStyleSheet("background-color: rgb(33, 37, 43);");
    } else if (gripPosition == Qt::BottomEdge) {
        containerBottom = new QFrame(this);
        containerBottom->setObjectName("container_bottom");
        containerBottom->setFrameShape(QFrame::NoFrame);
        containerBottom->setFrameShadow(QFrame::Raised);
        containerBottom->setGeometry(0, 0, parentWidget->width(), 10);
        containerBottom->setMinimumSize(QSize(0, 10));
        containerBottom->setMaximumSize(QSize(16777215, 10));
        layout->addWidget(containerBottom);

        bottomLeft = new QFrame(containerBottom);
        bottomLeft->setObjectName("bottom_left");
        bottomLeft->setFrameShape(QFrame::NoFrame);
        bottomLeft->setFrameShadow(QFrame::Raised);
        bottomLeft->setGeometry(0, 0, 10, 10);
        bottomLeft->setMinimumSize(QSize(10, 10));
        bottomLeft->setMaximumSize(QSize(10, 10));
        bottomLeft->setCursor(Qt::SizeBDiagCursor);
        bottomLeft->setStyleSheet("background-color: rgb(33, 37, 43);");

        m_bottom = new QFrame(containerBottom);
        m_bottom->setObjectName("m_bottom");
        m_bottom->setFrameShape(QFrame::NoFrame);
        m_bottom->setFrameShadow(QFrame::Raised);
        m_bottom->setGeometry(10, 0, parentWidget->width() - 20, 10);
        m_bottom->setCursor(Qt::SizeVerCursor);
        m_bottom->setStyleSheet("background-color: rgb(85, 170, 0);");

        bottomRight = new QFrame(containerBottom);
        bottomRight->setObjectName("bottom_right");
        bottomRight->setFrameShape(QFrame::NoFrame);
        bottomRight->setFrameShadow(QFrame::Raised);
        bottomRight->setGeometry(parentWidget->width() - 10, 0, 10, 10);
        bottomRight->setMinimumSize(QSize(10, 10));
        bottomRight->setMaximumSize(QSize(10, 10));
        bottomRight->setCursor(Qt::SizeFDiagCursor);
        bottomRight->setStyleSheet("background-color: rgb(33, 37, 43);");
    } else if (gripPosition == Qt::LeftEdge) {
        leftGrip = new QFrame(this);
        leftGrip->setObjectName("left");
        leftGrip->setGeometry(0, 10, 10, parentWidget->height() - 20);
        leftGrip->setMinimumSize(QSize(10, 0));
        leftGrip->setCursor(Qt::SizeHorCursor);
        leftGrip->setStyleSheet("background-color: rgb(255, 121, 198);");
    } else if (gripPosition == Qt::RightEdge) {
        rightGrip = new QFrame(this);
        rightGrip->setObjectName("right");
        rightGrip->setGeometry(parentWidget->width() - 10, 0, 10, parentWidget->height());
        rightGrip->setMinimumSize(QSize(10, 0));
        rightGrip->setCursor(Qt::SizeHorCursor);
        rightGrip->setStyleSheet("background-color: rgb(255, 0, 127);");
    }

    gripContainer->setLayout(layout);
}

void CustomGrip::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
}

void CustomGrip::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
}

void CustomGrip::resizeTop(QMouseEvent *event) {
    Q_UNUSED(event);
    int delta = event->pos().y();
    int height = std::max(parentWidget->minimumHeight(), parentWidget->height() - delta);
    QRect geo = parentWidget->geometry();
    geo.setTop(geo.bottom() - height);
    parentWidget->setGeometry(geo);
}

void CustomGrip::resizeBottom(QMouseEvent *event) {
    Q_UNUSED(event);
    int delta = event->pos().y();
    int height = std::max(parentWidget->minimumHeight(), parentWidget->height() + delta);
    parentWidget->resize(parentWidget->width(), height);
}

void CustomGrip::resizeLeft(QMouseEvent *event) {
    Q_UNUSED(event);
    int delta = event->pos().x();
    int width = std::max(parentWidget->minimumWidth(), parentWidget->width() - delta);
    QRect geo = parentWidget->geometry();
    geo.setLeft(geo.right() - width);
    parentWidget->setGeometry(geo);
}

void CustomGrip::resizeRight(QMouseEvent *event) {
    Q_UNUSED(event);
    int delta = event->pos().x();
    int width = std::max(parentWidget->minimumWidth(), parentWidget->width() + delta);
    parentWidget->resize(width, parentWidget->height());
}

// Now, the implementation for the Widgets class:

void CustomGrip::top(QWidget *form) {
    containerTop = new QFrame(form);
    containerTop->setObjectName("container_top");
    containerTop->setFrameShape(QFrame::NoFrame);
    containerTop->setFrameShadow(QFrame::Raised);
    containerTop->setGeometry(0, 0, 500, 10);
    containerTop->setMinimumSize(QSize(0, 10));
    containerTop->setMaximumSize(QSize(16777215, 10));
    QHBoxLayout *topLayout = new QHBoxLayout(containerTop);
    topLayout->setSpacing(0);
    topLayout->setContentsMargins(0, 0, 0, 0);

    topLeft = new QFrame(containerTop);
    topLeft->setObjectName("top_left");
    topLeft->setFrameShape(QFrame::NoFrame);
    topLeft->setFrameShadow(QFrame::Raised);
    topLeft->setGeometry(0, 0, 10, 10);
    topLeft->setMinimumSize(QSize(10, 10));
    topLeft->setMaximumSize(QSize(10, 10));
    topLeft->setCursor(Qt::SizeFDiagCursor);
    topLeft->setStyleSheet("background-color: rgb(33, 37, 43);");

    m_top = new QFrame(containerTop);
    m_top->setObjectName("m_top");
    m_top->setFrameShape(QFrame::NoFrame);
    m_top->setFrameShadow(QFrame::Raised);
    m_top->setGeometry(10, 0, 480, 10);
    m_top->setCursor(Qt::SizeVerCursor);
    m_top->setStyleSheet("background-color: rgb(85, 255, 255);");

    topRight = new QFrame(containerTop);
    topRight->setObjectName("top_right");
    topRight->setFrameShape(QFrame::NoFrame);
    topRight->setFrameShadow(QFrame::Raised);
    topRight->setGeometry(490, 0, 10, 10);
    topRight->setMinimumSize(QSize(10, 10));
    topRight->setMaximumSize(QSize(10, 10));
    topRight->setCursor(Qt::SizeBDiagCursor);
    topRight->setStyleSheet("background-color: rgb(33, 37, 43);");

    topLayout->addWidget(topLeft);
    topLayout->addWidget(m_top);
    topLayout->addWidget(topRight);
}

void CustomGrip::bottom(QWidget *form) {
    containerBottom = new QFrame(form);
    containerBottom->setObjectName("container_bottom");
    containerBottom->setFrameShape(QFrame::NoFrame);
    containerBottom->setFrameShadow(QFrame::Raised);
    containerBottom->setGeometry(0, 0, 500, 10);
    containerBottom->setMinimumSize(QSize(0, 10));
    containerBottom->setMaximumSize(QSize(16777215, 10));
    QHBoxLayout *bottomLayout = new QHBoxLayout(containerBottom);
    bottomLayout->setSpacing(0);
    bottomLayout->setContentsMargins(0, 0, 0, 0);

    bottomLeft = new QFrame(containerBottom);
    bottomLeft->setObjectName("bottom_left");
    bottomLeft->setFrameShape(QFrame::NoFrame);
    bottomLeft->setFrameShadow(QFrame::Raised);
    bottomLeft->setGeometry(0, 0, 10, 10);
    bottomLeft->setMinimumSize(QSize(10, 10));
    bottomLeft->setMaximumSize(QSize(10, 10));
    bottomLeft->setCursor(Qt::SizeBDiagCursor);
    bottomLeft->setStyleSheet("background-color: rgb(33, 37, 43);");

    m_bottom = new QFrame(containerBottom);
    m_bottom->setObjectName("m_bottom");
    m_bottom->setFrameShape(QFrame::NoFrame);
    m_bottom->setFrameShadow(QFrame::Raised);
    m_bottom->setGeometry(10, 0, 480, 10);
    m_bottom->setCursor(Qt::SizeVerCursor);
    m_bottom->setStyleSheet("background-color: rgb(85, 170, 0);");

    bottomRight = new QFrame(containerBottom);
    bottomRight->setObjectName("bottom_right");
    bottomRight->setFrameShape(QFrame::NoFrame);
    bottomRight->setFrameShadow(QFrame::Raised);
    bottomRight->setGeometry(490, 0, 10, 10);
    bottomRight->setMinimumSize(QSize(10, 10));
    bottomRight->setMaximumSize(QSize(10, 10));
    bottomRight->setCursor(Qt::SizeFDiagCursor);
    bottomRight->setStyleSheet("background-color: rgb(33, 37, 43);");

    bottomLayout->addWidget(bottomLeft);
    bottomLayout->addWidget(m_bottom);
    bottomLayout->addWidget(bottomRight);
}

void CustomGrip::left(QWidget *form) {
    leftGrip = new QFrame(form);
    leftGrip->setObjectName("left");
    leftGrip->setGeometry(0, 10, 10, 480);
    leftGrip->setMinimumSize(QSize(10, 0));
    leftGrip->setCursor(Qt::SizeHorCursor);
    leftGrip->setStyleSheet("background-color: rgb(255, 121, 198);");
}

void CustomGrip::right(QWidget *form) {
    form->resize(500, 500);
    rightGrip = new QFrame(form);
    rightGrip->setObjectName("right");
    rightGrip->setGeometry(0, 0, 10, 500);
    rightGrip->setMinimumSize(QSize(10, 0));
    rightGrip->setCursor(Qt::SizeHorCursor);
    rightGrip->setStyleSheet("background-color: rgb(255, 0, 127);");
}

