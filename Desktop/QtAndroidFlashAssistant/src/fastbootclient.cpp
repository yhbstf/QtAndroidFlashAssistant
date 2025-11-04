#include "fastbootclient.h"
#include "processrunner.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QProcess>

static QString findToolFB(const QString& name) {
    const QString exeDir = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath();
    const QString candidate = exeDir + "/tools/" + name;
    if (QFileInfo::exists(candidate)) return candidate;
    return name;
}

FastbootClient::FastbootClient(QObject* parent) : QObject(parent) {}

QString FastbootClient::fastbootPath() { return findToolFB("fastboot.exe"); }

QStringList FastbootClient::devices() {
    QProcess p; p.start(fastbootPath(), {"devices"}); p.waitForFinished();
#if defined(Q_OS_WIN)
    const QString text = QString::fromLocal8Bit(p.readAllStandardOutput());
#else
    const QString text = QString::fromUtf8(p.readAllStandardOutput());
#endif
    QStringList out;
    for (const QString& line : text.split('\n', QString::SkipEmptyParts)) {
        if (line.contains("\tfastboot")) out << line.split('\t').first().trimmed();
    }
    return out;
}

QString FastbootClient::getVar(const QString& serial, const QString& key) {
    QProcess p; p.start(fastbootPath(), {"-s", serial, "getvar", key}); p.waitForFinished();
#if defined(Q_OS_WIN)
    const QString all = QString::fromLocal8Bit(p.readAllStandardError());
#else
    const QString all = QString::fromUtf8(p.readAllStandardError());
#endif
    int idx = all.indexOf(':');
    if (idx >= 0) return all.mid(idx + 1).trimmed();
    return all.trimmed();
}

int FastbootClient::reboot(const QString& serial) {
    ProcessRunner r; QObject::connect(&r, &ProcessRunner::lineStdout, this, &FastbootClient::logLine);
    QObject::connect(&r, &ProcessRunner::lineStderr, this, &FastbootClient::logLine);
    return r.run(fastbootPath(), {"-s", serial, "reboot"});
}

int FastbootClient::rebootRecovery(const QString& serial) {
    ProcessRunner r; QObject::connect(&r, &ProcessRunner::lineStdout, this, &FastbootClient::logLine);
    QObject::connect(&r, &ProcessRunner::lineStderr, this, &FastbootClient::logLine);
    return r.run(fastbootPath(), {"-s", serial, "reboot", "recovery"});
}

int FastbootClient::flashingUnlock(const QString& serial) {
    ProcessRunner r; QObject::connect(&r, &ProcessRunner::lineStdout, this, &FastbootClient::logLine);
    QObject::connect(&r, &ProcessRunner::lineStderr, this, &FastbootClient::logLine);
    return r.run(fastbootPath(), {"-s", serial, "flashing", "unlock"});
}

int FastbootClient::flashingLock(const QString& serial) {
    ProcessRunner r; QObject::connect(&r, &ProcessRunner::lineStdout, this, &FastbootClient::logLine);
    QObject::connect(&r, &ProcessRunner::lineStderr, this, &FastbootClient::logLine);
    return r.run(fastbootPath(), {"-s", serial, "flashing", "lock"});
}

int FastbootClient::flashPartition(const QString& serial, const QString& part, const QString& imgPath) {
    ProcessRunner r; QObject::connect(&r, &ProcessRunner::lineStdout, this, &FastbootClient::logLine);
    QObject::connect(&r, &ProcessRunner::lineStderr, this, &FastbootClient::logLine);
    return r.run(fastbootPath(), {"-s", serial, "flash", part, imgPath});
}