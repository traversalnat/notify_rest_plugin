#ifndef NOTIFYWINDOW_H
#define NOTIFYWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QKeyEvent>

namespace Ui {
class NotifyWindow;
}

class NotifyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NotifyWindow(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event);
    void notify(QString string);
    void closeNotify();
    ~NotifyWindow();

public slots:
    void hideWindow();

    /*
protected:
    void keyPressEvent(QKeyEvent *event);
    */

private:
    Ui::NotifyWindow *ui;
    QLabel *graphic_label;
};

#endif // NOTIFYWINDOW_H
