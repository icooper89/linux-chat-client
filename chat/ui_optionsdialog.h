/********************************************************************************
** Form generated from reading UI file 'optionsdialog.ui'
**
** Created: 
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONSDIALOG_H
#define UI_OPTIONSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OptionsDialog
{
public:
    QDialogButtonBox *browse;
    QCheckBox *save;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLineEdit *filename;
    QPushButton *pushButton;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *userLabel;
    QLineEdit *username;

    void setupUi(QDialog *OptionsDialog)
    {
        if (OptionsDialog->objectName().isEmpty())
            OptionsDialog->setObjectName(QString::fromUtf8("OptionsDialog"));
        OptionsDialog->resize(400, 300);
        browse = new QDialogButtonBox(OptionsDialog);
        browse->setObjectName(QString::fromUtf8("browse"));
        browse->setGeometry(QRect(30, 240, 341, 32));
        browse->setOrientation(Qt::Horizontal);
        browse->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        save = new QCheckBox(OptionsDialog);
        save->setObjectName(QString::fromUtf8("save"));
        save->setGeometry(QRect(10, 110, 111, 22));
        widget = new QWidget(OptionsDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(11, 150, 341, 29));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        filename = new QLineEdit(widget);
        filename->setObjectName(QString::fromUtf8("filename"));

        horizontalLayout->addWidget(filename);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        widget1 = new QWidget(OptionsDialog);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        widget1->setGeometry(QRect(10, 40, 223, 29));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        userLabel = new QLabel(widget1);
        userLabel->setObjectName(QString::fromUtf8("userLabel"));

        horizontalLayout_2->addWidget(userLabel);

        username = new QLineEdit(widget1);
        username->setObjectName(QString::fromUtf8("username"));

        horizontalLayout_2->addWidget(username);


        retranslateUi(OptionsDialog);
        QObject::connect(browse, SIGNAL(accepted()), OptionsDialog, SLOT(accept()));
        QObject::connect(browse, SIGNAL(rejected()), OptionsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(OptionsDialog);
    } // setupUi

    void retranslateUi(QDialog *OptionsDialog)
    {
        OptionsDialog->setWindowTitle(QApplication::translate("OptionsDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        save->setText(QApplication::translate("OptionsDialog", "Save to File", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("OptionsDialog", "Browse", 0, QApplication::UnicodeUTF8));
        userLabel->setText(QApplication::translate("OptionsDialog", "Username", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class OptionsDialog: public Ui_OptionsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONSDIALOG_H
