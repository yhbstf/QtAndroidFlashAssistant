#include "adbclient.h"
#include "processrunner.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QProcess>
#include <iostream>
#include <QDebug>

static QString findTool(const QString& name) {
    // 获取应用程序路径及目录
    const QString appPath = QCoreApplication::applicationFilePath();
    const QFileInfo appInfo(appPath);
    const QString exeDir = appInfo.absolutePath();

    // 拼接候选路径
    const QString candidate = exeDir + "/tools/" + name;
    const QFileInfo candidateInfo(candidate);

    // 打印详细调试信息
    qDebug() << "===== Tool Search Debug =====";
    qDebug() << "Application file path: " << appPath;       // 应用程序完整路径（含可执行文件名）
    qDebug() << "Application directory: " << exeDir;       // 应用程序所在目录
    qDebug() << "Tools subdirectory: " << exeDir + "/tools"; // tools文件夹的完整路径
    qDebug() << "Candidate tool path: " << candidate;      // 候选工具的完整路径
    qDebug() << "Candidate exists? " << candidateInfo.exists(); // 是否存在
//    qDebug() << "Parent dir exists? " << candidateInfo.dir().exists(); // 父目录（tools）是否存在
    qDebug() << "Is absolute path? " << candidateInfo.isAbsolute(); // 是否为绝对路径（避免相对路径问题）
#ifdef Q_OS_WIN
//    qDebug() << "Windows path (without /): " << candidate.replace('/', '\\'); // Windows下转换为反斜杠查看
#endif
    qDebug() << "============================";

    if (candidateInfo.exists()) {
        return candidate;
    }
    return name;
}

ADBClient::ADBClient(QObject* parent) : QObject(parent) {}

QString ADBClient::adbPath() {
    std::cout <<"找2020adb" << std::endl;

    QString temp_adb = findTool("adb.exe");

    qDebug() << "获得的adb是"<< temp_adb;


    return findTool("adb.exe"); }

QStringList ADBClient::devices() {
    ProcessRunner r; QObject::connect(&r, &ProcessRunner::lineStdout, this, &ADBClient::logLine);
    QObject::connect(&r, &ProcessRunner::lineStderr, this, &ADBClient::logLine);
    r.run(adbPath(), {"devices"});
    QProcess p; p.start(adbPath(), {"devices"}); p.waitForFinished();
#if defined(Q_OS_WIN)
    const QString text = QString::fromLocal8Bit(p.readAllStandardOutput());
#else
    const QString text = QString::fromUtf8(p.readAllStandardOutput());
#endif
    QStringList out;
    for (const QString& line : text.split('\n', QString::SkipEmptyParts)) {
        if (line.contains("\tdevice")) out << line.split('\t').first().trimmed();
    }
    return out;
}

QString ADBClient::getProp(const QString& serial, const QString& key) {
    QProcess p; p.start(adbPath(), {"-s", serial, "shell", "getprop", key}); p.waitForFinished();
#if defined(Q_OS_WIN)
    return QString::fromLocal8Bit(p.readAllStandardOutput()).trimmed();
#else
    return QString::fromUtf8(p.readAllStandardOutput()).trimmed();
#endif
}

QString ADBClient::getState(const QString& serial) {
    QProcess p; p.start(adbPath(), {"-s", serial, "get-state"}); p.waitForFinished();
#if defined(Q_OS_WIN)
    return QString::fromLocal8Bit(p.readAllStandardOutput()).trimmed();
#else
    return QString::fromUtf8(p.readAllStandardOutput()).trimmed();
#endif
}

int ADBClient::reboot(const QString& serial, const QString& target) {
    ProcessRunner r; QObject::connect(&r, &ProcessRunner::lineStdout, this, &ADBClient::logLine);
    QObject::connect(&r, &ProcessRunner::lineStderr, this, &ADBClient::logLine);
    QStringList args = {"-s", serial, "reboot"};
    if (!target.isEmpty()) args << target;
    return r.run(adbPath(), args);
}

int ADBClient::installApk(const QString& serial, const QString& apkPath) {
    ProcessRunner r; QObject::connect(&r, &ProcessRunner::lineStdout, this, &ADBClient::logLine);
    QObject::connect(&r, &ProcessRunner::lineStderr, this, &ADBClient::logLine);
    return r.run(adbPath(), {"-s", serial, "install", "-r", apkPath});
}

int ADBClient::sideload(const QString& zipPath, int timeoutMs) {
    ProcessRunner r; QObject::connect(&r, &ProcessRunner::lineStdout, this, &ADBClient::logLine);
    QObject::connect(&r, &ProcessRunner::lineStderr, this, &ADBClient::logLine);
    return r.run(adbPath(), {"sideload", zipPath}, timeoutMs);
}
