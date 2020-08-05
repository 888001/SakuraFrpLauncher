﻿#include "SFLGroupTabWidget.h"

#include "SFLTunnelWidget.h"
#include "SFLDBMgr.h"

SFLGroupTabWidget::SFLGroupTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
}

SFLGroupTabWidget::~SFLGroupTabWidget()
{

}

void SFLGroupTabWidget::InitTabWidget(
) {
    // 当前服务器的节点列表
    QSqlDatabase db = SFLDBMgr::GetInstance()->GetSqlConn();
    QVector<NodeItemInfo> server_node_item_info_list;
    SFLDBMgr::GetInstance()->GetNodeInfoList(db, server_node_item_info_list);
    SFLDBMgr::GetInstance()->GiveBackSqlConn(db);

    // 删除已经不使用节点信息
    QVector<NodeItemInfo> local_node_item_info_list;
    for (int i = this->count() - 1; i >= 0; --i) {
        SFLTunnelWidget* tunnel_widget = dynamic_cast<SFLTunnelWidget*>(this->widget(i));
        if (nullptr != tunnel_widget) {
            NodeItemInfo node_item_info;
            tunnel_widget->GetNodeItemInfo(node_item_info);

            // 判断当前是否还有已显示节点列表
            bool exists = false;
            NodeItemInfo new_node_item_info;
            for (auto server_node_item_info : server_node_item_info_list) {
                if (server_node_item_info.node_id == node_item_info.node_id) {
                    new_node_item_info = server_node_item_info;
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                // 节点已经不存在
                // 先移除tab
                this->removeTab(i);
                // 再清空
                tunnel_widget->TerminateAllProcess();
                delete tunnel_widget;
                tunnel_widget = nullptr;
            } else {
                local_node_item_info_list.push_back(new_node_item_info);
                // 更新列表
                tunnel_widget->InitTunnelWidget(new_node_item_info);
            }
        }
    }

    // 创建新节点
    for (auto server_node_item_info : server_node_item_info_list) {
        bool exists = false;
        for (auto local_node_item_info : local_node_item_info_list) {
            if (server_node_item_info.node_id == local_node_item_info.node_id) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            SFLTunnelWidget* tunnel_widget = new SFLTunnelWidget(this);
            tunnel_widget->InitTunnelWidget(server_node_item_info);
            this->addTab(tunnel_widget, server_node_item_info.node_name);
        }
    }
}