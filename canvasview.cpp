#include "canvasview.h"
#include <QtAlgorithms>

CanvasView::CanvasView(QWidget *parent)
    : QFrame(parent)
{
    setFrameShape(QFrame::Shape::Box);
}

void CanvasView::setGraph(const Graph *graph)
{
    m_graph = graph;

    update();
}

void CanvasView::drawGraph(QPainter &painter)
{
    if (m_graph) {
        auto edges = m_graph->edges();

        for (int i = 0; i < edges.size(); ++i) {
            auto edge = edges[i];
            auto p1 = edge.p1();
            auto p2 = edge.p2();

            painter.save();
            painter.setPen(QPen(Qt::gray, m_graph->lineWidth()));
            painter.drawLine(
                QPointF(p1.x() * width(), p1.y() * height()),
                QPointF(p2.x() * width(), p2.y() * height())
            );
            painter.restore();
        }

        auto positions = m_graph->positions();

        for (int i = 0; i < positions.size(); ++i) {
            auto position = positions[i] * QVector2D(width(), height());

            painter.save();
            painter.setPen(QPen(Qt::black, m_graph->lineWidth()));
            painter.setBrush(QBrush(Qt::blue));
            painter.translate(position.x(), position.y());
            painter.drawEllipse(-m_graph->circleSize() / 2, -m_graph->circleSize() / 2, m_graph->circleSize(), m_graph->circleSize());
            painter.restore();

            auto font = this->font();
            font.setPointSize(m_graph->labelFontSize());

            painter.save();
            painter.translate(position.x(), position.y());
            painter.setFont(font);
            painter.drawText(m_graph->circleSize() / 2, -m_graph->circleSize() / 2, QString::number(i));
            painter.restore();
        }
    }

    update();
}

void CanvasView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing, true);

    painter.scale(m_zoomFactor, m_zoomFactor);
    painter.translate(m_translation / m_zoomFactor);

    drawGraph(painter);
}

void CanvasView::wheelEvent(QWheelEvent *event)
{
    double scaleStep = 0.1;

    if (event->angleDelta().y() > 0) {
        m_zoomFactor *= (1.0 + scaleStep);
    } else {
        m_zoomFactor /= (1.0 + scaleStep);
    }

    m_zoomFactor = qMax(0.1, qMin(m_zoomFactor, 3.0));

    update();
}

void CanvasView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_translationEnabled = true;
        m_lastMousePos = event->pos();
    }
}

void CanvasView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_translationEnabled) {
        m_translation += event->pos() - m_lastMousePos;
        m_lastMousePos = event->pos();

        update();
    }
}

void CanvasView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_translationEnabled = false;
    }
}
