package pl.PR.multiThreading;

import java.util.ArrayList;

public class MyExecutorService {
    ArrayList<Job> tasks = new ArrayList<>();
    ArrayList<ThreadExample> threads = new ArrayList<>();

    public MyExecutorService(int nThreads){
        for (int i = 0; i < nThreads; i++) {
            ThreadExample tmp = new ThreadExample(this);
            threads.add(tmp);
        }
    }

    public synchronized Job popTask(){
        if (!tasks.isEmpty() && tasks.size() > 0){
            Job ret = tasks.get(0);
            tasks.remove(0);
            return ret;
        }
        return null;
    }

    public synchronized void pushTask(Job task){
        this.tasks.add(task);
    }

    public void start(){
        for(ThreadExample t : threads){
            t.start();
        }

        for (ThreadExample t : threads){
            try{
                t.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
