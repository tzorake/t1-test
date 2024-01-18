#ifndef GRAPHVISUALIZER_H
#define GRAPHVISUALIZER_H

#include "graph.h"
#include <QApplication>
#include <QFrame>
#include <QVector>
#include <QRandomGenerator>
#include <QHBoxLayout>
#include <QWheelEvent>
#include <QtMath>
#include <QDebug>
#include <QPainter>

class CanvasView : public QFrame
{
    Q_OBJECT

public:
    CanvasView(QWidget *parent = nullptr);

    void setGraph(const Graph *graph);

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    const Graph *m_graph = nullptr;

    QPoint m_translation;
    QPoint m_lastMousePos;
    bool m_translationEnabled;

    double m_zoomFactor = 1.0;

    void drawGraph(QPainter &painter);
};

#endif // GRAPHVISUALIZER_H
