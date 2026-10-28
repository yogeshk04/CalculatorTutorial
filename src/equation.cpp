#include "equation.h"

#include <QChar>
#include <QString>
#include <algorithm>
#include <vector>

namespace
{
bool isOperatorSymbol(QChar ch)
{
    return ch == '+' || ch == '-' || ch == QChar(0x00D7) || ch == QChar(0x00F7);
}

bool isOperatorText(const QString &text)
{
    return text == "+" || text == "-" || text == QString::fromUtf8("×") || text == QString::fromUtf8("÷");
}
}

Number::Number(double value)
    : m_value(value)
{
}

QString Number::text() const
{
    return QString::number(m_value, 'g', 15);
}

double Number::value() const
{
    return m_value;
}

void Number::setValue(double value)
{
    m_value = value;
}

Operator::Operator(QChar symbol)
    : m_symbol(symbol)
{
}

QString Operator::text() const
{
    return QString(m_symbol);
}

QChar Operator::symbol() const
{
    return m_symbol;
}

Equation::Equation(QObject *parent)
    : QObject(parent)
{
}

void Equation::append(const QString &input)
{
    if (input.isEmpty())
    {
        return;
    }

    if (input == "C")
    {
        clear();
        emit changed();
        return;
    }

    if (input == "⌫" || input == "backspace")
    {
        backspace();
        emit changed();
        return;
    }

    if (input.size() == 1 && input.at(0).isDigit())
    {
        appendNumber(input.at(0));
        emit changed();
        return;
    }

    if (isOperatorText(input))
    {
        appendOperator(input.at(0));
        emit changed();
        return;
    }
}

void Equation::appendNumber(QChar digit)
{
    if (m_elements.empty() || std::dynamic_pointer_cast<Operator>(m_elements.back()))
    {
        m_elements.push_back(std::make_shared<Number>(digit.digitValue()));
        m_lastWasOperator = false;
        return;
    }

    auto current = std::dynamic_pointer_cast<Number>(m_elements.back());
    if (!current)
    {
        return;
    }

    current->setValue(current->value() * 10 + digit.digitValue());
    m_lastWasOperator = false;
}

void Equation::appendOperator(QChar symbol)
{
    if (m_elements.empty())
    {
        return;
    }

    if (std::dynamic_pointer_cast<Operator>(m_elements.back()))
    {
        m_elements.back() = std::make_shared<Operator>(symbol);
        m_lastWasOperator = true;
        return;
    }

    m_elements.push_back(std::make_shared<Operator>(symbol));
    m_lastWasOperator = true;
}

void Equation::backspace()
{
    if (m_elements.empty())
    {
        return;
    }

    if (std::dynamic_pointer_cast<Operator>(m_elements.back()))
    {
        m_elements.pop_back();
        emit changed();
        return;
    }

    auto current = std::dynamic_pointer_cast<Number>(m_elements.back());
    if (!current)
    {
        return;
    }

    const double value = current->value();
    if (value >= 10.0 || value <= -10.0)
    {
        current->setValue(std::floor(value / 10.0));
        emit changed();
        return;
    }

    m_elements.pop_back();
    emit changed();
}

void Equation::clear()
{
    m_elements.clear();
    m_lastWasOperator = false;
    emit changed();
}

void Equation::setText(const QString &text)
{
    clear();
    if (text.isEmpty())
    {
        return;
    }

    QString currentNumber;
    for (QChar ch : text)
    {
        if (ch.isDigit())
        {
            currentNumber += ch;
        }
        else if (isOperatorSymbol(ch))
        {
            if (!currentNumber.isEmpty())
            {
                bool ok = false;
                const double value = currentNumber.toDouble(&ok);
                if (ok)
                {
                    if (m_elements.empty() || std::dynamic_pointer_cast<Operator>(m_elements.back()))
                    {
                        m_elements.push_back(std::make_shared<Number>(value));
                    }
                    else
                    {
                        auto current = std::dynamic_pointer_cast<Number>(m_elements.back());
                        if (current)
                        {
                            current->setValue(value);
                        }
                    }
                }
                currentNumber.clear();
            }

            m_elements.push_back(std::make_shared<Operator>(ch));
        }
    }

    if (!currentNumber.isEmpty())
    {
        bool ok = false;
        const double value = currentNumber.toDouble(&ok);
        if (ok)
        {
            if (m_elements.empty() || std::dynamic_pointer_cast<Operator>(m_elements.back()))
            {
                m_elements.push_back(std::make_shared<Number>(value));
            }
            else
            {
                auto current = std::dynamic_pointer_cast<Number>(m_elements.back());
                if (current)
                {
                    current->setValue(value);
                }
            }
        }
    }
}

QString Equation::text() const
{
    if (m_elements.empty())
    {
        return "0";
    }

    QString output;
    for (const auto &element : m_elements)
    {
        output += element->text();
    }
    return output;
}

bool Equation::empty() const
{
    return m_elements.empty();
}

double Equation::calculate(bool &ok) const
{
    ok = false;

    if (m_elements.empty())
    {
        return 0.0;
    }

    std::vector<double> numbers;
    std::vector<QChar> operators;

    for (const auto &element : m_elements)
    {
        if (auto number = std::dynamic_pointer_cast<Number>(element))
        {
            numbers.push_back(number->value());
        }
        else if (auto op = std::dynamic_pointer_cast<Operator>(element))
        {
            operators.push_back(op->symbol());
        }
    }

    if (numbers.size() != operators.size() + 1)
    {
        return 0.0;
    }

    std::vector<double> values = numbers;
    std::vector<QChar> ops = operators;

    for (std::size_t i = 0; i < ops.size();)
    {
        if (ops[i] == QChar(0x00D7) || ops[i] == QChar(0x00F7))
        {
            if (ops[i] == QChar(0x00F7) && values[i + 1] == 0.0)
            {
                return 0.0;
            }

            if (ops[i] == QChar(0x00D7))
            {
                values[i] *= values[i + 1];
            }
            else
            {
                values[i] /= values[i + 1];
            }

            values.erase(values.begin() + static_cast<int>(i + 1));
            ops.erase(ops.begin() + static_cast<int>(i));
        }
        else
        {
            ++i;
        }
    }

    double result = values.front();
    for (std::size_t i = 0; i < ops.size(); ++i)
    {
        const QChar op = ops[i];
        const double next = values[i + 1];

        if (op == '+')
        {
            result += next;
        }
        else if (op == '-')
        {
            result -= next;
        }
        else
        {
            return 0.0;
        }
    }

    ok = true;
    return result;
}

QString Equation::calculatedText(bool &ok) const
{
    const double result = calculate(ok);
    if (!ok)
    {
        return "Error";
    }

    return QString::number(result, 'g', 15);
}
