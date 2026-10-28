#include "mainwindow.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

namespace
{
const QChar kMultiply(0x00D7);
const QChar kDivide(0x00F7);
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("CalculatorWithHistory");
    buildUi();
    connect(&m_equation, &Equation::changed, this, [this]() {
        m_display->setText(m_equation.text());
    });
    m_display->setText("0");
}

void MainWindow::buildUi()
{
    m_display = new QLabel("0", this);
    m_display->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_display->setMinimumHeight(52);
    m_display->setStyleSheet("font-size: 28px; background: white; border: 1px solid gray;");

    m_historyDisplay = new Display(this);
    m_historyDisplay->setMinimumHeight(120);

    auto *grid = new QGridLayout(this);
    grid->addWidget(m_display, 0, 0, 1, 4);
    grid->addWidget(m_historyDisplay, 1, 0, 2, 4);

    addButton("7", 3, 0);
    addButton("8", 3, 1);
    addButton("9", 3, 2);
    addButton(QString::fromUtf8("÷"), 3, 3);

    addButton("4", 4, 0);
    addButton("5", 4, 1);
    addButton("6", 4, 2);
    addButton(QString::fromUtf8("×"), 4, 3);

    addButton("1", 5, 0);
    addButton("2", 5, 1);
    addButton("3", 5, 2);
    addButton("-", 5, 3);

    QPushButton *clearButton = addButton("C", 6, 0);
    addButton("0", 6, 1);
    QPushButton *equalsButton = addButton("=", 6, 2);
    addButton("+", 6, 3);

    QPushButton *backspaceButton = addButton(QString::fromUtf8("⌫"), 7, 0, 4);

    disconnect(clearButton, &QPushButton::clicked, this, &MainWindow::appendButtonText);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearDisplay);

    disconnect(backspaceButton, &QPushButton::clicked, this, &MainWindow::appendButtonText);
    connect(backspaceButton, &QPushButton::clicked, this, &MainWindow::backspace);

    disconnect(equalsButton, &QPushButton::clicked, this, &MainWindow::appendButtonText);
    connect(equalsButton, &QPushButton::clicked, this, &MainWindow::calculateResult);
}

QPushButton *MainWindow::addButton(const QString &text, int row, int column, int columnSpan)
{
    auto *button = new QPushButton(text, this);
    button->setMinimumSize(56, 48);
    button->setStyleSheet("font-size: 18px;");

    auto *grid = qobject_cast<QGridLayout *>(layout());
    grid->addWidget(button, row, column, 1, columnSpan);
    connect(button, &QPushButton::clicked, this, &MainWindow::appendButtonText);
    return button;
}

void MainWindow::appendButtonText()
{
    auto *button = qobject_cast<QPushButton *>(sender());
    if (!button)
    {
        return;
    }

    const QString buttonText = button->text();
    if (m_justEvaluated && !buttonText.isEmpty() && buttonText.at(0).isDigit())
    {
        m_equation.clear();
        m_justEvaluated = false;
    }

    if (m_justEvaluated && (buttonText == "+" || buttonText == "-" || buttonText == QString::fromUtf8("×") || buttonText == QString::fromUtf8("÷")))
    {
        m_justEvaluated = false;
    }

    m_equation.append(buttonText);
}

void MainWindow::clearDisplay()
{
    m_justEvaluated = false;
    m_equation.clear();
}

void MainWindow::backspace()
{
    m_justEvaluated = false;
    m_equation.backspace();
}

void MainWindow::calculateResult()
{
    if (m_equation.empty())
    {
        return;
    }

    bool ok = false;
    const double result = m_equation.calculate(ok);
    if (!ok)
    {
        m_display->setText("Error");
        m_justEvaluated = true;
        return;
    }

    const QString equationText = m_equation.text();
    const QString computedText = QString::number(result, 'g', 15);
    m_history.append(equationText + "=" + computedText);
    m_historyDisplay->setHistory(m_history.entries());

    m_equation.clear();
    m_equation.setText(computedText);
    m_justEvaluated = true;
}
