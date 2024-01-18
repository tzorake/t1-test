#include "doublelineedit.h"

DoubleLineEdit::DoubleLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    init();
}

double DoubleLineEdit::value() const
{
    return text().toDouble();
}

void DoubleLineEdit::setValue(double value)
{
    setText(QString::number(value));
}

double DoubleLineEdit::min() const
{
    return m_min;
}

void DoubleLineEdit::setMin(double min)
{
    m_min = min;
}

double DoubleLineEdit::max() const
{
    return m_max;
}

void DoubleLineEdit::setMax(double max)
{
    m_max = max;
}

double DoubleLineEdit::defaultValue() const
{
    return m_defaultValue;
}

void DoubleLineEdit::setDefaultValue(double defaultValue)
{
    m_defaultValue = defaultValue;
}

void DoubleLineEdit::onTextChanged()
{
    QString text = this->text();
    bool ok;
    double value = text.toDouble(&ok);

    if (ok && value >= m_min && value <= m_max) {
        emit valueChanged(value);
    } else {
        setText(QString::number(m_defaultValue));
        emit valueChanged(m_defaultValue);
    }
}

void DoubleLineEdit::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key()== Qt::Key_Return) {
        clearFocus();
    } else {
        QLineEdit::keyPressEvent(event);
    }
}

void DoubleLineEdit::init()
{
    setupUi();
}

void DoubleLineEdit::setupUi()
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    connect(this, &QLineEdit::editingFinished, this, &DoubleLineEdit::onTextChanged);
}
