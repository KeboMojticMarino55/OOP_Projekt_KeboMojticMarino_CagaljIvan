#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <exception>
#include <stdexcept>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";

    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    try {
        QApplication a(argc, argv);

        qDebug() << "Application starting...";

        try {
            QFile styleFile("styles.qss");
            if (styleFile.exists() && styleFile.open(QFile::ReadOnly)) {
                QString styleSheet = QLatin1String(styleFile.readAll());
                a.setStyleSheet(styleSheet);
                qDebug() << "Stylesheet loaded successfully";
            } else {
                qWarning() << "Stylesheet file not found or cannot be opened";
            }
        } catch (const std::exception& e) {
            qCritical() << "Error loading stylesheet:" << e.what();
        } catch (...) {
            qCritical() << "Unknown error loading stylesheet";
        }

        MainWindow w;
        w.show();

        return a.exec();

    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Fatal Error",
                              QString("Unhandled exception: %1\n\nApplication will now close.").arg(e.what()));
        return -1;
    } catch (...) {
        QMessageBox::critical(nullptr, "Fatal Error",
                              "Unknown fatal error occurred.\n\nApplication will now close.");
        return -1;
    }
}
