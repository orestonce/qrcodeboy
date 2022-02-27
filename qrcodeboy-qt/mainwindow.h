#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QCamera;
class QCameraViewfinder;
class QCameraImageCapture;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    bool qrCodeDataGet(QByteArray ba);
    void stopCamera();
private slots:
    void on_pushButton_CreateQrCode_clicked();

    void on_actionCreateQrCode_triggered();

    void on_actionScanQrCode_triggered();

    void on_pushButton_CreateQrCode2_continue_clicked();

    void on_pushButton_ScanQrCode_clicked();

    void on_pushButton_ScanQrCode2_continue_clicked();

    void slot_imageCaptured(int, const QImage &preview);
    void slot_timeout_for_qrCode();
    void slot_reloadCamera();
    void on_pushButton_CreateQrCode2_saveToFile_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *m_timer;
    QCamera *m_camera;
    QCameraViewfinder *m_viewfinder;
    QCameraImageCapture *m_imageCapture;
};

#endif // MAINWINDOW_H
