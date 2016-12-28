#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonChooseFile_clicked();

    void on_pushButtonQuit_clicked();

    void on_pushButtonReset_clicked();

    void on_pushButtonFixSave_clicked();

    void on_pushButtonSavePath_clicked();

private:
    void addFileList2Widget(QStringList &fileList);
    QStringList getLOGFileList(QString &file);
    int fixFile(QString &srcFile, QString & toFile);

private:
    Ui::MainWindow *ui;
    QString m_defaultOpenPath;
    QStringList m_fileList;
    QString m_fileSavePath;
    QString m_fileOpenPath;
};

#endif // MAINWINDOW_H
