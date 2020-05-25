package pl.PR.multiThreading;

import java.lang.Math;

public class RunnableExample implements Runnable{

    private double min, max, step;
    private Outcome out;

    public RunnableExample(double minT, double maxT, double stepT, Outcome out){
        this.min = minT;
        this.max = maxT;
        this.step = stepT;
        this.out = out;
    }

    @Override
    public void run() {
        double sum = 0;
        for (double d = min; d < max; d+= step){
            if(d + step <= max){
                double rect = Math.sin(d + step/2) * step;
                sum += rect;
            }
            else{
                double dif = max - d;
                double rect = Math.sin(d + dif/2) * dif;
                sum += rect;
            }
        }
        out.addOutcome(sum);
        System.out.println("Sum equals to: " + String.valueOf(sum) +
                ", lower limit for this thread: " + String.valueOf(min) +
                ", higher limit for this thread: " + String.valueOf(max));
    }

}
