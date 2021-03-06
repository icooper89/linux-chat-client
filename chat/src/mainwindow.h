#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


    void addToDisplay(QString text);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;

private slots:
    void on_send_returnPressed();
    void on_sendButton_clicked();
    void on_actionOptions_triggered();
    void on_actionExit_triggered();
    void on_actionConnect_triggered();
};

#endif // MAINWINDOW_H
