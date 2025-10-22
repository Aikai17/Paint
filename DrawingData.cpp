#include "DrawingData.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>

DrawingData::DrawingData() : m_nextShapeId(1) {}

void DrawingData::addShape(QSharedPointer<Shape> shape) {
    m_shapes.append(shape);
}

void DrawingData::removeShape(int id) {
    removeConnectionsForShape(id);

    for (int i = m_shapes.size() - 1; i >= 0; --i) {
        if (m_shapes[i]->id() == id) {
            m_shapes.removeAt(i);
            break;
        }
    }
}

QSharedPointer<Shape> DrawingData::findShapeById(int id) const {
    for (const auto& shape : m_shapes) {
        if (shape->id() == id) {
            return shape;
        }
    }
    return nullptr;
}

QSharedPointer<Shape> DrawingData::findShapeAt(const QPoint& point) const {
    for (const auto& shape : m_shapes) {
        if (shape->contains(point)) {
            return shape;
        }
    }
    return nullptr;
}

void DrawingData::addConnection(int fromId, int toId) {
    m_connections.append(Connection(fromId, toId));
}

void DrawingData::removeConnectionsForShape(int id) {
    for (int i = m_connections.size() - 1; i >= 0; --i) {
        if (m_connections[i].fromId() == id || m_connections[i].toId() == id) {
            m_connections.removeAt(i);
        }
    }
}

bool DrawingData::saveToFile(const QString& filename) const {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QDataStream out(&file);

    out << m_shapes.count();
    for (const auto& shape : m_shapes) {
        out << static_cast<int>(shape->type()) << shape->boundingRect() << shape->id();
    }

    out << m_connections.count();
    for (const Connection& conn : m_connections) {
        out << conn.fromId() << conn.toId();
    }

    out << m_nextShapeId;
    file.close();
    return true;
}

bool DrawingData::loadFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream in(&file);

    m_shapes.clear();
    int shapeCount;
    in >> shapeCount;
    for (int i = 0; i < shapeCount; ++i) {
        int type, id;
        QRect rect;
        in >> type >> rect >> id;
        auto newShape = ShapeFactory::createShape(static_cast<Shape::Type>(type), id, rect);
        if (newShape) {
            m_shapes.append(newShape);
        }
    }

    m_connections.clear();
    int connCount;
    in >> connCount;
    for (int i = 0; i < connCount; ++i) {
        int fromId, toId;
        in >> fromId >> toId;
        m_connections.append(Connection(fromId, toId));
    }

    in >> m_nextShapeId;
    file.close();
    return true;
}

void DrawingData::clear() {
    m_shapes.clear();
    m_connections.clear();
    m_nextShapeId = 1;
}
