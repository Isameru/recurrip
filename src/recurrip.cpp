
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

namespace recurrip
{
    using Bit = bool;

    struct Block {
        int     m_index;
        int     m_cycle;
        int     m_length;
        double  m_score;
    };

    using BlockChain = std::vector<Block>;

    std::vector<Bit> ParseInputBits(std::string_view input) {
        std::vector<Bit> result;
        result.reserve(input.size());
        for (const auto c : input) {
            if (c == '0')
                result.push_back(0);
            else if (c == '1')
                result.push_back(1);
        }
        return result;
    }

    BlockChain FindRecurringPatterns(const std::vector<Bit>& inputBits, int maxHistory) {
        BlockChain result;
        std::vector<int> history;
        history.reserve(maxHistory);

        auto reduce = [&](int i, int j) {
            const int cycle = j + 1;
            if (history[j] >= cycle) {
                Block block;
                block.m_index   = i - history[j] - cycle;
                block.m_cycle   = cycle;
                block.m_length  = history[j] + cycle;
                block.m_score   = std::pow(static_cast<double>(block.m_length), 1.5) / static_cast<double>(cycle);

                result.push_back(block);
            }
            history[j] = 0;
        };

        // Perform the pattern matching.
        //
        for (int i = 1; i < inputBits.size(); ++i) {
            for (int j = 0; j < history.size(); ++j) {
                const int cycle = j + 1;
                if (inputBits[i] == inputBits[i - cycle]) {
                    ++history[j];
                }
                else {
                    reduce(i, j);
                }
            }

            if (history.size() < maxHistory) {
                history.push_back(inputBits[i] == inputBits[0]);
            }
        }

        for (int j = 0; j < history.size(); ++j) {
            reduce(inputBits.size(), j);
        }

        // Preserve the best patterns, get rid of overlapping ones.
        //
        std::sort(std::begin(result), std::end(result), [](const auto& a, const auto& b) { return a.m_score > b.m_score; });

        for (int k = 0; k < result.size(); ++k) {
            for (int l = k + 1; l < result.size(); ++l) {
                if (result[k].m_index < result[l].m_index + result[l].m_length &&
                    result[k].m_index + result[k].m_length > result[l].m_index)
                {
                    result.erase(std::begin(result) + l);
                    --l;
                }
            }
        }

        // Present the results sorted by index.
        //
        std::sort(std::begin(result), std::end(result), [](const auto& a, const auto& b) { return a.m_index < b.m_index; });

        return result;
    }

    void Print(std::ostream& out, const std::vector<Bit>& inputBits, int count, int indexFrom, int indexTo)
    {
        if (indexFrom > indexTo) return;
        out << '[' << count << ']' << ' ';
        for (int i = indexFrom; i <= indexTo; ++i) {
            out << inputBits[i] ? '1' : '0';
        }
        out << '\n';
    }

    void Print(std::ostream& out, const std::vector<Bit>& inputBits, BlockChain& blockChain)
    {
        int printIndex = 0;

        for (const auto& block : blockChain) {
            Print(out, inputBits, 1, printIndex, block.m_index - 1);
            printIndex = block.m_index;
            const auto count = block.m_length / block.m_cycle;
            Print(out, inputBits, count, printIndex, printIndex + block.m_cycle - 1);
            printIndex += block.m_cycle * count;
        }

        Print(out, inputBits, 1, printIndex, inputBits.size() - 1);
    }
}

int main(int argc, char* argv[])
{
    try {
        std::ifstream inputFile{argv[1]};
        auto fileContent = std::string{std::istreambuf_iterator<char>{inputFile}, std::istreambuf_iterator<char>{}};

        using namespace recurrip;

        auto bits = ParseInputBits(fileContent);
        auto patterns = FindRecurringPatterns(bits, 1024);

        Print(std::cout, bits, patterns);
        std::cout.flush();

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "error: " << ex.what() << std::endl;
        return -1;
    }
}
