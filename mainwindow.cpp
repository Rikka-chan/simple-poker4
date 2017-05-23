#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    radius = 0;
    threads_count = 0;
    points = 0;
    finished_count = 0;
    inner_count = 0;
    iters = 0;

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    main_layout = new QVBoxLayout(centralWidget);

    QHBoxLayout* btn_layout = new QHBoxLayout();

    QPushButton* btn_start = new QPushButton("Start", this);
    btn_start->setAccessibleName("btn_start");

    QPushButton* btn_stop = new QPushButton("Stop", this);
    btn_start->setAccessibleName("btn_stop");

    QPushButton* btn_reset = new QPushButton("Reset", this);
    btn_start->setAccessibleName("btn_reset");

    btn_layout->addWidget(btn_start);
    btn_layout->addWidget(btn_stop);
    btn_layout->addWidget(btn_reset);

    QHBoxLayout* input_layout = new QHBoxLayout();

    QLineEdit* radius_number = new QLineEdit(this);
    radius_number->setValidator(new QIntValidator(100, 1000000, this));
    radius_number->setAccessibleName("radius_number");

    QLineEdit* points_number = new QLineEdit(this);
    points_number->setValidator(new QIntValidator(1000, 1000000000, this));
    points_number->setAccessibleName("points_number");

    QLineEdit* threads_number = new QLineEdit(this);
    threads_number->setValidator(new QIntValidator(1, 100, this));
    threads_number->setAccessibleName("thread_number");

    QLabel* radius_label = new QLabel("Radius", this);
    QLabel* points_label = new QLabel("Points", this);
    QLabel* threads_label = new QLabel("Threads", this);

    input_layout->addWidget(radius_number);
    input_layout->addWidget(radius_label);
    input_layout->addWidget(points_number);
    input_layout->addWidget(points_label);
    input_layout->addWidget(threads_number);
    input_layout->addWidget(threads_label);


    main_layout->addLayout(btn_layout);
    main_layout->addLayout(input_layout);
    main_layout->setAlignment(Qt::AlignTop);

    QLabel* pi_label = new QLabel(this);
    main_layout->addWidget(pi_label);

    connect(this, SIGNAL(new_pi(QString)), pi_label, SLOT(setText(QString)));

    connect(radius_number, SIGNAL(textChanged(QString)), this, SLOT(radius_changed(QString)));
    connect(threads_number, SIGNAL(textChanged(QString)), this, SLOT(threads_changed(QString)));
    connect(points_number, SIGNAL(textChanged(QString)), this, SLOT(points_changed(QString)));

    connect(btn_start, SIGNAL(clicked(bool)), this, SLOT(start()));
    connect(btn_start, SIGNAL(clicked(bool)), btn_start, SLOT(setEnabled(bool)));
    connect(btn_start, SIGNAL(clicked(bool)), radius_number, SLOT(setEnabled(bool)));
    connect(btn_start, SIGNAL(clicked(bool)), points_number, SLOT(setEnabled(bool)));
    connect(btn_start, SIGNAL(clicked(bool)), threads_number, SLOT(setEnabled(bool)));

    connect(btn_stop, SIGNAL(clicked(bool)), this, SLOT(stop()));

    connect(btn_reset, SIGNAL(clicked(bool)), this, SLOT(reset()));
    connect(btn_reset, SIGNAL(clicked(bool)), radius_number, SLOT(setDisabled(bool)));
    connect(btn_reset, SIGNAL(clicked(bool)), points_number, SLOT(setDisabled(bool)));
    connect(btn_reset, SIGNAL(clicked(bool)), threads_number, SLOT(setDisabled(bool)));
    connect(btn_reset, SIGNAL(clicked(bool)), btn_start, SLOT(setDisabled(bool)));
    connect(btn_reset, SIGNAL(clicked(bool)), btn_stop, SLOT(setDisabled(bool)));

    connect(this, SIGNAL(finished(bool)), btn_stop, SLOT(setDisabled(bool)));
}


void MainWindow::radius_changed(QString new_val){
    radius = new_val.toInt();
}

void MainWindow::threads_changed(QString new_val){
    threads_count = new_val.toInt();
}

void MainWindow::points_changed(QString new_val){
    points = new_val.toLong();
}

void MainWindow::start(){
    for(int i = 0; i < threads_count; i++){
        MonteThread* thread;
        if(points % threads_count != 0){
            thread = new MonteThread(points % threads_count,
                                     radius,
                                     this);
            points = points -(points % threads_count);
            i--;
            finished_count--;
        }
        else{
            thread = new MonteThread(points / threads_count,
                                                  radius,
                                                  this);
        }


        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), this, SLOT(thread_finished()));
        connect(thread, SIGNAL(iter(ulong, ulong)), this, SLOT(update_pi(ulong,ulong)));
        connect(this, SIGNAL(deleteThreads()), thread, SLOT(terminate()));
        thread->start();
    }
}

void MainWindow::stop(){
    emit deleteThreads();
}

void MainWindow::reset(){
    emit deleteThreads();
    iters = 0;
    inner_count = 0;
    emit new_pi("Iterations: 0\tPi: 0");
}

void MainWindow::thread_finished(){
    finished_count++;
    emit finished(true);
}

void MainWindow::update_pi(ulong inner, ulong iter){
    iters += iter;
    inner_count += inner;

    QString label_str = "Iterations: %1\tPi: %2";
    label_str = label_str.arg(iters).arg(QString::number((double)inner_count / iters * 4));
    emit new_pi(label_str);
}
