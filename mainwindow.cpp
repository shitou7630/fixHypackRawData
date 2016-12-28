#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>

#include "fstream"
#include "iostream"
#include "string"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_defaultOpenPath(QString("C://HYPACK 2014//Projects")),
    m_fileSavePath(""),
    m_fileOpenPath("")
{
    ui->setupUi(this);

    m_fileList.clear ();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonChooseFile_clicked()
{
    QString file = QFileDialog::getOpenFileName(
                                    this,
                                    tr("Select LOG or RAW file"),
                                    m_defaultOpenPath,
                                    "LOG  (*.log);; RAW (*.raw)");

    //! no file
    if(file.isEmpty ())
    {
       return;
    }

    QFileInfo fileOpened(file);
    QString fileSuffix(fileOpened.suffix ());
    m_fileOpenPath = fileOpened.path ();

    ui->lineEditFile->setText ( fileOpened.filePath ());

    //! process LOG file
    m_fileList.clear ();
    ui->listWidget_FileList->clear ();
    if(0 == fileSuffix.compare(QString("LOG"), Qt::CaseInsensitive))
    {
        m_fileList << getLOGFileList ( file );
        //            qDebug() << getLOGFileList ( file );
    }
    else
    {
        //! process RAW files
        m_fileList << fileOpened.fileName ();
    }
    ui->listWidget_FileList->addItems ( m_fileList );
}

QStringList MainWindow::getLOGFileList(QString &file)
{
    QStringList strList;
    strList.clear ();

	ifstream inFile(file.toStdString());
	string strLine;

    if(!inFile)
    {
        //Error open file

    }

	while (getline( inFile, strLine))
	{
		strList.append( QString::fromStdString(strLine) );
	}
		
    return strList;
}

int MainWindow::fixFile(QString &srcFile, QString &toFile)
{
//    ifstream inFile(srcFile.toStdString());
    QFile inFile( srcFile );
    ofstream outFile;
    outFile.open (QByteArray(toFile.toLatin1 ()).data (), std::ios::app | std::ios::out );
    string strLine;
    QString strQLine;

    if(!inFile.open(QIODevice::ReadOnly/* | QIODevice::Text*/))
    {
        QMessageBox mess(QMessageBox::Critical, tr("Error"), tr("Open read_file failure."));
        mess.addButton (tr("OK") , QMessageBox::RejectRole);
        mess.setWindowFlags(Qt::Drawer);   //! delete window icon
        mess.exec ();
        qDebug() << "in file error";
        return false;
    }

    if(!outFile)
    {
        QMessageBox mess(QMessageBox::Critical, tr("Error"), tr("Open write_file failure."));
        mess.addButton (tr("OK") , QMessageBox::RejectRole);
        mess.setWindowFlags(Qt::Drawer);   //! delete window icon
        mess.exec ();
        qDebug() << "out file error";
        return false;
    }

    //!
    QTextStream txtInput(&inFile);
    QString lineStr;

    //! *** C ***
    //! Open write file.
    //! Method_01
    //!
    const char *enter = "\n";
    while(!inFile.atEnd ())
    {
        lineStr = txtInput.readLine();
        if(lineStr.contains (QRegExp("^MSG")))
        {
            if( !lineStr.contains ( QRegExp("^MSG[0-9.\x20]{11}[a-zA-Z(\x20).0-9,$*]+$")) )
            {
    //            lineStr.replace (QRegExp("[^\x00-\x7F]+"), QString("\x20"));
                lineStr = lineStr.section (QString("\x20"),0,2);
                qDebug() << lineStr;
            }
        }

        //outFile.write(QByteArray(lineStr.toLatin1 ()).data (), lineStr.count ());
        outFile.write(lineStr.toStdString ().c_str (), lineStr.count ());
        outFile.write(enter, 1);
    }
    inFile.close();
    outFile.close ();
    return true;
}

void MainWindow::on_pushButtonQuit_clicked()
{
    QMessageBox mess(QMessageBox::NoIcon, tr("Quit?"), tr("Decide to quit?") );
    QPushButton *okbtn = (mess.addButton (tr("OK") , QMessageBox::AcceptRole));
    mess.addButton (tr("Cancle"), QMessageBox::RejectRole);
    mess.setWindowFlags(Qt::Drawer);   //! delete window icon
    mess.exec ();

    if(mess.clickedButton () == okbtn)
        qApp->quit ();
}

void MainWindow::on_pushButtonReset_clicked()
{
    ui->lineEditFile->clear ();
    ui->lineEditSavePath->clear ();
    ui->listWidget_FileList->clear ();
}

void MainWindow::on_pushButtonFixSave_clicked()
{
    if(ui->lineEditFile->text () == "")
    {
        QMessageBox mess(QMessageBox::Critical, tr("Error"), tr("No needed process files.") );
        mess.addButton (tr("OK") , QMessageBox::RejectRole);
        mess.setWindowFlags(Qt::Drawer);   //! delete window icon
        mess.exec ();
        return;
    }
    //!
    m_fileSavePath = ui->lineEditSavePath->text ();
    if(m_fileSavePath == "")
    {
        QMessageBox mess(QMessageBox::Critical, tr("Error"), tr("Save path not set.") );
        mess.addButton (tr("OK") , QMessageBox::RejectRole);
        mess.setWindowFlags(Qt::Drawer);   //! delete window icon
        mess.exec ();
        return;
    }

    //!
    for(int i = 0; i < ui->listWidget_FileList->count (); ++i)
    {        
        QString fileOpened = m_fileOpenPath + QString("/") + ui->listWidget_FileList->item (i)->text ();
        QString fileSaved =  m_fileSavePath + QString("/") + ui->listWidget_FileList->item (i)->text ();
//        fileOpened.replace (QRegExp("/"),QString("//"));
//        fileSaved.replace (QRegExp("/"),QString("//"));        
        if(!fixFile(fileOpened, fileSaved))
        {
            QMessageBox mess(QMessageBox::Critical, tr("Error"),
                             tr("file") + QString( " %1 ").arg (ui->listWidget_FileList->item (i)->text ())
                                            +tr("fix failure."));
            mess.addButton (tr("OK") , QMessageBox::RejectRole);
            mess.setWindowFlags(Qt::Drawer);   //! delete window icon
            mess.exec ();
        }
    }

    QMessageBox mess(QMessageBox::Information, tr("Sucess"),tr("Fix complete!")) ;
    mess.addButton (tr("OK") , QMessageBox::RejectRole);
    mess.setWindowFlags(Qt::Drawer);   //! delete window icon
    mess.exec ();
}

void MainWindow::on_pushButtonSavePath_clicked()
{
    QString savePath = QFileDialog::getExistingDirectory ( this,
                                                           tr("Choose path of saving files."));

    if(savePath == m_fileOpenPath)
    {
        QMessageBox mess(QMessageBox::Warning , tr("Warning"), tr("In order to the data safety,"
                                                                   "save path must be differenet "
                                                                  "from source file path."));
        mess.addButton (tr("OK") , QMessageBox::RejectRole);
        mess.setWindowFlags(Qt::Drawer);   //! delete window icon
        mess.exec ();
        return;
    }
    else
    {
        ui->lineEditSavePath->setText (savePath);
    }
}
