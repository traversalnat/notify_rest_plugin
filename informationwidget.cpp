#include "informationwidget.h"
#include "homemonitorplugin.h"

#include <QVBoxLayout>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include "notifywindow.h"

#define REFRESH_COUNT_IN_ONE_MINITE 3
#define TIME_WORK (20 * REFRESH_COUNT_IN_ONE_MINITE) // 单位: 20s, 表示工作时间为 20 * 3 * 20s
#define TIME_REST 1		// 单位: 20s, 表示休息时间为 1 * 20s
#define TIME_LONGREST (5 * REFRESH_COUNT_IN_ONE_MINITE) // 单位: 20s, 表示一次长休息时间为 5 * 3 * 20s
#define WORKS_TO_GET_LONGREST 3

InformationWidget::InformationWidget(QWidget *parent)
    : QWidget(parent)
    , m_infoLabel(new QLabel)
    , m_refreshTimer(new QTimer(this))
	, work_state(WorkState::work)
	, time_count(TIME_WORK)
	, work_count(0)
	, timeCountState(false)
    , notifyWindow(new NotifyWindow)
{
    m_infoLabel->setStyleSheet("QLabel {"
                               "color: black;"
                               "}");
    m_infoLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_infoLabel);
    centralLayout->setSpacing(0);
    centralLayout->setMargin(0);

    setLayout(centralLayout);

    // 连接 Timer 超时的信号到更新数据的槽上
    connect(m_refreshTimer, &QTimer::timeout, this, &InformationWidget::refreshInfo);

    // 设置 Timer 超时为 20s，即每 20s 更新一次控件上的数据，并启动这个定时器
    m_refreshTimer->start(20 * 1000);

    refreshInfo();
}

void InformationWidget::refreshInfo()
{
	if (timeCountIsDisable()) 
		return;
	// 每20 s 刷新一次, 工作时间为 20 * 3 个 20s 即 20 分钟
	// 短休息时间为 1 个 20s
	// 长休息时间为 15 个 20s (一小时后)
	switch (work_state) {
	case WorkState::work:
		// 显示当前时间
        m_infoLabel->setText(QString("%1分钟")
                    .arg(static_cast<int>(time_count / REFRESH_COUNT_IN_ONE_MINITE)));
		time_count -= 1;
		if (time_count == 0) {
			work_count += 1;
			// 根据work_count 判断是否进入长休息
			if (work_count == WORKS_TO_GET_LONGREST) {
				work_count = 0;
				work_state = WorkState::longrest;
				time_count = TIME_LONGREST;
                //system("notify-send Notify 工作久了,休息5分钟");
                notifyWindow->notify("工作久了, 休息5分钟");
			} else {
				work_state = WorkState::rest;
				time_count = TIME_REST;
                //system("notify-send Notify 休息20s");
                notifyWindow->notify("   请看其它地方");
			}
		}
		break;
	case WorkState::rest:
	case WorkState::longrest:
		time_count -= 1;
		if (time_count == 0) {
            // 休息结束,关闭窗口
            //system("notify-send Notify 休息结束");
            notifyWindow->closeNotify();
			time_count = TIME_WORK;
			work_state = WorkState::work;
            m_infoLabel->setText(QString("%1分钟")
                    .arg(static_cast<int>(time_count / REFRESH_COUNT_IN_ONE_MINITE)));
        }
		break;
	}
}

bool InformationWidget::timeCountIsDisable()
{
	return timeCountState;
}

void InformationWidget::timeCountStateSwitched()
{
	timeCountState = !timeCountIsDisable();
}

void InformationWidget::resetTimeCount() {
	// 重置计时器
	// 工作状态重置为 工作, 计时器重置, 工作次数重置
	time_count = TIME_WORK;
	work_state = WorkState::work;
	work_count = 0;
	refreshInfo();
}
