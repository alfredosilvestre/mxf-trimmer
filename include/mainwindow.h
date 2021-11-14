#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>

namespace Ui { class MainWindow; class Dialog; }

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    public:
        explicit MainWindow(QWidget* parent = 0);
        ~MainWindow();
        void closeEvent(QCloseEvent* event);

    private:
        Ui::MainWindow* ui;

        QShortcut* spaceShortcut;
        QShortcut* iShortcut;
        QShortcut* oShortcut;
        QShortcut* jShortcut;
        QShortcut* kShortcut;
        QShortcut* lShortcut;
        QShortcut* leftShortcut;
        QShortcut* rightShortcut;
        QShortcut* shiftoShortcut;
        QShortcut* shiftiShortcut;
        QShortcut* qShortcut;
        QShortcut* wShortcut;
        QShortcut* homeShortcut;
        QShortcut* endShortcut;

    private:
        void initShortcuts();
        void initTrimShortcuts();
        void disableShortcuts();
        void disableTrimShortcuts();
};

#endif // MAINWINDOW_H
