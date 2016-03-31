#ifndef CONTROLLTRAY_H_
#define CONTROLLTRAY_H_

#include <QSystemTrayIcon>
#include <QDialog>
#include "ui_controller.h"
#include "qtservice.h"

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
QT_END_NAMESPACE

//! [0]
class ControllTray : public QDialog
{
    Q_OBJECT

public:
    ControllTray();
    ~ControllTray();

    void setVisible(bool visible);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();
    void startService();
    void stopService();
    void installService();
    void uninstallService();

private:
    void createActions();
    void createTrayIcon();
    void setIcon();
    void showMessage(QString title, QString info, int seconds);
    void createServiceController();

private:
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QtServiceController* m_pController;

private:
    Ui::CPanel ui;
};

#endif /* CONTROLLTRAY_H_ */
