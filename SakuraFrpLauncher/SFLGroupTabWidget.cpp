#include "SFLGroupTabWidget.h"

#include "SFLTunnelTableWidget.h"
#include "SFLDBMgr.h"

SFLGroupTabWidget::SFLGroupTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
}

SFLGroupTabWidget::~SFLGroupTabWidget()
{

}

void SFLGroupTabWidget::InitGroupTabWidget(
) {
    // ��ǰ�������Ľڵ��б�
    QSqlDatabase db = SFLDBMgr::GetInstance()->GetSqlConn();
    QVector<NodeItemInfo> server_node_item_info_list;
    SFLDBMgr::GetInstance()->GetNodeInfoList(db, server_node_item_info_list);
    SFLDBMgr::GetInstance()->GiveBackSqlConn(db);

    // ɾ���Ѿ���ʹ�ýڵ���Ϣ
    QVector<NodeItemInfo> local_node_item_info_list;
    for (int i = this->count() - 1; i >= 0; --i) {
        SFLTunnelTableWidget* tunnel_table_widget = dynamic_cast<SFLTunnelTableWidget*>(this->widget(i));
        if (nullptr != tunnel_table_widget) {
            NodeItemInfo node_item_info;
            tunnel_table_widget->GetNodeItemInfo(node_item_info);

            // �жϵ�ǰ�Ƿ�������ʾ�ڵ��б�
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
                // �ڵ��Ѿ�������
                // ���Ƴ�tab
                this->removeTab(i);
                // �����
                tunnel_table_widget->TerminateAllProcess();
                delete tunnel_table_widget;
                tunnel_table_widget = nullptr;
            } else {
                local_node_item_info_list.push_back(new_node_item_info);
                // �����б�
                tunnel_table_widget->InitTunnelTableWidget(new_node_item_info);
            }
        }
    }

    // �����½ڵ�
    for (auto server_node_item_info : server_node_item_info_list) {
        bool exists = false;
        for (auto local_node_item_info : local_node_item_info_list) {
            if (server_node_item_info.node_id == local_node_item_info.node_id) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            SFLTunnelTableWidget* tunnel_table_widget = new SFLTunnelTableWidget(this);
            tunnel_table_widget->InitTunnelTableWidget(server_node_item_info);
            this->addTab(tunnel_table_widget, server_node_item_info.node_name);
        }
    }
}