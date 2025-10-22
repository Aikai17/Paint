#include "Shape.h"

Shape::Shape(int id, const QRect &rect)
    : m_id(id), m_rect(rect)
{

}
QPoint Shape::center() const {
    return m_rect.center();
}
QRect Shape::boundingRect() const {
    return m_rect;
}
int Shape::id() const {
    return m_id;
}
void Shape::setRect(const QRect &rect) {
    m_rect = rect;
}
void Shape::moveBy(const QPoint &delta) {
    m_rect.translate(delta);
}


RectangleShape::RectangleShape(int id, const QRect &rect) :
    Shape(id, rect) {

}
void RectangleShape::draw(QPainter &painter) const {
    painter.drawRect(m_rect);
}
bool RectangleShape::contains(const QPoint &point) const {
    return m_rect.contains(point);
}
Shape::Type RectangleShape::type() const {
    return Rectangle;
}


TriangleShape::TriangleShape(int id, const QRect &rect) : Shape(id, rect) {

}
void TriangleShape::draw(QPainter &painter) const {
    QPoint p1(m_rect.left(), m_rect.bottom());
    QPoint p2(m_rect.center().x(), m_rect.top());
    QPoint p3(m_rect.right(), m_rect.bottom());

    QPolygon triangle;
    triangle << p1 << p2 << p3;
    painter.drawPolygon(triangle);
}
bool TriangleShape::contains(const QPoint &point) const {
    QPoint p1(m_rect.left(), m_rect.bottom());
    QPoint p2(m_rect.center().x(), m_rect.top());
    QPoint p3(m_rect.right(), m_rect.bottom());

    float area = 0.5 * (-p2.y() * p3.x() + p1.y() * (-p2.x() + p3.x()) +
                        p1.x() * (p2.y() - p3.y()) + p2.x() * p3.y());

    if (area == 0) return false;

    float s = 1 / (2 * area) * (p1.y() * p3.x() - p1.x() * p3.y() +
                                (p3.y() - p1.y()) * point.x() + (p1.x() - p3.x()) * point.y());
    float t = 1 / (2 * area) * (p1.x() * p2.y() - p1.y() * p2.x() +
                                (p1.y() - p2.y()) * point.x() + (p2.x() - p1.x()) * point.y());

    return s > 0 && t > 0 && (1 - s - t) > 0;
}
Shape::Type TriangleShape::type() const {
    return Triangle;
}

EllipseShape::EllipseShape(int id, const QRect &rect) : Shape(id, rect) {}

void EllipseShape::draw(QPainter &painter) const {
    painter.drawEllipse(m_rect);
}

bool EllipseShape::contains(const QPoint &point) const {
    QPoint center = m_rect.center();
    float rx = m_rect.width() / 2.0;
    float ry = m_rect.height() / 2.0;

    if (rx == 0 || ry == 0) return false;

    float dx = (point.x() - center.x()) / rx;
    float dy = (point.y() - center.y()) / ry;

    return (dx * dx + dy * dy) <= 1.0;
}

Shape::Type EllipseShape::type() const { return Ellipse; }

QSharedPointer<Shape> ShapeFactory::createShape(Shape::Type type, int id, const QRect& rect) {
    switch(type) {
    case Shape::Rectangle:
        return QSharedPointer<Shape>(new RectangleShape(id, rect));
    case Shape::Triangle:
        return QSharedPointer<Shape>(new TriangleShape(id, rect));
    case Shape::Ellipse:
        return QSharedPointer<Shape>(new EllipseShape(id, rect));
    }
    return nullptr;
}
