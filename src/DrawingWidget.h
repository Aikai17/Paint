#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include "DrawingData.h"
#include <QWidget>

class DrawingWidget : public QWidget {
    Q_OBJECT

public:
    enum Mode { None, DrawShape, Connect, Move, Delete };

    DrawingWidget(QWidget *parent = nullptr);

    void setMode(Mode mode);
    void setShapeType(Shape::Type type);
    DrawingData* document() { return &m_data; }
    const DrawingData* document() const { return &m_data; }

    void saveToFile(const QString& filename);
    void loadFromFile(const QString& filename);
    void clear();

signals:
    void documentModified();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void drawConnections(QPainter& painter);
    void drawTemporaryConnection(QPainter& painter);
    void drawShapes(QPainter& painter);
    void drawTemporaryShape(QPainter& painter);

    DrawingData m_data;
    Mode currentMode;
    Shape::Type currentShapeType;

    bool isDrawing;
    bool isMoving;
    bool isConnecting;

    QPoint moveStartPos;
    QPoint startPoint;
    QRect tempRect;
    int tempConnectionFrom;
    int movingShapeId;
    QPoint lastMousePos;

    void cancelCurrentOperation();
    void updateCursor();
    void drawTriangle(QPainter& painter, const QRect& rect);
};

#endif // DRAWINGWIDGET_H
