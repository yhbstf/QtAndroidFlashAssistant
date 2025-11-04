#pragma once
#include <QMainWindow>
#include <memory>
#include "adbclient.h"
#include "fastbootclient.h"

class QTextEdit;
class QLineEdit;
class QPushButton;
class QComboBox;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onRefreshDevices();
    void onReadInfo();
    void onRebootSystem();
    void onRebootRecovery();
    void onRebootBootloader();
    void onInstallApk();
    void onSideloadZip();
    void onFlashPartition();
    void onUnlock();
    void onLock();

    void appendLog(const QString& line);

private:
    std::unique_ptr<ADBClient> m_adb;
    std::unique_ptr<FastbootClient> m_fb;

    QComboBox* m_modeCombo {nullptr}; // ADB / Fastboot
    QComboBox* m_deviceCombo {nullptr};
    QLabel* m_stateLabel {nullptr};
    QTextEdit* m_log {nullptr};
    QLineEdit* m_partEdit {nullptr};

    QString currentSerial() const;
    void setupUi();
    void infoBox(const QString& title, const QString& text, bool isWarn=false);
};