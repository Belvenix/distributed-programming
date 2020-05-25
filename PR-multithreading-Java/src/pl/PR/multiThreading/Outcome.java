package pl.PR.multiThreading;

import java.util.ArrayList;

public class Outcome {
    ArrayList<Double> outcomes = new ArrayList<>();

    public synchronized ArrayList<Double> getOutcomes(){
        return outcomes;
    }

    public synchronized void addOutcome(double e){
        outcomes.add(e);
    }
}
