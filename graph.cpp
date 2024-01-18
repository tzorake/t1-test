#include "graph.h"
#include <QLineF>
#include <QRandomGenerator>

Graph::Graph(const QVector<QVector<int> > &adjacencyMatrix)
{
    m_adjacencyMatrix = adjacencyMatrix;
}

Graph::~Graph()
{
    while (!m_nodes.isEmpty()) {
        delete m_nodes.takeAt(0);
    }
}

void Graph::applySpringForce(Node *node)
{
    for (Node *neighboringNode : node->neighbors) {
        QVector2D displacement = node->position - neighboringNode->position;
        QVector2D springForce = springConstant() * displacement;
        node->velocity -= springForce;
        neighboringNode->velocity += springForce;
    }
}

void Graph::applyRepulsiveForce(Node *node)
{
    for (Node *otherNode : m_nodes) {
        if (node != otherNode) {
            QVector2D displacement = node->position - otherNode->position;
            float distance = displacement.length();
            float repulsiveForce = repulsiveConstant() / distance;
            QVector2D unitVector = displacement.normalized();
            node->velocity += unitVector * repulsiveForce;
        }
    }
}

void Graph::updateNodePositions(float scale)
{
    for (Node *node : m_nodes) {
        node->position += node->velocity * scale;
    }
}

void Graph::init()
{
    while (!m_nodes.isEmpty()) {
        delete m_nodes.takeAt(0);
    }

    for (int i = 0; i < m_adjacencyMatrix.size(); ++i) {
        m_nodes.append(new Node);
    }

    for (Node *node : m_nodes) {
        node->position = QVector2D(
            QRandomGenerator::global()->generateDouble(),
            QRandomGenerator::global()->generateDouble()
        );
        node->velocity = QVector2D(0, 0);
    }

    for (int i = 0; i < m_nodes.size(); ++i) {
        m_nodes[i]->neighbors.clear();
    }

    for (int i = 0; i < m_adjacencyMatrix.size(); ++i) {
        for (int j = 0; j < m_adjacencyMatrix.size(); ++j) {
            if (m_adjacencyMatrix[i][j] == 1) {
                m_nodes[i]->neighbors.append(m_nodes[j]);
            }
        }
    }
}

void Graph::forceDirectedLayout(int iterations, double scale, double coolingFactor)
{
    for (int iteration = 0; iteration < iterations; ++iteration) {
        for (Node *node : m_nodes) {
            applySpringForce(node);
            applyRepulsiveForce(node);
        }

        updateNodePositions(scale);
        scale *= coolingFactor;
    }
}

QVector<QVector2D> Graph::positions() const
{
    QVector<QVector2D> ps;
    std::transform(m_nodes.begin(), m_nodes.end(), std::back_inserter(ps), [](const Node *node) {
       return node->position;
    });

    return ps;
}

QVector<QLineF> Graph::edges() const
{
    QVector<QLineF> edges;

    for (Node *node : m_nodes) {
        for (Node *neighbor : node->neighbors) {
            if (!edges.contains(QLineF(node->position.toPointF(), neighbor->position.toPointF())) &&
                !edges.contains(QLineF(neighbor->position.toPointF(), node->position.toPointF()))) {
                edges.append(QLineF(node->position.toPointF(), neighbor->position.toPointF()));
            }
        }
    }

    return edges;
}

double Graph::springConstant() const
{
    return m_springConstant;
}

void Graph::setSpringConstant(double springConstant)
{
    m_springConstant = springConstant;
}

double Graph::repulsiveConstant() const
{
    return m_repulsiveConstant;
}

void Graph::setRepulsiveConstant(double repulsiveConstant)
{
    m_repulsiveConstant = repulsiveConstant;
}

int Graph::circleSize() const
{
    return m_circleSize;
}

void Graph::setCircleSize(int circleSize)
{
    m_circleSize = circleSize;
}

int Graph::lineWidth() const
{
    return m_lineWidth;
}

void Graph::setLineWidth(int lineWidth)
{
    m_lineWidth = lineWidth;
}

int Graph::labelFontSize() const
{
    return m_labelFontSize;
}

void Graph::setLabelFontSize(int labelFontSize)
{
    m_labelFontSize = labelFontSize;
}


