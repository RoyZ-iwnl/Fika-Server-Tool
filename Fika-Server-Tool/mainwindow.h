#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

enum EFTNotificationIconType {
    Default = 0,
    Alert = 1,
    Friend = 2,
    Mail = 3,
    Note = 4,
    Quest = 5,
    Achievement = 6,
    EntryPoint = 7,
    RagFair = 8,
    Hideout = 9,
    WishlistQuest = 10,
    WishlistHideout = 11,
    WishlistTrading = 12,
    WishlistEquipment = 13,
    WishlistOther = 14
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonCONNECT_clicked();
    void on_pushButtonSEND_clicked();
    void on_comboBoxLanguage_currentIndexChanged(int index);
    void handleNetworkData(QNetworkReply* reply);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QTranslator translator;

    void setLanguage(const QString &language);
};

#endif // MAINWINDOW_H
