#include "Connection.h"

Connection::Connection() : m_fromId(-1), m_toId(-1) {}

Connection::Connection(int from, int to) : m_fromId(from), m_toId(to) {}

int Connection::fromId() const{
    return m_fromId;
}
int Connection::toId() const{
    return m_toId;
}
