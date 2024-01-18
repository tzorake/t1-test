#ifndef INTLINEEDIT_H
#define INTLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class IntLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    IntLineEdit(QWidget *parent = nullptr);
    IntLineEdit(int min, int max, int defaultValue, QWidget *parent = nullptr);

    int value() const;
    void setValue(int value);

    int min() const;
    void setMin(int min);

    int max() const;
    void setMax(int max);

    int defaultValue() const;
    void setDefaultValue(int defaultValue);

signals:
    void valueChanged(int newValue);

private slots:
    void onTextChanged();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void init();

    void setupUi();

    int m_min = 0;
    int m_max = std::numeric_limits<int>::max();
    int m_defaultValue = 0;
};

#endif // INTLINEEDIT_H
