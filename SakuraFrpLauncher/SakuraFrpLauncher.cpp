#include "SakuraFrpLauncher.h"

#include <QMenu>
#include <QLineEdit>
#include <QBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>

#include "SFLNetworkMgr.h"
#include "SFLDBMgr.h"
#include "SFLLoadingDlg.h"
#include "SFLGlobalMgr.h"
#include "SFLJsonHelper.h"
#include "SFLGroupTabWidget.h"

SakuraFrpLauncher::SakuraFrpLauncher(QWidget *parent)
    : SFLDialogBase(parent),
    m_system_tray_icon(nullptr),
    m_line_edit(nullptr),
    m_group_tab_widget(nullptr)
{
    this->setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    QString version = "";
    QSqlDatabase db = SFLDBMgr::GetInstance()->GetSqlConn();
    SFLDBMgr::GetInstance()->GetValueByKey(db, "sfl_version", version);
    SFLDBMgr::GetInstance()->GiveBackSqlConn(db);
    this->setWindowTitle(QStringLiteral("SakuraFrpLauncher ") + version);
    this->setMinimumSize(750, 400);

    // 初始化登录
    QWidget* login_widget = InitLoginWidget();
    m_group_tab_widget = new SFLGroupTabWidget(this);

    QVBoxLayout* v_layout = new QVBoxLayout(this);
    v_layout->addWidget(login_widget);
    v_layout->addWidget(m_group_tab_widget);
    this->setLayout(v_layout);

    // 初始化托盘图标
    InitTrayIcon();

    // 初始化加载框
    SFLGlobalMgr::GetInstance()->SetLoadingDlg(new SFLLoadingDlg());

    SFLGlobalMgr::GetInstance()->SetLauncher(this);
}

SakuraFrpLauncher::~SakuraFrpLauncher()
{

}

void SakuraFrpLauncher::InitTrayIcon(
) {
    // 创建系统托盘图标
    QMenu* tray_menu = new QMenu(this);
    QAction* open_action = new QAction(this);
    open_action->setText(QStringLiteral("打开"));
    connect(open_action, &QAction::triggered, this, &SakuraFrpLauncher::OnOpenAction);
    QAction* quit_action = new QAction(this);
    quit_action->setText(QStringLiteral("退出"));
    connect(quit_action, &QAction::triggered, this, &SakuraFrpLauncher::OnCloseAction);
    tray_menu->addAction(open_action);
    tray_menu->addAction(quit_action);

    m_system_tray_icon = new QSystemTrayIcon(this);
    m_system_tray_icon->setToolTip(QStringLiteral("Sakura Frp客户端"));
    m_system_tray_icon->setIcon(QIcon(":/Resources/images/icon.ico"));
    connect(m_system_tray_icon, &QSystemTrayIcon::activated, this, &SakuraFrpLauncher::OnActivated);
    connect(m_system_tray_icon, &QSystemTrayIcon::messageClicked, this, &SakuraFrpLauncher::showNormal);
    m_system_tray_icon->setContextMenu(tray_menu);
    m_system_tray_icon->show();
}

void SakuraFrpLauncher::OnActivated(
    QSystemTrayIcon::ActivationReason reason
) {
    switch (reason) {
        case QSystemTrayIcon::Unknown: {
            break;
        }
        case QSystemTrayIcon::Context: {
            break;
        }
        case QSystemTrayIcon::DoubleClick: {
            this->showNormal();
            break;
        }
        case QSystemTrayIcon::Trigger: {
            this->showNormal();
            break;
        }
        case QSystemTrayIcon::MiddleClick: {
            break;
        }
        default: {
            break;
        }
    }
}

void SakuraFrpLauncher::OnOpenAction(
) {
    this->showNormal();
}

void SakuraFrpLauncher::OnCloseAction(
) {
    this->close();
}

void SakuraFrpLauncher::changeEvent(
    QEvent* e
) {
    // 最小化到托盘
    if (this->windowState() == Qt::WindowMinimized) {
        this->hide();
    }
}

void SakuraFrpLauncher::closeEvent(
    QCloseEvent* e
) {
    SFLDialogBase::closeEvent(e);
}

QWidget* SakuraFrpLauncher::InitLoginWidget(
) {
    QWidget* login_widget = new QWidget(this);
    QHBoxLayout* login_widget_h_layout = new QHBoxLayout(login_widget);
    login_widget_h_layout->setContentsMargins(QMargins(0, 0, 0, 0));
    m_line_edit = new QLineEdit(login_widget);
    m_line_edit->setPlaceholderText(QStringLiteral("请输入登录密钥"));
    m_line_edit->setEchoMode(QLineEdit::Password);

    QPushButton* login_btn = new QPushButton(login_widget);
    login_btn->setText(QStringLiteral("登录"));
    connect(login_btn, &QPushButton::clicked, this, &SakuraFrpLauncher::OnLoginBtnClicked);

    m_tray_message_check_box = new QCheckBox(login_widget);
    m_tray_message_check_box->setText(QStringLiteral("托盘通知"));
    m_tray_message_check_box->setChecked(true);

    login_widget_h_layout->addWidget(m_line_edit);
    login_widget_h_layout->addWidget(login_btn);
    login_widget_h_layout->addStretch();
    login_widget_h_layout->addWidget(m_tray_message_check_box);

    login_widget->setLayout(login_widget_h_layout);

    // 读取存储的token
    QString token = "";
    QSqlDatabase db = SFLDBMgr::GetInstance()->GetSqlConn();
    SFLDBMgr::GetInstance()->GetValueByKey(db, "sfl_token", token);
    SFLDBMgr::GetInstance()->GiveBackSqlConn(db);
    m_line_edit->setText(token);

    return login_widget;
}

