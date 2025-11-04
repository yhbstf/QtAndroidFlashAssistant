#pragma once
#include <QObject>
#include <QString>
#include <QStringList>

class ADBClient : public QObject {
    Q_OBJECT
public:
    explicit ADBClient(QObject* parent = nullptr);

    static QString adbPath();
    QStringList devices();
    QString getProp(const QString& serial, const QString& key);
    QString getState(const QString& serial);

    int reboot(const QString& serial, const QString& target);
    int installApk(const QString& serial, const QString& apkPath);
    int sideload(const QString& zipPath, int timeoutMs = 0);

signals:
    void logLine(const QString& line);
};