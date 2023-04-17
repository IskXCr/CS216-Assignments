#include <ranges>
#include <vector>

std::vector<int> filter_view(const std::vector<int>& input, int threshold) {
    auto filtered = input | std::views::filter([threshold](int value) { return value > threshold; });
    return {filtered.begin(), filtered.end()};
}

int main()
{
    return 0;
}