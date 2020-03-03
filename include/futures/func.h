#ifndef H_URIPP_FUTURES_FUNC_H
#define H_URIPP_FUTURES_FUNC_H

#include <functional>

namespace uripp::futures {

template <typename TFunc>
class MovableFunc;

template <typename TRet, class... TArgs>
class MovableFunc<TRet(TArgs...)> {

    struct HolderBase {
        virtual TRet operator() (TArgs... args) = 0;
        virtual ~HolderBase() = default;
    };
    template <typename TFunc>
    class Holder : public HolderBase {
        TFunc f_;
    public:
        explicit Holder(TFunc f) : f_(std::move(f)) { }
        TRet operator() (TArgs... args) override {
            return f_(args...);
        }
    };
    std::unique_ptr<HolderBase> held_;
public:
    using self_type = MovableFunc<TRet(TArgs...)>;
    template <typename TFunc>
    explicit MovableFunc(TFunc f) : held_(new Holder(std::move(f))) { }

    MovableFunc() : held_(nullptr) { };

    MovableFunc(self_type &&) noexcept = default;

    MovableFunc& operator= (std::nullptr_t) {
        held_.reset();
        return *this;
    }

    MovableFunc& operator= (self_type &&) noexcept = default;
    MovableFunc(const self_type &) = delete;
    MovableFunc& operator= (const self_type &) = delete;
    bool empty() const { return !held_; } // NOLINT(modernize-use-nodiscard)

    TRet operator() (TArgs... args) {
        return held_->operator ()(args...);
    }
    explicit operator bool() {
        return (bool) held_;
    }
};
namespace detail {
using task_type = MovableFunc<void()>;
}
}
#endif
