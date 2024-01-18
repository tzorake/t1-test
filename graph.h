#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

class Graph
{
public:
    Graph(const QVector<QVector<int>> &adjacencyMatrix);
    ~Graph();

    void init();
    void forceDirectedLayout(int iterations, double scale, double coolingFactor);

    QVector<QVector2D> positions() const;
    QVector<QLineF> edges() const;

    double springConstant() const;
    void setSpringConstant(double springConstant);

    double repulsiveConstant() const;
    void setRepulsiveConstant(double repulsiveConstant);

    int circleSize() const;
    void setCircleSize(int circleSize);

    int lineWidth() const;
    void setLineWidth(int lineWidth);

    int labelFontSize() const;
    void setLabelFontSize(int labelFontSize);

private:
    void applySpringForce(Node *node);
    void applyRepulsiveForce(Node *node);
    void updateNodePositions(float scale);

    QVector<QVector<int>> m_adjacencyMatrix;
    QVector<Node *> m_nodes;

    double m_springConstant;
    double m_repulsiveConstant;

    int m_circleSize;
    int m_lineWidth;
    int m_labelFontSize;
};

#endif // GRAPH_H
