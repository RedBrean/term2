#include <iostream>
#include <memory>

template <typename element_t>
struct Sequence {
    virtual element_t next() = 0;
    virtual bool has_next() const = 0;
    virtual ~Sequence() = default;
};

template <typename element_t>
struct MergeStrategy {
    virtual bool check_order(element_t const &a, element_t const &b) = 0;
    virtual ~MergeStrategy() = default;
};

struct IntegerGrowingSequence : public Sequence<int> {
    IntegerGrowingSequence(int first, unsigned count) : current(first), count(count) {}

    int next() override {
        if (count == 0)
            throw std::out_of_range("No more elements.");
        --count;
        return current++;
    }

    bool has_next() const override {
        return count != 0;
    }

private:
    int current;
    unsigned count;
};

template <typename element_t>
struct SwitchStrategy final : public MergeStrategy<element_t> {
    bool check_order(element_t const &a, element_t const &b) override {
        return (is_a = !is_a);  // Toggle between true and false
    }

    SwitchStrategy(bool left_first) : is_a(!left_first) {}

private:
    bool is_a;
};

// Ваш код для слияния последовательностей
template <typename element_t>
class MyMerge : public Sequence<element_t> {
private:
    const std::unique_ptr<Sequence<element_t>> a;
    const std::unique_ptr<Sequence<element_t>> b;
    const std::unique_ptr<MergeStrategy<element_t>> strategy;
    element_t next_a;
    element_t next_b;
    bool has_next_a;
    bool has_next_b;

    void prepare_a() {
        has_next_a = a->has_next();
        if (has_next_a) {
            next_a = a->next();
        }
    }

    void prepare_b() {
        has_next_b = b->has_next();
        if (has_next_b) {
            next_b = b->next();
        }
    }

    void return_a()
    {
        auto next_el = next_a;
        prepare_a();
        return next_el;
    }

    void return_b()
    {
        auto next_el = next_b;
        prepare_b();
        return next_el;
    }

public:
    MyMerge(std::unique_ptr<Sequence<element_t>> a, std::unique_ptr<Sequence<element_t>> b,
            std::unique_ptr<MergeStrategy<element_t>> strategy)
        : a(std::move(a)), b(std::move(b)), strategy(std::move(strategy)) {
        prepare_a();
        prepare_b();
    }

    element_t next() override {
        if (!has_next()) {
            throw std::out_of_range("No more elements.");
        }

        if (!has_next_a) {
            element_t result = next_b;
            prepare_b();
            return result;
        } else if (!has_next_b) {
            element_t result = next_a;
            prepare_a();
            return result;
        }

        if (strategy->check_order(next_a, next_b)) {
            element_t result = next_a;
            prepare_a();
            return result;
        } else {
            element_t result = next_b;
            prepare_b();
            return result;
        }
    }

    bool has_next() const override {
        return has_next_a || has_next_b;
    }
};

template <typename element_t>
std::unique_ptr<Sequence<element_t>> merge_by(
    std::unique_ptr<Sequence<element_t>> a,
    std::unique_ptr<Sequence<element_t>> b,
    std::unique_ptr<MergeStrategy<element_t>> strategy) {

    return std::make_unique<MyMerge<element_t>>(std::move(a), std::move(b), std::move(strategy));
}




int main() {
    std::unique_ptr<Sequence<int>> m = merge_by<int>(
        std::make_unique<IntegerGrowingSequence>(2, 3),
        std::make_unique<IntegerGrowingSequence>(-3, 5),
        std::make_unique<SwitchStrategy<int>>(true)
    );

    while (m->has_next()) {
        std::cout << m->next() << ' ';
    }
    std::cout << std::endl;
    return 0;
}
