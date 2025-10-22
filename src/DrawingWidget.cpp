#include "DrawingWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>

DrawingWidget::DrawingWidget(QWidget *parent) : QWidget(parent),
    currentMode(None), currentShapeType(Shape::Rectangle),
    isDrawing(false), isMoving(false), isConnecting(false),
    tempConnectionFrom(-1), movingShapeId(-1) {

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void DrawingWidget::setMode(Mode mode) {
    currentMode = mode;
    updateCursor();
}

void DrawingWidget::setShapeType(Shape::Type type) {
    currentShapeType = type;
}

void DrawingWidget::saveToFile(const QString& filename) {
    if (!m_data.saveToFile(filename)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл");
    }
}

void DrawingWidget::loadFromFile(const QString& filename) {
    if (!m_data.loadFromFile(filename)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить файл");
    }
    update();
}

void DrawingWidget::clear() {
    m_data.clear();
    update();
}

void DrawingWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(Qt::black, 2));
    drawConnections(painter);
    drawTemporaryConnection(painter);
    painter.setPen(QPen(Qt::black, 2));
    drawShapes(painter);

    painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
    painter.setBrush(Qt::NoBrush);
    drawTemporaryShape(painter);
}

void DrawingWidget::mousePressEvent(QMouseEvent* event) {
    lastMousePos = event->pos();

    if (event->button() == Qt::LeftButton) {
        switch(currentMode) {
        case DrawShape:
            startPoint = event->pos();
            tempRect = QRect(startPoint, QSize());
            isDrawing = true;

            break;

        case Connect:
            if (!isConnecting) {
                auto shape = m_data.findShapeAt(event->pos());
                if (shape) {
                    tempConnectionFrom = shape->id();
                    isConnecting = true;
                }
            } else {
                auto shape = m_data.findShapeAt(event->pos());
                if (shape && shape->id() != tempConnectionFrom) {
                    m_data.addConnection(tempConnectionFrom, shape->id());
                    isConnecting = false;
                    tempConnectionFrom = -1;
                    emit documentModified();
                } else {
                    isConnecting = false;
                    tempConnectionFrom = -1;
                }
            }
            break;

        case Move:
            { //figure brackets to deal with switch/case scope
                auto shape = m_data.findShapeAt(event->pos());
                if (shape) {
                    movingShapeId = shape->id();
                    moveStartPos = event->pos();
                    isMoving = true;
                    setCursor(Qt::ClosedHandCursor);
                }
            }
            break;

        case Delete:
            auto shape = m_data.findShapeAt(event->pos());
            if (shape) {
                m_data.removeShape(shape->id());
                emit documentModified();
            }
            break;
        }
    } else if (event->button() == Qt::RightButton) {
        cancelCurrentOperation();
    }

    update();
}

void DrawingWidget::mouseMoveEvent(QMouseEvent* event) {
    lastMousePos = event->pos();

    if (isDrawing) {
        tempRect = QRect(startPoint, event->pos()).normalized();
    } else if (isMoving) {
        auto shape = m_data.findShapeById(movingShapeId);
        if (shape) {
            QPoint delta = lastMousePos - moveStartPos;
            shape->moveBy(delta);
            moveStartPos = lastMousePos;
        }
    }

    update();
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if (isDrawing) {
            auto newShape = ShapeFactory::createShape(currentShapeType, m_data.nextShapeId(), tempRect);
            if (newShape) {
                m_data.addShape(newShape);
                m_data.setNextShapeId(m_data.nextShapeId() + 1);
                emit documentModified();
            }
            isDrawing = false;
            tempRect = QRect();
        } else if (isMoving) {
            isMoving = false;
            updateCursor();
            emit documentModified();
        }
    }
    update();
}

void DrawingWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        cancelCurrentOperation();
    }
}

void DrawingWidget::cancelCurrentOperation() {
    if (isDrawing) {
        isDrawing = false;
        tempRect = QRect();
    } else if (isConnecting) {
        isConnecting = false;
        tempConnectionFrom = -1;
    } else if (isMoving) {
        isMoving = false;
        updateCursor();
    }
    update();
}

void DrawingWidget::updateCursor() {
    switch(currentMode) {
    case Move:
        setCursor(isMoving ? Qt::ClosedHandCursor : Qt::OpenHandCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
        break;
    }
}

void DrawingWidget::drawTriangle(QPainter& painter, const QRect& rect) {
    QPoint p1(rect.left(), rect.bottom());
    QPoint p2(rect.center().x(), rect.top());
    QPoint p3(rect.right(), rect.bottom());

    QPolygon triangle;
    triangle << p1 << p2 << p3;
    painter.drawPolygon(triangle);
}

void DrawingWidget::drawConnections(QPainter& painter) {
    for (const Connection& conn : m_data.connections()) {
        auto fromShape = m_data.findShapeById(conn.fromId());
        auto toShape = m_data.findShapeById(conn.toId());
        if (fromShape && toShape) {
            painter.drawLine(fromShape->center(), toShape->center());
        }
    }
}

void DrawingWidget::drawTemporaryConnection(QPainter& painter) {
    if (isConnecting && tempConnectionFrom != -1 && !lastMousePos.isNull()) {
        auto fromShape = m_data.findShapeById(tempConnectionFrom);
        if (fromShape) {
            painter.setPen(QPen(Qt::gray, 2, Qt::DashLine));
            painter.drawLine(fromShape->center(), lastMousePos);
        }
    }
}

void DrawingWidget::drawShapes(QPainter& painter) {
    for (const auto& shape : m_data.shapes()) {
        shape->draw(painter);
    }
}

void DrawingWidget::drawTemporaryShape(QPainter& painter) {
    if (isDrawing && !tempRect.isNull()) {
        switch(currentShapeType) {
        case Shape::Rectangle:
            painter.drawRect(tempRect);
            break;
        case Shape::Triangle:
            drawTriangle(painter, tempRect);
            break;
        case Shape::Ellipse:
            painter.drawEllipse(tempRect);
            break;
        }
    }
}
