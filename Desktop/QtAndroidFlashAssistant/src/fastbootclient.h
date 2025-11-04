#pragma once
#include <QObject>
#include <QString>
#include <QStringList>

class FastbootClient : public QObject {
    Q_OBJECT
public:
    explicit FastbootClient(QObject* parent = nullptr);

    static QString fastbootPath();
    QStringList devices();
    QString getVar(const QString& serial, const QString& key);

    int reboot(const QString& serial);
    int rebootRecovery(const QString& serial);
    int flashingUnlock(const QString& serial);
    int flashingLock(const QString& serial);
    int flashPartition(const QString& serial, const QString& part, const QString& imgPath);

signals:
    void logLine(const QString& line);
};