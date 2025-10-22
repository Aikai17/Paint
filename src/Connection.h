#ifndef CONNECTION_H
#define CONNECTION_H

class Connection {
public:
    Connection();
    Connection(int from, int to);

    int fromId() const;
    int toId() const;
private:
    int m_fromId;
    int m_toId;
};

#endif // CONNECTION_H
