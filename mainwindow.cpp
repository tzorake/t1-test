#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtConcurrent>
#include <QFuture>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Force Directed Layout");

    m_timer.setSingleShot(true);

    connect(ui->edit, &QPlainTextEdit::textChanged, this, &MainWindow::onLazyChange);
    connect(&m_timer, &QTimer::timeout, this, &MainWindow::onTextChanged);

    connect(ui->randomMatrixButton, &QPushButton::clicked, this, &MainWindow::onRandomMatrixButtonClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLazyChange()
{
    if (m_timer.isActive()) {
        m_timer.stop();
    }
    m_timer.start(600);
}

void MainWindow::onTextChanged()
{
    qDebug() << "MainWindow::textChanged()";
    auto matrix = parseMatrix(ui->edit->toPlainText());

    if (!matrix.isEmpty()) {
        QFuture<void> future = QtConcurrent::run([=]() {
            return forceDirectedLayout(matrix, ui->iterationsEdit->text().toInt(), ui->scaleEdit->text().toDouble(), ui->coolingFactorEdit->text().toDouble());
        });

        QFutureWatcher<void> watcher;
        watcher.setFuture(future);

        QEventLoop loop;

        connect(&watcher, &QFutureWatcher<QVector<QPoint>>::finished, this, [&]() {
            loop.exit();
        });

        loop.exec();
    }
}

void MainWindow::onRandomMatrixButtonClicked()
{
    bool ok;
    int size;

    do {
        size = QInputDialog::getInt(nullptr, "Enter a number", "Please enter a positive integer:", 3, 1, 100, 1, &ok);

        if (ok & (size <= 0 || size > 100)) {
            QMessageBox::warning(nullptr, "Warning", "Please enter a positive integer.");
        }
    } while (size <= 0 || size > 100);

    if (ok) {
        auto matrix = generateMatrix(size);
        auto string = matrixToString(matrix);

        ui->edit->setPlainText(string);
    }
}

void MainWindow::onRefreshButtonClicked()
{
    onTextChanged();
}

QVector<QVector<int>> MainWindow::parseMatrix(const QString &input)
{
    if (input.isEmpty()) {
        qDebug() << "Error: Passed string is empty.";
        return QVector<QVector<int>>();
    }

    QRegularExpression allowedChars("[01 ]*");
    QRegularExpressionMatch match = allowedChars.match(input);
    if (!match.hasMatch()) {
        qDebug() << "String contains invalid characters.";
        return QVector<QVector<int>>();
    }

    QVector<QVector<int>> matrix;

    QStringList rows = input.trimmed().split('\n');

    int rowCount = rows.size();
    if (rowCount == 0) {
        qDebug() << "Error: Empty matrix.";
        return QVector<QVector<int>>();
    }

    int columnCount = rows[0].trimmed().split(' ').size();

    if (rowCount != columnCount) {
        qDebug() << "Error: Not a square matrix.";
        return QVector<QVector<int>>();
    }

    for (int i = 0; i < rows.size(); ++i) {
        QVector<int> rowData;

        QStringList elements = rows[i].trimmed().split(' ');

        if (elements.size() != columnCount) {
            qDebug() << "Error: Rows have different lengths.";
            return QVector<QVector<int>>();
        }

        for (int j = 0; j < elements.size(); ++j) {
            auto element = elements[j].toInt();
            qDebug() << i << j << element;

            if (i == j && element == 1) {
                qDebug() << "Error: 1 is not allowed on the main diagonal.";
                return QVector<QVector<int>>();
            }

            rowData.append(element);
        }

        matrix.append(rowData);
    }

    return matrix;
}

QVector<QVector<int> > MainWindow::generateMatrix(int size)
{
    QVector<QVector<int>> matrix(size, QVector<int>(size, 0));

    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            matrix[i][j] = QRandomGenerator::global()->bounded(2);
            matrix[j][i] = matrix[i][j];
        }
    }

    return matrix;
}

QString MainWindow::matrixToString(const QVector<QVector<int>> &matrix)
{
    QString result;

    for (const QVector<int> &row : matrix) {
        for (int value : row) {
            result += QString::number(value) + " ";
        }
        result.chop(1);
        result += "\n";
    }

    result.chop(1);

    return result;
}

void MainWindow::forceDirectedLayout(QVector<QVector<int>> adjacencyMatrix, int iterations, double scale, double coolingFactor)
{
    ui->view->setGraph(nullptr);

    if (m_graph) {
        delete m_graph;
    }
    m_graph = new Graph(adjacencyMatrix);
    m_graph->setSpringConstant(ui->springConstantEdit->value());
    m_graph->setRepulsiveConstant(ui->repulsiveConstantEdit->value());
    m_graph->setCircleSize(ui->circleSizeEdit->value());
    m_graph->setLineWidth(ui->lineWidthEdit->value());
    m_graph->setLabelFontSize(ui->labelFontSizeEdit->value());
    m_graph->init();

    ui->view->setGraph(m_graph);
    m_graph->forceDirectedLayout(iterations, scale, coolingFactor);
}

