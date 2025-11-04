#pragma once
#include <QObject>
#include <QProcess>

class ProcessRunner : public QObject {
    Q_OBJECT
public:
    explicit ProcessRunner(QObject* parent = nullptr);
    int run(const QString& program, const QStringList& args, int timeoutMs = 0);

signals:
    void lineStdout(const QString& line);
    void lineStderr(const QString& line);

private slots:
    void onReadyStdout();
    void onReadyStderr();

private:
    QProcess m_proc;
    QByteArray m_bufOut, m_bufErr;
    void drainBuffer(QByteArray& buf, const QByteArray& chunk, bool isStdout);
};