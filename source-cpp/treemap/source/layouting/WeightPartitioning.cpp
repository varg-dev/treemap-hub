
#include <treemap/layouting/WeightPartitioning.h>

#include <numeric>
#include <cmath>

std::vector<LinearizedTreeNode::NodeRange> partitionMinMax(const LinearizedTreeNode::NodeRange & range, const LinearizedBuffer<float> &weights)
{
    using precision_type = double;
    const auto numCuts = 4;

    std::vector<precision_type> prefix(range.size());
    std::partial_sum(weights.begin() + range.first->index(), weights.begin() + range.first->index() + range.size(), prefix.begin());

    const auto sum = prefix.back();

    constexpr auto threshold = 1.0f;
    const auto prefixBegin = prefix.begin();
    const auto prefixEnd = prefix.end();

    std::vector<LinearizedTreeNode::NodeRange> cuts; // end iterator for each cut (cut before this index)
    const auto eval = [& range, &cuts, &prefix, prefixBegin, prefixEnd] (precision_type partLimit)
    {
        cuts.clear();
        auto currentEnd = prefixBegin;
        for (int i = 0; i < numCuts && currentEnd != prefixEnd; ++i) // find numCuts-1 cuts
        {
            const auto currentBegin = currentEnd;
            currentEnd = std::upper_bound(currentBegin, prefixEnd, partLimit + *(currentEnd - 1));
            const auto nodeBegin = range.first + (currentBegin - prefixBegin);
            cuts.emplace_back(nodeBegin, nodeBegin + (currentEnd - currentBegin));
        }
        return currentEnd == prefixEnd;
    };

    precision_type low = sum / numCuts - threshold;
    precision_type high = sum;
    std::vector<LinearizedTreeNode::NodeRange> bestCutsNodes;
    while(high - low > threshold) // precision of binary search
    {
        const auto mid = (high+low)/2.0f;

        if (eval(mid))
        {
            high = mid;
            // Store currently best result for later
            bestCutsNodes = cuts;
        }
        else
        {
            low = mid;
        }
    }

    return bestCutsNodes;
}

std::vector<LinearizedTreeNode::NodeRange> partitionVariance(const LinearizedTreeNode::NodeRange & range, const LinearizedBuffer<float> &weights)
{
    std::vector<double> prefix(range.size()+1, 0.0); // exclusive prefix sum
    std::partial_sum(weights.begin() + range.first->index(), weights.begin() + range.first->index() + range.size(), prefix.begin()+1);

    int n = range.size();
    auto mean = prefix.back() / 4.0;
    auto diff = [&prefix](int b, int c, int e) { // unbalance of bisection at point m of array [b..e)
        return fabs(prefix[c] - (prefix[e]+prefix[b])/2.0);
    };

    double best = std::numeric_limits<double>::infinity();
    std::vector<LinearizedTreeNode::NodeRange> cuts{range}; // end iterator for each cut (cut before this index)
    for(int l=1, m=2, r=3; m<n-1; m++) {
        while(diff(0,l+1,m) < diff(0,l,m)) l++;
        while(diff(m,r+1,n) < diff(m,r,n)) r++;
        assert(0<l&&l<m&&m<r&&r<n);
        auto s0 = prefix[l] - prefix[0] - mean;
        auto s1 = prefix[m] - prefix[l] - mean;
        auto s2 = prefix[r] - prefix[m] - mean;
        auto s3 = prefix[n] - prefix[r] - mean;
        auto cur = s0*s0 + s1*s1 + s2*s2 + s3*s3;
        if (cur < best) {
            best = cur;
            cuts = {{range.first,     range.first + l},
                    {range.first + l, range.first + m},
                    {range.first + m, range.first + r},
                    {range.first + r, range.last},
            };
        }
    }

    return cuts;
}


std::vector<LinearizedTreeNode::NodeRange> partitionGreedy(const LinearizedTreeNode::NodeRange & range, const LinearizedBuffer<float> &weights) {

    int n = range.size();
    assert(n>=4);

    auto OPT = accumulate(weights.begin() + range.first->index(), weights.begin() + range.first->index() + range.size(), 0.0) / 4.0;

    auto last = 0;
    double sum = weights[range.first->index() + 0];
    std::vector<LinearizedTreeNode::NodeRange> cuts;
    for(int cur = 1; cur<n && cuts.size()<3; ++cur) {
        if(n-cur == 4-cuts.size()-1) { // just enough items to make 4 cuts
            cuts.emplace_back(range.first + last, range.first + cur); // make cut before cur
            last = cur;
            continue;
        }

        auto w = weights[range.first->index() + cur];
        if(fabs(sum-OPT) > fabs(sum+w-OPT)) { // extend current quadrant
            sum += w;
        } else { // make cut before cur
            sum = w;
            cuts.emplace_back(range.first + last, range.first + cur);
            last = cur;
        }
    }

    cuts.emplace_back(range.first + last, range.last);

    assert(cuts.size()==4);

    return cuts;
}
