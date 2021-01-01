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

#include <QFileDialog>
#include <QInputDialog>
#include <QDesktopServices>
#include <QUrl>

// qt vlc
#include <VLCQtWidgets/WidgetVideo.h>
#include <VLCQtWidgets/WidgetVolumeSlider.h>
#include <VLCQtWidgets/WidgetSeek.h>

//other window
#include "form.h"

/**
 * qt-vlc class
 */
class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

extern QString playURL;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Form *form_window;

    void setUrl(QString url);
    void startStream();
    void stopStream();

private:
    Ui::MainWindow *ui;


    /* VLC media */
    // A basic Instance manager for VLC-Qt library. It provides main instance controls.
    VlcInstance *vlc_Instance;
    // An abstract representation of a playable media
    VlcMedia *vlc_Media;
    // A basic MediaPlayer manager for VLC-Qt library. It provides main playback controls.
    VlcMediaPlayer *vlc_player;
//    // This is one of VLC-Qt GUI classes. It provides video display and mouse control.
//    VlcWidgetVideo *vlc_video;



//    /* UI */
//    //Top bar
//    QPushButton *openUrlButton;       // 打开url流
//    QPushButton *openLocalFileButton; // 打开本地文件

//    //player contrl bar
//    QPushButton *PlayButton;
//    VlcWidgetVolumeSlider *volume;
//    VlcWidgetSeek *seek;

    bool playButtonFlag;
    bool volumeButtonFlag;

    bool Init();

private slots:
    void openLocal();
    void openUrl();
    void playButtonStatus();
    void on_volumeButton_clicked();
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
