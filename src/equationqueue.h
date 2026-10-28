#ifndef EQUATIONQUEUE_H
#define EQUATIONQUEUE_H

#include <deque>
#include <memory>

#include "equation.h"

class EquationQueue
{
public:
    void append(const QString &equationText);
    void append(std::shared_ptr<Equation> equation);
    void clear();
    const std::deque<std::shared_ptr<Equation>> &entries() const;
    bool empty() const;

private:
    std::deque<std::shared_ptr<Equation>> m_entries;
};

#endif // EQUATIONQUEUE_H
