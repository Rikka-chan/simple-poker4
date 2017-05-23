#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "montethread.h"
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QList>
#include <QTimer>


//enum status {RUNNING, STOPED, PAUSE};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
private:
    QWidget* centralWidget;
    QVBoxLayout* main_layout;
    QTimer* timer;
    ulong inner_count;
    ulong iters;
    int radius, threads_count, finished_count;
    ulong points;
private slots:
    void update_pi(ulong, ulong);
    void start();
    void stop();
    void reset();
    void thread_finished();
    void radius_changed(QString);
    void threads_changed(QString);
    void points_changed(QString);
signals:
    void deleteThreads();
    void finished(bool);
    void new_pi(QString);
};

#endif // MAINWINDOW_H
