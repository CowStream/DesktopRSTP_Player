#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMutex>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QPushButton>
#include <QBoxLayout>
#include <QLCDNumber>

/**
 * FFMPEG LIB
 */
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>
#include <libavformat/version.h>
#include <libavutil/time.h>
#include <libavutil/mathematics.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setUrl(QString url);
    void startStream();
    void stopStream();

private:
    Ui::MainWindow *ui;

    QMutex mutex;
    AVPicture  pAVPicture;
    AVFormatContext *pAVFormatContext;
    AVCodecContext *pAVCodecContext;
    AVFrame *pAVFrame;
    SwsContext *pSwsContext;
    AVPacket pAVPacket;

    /* timer ctrl temp */
    QTimer *m_timerPlay;
    int minRecord;
    int secRecord;


    int m_i_frameFinished;
    int videoStreamIndex;
    int m_i_w;
    int m_i_h;
    QLabel *m_label;
    QString m_str_url;
    int videoWidth;
    int videoHeight;

    /* UI */
    QPushButton *PlayButton;
    QLabel *playScreen;
    QLCDNumber *minLCD;
    QLCDNumber *secLCD;
    QImage *frame;

    bool Init();

signals:
    void GetImage(QImage image);

private slots:
    void SetImageSlots(const QImage &image);
    void playSlots();
    void on_start_pushButton_clicked();
    void on_stop_pushButton_clicked();

    /* timer ctrl */
    void updateRecordTimer();
};
#endif // MAINWINDOW_H
