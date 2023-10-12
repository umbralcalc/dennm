#include <torch/torch.h>

int main() {
    torch::Tensor tensor = torch::rand({2, 2});
    std::cout << tensor << std::endl;
    return 0;
}