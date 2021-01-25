#ifndef INFORMATIONWIDGET_H
#define INFORMATIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QStorageInfo>
#include "notifywindow.h"

class InformationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InformationWidget(QWidget *parent = nullptr);
	// timeCount 计时器操作函数
	bool timeCountIsDisable();
	void timeCountStateSwitched();
	void resetTimeCount();


private slots:
    // 用于更新数据的槽函数
    void refreshInfo();

private:
    // 真正的数据显示在这个 Label 上
    QLabel *m_infoLabel;
    // 处理时间间隔的计时器
    QTimer *m_refreshTimer;
	enum class WorkState {work, rest, longrest};
	// 是否启用该插件 (菜单中关闭)
	bool is_used;
	// 当前工作状态
	WorkState work_state;
	// 倒数计数
	int time_count;
	// 工作次数计数 (第三次工作结束后进入长休息)
	int work_count;
	// time_count 工作状态
	bool timeCountState;

    // Notify window
    NotifyWindow *notifyWindow;
};

#endif // INFORMATIONWIDGET_H
