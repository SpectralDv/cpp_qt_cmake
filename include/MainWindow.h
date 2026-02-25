#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include "ModelButton.h"


namespace Ui 
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
private:
    std::map<std::string,ModelButton*> mapButton;
    Q_OBJECT;
    QPushButton *button1;
    //QTextEdit *textEdit1;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
public:
    void paintEvent(QPaintEvent *e)override;
};

#endif // MAINWINDOW_H
