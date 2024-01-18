#ifndef DOUBLELINEEDIT_H
#define DOUBLELINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class DoubleLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    DoubleLineEdit(QWidget *parent = nullptr);
    DoubleLineEdit(double min, double max, double defaultValue, QWidget *parent = nullptr);

    double value() const;
    void setValue(double value);

    double min() const;
    void setMin(double min);

    double max() const;
    void setMax(double max);

    double defaultValue() const;
    void setDefaultValue(double defaultValue);

signals:
    void valueChanged(double newValue);

private slots:
    void onTextChanged();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void init();

    void setupUi();

    double m_min = 0.0;
    double m_max = std::numeric_limits<double>::max();
    double m_defaultValue = 0.0;
};

#endif // DOUBLELINEEDIT_H
