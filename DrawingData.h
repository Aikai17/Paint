#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Shape.h"
#include "Connection.h"
#include <QList>
#include <QSharedPointer>

class DrawingData {
public:
    DrawingData();

    // Управление фигурами
    void addShape(QSharedPointer<Shape> shape);
    void removeShape(int id);
    QSharedPointer<Shape> findShapeById(int id) const;
    QSharedPointer<Shape> findShapeAt(const QPoint& point) const;
    const QList<QSharedPointer<Shape>>& shapes() const { return m_shapes; }

    // Управление связями
    void addConnection(int fromId, int toId);
    void removeConnectionsForShape(int id);
    const QList<Connection>& connections() const { return m_connections; }

    // Сериализация
    bool saveToFile(const QString& filename) const;
    bool loadFromFile(const QString& filename);
    void clear();

    int nextShapeId() const { return m_nextShapeId; }
    void setNextShapeId(int id) { m_nextShapeId = id; }

private:
    QList<QSharedPointer<Shape>> m_shapes;
    QList<Connection> m_connections;
    int m_nextShapeId;
};

#endif // DOCUMENT_H
