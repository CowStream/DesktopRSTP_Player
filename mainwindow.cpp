#include "mainwindow.h"
#include "ui_mainwindow.h"

// QT VLC
#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>

QString playURL = "NULL";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      vlc_Media(0)
{
    ui->setupUi(this);

    /**
     * @brief qt vlc module
     * @date 20-12-4
     */
    //A basic Instance manager for VLC-Qt library
    vlc_Instance = new VlcInstance(VlcCommon::args(), this);
    //A basic MediaPlayer manager for VLC-Qt library
    vlc_player = new VlcMediaPlayer(vlc_Instance);
    vlc_player->setVideoWidget(ui->vlc_video);

    ui->vlc_video->setMediaPlayer(vlc_player);
    ui->volume->setMediaPlayer(vlc_player);
    ui->volume->setVolume(50);
    ui->seek->setMediaPlayer(vlc_player);

    setWindowTitle(tr("RTSP Player"));

    /**
      * @author marktlen
      * @brief Action and Button connect to slot
      */
    // menu bar
    connect(ui->actionOpenLocalFile,&QAction::triggered,this,&MainWindow::openLocal);
    connect(ui->actionOpenURL,&QAction::triggered,this,&MainWindow::openUrl);
    connect(ui->actionPause, &QAction::toggled, vlc_player, &VlcMediaPlayer::togglePause);
    connect(ui->actionStop, &QAction::triggered, vlc_player, &VlcMediaPlayer::stop);

    // top bar
    connect(ui->openLocalFileButton,&QPushButton::clicked,this,&MainWindow::openLocal);
    connect(ui->openURLButton,&QPushButton::clicked,this,&MainWindow::openUrl);

    //player bar
    connect(ui->PlayButton,&QPushButton::clicked,this,&MainWindow::playButtonStatus);


    /**
      * @brief Button Init
      */
    ui->PlayButton->setIcon(QIcon(":/icon/play.ico"));
    playButtonFlag = true;
    volumeButtonFlag = false;
    ui->volume->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete vlc_player;
    delete vlc_Media;
    delete vlc_Instance;
    delete ui;
}

void MainWindow::playButtonStatus()
{
    playButtonFlag = !playButtonFlag;
    if(playButtonFlag)
    {
        ui->PlayButton->setIcon(QIcon(":/icon/pause.ico"));
        vlc_player->play();
    }
    else
    {
        ui->PlayButton->setIcon(QIcon(":/icon/play.ico"));
        vlc_player->pause();
    }
}

/**
 * @brief MainWindow::setUrl set the source of Stream
 * @param url input a RTSP url
 */

void MainWindow::openLocal()
{
    QString file =
            QFileDialog::getOpenFileName(this, tr("Open file"),
                                         QDir::homePath(),
                                         tr("Multimedia files(*)"));

    if (file.isEmpty())
        return;

    vlc_Media = new VlcMedia(file, true, vlc_Instance);
    vlc_player->open(vlc_Media);
}

void MainWindow::openUrl()
{
    QString url;

    if (url.isEmpty() && playURL.isEmpty())
        return;
    else
    {
        url = QInputDialog::getText(this, tr("Open Url"), tr("Enter the URL you want to play"));
    }

    vlc_Media = new VlcMedia(url, vlc_Instance);

    vlc_player->open(vlc_Media);
}

void MainWindow::on_volumeButton_clicked()
{
    volumeButtonFlag = !volumeButtonFlag;
    if(volumeButtonFlag)
    {
        ui->videoLayout->setStretch(2,10);
        ui->volume->setVisible(true);
    }
    else
    {
        ui->videoLayout->setStretch(2,0);
        ui->volume->setVisible(false);
    }

}

void MainWindow::on_pushButton_clicked()
{
    form_window = new Form;
    form_window->show();
}
