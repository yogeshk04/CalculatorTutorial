#include "display.h"

#include <QLabel>
#include <QVBoxLayout>

Display::Display(QWidget *parent)
    : QWidget(parent)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(4);
}

void Display::setHistory(const std::deque<std::shared_ptr<Equation>> &history)
{
    while (QLayoutItem *item = m_layout->takeAt(0))
    {
        if (QWidget *widget = item->widget())
        {
            widget->deleteLater();
        }
        delete item;
    }

    if (history.empty())
    {
        auto *label = new QLabel("No history yet", this);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        label->setStyleSheet("color: gray; font-size: 16px;");
        m_layout->addWidget(label);
        return;
    }

    for (const auto &equation : history)
    {
        if (!equation)
        {
            continue;
        }

        auto *label = new QLabel(equation->text(), this);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        label->setStyleSheet("font-size: 18px; color: #1f1f1f; background: #f3f3f3; border: 1px solid #d9d9d9; padding: 4px 6px;");
        m_layout->addWidget(label);
    }
}
