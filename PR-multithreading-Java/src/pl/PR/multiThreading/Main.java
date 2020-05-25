package pl.PR.multiThreading;

import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class Main {

    private static final Scanner SCANNER = new Scanner(System.in);
    private static final double STEP = 0.001;
    private static Outcome OutcomeObject = new Outcome();

    public static void main(String[] args){
        System.out.println("Welcome to Integral Calculator. Please specify whether you want to use Runnable or Thread class (r/t):");
        String type = SCANNER.next().trim().toLowerCase();
        if(!type.equals("r") && !type.equals("t")){
            type = "r";
        }
        System.out.println("Please specify how many threads you want to use:");
        int nThreads = SCANNER.nextInt();
        if (nThreads <= 0 || nThreads > 20){
            nThreads = 4;
        }
        System.out.println("Please specify how many segments you want");
        int segments = SCANNER.nextInt();
        System.out.println("Now please enter lower range limit");
        double minRange = SCANNER.nextDouble();
        System.out.println("Now please enter higher range limit");
        double maxRange = SCANNER.nextDouble();

        if(type.equals("r")){
            ExecutorService executorService = Executors.newFixedThreadPool(nThreads);
            ArrayList<RunnableExample> runnables = new ArrayList<>();
            for (int i=0; i < segments; i++)
            {
                double minT, maxT, stepT;
                minT = minRange + (i * ((maxRange-minRange)/segments));
                maxT = minRange + ((i + 1) * ((maxRange-minRange)/segments));
                stepT = STEP;
                RunnableExample r = new RunnableExample(minT, maxT, stepT, OutcomeObject);
                runnables.add(r);
                executorService.submit(r);
            }
            //Now we shutdown the threads - which are still running! hence we wait with the try catch clause
            executorService.shutdown();
            try {
                //Now we wait for the termination up to 60 seconds
                executorService.awaitTermination(60, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                executorService.shutdownNow();
            }

            double FinalSum = 0;
            for(double r : OutcomeObject.getOutcomes()){
                FinalSum += r;
            }

            System.out.println("The final sum for RunnableExample equals to: " + FinalSum);
        }
        else {
            MyExecutorService executorService = new MyExecutorService(nThreads);
            for (int i=0; i < segments; i++)
            {
                double minT, maxT, stepT;
                minT = minRange + (i * ((maxRange-minRange)/segments));
                maxT = minRange + ((i + 1) * ((maxRange-minRange)/segments));
                stepT = STEP;
                Job task = new Job(minT, maxT, stepT, OutcomeObject);
                executorService.pushTask(task);
            }
            executorService.start();
            double FinalSum = 0;
            for(double t : OutcomeObject.getOutcomes()){
                FinalSum += t;
            }
            System.out.println("The final sum for ThreadExample equals to: " + FinalSum);
        }
    }
}
