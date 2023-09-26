#include "FunctioanlDependencyStrategy.h"

void FunctionalDependencyStrategy::ensureInitialized(const SearchSpace& searchSpace) {
    
}
DependencyCandidate FunctionalDependencyStrategy::createDependencyCandidate(const std::shared_ptr<Vertical>& candidate) {

}

double FunctionalDependencyStrategy::calculateError(const std::shared_ptr<Vertical>& candidate) {

}

void FunctionalDependencyStrategy::registerDependency(const std::shared_ptr<Vertical>& vertical, double error, std::vector<PartialFD>& discoveryUnit) {

}
bool FunctionalDependencyStrategy::shouldResample(const std::shared_ptr<Vertical>& vertical, double boostFactor) {
    if (context.configuration.sampleSize <= 0 || vertical.getArity() < 1) return false;

        // Do we have an exact sample already?
        AgreeSetSample currentSample = context.getAgreeSetSample(vertical);
        if (currentSample.isExact()) return false;

        // Get an estimate of the number of equality pairs in the vertical.
        PositionListIndex pli = context.pliCache.get(vertical);
        double nep = pli != null ?
                pli.getNep() :
                currentSample.estimateAgreements(vertical) * context.getRelationData().getNumTuplePairs();

        // Would the new sample be exact?
        if (nep <= context.configuration.sampleSize * boostFactor) return true;

        // Will we achieve an improved sampling ratio?
        double newSamplingRatio = context.configuration.sampleSize * boostFactor / nep;
        return newSamplingRatio >= 2 * currentSample.getSamplingRatio();
}
bool FunctionalDependencyStrategy::isIrrelevantColumn(size_t columnIndex) {

} 

int FunctionalDependencyStrategy::getNumIrrelevantColumns() {

}
std::shared_ptr<Vertical> FunctionalDependencyStrategy::getIrrelevantColumns() {

}