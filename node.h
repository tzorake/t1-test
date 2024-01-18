#ifndef NODE_H
#define NODE_H

#include <QVector2D>
#include <QVector>

class Node {
public:
    QVector2D position;
    QVector2D velocity;
    QVector<Node *> neighbors;
};

#endif // NODE_H