void SakuraFrpLauncher::OnLoginBtnClicked(
) {
    QString sfl_token = m_line_edit->text();
    if (sfl_token.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("密钥不能为空"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    SFLLoadingDlg* login_dlg = SFLGlobalMgr::GetInstance()->LoadingDlg();
    login_dlg->SetText(QStringLiteral("正在登录..."));
    login_dlg->show();

    QString node_url = sakura_frp_domain + uri_get_nodes;
    node_url += "token=" + sfl_token;
    QString node_retsult = "";
    SFLNetworkMgr().GetData(node_url, node_retsult, 20000);

    // 进行json解析
    NodeInfo node_info;
    bool node_suc = SFLJsonHelper().ParseNodesStringToStruct(node_retsult, node_info);
    if (!node_suc) {
        login_dlg->hide();
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("网络或服务器错误，请稍后重试"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (!node_info.success) {
        login_dlg->hide();
        QMessageBox::warning(this, QStringLiteral("提示"), node_info.message, QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    SFLGlobalMgr::GetInstance()->SetNodeInfo(node_info);

    // token入库
    QSqlDatabase db = SFLDBMgr::GetInstance()->GetSqlConn();
    SFLDBMgr::GetInstance()->UpdateValueByKey(db, "sfl_token", sfl_token);
    SFLDBMgr::GetInstance()->GiveBackSqlConn(db);

    // 获取隧道列表
    login_dlg->SetText(QStringLiteral("登录成功，正在获取隧道列表..."));

    QString tunnel_url = sakura_frp_domain + uri_get_tunnels;
    tunnel_url += "token=" + sfl_token;
    QString tunnel_retsult = "";
    SFLNetworkMgr().GetData(tunnel_url, tunnel_retsult, 20000);

    login_dlg->hide();

    // 进行json解析
    TunnelInfo tunnel_info;
    bool tunnel_suc = SFLJsonHelper().ParseTunnelsStringToStruct(tunnel_retsult, tunnel_info);
    if (!tunnel_suc) {
        QMessageBox::warning(this, QStringLiteral("提示"), QStringLiteral("网络或服务器错误，请稍后重试"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (!tunnel_info.success) {
        QMessageBox::warning(this, QStringLiteral("提示"), tunnel_info.message, QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    SFLGlobalMgr::GetInstance()->SetTunnelInfo(tunnel_info);

    // 初始化隧道和组数据库
    InitTunnelsGroup(node_info.node_item_info_list, tunnel_info.tunnel_item_info_list);

    // 初始化隧道组tab
    m_group_tab_widget->InitGroupTabWidget();
}

void SakuraFrpLauncher::InitTunnelsGroup(
    QVector<NodeItemInfo> node_item_info_list,
    QVector<TunnelItemInfo> tunnel_item_info_list
) {
    for (int i = 0; i < tunnel_item_info_list.size(); ++i) {
        tunnel_item_info_list[i].tunnel_index = i;
		for (auto node_item_info : node_item_info_list) {
			if (node_item_info.node_id == tunnel_item_info_list[i].node_item_info.node_id) {
				tunnel_item_info_list[i].node_item_info.node_name = node_item_info.node_name;
                tunnel_item_info_list[i].node_item_info.node_accept_new = node_item_info.node_accept_new;
			}
		}
    }
	QSqlDatabase db = SFLDBMgr::GetInstance()->GetSqlConn();
    SFLDBMgr::GetInstance()->BeginTransaction(db);
    SFLDBMgr::GetInstance()->DeleteTunnel(db);
    SFLDBMgr::GetInstance()->InsertTunnel(db, tunnel_item_info_list);
    SFLDBMgr::GetInstance()->EndTransaction(db);
    SFLDBMgr::GetInstance()->GiveBackSqlConn(db);
}

void SakuraFrpLauncher::ShowTrayMessage(
    const TunnelProcess& tunnel_process
) {
    if (!m_tray_message_check_box->isChecked()) {
        return;
    }

    QString title = "";
    QSystemTrayIcon::MessageIcon icon;
    if (e_running_state_warnning == tunnel_process.running_state) {
        title = QStringLiteral("警告");
        icon = QSystemTrayIcon::Warning;
    } else if (e_running_state_error == tunnel_process.running_state) {
        title = QStringLiteral("错误");
        icon = QSystemTrayIcon::Critical;
    } else {
        return;
    }

    QString message = "";
    message += QString::number(tunnel_process.tunnel_item_info.tunnel_id) + " " + tunnel_process.tunnel_item_info.tunnel_name + " " + title;
    m_system_tray_icon->showMessage(title, message, icon);
}