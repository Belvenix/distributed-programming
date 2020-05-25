package pl.PR.multiThreading;

import java.lang.Math;

public class ThreadExample extends Thread {

    private Job task;
    private MyExecutorService executor;

    public ThreadExample(MyExecutorService mes){
        this.task = null;
        this.executor = mes;
    }

    public void run(){
        do {
            task = this.executor.popTask();
            if (task != null){
                task.calculate();
            }
        }while(task != null);
    }

}
