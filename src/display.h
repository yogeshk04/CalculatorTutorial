#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>
#include <deque>
#include <memory>

#include "equation.h"

class QVBoxLayout;

class Display : public QWidget
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = nullptr);

    void setHistory(const std::deque<std::shared_ptr<Equation>> &history);

private:
    QVBoxLayout *m_layout = nullptr;
};

#endif // DISPLAY_H
