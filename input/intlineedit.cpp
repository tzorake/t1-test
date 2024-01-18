#include "intlineedit.h"

IntLineEdit::IntLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    init();
}

int IntLineEdit::value() const
{
    return text().toInt();
}

void IntLineEdit::setValue(int value)
{
    setText(QString::number(value));
}

int IntLineEdit::min() const
{
    return m_min;
}

void IntLineEdit::setMin(int min)
{
    m_min = min;
}

int IntLineEdit::max() const
{
    return m_max;
}

void IntLineEdit::setMax(int max)
{
    m_max = max;
}

int IntLineEdit::defaultValue() const
{
    return m_defaultValue;
}

void IntLineEdit::setDefaultValue(int defaultValue)
{
    m_defaultValue = defaultValue;
}

void IntLineEdit::onTextChanged()
{
    QString text = this->text();
    bool ok;
    int value = text.toInt(&ok);

    if (ok && value >= m_min && value <= m_max) {
        emit valueChanged(value);
    } else {
        setText(QString::number(m_defaultValue));
        emit valueChanged(m_defaultValue);
    }
}

void IntLineEdit::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key()== Qt::Key_Return) {
        clearFocus();
    } else {
        QLineEdit::keyPressEvent(event);
    }
}

void IntLineEdit::init()
{
    setupUi();
}

void IntLineEdit::setupUi()
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    connect(this, &QLineEdit::editingFinished, this, &IntLineEdit::onTextChanged);
}
