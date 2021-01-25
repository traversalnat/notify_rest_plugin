#include "notifywindow.h"
#include "ui_notifywindow.h"
#include <QScreen>
#include <QGraphicsBlurEffect>
#include <QFont>

NotifyWindow::NotifyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NotifyWindow)
{
    ui->setupUi(this);
    ui->label->setText("   请看其它地方");
    QFont *fontSize = new QFont;
    fontSize->setPointSize(70);
    ui->label->setFont(*fontSize);

    ui->pushButton->setText("略过");
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(hideWindow()));

    /* 先截图，再使用label 显示该图片，并使 label 模糊*/
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(0);
    graphic_label = new QLabel(this);
    graphic_label->resize(this->size());
    graphic_label->setPixmap(pixmap);
    graphic_label->setScaledContents(true);
    graphic_label->lower();

    QGraphicsBlurEffect *blureffect = new QGraphicsBlurEffect;
    blureffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
    blureffect->setBlurRadius(80);
    graphic_label->setGraphicsEffect(blureffect);
}


NotifyWindow::~NotifyWindow()
{
    delete ui;
}

void NotifyWindow::resizeEvent(QResizeEvent *event)
{
    graphic_label->resize(this->size());
}

// 设置 按键事件 当空格按下时，关闭当前窗口
/*
void NotifyWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space)
        this->close();
}
*/


void NotifyWindow::hideWindow()
{
    this->close();
}

void NotifyWindow::notify(QString string)
{
    this->setWindowFlags(Qt::FramelessWindowHint  |
                         Qt::WindowSystemMenuHint |
                         Qt::WindowMinimizeButtonHint);
    this->ui->label->setText(string);
    this->showFullScreen();
}

void NotifyWindow::closeNotify()
{
    this->hideWindow();
}
