// Phase 2: First Qt Window.
// MainWindow is a QWidget subclass: it *is* the window. It owns the
// QLabel display and all the buttons, and wires button clicks to slots
// that edit the display's text directly.
//
// There is no proper "math brain" class yet (that's Phase 3's Equation
// class) - clicking a digit or operator just appends its character to
// the label. "=" does do a quick, left-to-right calculation (reusing
// Phase 1's style of logic) so the calculator is actually useful while
// we wait for Phase 3/4 to replace it with the real Equation-based
// model.
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "display.h"
#include "equation.h"
#include "equationqueue.h"

class QLabel;
class QPushButton;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void appendButtonText();
    void clearDisplay();
    void backspace();
    void calculateResult();

private:
    void buildUi();
    QPushButton *addButton(const QString &text, int row, int column, int columnSpan = 1);

    QLabel *m_display = nullptr;
    Display *m_historyDisplay = nullptr;
    Equation m_equation;
    EquationQueue m_history;
    bool m_justEvaluated = false;
};

#endif // MAINWINDOW_H
