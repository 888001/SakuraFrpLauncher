#include "SakuraFrpLauncher.h"

#include <QtWidgets/QApplication>
#include <QDir>
#include <QSharedMemory>

#include "SFLDBMgr.h"
#include "SFLMsgBox.h"

void SetupQssFile(
) {
    QFile qss_file(":/Resources/qss/SakuraFrpLauncher.qss");
    if (qss_file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(qss_file.readAll());  
        qApp->setStyleSheet(qss);  
        qss_file.close();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ���õ�ǰĿ¼��֮��ʹ�����·��
    QString app_dir_path = QApplication::applicationDirPath();
    QString native_app_dir_path =QDir::toNativeSeparators(app_dir_path);
    QDir::setCurrent(native_app_dir_path);

    // ���ؿ�·��
    qApp->addLibraryPath("plugins");

    // ������ʽ
    SetupQssFile();

    // ֻ������һ��ʵ��
    QSharedMemory share_mem(QApplication::applicationFilePath() + "SakuraFrpLauncher");
    if (!share_mem.create(1)) {
        SFLMsgBox::GetInstance()->SetBoxType(e_information_type_ok);
        SFLMsgBox::GetInstance()->setText(QStringLiteral("��Ҫ�ظ�����Sakura Frp�ͻ��ˣ��������ж���ͻ����뽫������Ƶ�����Ŀ¼��������"));
        SFLMsgBox::GetInstance()->exec();
        return 0;
    }

    // ����/�򿪱������ݿ�
    SFLDBMgr::GetInstance()->OpenLocalDB("data.db");

    SakuraFrpLauncher w;
    w.show();
    return a.exec();
}
