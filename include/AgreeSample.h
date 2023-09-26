#ifndef AGREE_SAMPLE_H
#define AGREE_SAMPLE_H

#include "ConfidenceInterval.h"
#include "PLICache.h"
#include "Trie.h"
#include "Vertical.h"
#include <memory>
#include <unordered_set>
class AgreeSetSample {
    double stdDevSmoothing = 1;
protected:
    RelationalData& relationalData;
    std::shared_ptr<Vertical> focus;
    long populationSize;
    double obseverationsToRelationRatio(double numObservations);
    double ratioToRelationRatio(double ratio);
    static double calculateNonNegativeFraction(double a, double b);


public:
    int sampleSize;

    AgreeSetSample(RelationalData& relationData, std::shared_ptr<Vertical> focus, int sampleSize, long populationSize);
    AgreeSetSample(RelationalData& relationData, std::shared_ptr<Vertical> focus, int sampleSize, long populationSize, std::unordered_set<BitSet> agreeSetCounter);

    AgreeSetSample create(RelationalData& relationData, const std::shared_ptr<Vertical>& focus, int sampleSize, long populationSize,
        std::unordered_set<BitSet> agreeSetCounter);
    AgreeSetSample createFor(RelationalData& relationData, int sampleSize);
    AgreeSetSample createFor(RelationalData& relation, double samplingFactor)
;    AgreeSetSample createFocusedFor(RelationalData& relationData, const std::shared_ptr<Vertical>& restrictionVertical,
        std::shared_ptr<PLI> restrictionPli, double samplingFactor);
    AgreeSetSample createFocusedFor(RelationalData& relationData, const std::shared_ptr<Vertical>& restrictionVertical,
        std::shared_ptr<PLI> restrictionPli, int sampleSize);
    long getNumAgreeSupersets(const std::shared_ptr<Vertical>& agreement);
    long getNumAgreeSupersets(const std::shared_ptr<Vertical>& agreement, const std::shared_ptr<Vertical>& disagreement);
    std::vector<long> getNumAgreeSupersetsExt(const std::shared_ptr<Vertical>& agreement, const std::shared_ptr<Vertical>& disagreement);
    double estimateAgreements(const std::shared_ptr<Vertical>& agreement);
    ConfidenceInterval estimateAgreements(const std::shared_ptr<Vertical>& agreement, double confidence);
    double estimateMixed(const std::shared_ptr<Vertical>& agreement, const std::shared_ptr<Vertical>& disagreement);
    ConfidenceInterval estimateMixed(const std::shared_ptr<Vertical>& agreement, const std::shared_ptr<Vertical>& disagreement, double confidence);
    double estimateConditionalDisagreements(const std::shared_ptr<Vertical>& agreement, const std::shared_ptr<Vertical>& disagreement);
    ConfidenceInterval estimateConditionalDisagreements(const std::shared_ptr<Vertical>& agreement, const std::shared_ptr<Vertical>& disagreement, double confidence);
    double getFocusSelectivity();
    double getSamplingRatio();
    std::shared_ptr<Vertical> getFocus();
    bool isExact();
    long getPopulationSize();
};

#endif