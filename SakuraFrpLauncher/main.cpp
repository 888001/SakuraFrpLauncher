#include "SakuraFrpLauncher.h"

#include <QtWidgets/QApplication>
#include <QDir>
#include <QSharedMemory>

#include "SFLDBMgr.h"
#include "SFLMsgBox.h"

void AddLibraryPath(
) {
    QString app_dir_path = QDir::toNativeSeparators(QApplication::instance()->applicationDirPath());
    qApp->addLibraryPath(app_dir_path + "\\plugins");
}

void CreateDataDB(
) {
    QString app_dir_path = QDir::toNativeSeparators(QApplication::instance()->applicationDirPath());
    SFLDBMgr::GetInstance()->OpenLocalDB(app_dir_path + "\\data.db");
}

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

    // ���ؿ�·��
    AddLibraryPath();

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
    CreateDataDB();

    SakuraFrpLauncher w;
    w.show();
    return a.exec();
}
