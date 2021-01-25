#include "homemonitorplugin.h"
#include <QListWidget>

HomeMonitorPlugin::HomeMonitorPlugin(QObject *parent)
    : QObject(parent)
{

}

const QString HomeMonitorPlugin::pluginDisplayName() const
{
    return QString("Notify Rest");
}

const QString HomeMonitorPlugin::pluginName() const
{
    return QStringLiteral("notify_rest");
}

void HomeMonitorPlugin::init(PluginProxyInterface *proxyInter)
{
    m_proxyInter = proxyInter;

    m_pluginWidget = new InformationWidget;
    m_tipsWidget = new QLabel;
    m_appletWidget = new QLabel;

    // 如果插件没有被禁用则在初始化插件时才添加主控件到面板上
    if (!pluginIsDisable()) {
        m_proxyInter ->itemAdded(this, pluginName());
    }

	// 打开 m_pluginWidget 的 timeCount 计时器
	if (m_pluginWidget->timeCountIsDisable()) {
		m_pluginWidget->timeCountStateSwitched();
	}
}

QWidget *HomeMonitorPlugin::itemWidget(const QString &itemKey)
{
    Q_UNUSED(itemKey);

    return m_pluginWidget;
}

QWidget *HomeMonitorPlugin::itemTipsWidget(const QString &itemKey)
{
	// 鼠标放在上方时的操作
    Q_UNUSED(itemKey);
	QString text = QString(m_pluginWidget->timeCountIsDisable()?"暂停计时":"正在计时");
    m_tipsWidget->setText(text);
    return m_tipsWidget;
}

QWidget *HomeMonitorPlugin::itemPopupApplet(const QString &itemKey)
{
	// 鼠标左键点击弹出的操作
    Q_UNUSED(itemKey);
	QListWidget *listWidget = new QListWidget(m_pluginWidget);

	if (m_pluginWidget->timeCountIsDisable()) {
		listWidget->addItem(QString("开启计时器"));
	} else {
		listWidget->addItem(QString("暂停计时器"));
	}
    return nullptr;
}

bool HomeMonitorPlugin::pluginIsAllowDisable()
{
    // 告诉 dde-dock 本插件允许禁用
    return true;
}

bool HomeMonitorPlugin::pluginIsDisable()
{
    // 第二个参数 “disabled” 表示存储这个值的键（所有配置都是以键值对的方式存储的）
    // 第三个参数表示默认值，即默认不禁用
    return m_proxyInter->getValue(this, "disabled", false).toBool();
}

void HomeMonitorPlugin::pluginStateSwitched()
{
    // 获取当前禁用状态的反值作为新的状态值
    const bool disabledNew = !pluginIsDisable();
    // 存储新的状态值
    m_proxyInter->saveValue(this, "disabled", disabledNew);

    // 根据新的禁用状态值处理主控件的加载和卸载
    if (disabledNew) {
        m_proxyInter->itemRemoved(this, pluginName());
    } else {
        m_proxyInter->itemAdded(this, pluginName());
    }

	// 在pluginState 关闭时关闭 m_pluginWidget 的 timeCount 计时器
	if (disabledNew != m_pluginWidget->timeCountIsDisable())
		m_pluginWidget->timeCountStateSwitched();
}

const QString HomeMonitorPlugin::itemContextMenu(const QString &itemKey)
{
	// 菜单栏, 右键点击的操作
    Q_UNUSED(itemKey);

    QList<QVariant> items;
    items.reserve(2);

    QMap<QString, QVariant> switchTimeCountState;
    switchTimeCountState["itemId"] = "switchTimeCountState";
	if (m_pluginWidget->timeCountIsDisable()) {
		switchTimeCountState["itemText"] = ("开启计时器");
	} else {
		switchTimeCountState["itemText"] = ("暂停计时器");
	}
    switchTimeCountState["isActive"] = true;
    items.push_back(switchTimeCountState);

    QMap<QString, QVariant> resetTimeCount;
    resetTimeCount["itemId"] = "resetTimeCount";
    resetTimeCount["itemText"] = ("重置计时器");
    resetTimeCount["isActive"] = true;
    items.push_back(resetTimeCount);


    QMap<QString, QVariant> menu;
    menu["items"] = items;
    menu["checkableMenu"] = false;
    menu["singleCheck"] = false;

    // 返回 JSON 格式的菜单数据
    return QJsonDocument::fromVariant(menu).toJson();
}

void HomeMonitorPlugin::invokedMenuItem(const QString &itemKey,
                            const QString &menuId, const bool checked)
{
    Q_UNUSED(itemKey);

    // 根据上面接口设置的 id 执行不同的操作
    if (menuId == "switchTimeCountState") {
		m_pluginWidget->timeCountStateSwitched();
    } else if (menuId == "resetTimeCount") {
		m_pluginWidget->resetTimeCount();
	}
}
