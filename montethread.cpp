#include "montethread.h"

void MonteThread::run(){
    ulong inner_points = 0;
    ulong last_inner_points = 0;
    ulong iters = 0;

    QTime some(0,0,0);
    qsrand(some.secsTo(QTime::currentTime()));

    for(ulong i=1; i <= points_count; i++){
        long x = (qrand() % (radius * 2)) - radius;
        long y = (qrand() % (radius * 2)) - radius;


        if(qPow(x, 2) + qPow(y, 2) <= qPow(radius, 2))
            inner_points++;

        if(i == points_count || i % 1000 == 0){
            emit iter(inner_points - last_inner_points, i - iters);
            iters = i;
            last_inner_points = inner_points;
        }
    }
}
