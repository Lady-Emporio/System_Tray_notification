#include "app1.h"
#include <QInputDialog>
#include <QCloseEvent>
#include <QAction>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMenu>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QSystemTrayIcon>

void APP::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}

APP::APP(QWidget *parent) : QWidget(parent),pauseWork(Quest("Pause"))
{
    trayTest = new QSystemTrayIcon();
    isPause=false;
    menu=new QMenu(this);
    //nowQuest=new QAction("Nothing.",this);
    questionLineEdit=new QLineEdit(this);
    save_button=new QPushButton("Save", this);
    close_button=new QPushButton("Close work?", this);
    QVBoxLayout * mainLayout=new QVBoxLayout (this);
    this->setLayout(mainLayout );

    mainLayout->addWidget(questionLineEdit);
    mainLayout->addWidget(save_button);
    mainLayout->addWidget(close_button);

    connect(save_button, SIGNAL (released()), this, SLOT (save_Slot()));
    connect(close_button, SIGNAL (released()), this, SLOT (closeQuest_Slot()));
}

void APP::addNewQuest_Slot()
{
    questionLineEdit->setReadOnly(false);
    questionLineEdit->setText("");
    this->setWindowTitle("Create quest.");
    close_button->setHidden(true);
    save_button->setHidden(false);

    questionLineEdit->setFocus();
    this->show();
}

void APP::exit_Slot()
{
    qDebug()<<"exit_Slot";
    exit(0);
}

void APP::save_Slot()
{
    QString nameWork=questionLineEdit->text();
    if (nameWork.isEmpty()){
        return;
    }
    Quest *activeWork=new Quest(nameWork);
    QAction *nextWork=new QAction(activeWork->getName());
    connect(nextWork, SIGNAL (triggered()), this, SLOT (workClick_Slot()));

    activeWorks[nextWork]=activeWork;
    menu->addAction(nextWork);
    questionLineEdit->setText("");
    questionLineEdit->setReadOnly(false);
    this->hide();
    trayTest->showMessage( "In memory!!!", activeWork->getName(),QIcon("./InMemory.png"),1500);
}

void APP::pauseSlot()
{
    //is not set pause
    if(isPause){
        isPause=false;
        pauseAct->setText("Pause");
        pauseWork.setEnd();
        workList.append(pauseWork);
        closeQuest(&pauseWork, trayTest);

        QList<QAction*>keys=activeWorks.keys();
        for(int i=0;i!=keys.size();++i){
            keys[i]->setText( activeWorks[keys[i]]->getName()  );
        }

    }else{
        isPause=true;
        pauseAct->setText("Продолжить");
        pauseWork=Quest("Pause");
        QList<QAction*>keys=activeWorks.keys();
        for(int i=0;i!=keys.size();++i){
            keys[i]->setText("Пауза: "+ activeWorks[keys[i]]->getName()  );
        }
    }
}

void APP::workClick_Slot()
{
    QAction* pAction = qobject_cast<QAction*>(sender());
    Quest * res=activeWorks[pAction];
    needClose=pAction;
    this->setWindowTitle("Close");
    close_button->setHidden(false);
    save_button->setHidden(true);
    questionLineEdit->setReadOnly(true);
    questionLineEdit->setText(res->getName());
    this->show();

}

void APP::closeQuest_Slot()
{
    Quest*toCloseQuest=activeWorks[needClose];
    toCloseQuest->setEnd();
    activeWorks.remove(needClose);
    menu->removeAction(needClose);
    delete needClose;
    needClose=nullptr;
    this->hide();
    closeQuest(toCloseQuest,trayTest);
    delete toCloseQuest;
}

void closeQuest(Quest * toClose,QSystemTrayIcon *trayTest){
    QString filename = "Data.txt";
    QFile file(filename);
    QString dataFormat="yyyy.MM.dd HH:mm:ss";
    if (file.open(QIODevice::OpenModeFlag::Append)) {
        QTextStream stream(&file);
        QString texttoWrite=toClose->begin.toString(dataFormat)+"|"+ toClose->end.toString(dataFormat)+"|"+toClose->name+"|\r\n";
        stream <<texttoWrite << endl;
        trayTest->showMessage( "Write to file.", texttoWrite,QIcon("./ToFile.png"),1500);
    }

}
