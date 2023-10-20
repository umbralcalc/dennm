#include <torch/torch.h>

class SecondOrderProbsState {
public:
    static const int SPACE_AXIS = 0;
    static const int TIME_AXIS = 1;

    SecondOrderProbsState(torch::Tensor initialProbs, double spaceStepsize) {
        probs = torch::ones({0, 1});
        probs = torch::cat({probs, initialProbs}, TIME_AXIS); 
        spaceStep = spaceStepsize;
    }

    void update(torch::Tensor conditionalProbs) {
        auto numberOfTimesteps = double(probs.size(TIME_AXIS));
        torch::Tensor nextProbs = (
            (spaceStep / numberOfTimesteps) * torch::tensordot(
                probs, 
                conditionalProbs, 
                {SPACE_AXIS, TIME_AXIS}, 
                {SPACE_AXIS + 2, TIME_AXIS + 2}
            ) - probs
        );
        probs = torch::cat({probs, nextProbs}, TIME_AXIS);
    }

    torch::Tensor getProbs() const {
        return probs;
    }
private:
    torch::Tensor probs;
    double spaceStep;
};

class ThirdOrderProbsState : public SecondOrderProbsState {
public:
    ThirdOrderProbsState(
        torch::Tensor initialProbs, 
        torch::Tensor initialConditionalProbs,
        double spaceStepsize
    ) : SecondOrderProbsState(initialProbs, spaceStepsize), 
        conditionalProbs(initialConditionalProbs) {
        spaceStep = spaceStepsize;
    }

    void update(torch::Tensor twiceConditionalProbs) {
        SecondOrderProbsState::update(conditionalProbs);
        auto numberOfTimesteps = double(conditionalProbs.size(SecondOrderProbsState::TIME_AXIS));
        torch::Tensor nextConditionalProbs = (
            (spaceStep / numberOfTimesteps) * torch::tensordot(
                conditionalProbs, 
                twiceConditionalProbs, 
                {SecondOrderProbsState::SPACE_AXIS, SecondOrderProbsState::TIME_AXIS}, 
                {SecondOrderProbsState::SPACE_AXIS + 2, SecondOrderProbsState::TIME_AXIS + 2}
            ) - conditionalProbs
        );
        conditionalProbs = torch::cat(
            {conditionalProbs, nextConditionalProbs}, 
            SecondOrderProbsState::TIME_AXIS
        );
    }

    torch::Tensor getProbs() const {
        return SecondOrderProbsState::getProbs();
    }

    torch::Tensor getConditionalProbs() const {
        return conditionalProbs;
    }
private:
    torch::Tensor conditionalProbs;
    double spaceStep;
};


int main() {
    double spaceStepsize = 1.0;
    torch::Tensor tensor = torch::rand({2, 2});
    ThirdOrderProbsState state(tensor, tensor, spaceStepsize);
    for (int i = 0; i <= 10; i++) {
        state.update(tensor);
    }
    std::cout << state.getProbs() << std::endl;
    return 0;
}