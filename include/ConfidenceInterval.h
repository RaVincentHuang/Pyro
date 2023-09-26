#ifndef CONFIDENCE_INTERVAL_H
#define CONFIDENCE_INTERVAL_H

class ConfidenceInterval {
    double min, mean, max;
public:
    explicit ConfidenceInterval(double value)
        : min(value), mean(value), max(value) {}
    
    ConfidenceInterval(double min, double mean, double max)
        : min(min), mean(mean), max(max) {}
    
    double getMin() const {
        return min;
    }
    
    double getMax() const {
        return max;
    }

    double getMean() const {
        return (min + max) / 2;
    }

    double get() const {
        return mean;
    }

    ConfidenceInterval multiply(double scalar) const {
        return {min * scalar, mean * scalar, max * scalar};
    }

    bool isPoint() const {
        return min == max;
    }

    bool operator==(const ConfidenceInterval& that) const {
        return min == that.min && mean == that.mean && max == that.max;
    }
};

#endif