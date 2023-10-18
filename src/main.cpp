#include <torch/torch.h>

struct Dimensions {
    double spacestep;
};

struct Axes {
    int space;
    int time;
};

class SecondOrderProbsState {
public:
    SecondOrderProbsState(torch::Tensor initialProbs, Axes axesSetup) : 
        probs(initialProbs), axes(axesSetup) {}

    void update(Dimensions dims, torch::Tensor conditionalProbs) {
        auto numberOfTimesteps = double(probs.size(axes.time));
        probs += (
            (dims.spacestep / numberOfTimesteps) * torch::tensordot(
                probs, 
                conditionalProbs, 
                {axes.space, axes.time}, 
                {axes.space + 2, axes.time + 2}
            ) - probs
        );
    }

    torch::Tensor getProbs() const {
        return probs;
    }
private:
    torch::Tensor probs;
    const Axes axes;
};

class ThirdOrderProbsState : public SecondOrderProbsState {
public:
    ThirdOrderProbsState(
        torch::Tensor initialProbs, 
        torch::Tensor initialConditionalProbs,
        Axes axesSetup
    ) : SecondOrderProbsState(initialProbs, axesSetup), 
        conditionalProbs(initialConditionalProbs),
        axes(axesSetup) {}

    void update(Dimensions dims) {
        SecondOrderProbsState::update(dims, conditionalProbs);
    }

    torch::Tensor getProbs() const {
        return SecondOrderProbsState::getProbs();
    }

    torch::Tensor getConditionalProbs() const {
        return conditionalProbs;
    }
private:
    torch::Tensor conditionalProbs;
    const Axes axes;
};


int main() {
    Dimensions dims;
    dims.spacestep = 1.0;
    Axes axes;
    axes.space = 0;
    axes.time = 1;
    torch::Tensor tensor = torch::rand({2, 2});
    ThirdOrderProbsState state(tensor, tensor, axes);
    for (int i = 0; i <= 10; i++) {
        state.update(dims);
    }
    std::cout << state.getProbs() << std::endl;
    return 0;
}