#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QrcodeBoyGo.h"
#include "toast.h"
#include <QImage>
#include <QFileDialog>
#include <QCameraInfo>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QBuffer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_timer(NULL),
    m_camera(NULL),
    m_viewfinder(NULL),
    m_imageCapture(NULL)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_CreateQrCode_clicked()
{
    QString text = ui->plainTextEdit_CreateQrCode_text->toPlainText();
    if (text.isEmpty()) {
        return;
    }
    TextToQrcodePng_Resp resp= TextToQrcodePng(text.toStdString(), 256);
    if (!resp.ErrMsg.empty()) {
        Toast::Instance()->setError(resp.ErrMsg.c_str());
        return;
    }
    QImage img;
    img.loadFromData(QByteArray::fromStdString(resp.PngBytes));
    ui->label_CreateQrCode2->setPixmap(QPixmap::fromImage(img));
    ui->stackedWidget->setCurrentWidget(ui->page_CreateQrCode2);
}

void MainWindow::on_actionCreateQrCode_triggered()
{
    ui->plainTextEdit_CreateQrCode_text->clear();
    ui->stackedWidget->setCurrentWidget(ui->page_CreateQrCode);
    this->stopCamera();
}

void MainWindow::on_actionScanQrCode_triggered()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ScanQrCode);
    ui->comboBox_ScanQrCode->clear();
    for(QCameraInfo info : QCameraInfo::availableCameras()) {
        ui->comboBox_ScanQrCode->addItem(info.description(), QVariant::fromValue<QByteArray>(info.deviceName().toLatin1()));
    }
    connect(ui->comboBox_ScanQrCode, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_reloadCamera()));
    this->slot_reloadCamera();
}

void MainWindow::on_pushButton_CreateQrCode2_continue_clicked()
{
    ui->plainTextEdit_CreateQrCode_text->clear();
    ui->stackedWidget->setCurrentWidget(ui->page_CreateQrCode);
}

void MainWindow::on_pushButton_ScanQrCode_clicked()
{
    QString imgFileNameStr = QFileDialog::getOpenFileName(this, "",  "", "图片文件(*.jpg;*.png)");
    if (imgFileNameStr.isEmpty()) {
        return;
    }
    QFile file(imgFileNameStr);
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    file.close();

    if (this->qrCodeDataGet(ba)) {
        return;
    }
    Toast::Instance()->setError("未识别出二维码");
}

bool MainWindow::qrCodeDataGet(QByteArray ba)
{
    QrcodePngToText_Resp resp= QrcodePngToText(ba.toStdString());
    if (!resp.ErrMsg.empty()) {
        return false;
    }
    ui->plainTextEdit_ScanQrCode2_text->setPlainText(QString::fromStdString(resp.Text));
    ui->stackedWidget->setCurrentWidget(ui->page_ScanQrCode2);
    m_timer->stop();
    return true;
}

void MainWindow::slot_reloadCamera()
{
    QByteArray deviceName = ui->comboBox_ScanQrCode->currentData().value<QByteArray>();
    if (deviceName.isEmpty()) {
        return;
    }
    this->stopCamera();
    m_camera=new QCamera(deviceName, this);
    m_viewfinder=new QCameraViewfinder(this);
    m_imageCapture=new QCameraImageCapture(m_camera);
    m_timer = new QTimer(this);

    ui->verticalLayout_ScanQrCode->addWidget(m_viewfinder);

    m_camera->setViewfinder(m_viewfinder);
    m_camera->start();
    connect(m_imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(slot_imageCaptured(int,QImage)));
    m_camera->start();
    m_timer->start(100);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timeout_for_qrCode()));
}

void MainWindow::slot_timeout_for_qrCode()
{
    if (m_imageCapture == NULL) {
        return;
    }
    m_imageCapture->capture();
}

void MainWindow::slot_imageCaptured(int, const QImage &preview)
{
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    preview.save(&buffer, "PNG");
    this->qrCodeDataGet(ba);
}

void MainWindow::on_pushButton_ScanQrCode2_continue_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_ScanQrCode);
    m_timer->start();
}

void MainWindow::stopCamera()
{
    if (m_camera != NULL) {
        m_camera->stop();
        m_camera->deleteLater();
        m_camera = NULL;
    }
    if (m_imageCapture != NULL) {
        m_imageCapture->deleteLater();
        m_imageCapture = NULL;
    }
    if (m_viewfinder != NULL) {
        m_viewfinder->deleteLater();
        m_viewfinder = NULL;
    }
    if (m_timer != NULL) {
        m_timer->deleteLater();
        m_timer = NULL;
    }
}

void MainWindow::on_pushButton_CreateQrCode2_saveToFile_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "",  "", "图片文件(*.jpg;*.png)");
    if (fileName.isEmpty()) {
        return;
    }
    QPixmap pm = ui->label_CreateQrCode2->pixmap()->copy();
    QImage img = pm.toImage();
    bool ok = img.save(fileName);
    if (!ok) {
        Toast::Instance()->setError(QString("保存文件失败"));
        return;
    }
    Toast::Instance()->setSuccess(QString("保存成功"));
}
