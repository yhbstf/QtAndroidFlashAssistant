#include "processrunner.h"

ProcessRunner::ProcessRunner(QObject* parent) : QObject(parent) {
    connect(&m_proc, &QProcess::readyReadStandardOutput, this, &ProcessRunner::onReadyStdout);
    connect(&m_proc, &QProcess::readyReadStandardError, this, &ProcessRunner::onReadyStderr);
}

int ProcessRunner::run(const QString& program, const QStringList& args, int timeoutMs) {
    m_bufOut.clear();
    m_bufErr.clear();

    m_proc.setProgram(program);
    m_proc.setArguments(args);
    m_proc.setProcessChannelMode(QProcess::SeparateChannels);
    m_proc.start();
    if (!m_proc.waitForStarted(5000)) {
        emit lineStderr(QStringLiteral("[ERROR] 进程启动失败: %1").arg(program));
        return -1;
    }

    if (timeoutMs > 0) m_proc.waitForFinished(timeoutMs);
    else m_proc.waitForFinished(-1);

    onReadyStdout();
    onReadyStderr();

    return m_proc.exitCode();
}

void ProcessRunner::drainBuffer(QByteArray& buf, const QByteArray& chunk, bool isStdout) {
    buf += chunk;
    int idx;
    while ((idx = buf.indexOf('\n')) >= 0) {
        QByteArray line = buf.left(idx + 1);
        buf.remove(0, idx + 1);
#if defined(Q_OS_WIN)
        QString s = QString::fromLocal8Bit(line).trimmed();
#else
        QString s = QString::fromUtf8(line).trimmed();
#endif
        if (isStdout) emit lineStdout(s);
        else emit lineStderr(s);
    }
}

void ProcessRunner::onReadyStdout() { drainBuffer(m_bufOut, m_proc.readAllStandardOutput(), true); }
void ProcessRunner::onReadyStderr() { drainBuffer(m_bufErr, m_proc.readAllStandardError(), false); }