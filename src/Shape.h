#ifndef SHAPE_H
#define SHAPE_H

#include <QRect>
#include <QPoint>
#include <QPainter>
#include <QScopedPointer>

class Shape {
public:
    enum Type { Rectangle, Triangle, Ellipse };

    virtual ~Shape() = default;

    virtual void draw(QPainter& painter) const = 0;
    virtual bool contains(const QPoint& point) const = 0;
    virtual Type type() const = 0;

    QPoint center() const;
    QRect boundingRect() const;
    int id() const;
    void setRect(const QRect& rect);
    void moveBy(const QPoint& delta);

protected:
    Shape(int id, const QRect& rect);
    int m_id;
    QRect m_rect;
};

class RectangleShape : public Shape {
public:
    RectangleShape(int id, const QRect& rect);
    void draw(QPainter& painter) const override;
    bool contains(const QPoint& point) const override;
    Type type() const override;
};

class TriangleShape : public Shape {
public:
    TriangleShape(int id, const QRect& rect);
    void draw(QPainter& painter) const override;
    bool contains(const QPoint& point) const override;
    Type type() const override;
};

class EllipseShape : public Shape {
public:
    EllipseShape(int id, const QRect& rect);
    void draw(QPainter& painter) const override;
    bool contains(const QPoint& point) const override;
    Type type() const override;
};

// Фабрика для создания фигур
namespace ShapeFactory {
    QSharedPointer<Shape> createShape(Shape::Type type, int id, const QRect& rect);
}

#endif // SHAPE_H
