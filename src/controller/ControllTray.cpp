#include <QtGui>

#include "ControllTray.h"

static const QString g_bridgerName = "Water Bridger";

ControllTray::ControllTray()
: m_pController(NULL)
{
    ui.setupUi(this);
    createActions();
    createTrayIcon();
    setVisible(true);
    setIcon();

    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(ui.startButton, SIGNAL(clicked()), this, SLOT(startService()));
    connect(ui.stopButton, SIGNAL(clicked()), this, SLOT(stopService()));
    connect(ui.installButton, SIGNAL(clicked()), this, SLOT(installService()));
    connect(ui.uninstallButton, SIGNAL(clicked()), this, SLOT(uninstallService()));

    m_pController = new QtServiceController(g_bridgerName);
    createServiceController();

    trayIcon->show();
}

ControllTray::~ControllTray()
{
    if (m_pController)
    {
        delete m_pController;
        m_pController = NULL;
    }
}

void ControllTray::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}

void ControllTray::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Systray"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void ControllTray::setIcon()
{
    QIcon icon(":/images/search.png");
    trayIcon->setIcon(icon);
    setWindowIcon(QIcon(":/images/controller.ico"));

    trayIcon->setToolTip("Water Mate, a local search engine.");
}

void ControllTray::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
        //showMessage();
        break;
    default:
        ;
    }
}

void ControllTray::showMessage(QString title, QString info, int seconds)
{
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(1);
    trayIcon->showMessage(title, info, icon, seconds*1000);
}

void ControllTray::createServiceController()
{
    if ( !m_pController->isInstalled() )
    {
        ui.installButton->setEnabled(true);
        ui.uninstallButton->setEnabled(false);
        ui.startButton->setEnabled(false);
        ui.stopButton->setEnabled(false);
        return;
    }

    ui.installButton->setEnabled(false);
    ui.uninstallButton->setEnabled(true);

    if ( m_pController->isRunning() )
    {
        ui.startButton->setEnabled(false);
        ui.stopButton->setEnabled(true);
    } else {
        ui.startButton->setEnabled(true);
        ui.stopButton->setEnabled(false);
    }
}

void ControllTray::messageClicked()
{
    QMessageBox::information(0, tr("Systray"),
                             tr("Sorry, I already gave what help I could.\n"
                                "Maybe you should try asking a human?"));
}

void ControllTray::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void ControllTray::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void ControllTray::startService()
{
    if ( m_pController->start() )
    {
        ui.startButton->setEnabled(false);
        ui.stopButton->setEnabled(true);
        showMessage("Info", "Service was started!", 5);
    } else {
        showMessage("Info", "Service was not started!", 5);
    }
}

void ControllTray::stopService()
{
    if ( m_pController->stop() )
    {
        ui.startButton->setEnabled(true);
        ui.stopButton->setEnabled(false);
        showMessage("Info", "Service was stoped!", 5);
    } else {
        showMessage("Info", "Service was not stoped!", 5);
    }
}

void ControllTray::installService()
{
    QString path = QDir::currentPath() + "\\bridger.exe";
    if ( QtServiceController::install(path) )
    {
        showMessage("Info", "Install Bridger successfully!", 5);
    } else {
        showMessage("Info", "Install Bridger failed!", 5);
    }

    createServiceController();
}

void ControllTray::uninstallService()
{
    if ( !m_pController->stop() )
    {
        showMessage("Info", "Can't stop the service, please stop the service first!", 5);
        return;
    }

    if ( m_pController->uninstall() )
    {
        showMessage("Info", "Uninstall service successfully!", 5);
    } else {
        showMessage("Info", "Uninstall service failed!", 5);
    }

    createServiceController();
}