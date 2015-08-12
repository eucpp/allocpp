#ifndef TEST_TYPES_HPP
#define TEST_TYPES_HPP

#include <type_traits>

#include "alloc_traits.hpp"
#include "none_policy.hpp"
#include "macro.hpp"

struct empty_class
{};

struct class_with_rebind
{
    template <typename T>
    using rebind = T;
};

struct class_with_comparison_ops
{
    bool operator==(const class_with_comparison_ops& other)
    {
        ALLOC_UNUSED(other);
        return true;
    }

    bool operator!=(const class_with_comparison_ops& other)
    {
        ALLOC_UNUSED(other);
        return false;
    }
};

template <typename T, typename alloc_traits = alloc_utility::allocation_traits<T>,
          typename base_policy = alloc_utility::none_policy<T>>
struct well_defined_policy: public base_policy
{
    DECLARE_ALLOC_TRAITS(T, alloc_traits)
    DECLARE_REBIND_ALLOC(well_defined_policy, T, alloc_traits, base_policy)

    well_defined_policy() = default;
    well_defined_policy(const well_defined_policy&) = default;

    typedef std::true_type propagate_on_container_copy_assignment;
    typedef std::true_type propagate_on_container_move_assignment;
    typedef std::true_type propagate_on_container_swap;

    template <typename U>
    well_defined_policy(const rebind<U>& other):
        flag(other.flag)
    {}

    bool operator==(const well_defined_policy& other) const noexcept
    {
        return flag == other.flag;
    }

    bool operator!=(const well_defined_policy& other) const noexcept
    {
        return flag != other.flag;
    }

    bool flag;
};

template <typename T, typename alloc_traits = alloc_utility::allocation_traits<T>,
          typename base_policy = alloc_utility::none_policy<T>>
struct poorly_defined_policy: public base_policy
{
    DECLARE_ALLOC_TRAITS(T, alloc_traits)
    DECLARE_REBIND_ALLOC(poorly_defined_policy, T, alloc_traits, base_policy)

    poorly_defined_policy() = default;
    poorly_defined_policy(const poorly_defined_policy&) = default;

    template <typename U>
    poorly_defined_policy(const rebind<U>& other)
    {
        ALLOC_UNUSED(other);
    }
};

#endif // TEST_TYPES_HPP