#include <torch/torch.h>


torch::Tensor stateProbsUpdate(torch::Tensor stateProbs, torch::Tensor times) {
    return 2 * stateProbs;
}

int main() {
    torch::Tensor tensor = torch::rand({2, 2});
    for (int i = 0; i <= 10; i++) {
        tensor = stateProbsUpdate(tensor, tensor);
    }
    std::cout << tensor << std::endl;
    return 0;
}