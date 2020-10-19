#ifndef APP1_H
#define APP1_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QLineEdit>
#include <QDateTime>
#include <QAction>
#include <QPushButton>
#include <QSystemTrayIcon>

class Quest{
public:
    QDateTime begin;
    QDateTime end;
    QString name;
    Quest(QString text):name(text){
        begin = QDateTime::currentDateTime();
    }
    QString getName(){
        return QString::number(this->begin.time().hour())+":"+QString::number(this->begin.time().minute())+" - "+name;
    }
    void setEnd(){
        end = QDateTime::currentDateTime();
    }
};


void closeQuest(Quest * toClose,QSystemTrayIcon *trayTest);

class APP : public QWidget
{
    Q_OBJECT
    void closeEvent(QCloseEvent *event);
    bool isPause;
public:
    explicit APP(QWidget *parent = nullptr);
    QAction *pauseAct;
    QSystemTrayIcon *trayTest;
    QPushButton * save_button;
    QPushButton * close_button;
    QLineEdit *questionLineEdit;
    Quest pauseWork;
    QAction *needClose;
    QList<Quest>workList;
    QMenu *menu;
    QMap<QAction*,Quest*>activeWorks;

signals:

public slots:
    void addNewQuest_Slot();
    void exit_Slot();
    void save_Slot();
    void pauseSlot();
    void workClick_Slot();
    void closeQuest_Slot();
};

#endif // APP1_H
