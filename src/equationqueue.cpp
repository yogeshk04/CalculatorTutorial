#include "equationqueue.h"

void EquationQueue::append(const QString &equationText)
{
    auto equation = std::make_shared<Equation>();
    equation->setText(equationText);
    append(equation);
}

void EquationQueue::append(std::shared_ptr<Equation> equation)
{
    if (!equation)
    {
        return;
    }

    m_entries.push_back(equation);
    if (m_entries.size() > 12)
    {
        m_entries.pop_front();
    }
}

void EquationQueue::clear()
{
    m_entries.clear();
}

const std::deque<std::shared_ptr<Equation>> &EquationQueue::entries() const
{
    return m_entries;
}

bool EquationQueue::empty() const
{
    return m_entries.empty();
}
