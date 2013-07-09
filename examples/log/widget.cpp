#include "widget.h"
#include "ui_widget.h"

#include <QtCore/QDebug>

static const char myDebugCategory[] = "example.debug";
static const char myWarningCategory[] = "example.warning";
static const char myCriticalCategory[] = "example.critical";
#define myDebug QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO, myDebugCategory).debug
#define myWarning QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO, myWarningCategory).warning
#define myCritical QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO, myCriticalCategory).critical

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->logText, SIGNAL(returnPressed()), ui->logButton, SLOT(animateClick()));
    connect(ui->warningText, SIGNAL(returnPressed()), ui->warningButton, SLOT(animateClick()));
    connect(ui->criticalText, SIGNAL(returnPressed()), ui->criticalButton, SLOT(animateClick()));

    connect(ui->logButton, &QPushButton::clicked, [&](){
        if (!ui->logText->text().isEmpty()) {
            myDebug() << ui->logText->text();
        }
    });
    connect(ui->warningButton, &QPushButton::clicked, [&](){
        if (!ui->warningText->text().isEmpty()) {
            myWarning() << ui->warningText->text();
        }
    });
    connect(ui->criticalButton, &QPushButton::clicked, [&](){
        if (!ui->criticalText->text().isEmpty()) {
            myCritical() << ui->criticalText->text();
        }
    });
}

Widget::~Widget()
{
    delete ui;
}
