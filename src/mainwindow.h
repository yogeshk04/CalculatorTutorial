// Phase 2: First Qt Window.
// MainWindow is a QWidget subclass: it *is* the window. It owns the
// QLabel display and all the buttons, and wires button clicks to slots
// that edit the display's text directly.
//
// There is no "math brain" yet (that's Phase 3's Equation class) -
// clicking a digit just appends its character to the label, clicking an
// operator appends the operator, and "=" / "C" are the only buttons that
// do anything special right now.
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

private:
    void buildUi();
    QPushButton *addButton(const QString &text, int row, int column);

    QLabel *m_display = nullptr;
};

#endif // MAINWINDOW_H
