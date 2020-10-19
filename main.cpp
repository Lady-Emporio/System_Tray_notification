
#include <QApplication>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMenu>
#include <QDebug>
#include <QObject>

#include "app1.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    APP * app=new APP();


    app->trayTest->setIcon( QIcon( "./PNG_16_16.jpg" ) );
    app->trayTest->show();
    app->trayTest->showMessage( "We are", "...",QIcon(),1000);

    app->trayTest->setContextMenu(app->menu);




    QAction *newAct;
    QAction *exitAct;

    {
        newAct = new QAction("Добавить задачу");
        QObject::connect(newAct, &QAction::triggered, app,&APP::addNewQuest_Slot);
    }
    {
        exitAct = new QAction("Exit");
        QObject::connect(exitAct, &QAction::triggered, app,&APP::exit_Slot);
    }
    {
        app->pauseAct = new QAction("Pause");
        QObject::connect(app->pauseAct, &QAction::triggered, app,&APP::pauseSlot);
    }

    app->menu->addAction(newAct);
    app->menu->addAction(app->pauseAct);
    app->menu->addAction(exitAct);


    if (!a.exec()){
        qDebug()<<"a.exec close";
    }
}
