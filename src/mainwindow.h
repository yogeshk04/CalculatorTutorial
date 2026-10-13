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

class QLabel;
class QPushButton;

// Q_OBJECT is required on any class that wants to use signals & slots.
// It marks the class for Qt's "moc" (meta-object compiler) code
// generator, which CMake runs automatically for us (CMAKE_AUTOMOC).
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    // One slot handles every digit/operator button: each button "knows"
    // its own text, so we just read button->text() and append it.
    void appendButtonText();
    void clearDisplay();
    void backspace();
    void calculateResult();

private:
    void buildUi();
    QPushButton *addButton(const QString &text, int row, int column, int columnSpan = 1);

    QLabel *m_display = nullptr;
    // True right after "=" is pressed: lets the next button decide
    // whether to start a fresh number (digit) or keep building onto the
    // result (operator), like a real calculator does.
    bool m_justEvaluated = false;
};

#endif // MAINWINDOW_H
