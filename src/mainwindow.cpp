#include "mainwindow.h"

#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QPushButton>

namespace
{
// Same two Unicode operator characters used on the × and ÷ buttons.
const QChar kMultiply(0x00D7);
const QChar kDivide(0x00F7);

bool isOperatorChar(QChar c)
{
    return c == '+' || c == '-' || c == kMultiply || c == kDivide;
}

// Reimplemented from Phase 1's console calculator: given two numbers and
// an operator, return the result. `ok` is set to false for an unknown
// operator or divide-by-zero, same idea as Phase 1.
double calculate(double first, QChar op, double second, bool &ok)
{
    ok = true;

    if (op == '+')
    {
        return first + second;
    }
    else if (op == '-')
    {
        return first - second;
    }
    else if (op == kMultiply)
    {
        return first * second;
    }
    else if (op == kDivide)
    {
        if (second == 0.0)
        {
            ok = false;
            return 0.0;
        }
        return first / second;
    }

    ok = false; // not one of + - × ÷
    return 0.0;
}
} // namespace

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
    QPushButton *equalsButton = addButton("=", 4, 2);
    addButton("+", 4, 3);

    // Backspace gets its own row, spanning the full width of the keypad.
    QPushButton *backspaceButton = addButton(QString::fromUtf8("\xe2\x8c\xab"), 5, 0, 4); // ⌫

    // "C", "⌫" and "=" each behave differently from the plain
    // digit/operator buttons, so reconnect them to their own slots.
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

    // Signals & slots: "when this button is clicked, call appendButtonText".
    // Every digit/operator button connects here; buildUi() reconnects
    // "C", "⌫" and "=" to their own slots instead.
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

    const QString buttonText = button->text();

    if (m_justEvaluated)
    {
        m_justEvaluated = false;

        if (!buttonText.isEmpty() && buttonText.at(0).isDigit())
        {
            // Digit right after "=": start a brand new number instead of
            // continuing to type onto the old result, like a real
            // calculator does.
            m_display->setText(buttonText);
            return;
        }
        // Operator right after "=": fall through and keep building a new
        // equation starting from the result already on screen.
    }

    if (m_display->text() == "0")
    {
        m_display->setText(buttonText);
    }
    else
    {
        m_display->setText(m_display->text() + buttonText);
    }
}

void MainWindow::clearDisplay()
{
    m_justEvaluated = false;
    m_display->setText("0");
}

void MainWindow::backspace()
{
    m_justEvaluated = false;

    QString text = m_display->text();
    if (text.size() <= 1)
    {
        m_display->setText("0");
        return;
    }

    text.chop(1); // drop the single most-recently-typed digit/operator
    m_display->setText(text);
}

void MainWindow::calculateResult()
{
    const QString text = m_display->text();

    // Tokenize the typed text into numbers and operators, e.g. "12+8-3"
    // becomes numbers=[12, 8, 3], operators=['+', '-'].
    QList<double> numbers;
    QList<QChar> operators;
    QString currentNumber;

    for (QChar ch : text)
    {
        if (ch.isDigit())
        {
            currentNumber += ch;
        }
        else if (isOperatorChar(ch))
        {
            if (currentNumber.isEmpty())
            {
                return; // e.g. text is just "+" so far - nothing to do yet
            }
            numbers.append(currentNumber.toDouble());
            operators.append(ch);
            currentNumber.clear();
        }
    }

    if (currentNumber.isEmpty())
    {
        return; // equation ends in an operator, e.g. "8+" - nothing to do yet
    }
    numbers.append(currentNumber.toDouble());

    // Evaluate strictly left-to-right. This simple version doesn't know
    // about × / ÷ having higher precedence than + / - yet - teaching
    // that correctly is Phase 3's job, once Equation exists.
    double result = numbers.first();
    for (int i = 0; i < operators.size(); ++i)
    {
        bool ok = false;
        result = calculate(result, operators.at(i), numbers.at(i + 1), ok);

        if (!ok)
        {
            m_display->setText(operators.at(i) == kDivide ? "Error: ÷0" : "Error");
            m_justEvaluated = true;
            return;
        }
    }

    // 'g' with 15 significant digits mirrors Phase 1's
    // std::setprecision(15): full double precision without switching to
    // scientific notation for everyday-sized numbers.
    m_display->setText(QString::number(result, 'g', 15));
    m_justEvaluated = true;
}
