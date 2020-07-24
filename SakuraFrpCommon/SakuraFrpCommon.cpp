#include "SakuraFrpCommon.h"

#include <QUuid>
#include <qt_windows.h>

SakuraFrpCommon::SakuraFrpCommon()
{

}

SakuraFrpCommon::~SakuraFrpCommon()
{

}

void SakuraFrpCommon::UnicodeTransfrom(
    QString& str
) {
    do {
        int idx = str.indexOf("\\u");
        QString str_hex = str.mid(idx, 6);
        str_hex = str_hex.replace("\\u", QString());
        int n_hex = str_hex.toInt(0, 16);
        str.replace(idx, 6, QChar(n_hex));
    } while (str.indexOf("\\u") != -1);
}

QString SakuraFrpCommon::NextUuid(
) {
    return QUuid::createUuid().toString().remove("{").remove("}").remove("-");
}

bool SakuraFrpCommon::Is64BitSystem(
) {
    typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
    PGNSI pGNSI;
    SYSTEM_INFO si;
    ZeroMemory(&si, sizeof(SYSTEM_INFO));
    pGNSI = (PGNSI)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo");
    if (pGNSI) {
        pGNSI(&si);
        if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
            si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64
        ) {
            return true;
        }
    }
    return false;
}