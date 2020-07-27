#include "SFLMsgBox.h"

#include <QApplication>
#include <QDialogButtonBox>

SFLMsgBox* SFLMsgBox::m_msg_box_instance = nullptr;

SFLMsgBox::SFLMsgBox()
    : QMessageBox()
{
    QDialogButtonBox* btn_boxs = this->findChild<QDialogButtonBox*>();
    if (nullptr != btn_boxs) {
        btn_boxs->setCenterButtons(true);
    }
    ResetItems();
}

SFLMsgBox::~SFLMsgBox(
) {
}

SFLMsgBox* SFLMsgBox::GetInstance(
) {
    if (nullptr == m_msg_box_instance) {
        m_msg_box_instance = new SFLMsgBox();
        if (nullptr == m_msg_box_instance) {
            return nullptr;
        }
    }
    return m_msg_box_instance;
}

void SFLMsgBox::DeleteInstance(
) {
    if (nullptr != m_msg_box_instance) {
        delete m_msg_box_instance;
        m_msg_box_instance = nullptr;
    }
}

void SFLMsgBox::ResetItems(
) {
    setWindowTitle(tr(""));
    setDetailedText(tr(""));
    setIcon(QMessageBox::NoIcon);
    setStandardButtons(QMessageBox::NoButton);
}

void SFLMsgBox::SetBoxType( 
    MsgBoxType msg_box_type 
) {
    ResetItems();
    switch (msg_box_type) {
        case e_warning_type_yes_no: {
            setWindowTitle(QStringLiteral("����"));
            setIcon(QMessageBox::Warning);
            setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            setButtonText(QMessageBox::Yes, QStringLiteral("��"));
            setButtonText(QMessageBox::No, QStringLiteral("��"));
            break;
        }
        case e_information_type_ok: {
            setWindowTitle(QStringLiteral("��ʾ"));
            setIcon(QMessageBox::Information);
            setStandardButtons(QMessageBox::Ok);
            setButtonText(QMessageBox::Ok, QStringLiteral("ȷ��"));
            break;
        }
        default: {
            setWindowTitle(QStringLiteral("��ʾ"));
            setIcon(QMessageBox::Information);
            setStandardButtons(QMessageBox::Ok);
            setButtonText(QMessageBox::Ok, QStringLiteral("ȷ��"));
            break;
        }
    }
}