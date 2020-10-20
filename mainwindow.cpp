#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    /**
      * @author marktlen
      * @brief Palyer UI set up
      */

    uchar* pp = (unsigned char *)malloc(320 * 240/*QWidget::width()*QWidget::height()*/* 3 * sizeof(char));
    PlayButton = new QPushButton(this);
    frame = new QImage(pp,320,240,QImage::Format_RGB888);
    playScreen->setPixmap(QPixmap::fromImage(*frame,Qt::AutoColor)); //label 打印图片


    /**
      * @date 20-7-14 @author marktlen
      * @brief 录制时间计算
      */
    m_timerPlay = new QTimer(this); //播放时间
    connect(m_timerPlay,SIGNAL(timeout()),this,SLOT(updateRecordTimer()));

    minLCD = new QLCDNumber(2); //只显示两位
    secLCD = new QLCDNumber(2);
    minRecord = 0;              //初始化时间
    secRecord = 0;
    QLabel *timepoint = new QLabel(":");

    QHBoxLayout *timerHLayout = new QHBoxLayout();
    timerHLayout->addWidget(minLCD);
    timerHLayout->addWidget(timepoint);
    timerHLayout->addWidget(secLCD);


    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(playScreen);
    vLayout->addLayout(timerHLayout);

    //lines up laber and buttons
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(timerHLayout);
    mainLayout->addLayout(vLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("RTSP Player"));



    ui->setupUi(this);
}

/**
  * @date 20-7-14 @author marktlen
  * @brief 录制时钟刷新槽函数，超时触发
  */
void MainWindow::updateRecordTimer()
{
    secRecord++;
    if(secRecord >= 60*10)
    {
        secRecord = 0;
        minRecord++;
    }
    minLCD->display(minRecord);
    secLCD->display(secRecord/10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::setUrl set the source of Stream
 * @param url input a RTSP url
 */
void MainWindow::setUrl(QString url)
{
    m_str_url = url;
}

void MainWindow::startStream()
{
    videoStreamIndex=-1;
    av_register_all();      //注册库中所有可用的文件格式和解码器
    avformat_network_init();//初始化网络流格式,使用RTSP网络流时必须先执行

    //申请一个AVFormatContext结构的内存,并进行简单初始化
    pAVFormatContext = avformat_alloc_context();
    pAVFrame=av_frame_alloc();
    if(this->Init())
    {
        m_timerPlay->start();
    }
}

void MainWindow::stopStream()
{
    m_timerPlay->stop();

    avformat_close_input(&pAVFormatContext);
    avformat_free_context(pAVFormatContext);
    av_frame_free(&pAVFrame);
    sws_freeContext(pSwsContext);
}

bool MainWindow::Init()
{
    if(m_str_url.isEmpty())
        return false;

    // open input video stream
    int result = avformat_open_input(&pAVFormatContext, m_str_url.toStdString().c_str(), nullptr, nullptr);
    if(result < 0)
    {
        qDebug() << "打开视频流失败";
        return false;
    }

    // get video stream info
    result = avformat_find_stream_info(pAVFormatContext, nullptr);
    if(result < 0)
    {
        qDebug() << "获取视频流信息失败";
        return  false;
    }

    // get video stream index
    videoStreamIndex = -1;
    for (uint i = 0; i < pAVFormatContext->nb_streams; i++)
    {
        if (pAVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStreamIndex = i;
            break;
        }
    }
    if (videoStreamIndex == -1)
    {
        qDebug() << "获取视频流索引失败";
        return false;
    }

    // get the resolution size of video stream
    pAVCodecContext = pAVFormatContext->streams[videoStreamIndex]->codec;
    videoWidth = pAVCodecContext->width;
    videoHeight = pAVCodecContext->height;

    m_i_w = videoWidth;
    m_i_h = videoHeight;

    avpicture_alloc(&pAVPicture, AV_PIX_FMT_RGB24, videoWidth, videoHeight);
    AVCodec *pAVCodec;

    // get video stream decoder
    pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);
    pSwsContext = sws_getContext(videoWidth, videoHeight, AV_PIX_FMT_YUV420P, videoWidth, videoHeight, AV_PIX_FMT_RGB24, SWS_BICUBIC, 0, 0, 0);

    // open video stream decoder
    result = avcodec_open2(pAVCodecContext, pAVCodec, NULL);
    if (result < 0)
    {
        qDebug() << "解码器启动失败";
        return false;
    }

    qDebug() << "初始化视频流成功";
    return true;
}

void MainWindow::playSlots()
{
    // Read video stream frame by frame
    if (av_read_frame(pAVFormatContext, &pAVPacket) >= 0)
    {
        if (pAVPacket.stream_index == videoStreamIndex)
        {
            qDebug() << "开始解码" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            avcodec_decode_video2(pAVCodecContext, pAVFrame, &m_i_frameFinished, &pAVPacket);
            if (m_i_frameFinished)
            {
                mutex.lock();
                sws_scale(pSwsContext, (const uint8_t *const *)pAVFrame->data, pAVFrame->linesize, 0, videoHeight, pAVPicture.data, pAVPicture.linesize);

                // send a frame to UI
                QImage image(pAVPicture.data[0], videoWidth, videoHeight, QImage::Format_RGB888);
                emit GetImage(image);
                mutex.unlock();
            }
        }
    }
    // free memory resources
    av_free_packet(&pAVPacket);
}

void MainWindow::SetImageSlots(const QImage &image)
{
    if(image.height()>0)
    {
        QPixmap pix = QPixmap::fromImage(image.scaled(m_i_w,m_i_h));
        m_label->setPixmap(pix);
    }
}

void MainWindow::on_start_pushButton_clicked()
{

}

void MainWindow::on_stop_pushButton_clicked()
{

}
