#ifndef MONTETHREAD_H
#define MONTETHREAD_H

#include <QThread>
#include <QtMath>
#include <QTime>
#include <QWidget>
#include <QDebug>
#include <QTimer>

class MonteThread : public QThread
{
    Q_OBJECT

public:
    MonteThread(ulong count=1000, ulong rad=1000000, QWidget* parent=nullptr)
        :points_count(count), radius(rad), QThread(parent){}
    void set_points_count(ulong count){points_count = count;}
    ulong get_points_count(){return points_count;}

protected:
    void run() Q_DECL_OVERRIDE;

private:
    QTimer* timer;
    bool is_pause;
    ulong points_count;
    ulong radius;

signals:
    void iter(ulong, ulong);
};

#endif // MONTETHREAD_H
