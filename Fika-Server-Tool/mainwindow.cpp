#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , manager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle(tr("Fika Server Tool"));

    // 设置固定大小
    setFixedSize(740, 340);

    // 初始化语言选项
    ui->comboBoxLanguage->addItem("English", "en");
    ui->comboBoxLanguage->addItem("中文", "zh_CN");

    connect(ui->comboBoxLanguage, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBoxLanguage_currentIndexChanged(int)));

    // 默认设置语言
    setLanguage("en");

    // 设置label的文本为可翻译的字符串
    ui->labelIP->setText(tr("IP"));
    ui->labelPORT->setText(tr("Port"));
    ui->labelINFO->setText(tr("Online Info："));
    ui->labelNOTIFY->setText(tr("Notification"));
    ui->labelICON->setText(tr("Icon"));
    ui->labelLanguage->setText(tr("Language"));

    // 添加ComboBox项目
    ui->comboBox->addItem(tr("Default"), EFTNotificationIconType::Default);
    ui->comboBox->addItem(tr("Alert"), EFTNotificationIconType::Alert);
    ui->comboBox->addItem(tr("Friend"), EFTNotificationIconType::Friend);
    ui->comboBox->addItem(tr("Mail"), EFTNotificationIconType::Mail);
    ui->comboBox->addItem(tr("Note"), EFTNotificationIconType::Note);
    ui->comboBox->addItem(tr("Quest"), EFTNotificationIconType::Quest);
    ui->comboBox->addItem(tr("Achievement"), EFTNotificationIconType::Achievement);
    ui->comboBox->addItem(tr("EntryPoint"), EFTNotificationIconType::EntryPoint);
    ui->comboBox->addItem(tr("RagFair"), EFTNotificationIconType::RagFair);
    ui->comboBox->addItem(tr("Hideout"), EFTNotificationIconType::Hideout);
    ui->comboBox->addItem(tr("WishlistQuest"), EFTNotificationIconType::WishlistQuest);
    ui->comboBox->addItem(tr("WishlistHideout"), EFTNotificationIconType::WishlistHideout);
    ui->comboBox->addItem(tr("WishlistTrading"), EFTNotificationIconType::WishlistTrading);
    ui->comboBox->addItem(tr("WishlistEquipment"), EFTNotificationIconType::WishlistEquipment);
    ui->comboBox->addItem(tr("WishlistOther"), EFTNotificationIconType::WishlistOther);

    // 其他初始化代码...
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::handleNetworkData);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
}

void MainWindow::on_pushButtonCONNECT_clicked()
{
    QString ip = ui->lineEditIP->text();
    QString port = ui->lineEditPORT->text();
    QString url = QString("http://%1:%2/fika/presence/get").arg(ip).arg(port);

    QNetworkRequest request(url);
    request.setRawHeader("responsecompressed", "0");
    manager->get(request);
}

void MainWindow::on_pushButtonSEND_clicked()
{
    QString ip = ui->lineEditIP->text();
    QString port = ui->lineEditPORT->text();
    QString url = QString("http://%1:%2/fika/notification/push").arg(ip).arg(port);

    QNetworkRequest request(url);
    request.setRawHeader("requestcompressed", "0");

    QJsonObject json;
    json["notification"] = ui->lineEditPOST->text();
    json["notificationIcon"] = ui->comboBox->currentIndex();

    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());
}

void MainWindow::handleNetworkData(QNetworkReply* reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        ui->textBrowser->setText(tr("Error: ") + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObject = jsonDoc.object();

    QString displayText;
    displayText.append(tr("Nickname: %1\n").arg(jsonObject["nickname"].toString()));
    displayText.append(tr("Level: %1\n").arg(jsonObject["level"].toInt()));
    displayText.append(tr("Activity: %1\n").arg(jsonObject["activity"].toString()));
    displayText.append(tr("Activity Started: %1\n").arg(jsonObject["activityStartedTimestamp"].toInt()));

    if (jsonObject.contains("raidInformation")) {
        QJsonObject raidInfo = jsonObject["raidInformation"].toObject();
        displayText.append(tr("Raid Info: %1\n").arg(raidInfo["info"].toString()));
    }

    ui->textBrowser->setText(displayText);
    reply->deleteLater();
}

void MainWindow::on_comboBoxLanguage_currentIndexChanged(int index)
{
    QString language = ui->comboBoxLanguage->itemData(index).toString();
    setLanguage(language);
}

void MainWindow::setLanguage(const QString &language)
{
    if (translator.load(":/translations/translation_" + language + ".qm")) {
        qApp->installTranslator(&translator);
    }

    // 更新UI
    ui->retranslateUi(this);
}
