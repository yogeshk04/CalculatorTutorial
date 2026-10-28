#ifndef EQUATION_H
#define EQUATION_H

#include <QObject>
#include <QString>
#include <memory>
#include <vector>

class Element
{
public:
    virtual ~Element() = default;
    virtual QString text() const = 0;
};

class Number : public Element
{
public:
    explicit Number(double value = 0.0);

    QString text() const override;
    double value() const;
    void setValue(double value);

private:
    double m_value = 0.0;
};

class Operator : public Element
{
public:
    explicit Operator(QChar symbol);

    QString text() const override;
    QChar symbol() const;

private:
    QChar m_symbol;
};

class Equation : public QObject
{
    Q_OBJECT

public:
    explicit Equation(QObject *parent = nullptr);

    void append(const QString &input);
    void backspace();
    void clear();
    void setText(const QString &text);
    QString text() const;
    bool empty() const;
    double calculate(bool &ok) const;
    QString calculatedText(bool &ok) const;

signals:
    void changed();

private:
    std::vector<std::shared_ptr<Element>> m_elements;
    bool m_lastWasOperator = false;

    void appendNumber(QChar digit);
    void appendOperator(QChar symbol);
};

#endif // EQUATION_H
