#include "mainwindow.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("CalculatorWithHistory");
    buildUi();
}

void MainWindow::buildUi()
{
    // The display just shows whatever text we've built up so far.
    m_display = new QLabel("0", this);
    m_display->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_display->setMinimumHeight(48);
    m_display->setStyleSheet("font-size: 28px; background: white; border: 1px solid gray;");

    // QGridLayout arranges widgets in rows/columns, like a spreadsheet,
    // so we don't have to calculate pixel positions ourselves.
    auto *grid = new QGridLayout(this);
    grid->addWidget(m_display, 0, 0, 1, 4); // span all 4 columns

    // row, column of each button on the keypad.
    addButton("7", 1, 0);
    addButton("8", 1, 1);
    addButton("9", 1, 2);
    addButton(QString::fromUtf8("\xc3\xb7"), 1, 3); // ÷

    addButton("4", 2, 0);
    addButton("5", 2, 1);
    addButton("6", 2, 2);
    addButton(QString::fromUtf8("\xc3\x97"), 2, 3); // ×

    addButton("1", 3, 0);
    addButton("2", 3, 1);
    addButton("3", 3, 2);
    addButton("-", 3, 3);

    QPushButton *clearButton = addButton("C", 4, 0);
    addButton("0", 4, 1);
    addButton("=", 4, 2);
    addButton("+", 4, 3);

    // "C" is the only button that behaves differently from the rest.
    disconnect(clearButton, &QPushButton::clicked, this, &MainWindow::appendButtonText);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearDisplay);
}

QPushButton *MainWindow::addButton(const QString &text, int row, int column)
{
    auto *button = new QPushButton(text, this);
    button->setMinimumSize(56, 48);
    button->setStyleSheet("font-size: 18px;");

    auto *grid = qobject_cast<QGridLayout *>(layout());
    grid->addWidget(button, row, column);

    // Signals & slots: "when this button is clicked, call appendButtonText".
    // Every digit/operator button connects here; addButton() for "C"
    // reconnects it to clearDisplay() instead.
    connect(button, &QPushButton::clicked, this, &MainWindow::appendButtonText);

    return button;
}

void MainWindow::appendButtonText()
{
    // sender() tells us which button was actually clicked, since every
    // button is wired to this same slot.
    auto *button = qobject_cast<QPushButton *>(sender());
    if (!button)
    {
        return;
    }

    if (m_display->text() == "0")
    {
        m_display->setText(button->text());
    }
    else
    {
        m_display->setText(m_display->text() + button->text());
    }
}

void MainWindow::clearDisplay()
{
    m_display->setText("0");
}
