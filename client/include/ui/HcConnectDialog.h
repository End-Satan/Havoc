#ifndef HAVOCCLIENT_HCCONNECTDIALOG_H
#define HAVOCCLIENT_HCCONNECTDIALOG_H

#include <Common.h>

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class HcConnectDialog : public QDialog
{
    bool PressedConnect = false;

public:
    QGridLayout* gridLayout       = nullptr;
    QHBoxLayout* horizontalLayout = nullptr;
    QWidget*     ConnectionWidget = nullptr;
    QLabel*      LabelHavoc       = nullptr;
    QLineEdit*   InputProfileName = nullptr;
    QLineEdit*   InputHost        = nullptr;
    QLineEdit*   InputPort        = nullptr;
    QLineEdit*   InputUsername    = nullptr;
    QLineEdit*   InputPassword    = nullptr;
    QPushButton* ButtonConnect    = nullptr;
    QPushButton* ButtonAdd        = nullptr;
    QListWidget* ListConnection   = nullptr;

    explicit HcConnectDialog( );
    ~HcConnectDialog( ) override;

    auto sanityCheckInput() -> bool;
    auto retranslateUi() -> void;
    auto start() -> json;
    auto pressedConnect() const -> bool;
};

QT_END_NAMESPACE

#endif //HAVOCCLIENT_HCCONNECTDIALOG_H