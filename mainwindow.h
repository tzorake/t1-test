#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graph.h"
#include <QMainWindow>
#include <QTextStream>
#include <QVector>
#include <QDebug>
#include <QTimer>
#include <QtMath>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void parameterChanged();

private slots:
    void onLazyChange();
    void onTextChanged();

    void onRandomMatrixButtonClicked();
    void onRefreshButtonClicked();

private:
    QVector<QVector<int>> parseMatrix(const QString &input);
    QVector<QVector<int>> generateMatrix(int size);
    QString matrixToString(const QVector<QVector<int>> &matrix);

    void createNodes(int count);
    void forceDirectedLayout(QVector<QVector<int>> adjacencyMatrix, int iterations = 100, double scale = 1.0, double coolingFactor = 0.1);

private:
    Graph *m_graph = nullptr;
    QTimer m_timer;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
