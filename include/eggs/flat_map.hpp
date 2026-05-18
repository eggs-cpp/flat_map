// Eggs.FlatMap
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2026
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <algorithm>
#include <compare>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace eggs {

namespace flat_map_detail {

template <typename Compare, typename K>
inline constexpr bool is_transparent =
    requires { typename Compare::is_transparent; };

} // namespace flat_map_detail


// ============================================================================
// key_of — extracts Key from function signature type, used to default Compare
// ============================================================================

template <typename Sig>
struct key_of;

template <typename Key, typename... Values>
struct key_of<Key(Values...)>
{
    using type = Key;
};

template <typename Sig>
using key_of_t = typename key_of<Sig>::type;

// ============================================================================
// Forward declaration
// ============================================================================

template <typename Sig, typename Compare = std::less<key_of_t<Sig>>>
class flat_map;

// ============================================================================
// sorted_unique_t tag  (mirrors std)
// ============================================================================

struct sorted_unique_t
{
    explicit sorted_unique_t() = default;
};

inline constexpr sorted_unique_t sorted_unique{};

// ============================================================================
// value_type
// ============================================================================

template <typename Key, typename... Values>
struct value_type
{
    const Key first;

    template <typename K, typename... Args>
        requires std::convertible_to<K, Key>
    value_type(K&& k, Args&&... args)
        : first(std::forward<K>(k)),
          values_(std::forward<Args>(args)...)
    {
    }

    template <size_t I>
    auto& value()
    {
        return std::get<I>(values_);
    }

    template <size_t I>
    const auto& value() const
    {
        return std::get<I>(values_);
    }

    template <size_t I>
    friend auto& get(value_type& v)
    {
        if constexpr (I == 0)
            return v.first;
        else
            return std::get<I - 1>(v.values_);
    }

    template <size_t I>
    friend const auto& get(const value_type& v)
    {
        if constexpr (I == 0)
            return v.first;
        else
            return std::get<I - 1>(v.values_);
    }

    template <size_t I>
    friend auto get(value_type&& v)
    {
        if constexpr (I == 0)
            return std::move(v.first);
        else
            return std::move(std::get<I - 1>(v.values_));
    }

  private:
    std::tuple<Values...> values_;
};

// ============================================================================
// reference
// ============================================================================

template <typename Key, typename... Values>
struct reference
{
    const Key& first;

    template <size_t I>
    auto& value()
    {
        return std::get<I>(values_);
    }

    template <size_t I>
    const auto& value() const
    {
        return std::get<I>(values_);
    }

    reference& operator=(const value_type<Key, Values...>& v)
    {
        assign_(v, std::index_sequence_for<Values...>{});
        return *this;
    }

    reference& operator=(value_type<Key, Values...>&& v)
    {
        assign_(std::move(v), std::index_sequence_for<Values...>{});
        return *this;
    }

    template <size_t I>
    friend auto& get(reference& r)
    {
        if constexpr (I == 0)
            return r.first;
        else
            return std::get<I - 1>(r.values_);
    }

    template <size_t I>
    friend const auto& get(const reference& r)
    {
        if constexpr (I == 0)
            return r.first;
        else
            return std::get<I - 1>(r.values_);
    }

  private:
    std::tuple<Values&...> values_;

    template <typename, typename>
    friend class flat_map;

    reference(const Key& k, Values&... vs)
        : first(k),
          values_(vs...)
    {
    }

    template <size_t... Is>
    void
    assign_(const value_type<Key, Values...>& v, std::index_sequence<Is...>)
    {
        ((std::get<Is>(values_) = v.template value<Is>()), ...);
    }

    template <size_t... Is>
    void assign_(value_type<Key, Values...>&& v, std::index_sequence<Is...>)
    {
        ((std::get<Is>(values_) = std::move(v.template value<Is>())), ...);
    }
};

// ============================================================================
// const_reference
// ============================================================================

template <typename Key, typename... Values>
struct const_reference
{
    const Key& first;

    template <size_t I>
    const auto& value() const
    {
        return std::get<I>(values_);
    }

    template <size_t I>
    friend const auto& get(const const_reference& r)
    {
        if constexpr (I == 0)
            return r.first;
        else
            return std::get<I - 1>(r.values_);
    }

  private:
    std::tuple<const Values&...> values_;

    template <typename, typename>
    friend class flat_map;

    const_reference(const Key& k, const Values&... vs)
        : first(k),
          values_(vs...)
    {
    }
};

} // namespace eggs

// ============================================================================
// tuple_size / tuple_element
// ============================================================================

template <typename Key, typename... Values>
struct std::tuple_size<eggs::value_type<Key, Values...>>
    : std::integral_constant<size_t, sizeof...(Values) + 1>
{};

template <typename Key, typename... Values>
struct std::tuple_size<eggs::reference<Key, Values...>>
    : std::integral_constant<size_t, sizeof...(Values) + 1>
{};

template <typename Key, typename... Values>
struct std::tuple_size<eggs::const_reference<Key, Values...>>
    : std::integral_constant<size_t, sizeof...(Values) + 1>
{};

template <typename Key, typename... Values>
struct std::tuple_element<0, eggs::value_type<Key, Values...>>
{
    using type = const Key;
};

template <size_t I, typename Key, typename... Values>
struct std::tuple_element<I, eggs::value_type<Key, Values...>>
    : std::tuple_element<I - 1, std::tuple<Values...>>
{};

// const — needed for `auto [k,v] = vt` (hidden variable is const value_type)
template <typename Key, typename... Values>
struct std::tuple_element<0, const eggs::value_type<Key, Values...>>
{
    using type = const Key;
};

template <size_t I, typename Key, typename... Values>
struct std::tuple_element<I, const eggs::value_type<Key, Values...>>
    : std::tuple_element<I - 1, std::tuple<const Values...>>
{};

template <typename Key, typename... Values>
struct std::tuple_element<0, eggs::reference<Key, Values...>>
{
    using type = const Key&;
};

template <size_t I, typename Key, typename... Values>
struct std::tuple_element<I, eggs::reference<Key, Values...>>
    : std::tuple_element<I - 1, std::tuple<Values&...>>
{};

template <typename Key, typename... Values>
struct std::tuple_element<0, eggs::const_reference<Key, Values...>>
{
    using type = const Key&;
};

template <size_t I, typename Key, typename... Values>
struct std::tuple_element<I, eggs::const_reference<Key, Values...>>
    : std::tuple_element<I - 1, std::tuple<const Values&...>>
{};

namespace eggs {

// ============================================================================
// containers — returned by flat_map::extract(), passed to flat_map::replace()
// ============================================================================

template <typename Key, typename... Values>
struct containers
{
    std::vector<Key> keys;

    template <size_t I>
    auto& values()
    {
        return std::get<I>(values_);
    }

    template <size_t I>
    const auto& values() const
    {
        return std::get<I>(values_);
    }

    template <size_t I>
    friend auto& get(containers& c)
    {
        if constexpr (I == 0)
            return c.keys;
        else
            return std::get<I - 1>(c.values_);
    }

    template <size_t I>
    friend const auto& get(const containers& c)
    {
        if constexpr (I == 0)
            return c.keys;
        else
            return std::get<I - 1>(c.values_);
    }

    template <size_t I>
    friend auto get(containers&& c)
    {
        if constexpr (I == 0)
            return std::move(c.keys);
        else
            return std::move(std::get<I - 1>(c.values_));
    }

  private:
    std::tuple<std::vector<Values>...> values_;
    template <typename, typename>
    friend class flat_map;
};

} // namespace eggs

template <typename Key, typename... Values>
struct std::tuple_size<eggs::containers<Key, Values...>>
    : std::integral_constant<size_t, sizeof...(Values) + 1>
{};

template <typename Key, typename... Values>
struct std::tuple_element<0, eggs::containers<Key, Values...>>
{
    using type = std::vector<Key>;
};

template <size_t I, typename Key, typename... Values>
struct std::tuple_element<I, eggs::containers<Key, Values...>>
    : std::tuple_element<I - 1, std::tuple<std::vector<Values>...>>
{};

template <typename Key, typename... Values>
struct std::tuple_element<0, const eggs::containers<Key, Values...>>
{
    using type = const std::vector<Key>;
};

template <size_t I, typename Key, typename... Values>
struct std::tuple_element<I, const eggs::containers<Key, Values...>>
    : std::tuple_element<I - 1, std::tuple<const std::vector<Values>...>>
{};

namespace eggs {


namespace detail {

template <size_t... Is, typename F>
void for_each_index(std::index_sequence<Is...>, F&& f)
{
    (f(std::integral_constant<size_t, Is>{}), ...);
}

} // namespace detail

// ============================================================================
// flat_map
// ============================================================================

template <typename Key, typename... Values, typename Compare>
class flat_map<Key(Values...), Compare>
{
  public:
    // --- Member types -------------------------------------------------------

    using key_type = Key;
    using mapped_type = std::tuple<Values...>;
    using value_type = eggs::value_type<Key, Values...>;
    using key_compare = Compare;
    using reference = eggs::reference<Key, Values...>;
    using const_reference = eggs::const_reference<Key, Values...>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    // using key_container_type    = std::vector<Key>;
    // using mapped_container_type = std::tuple<std::vector<Values>...>;

    // --- iterator -----------------------------------------------------------

    struct iterator
    {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = flat_map::value_type;
        using difference_type = flat_map::difference_type;
        using reference = flat_map::reference;

        struct pointer
        {
            reference ref;

            reference* operator->() { return &ref; }
        };

        iterator() = default;

        reference operator*() const
        {
            return deref_(std::index_sequence_for<Values...>{});
        }

        pointer operator->() const
        {
            return {deref_(std::index_sequence_for<Values...>{})};
        }

        reference operator[](difference_type n) const { return *(*this + n); }

        iterator& operator++()
        {
            ++idx_;
            return *this;
        }

        iterator operator++(int)
        {
            auto t = *this;
            ++idx_;
            return t;
        }

        iterator& operator--()
        {
            --idx_;
            return *this;
        }

        iterator operator--(int)
        {
            auto t = *this;
            --idx_;
            return t;
        }

        iterator& operator+=(difference_type n)
        {
            idx_ += n;
            return *this;
        }

        iterator& operator-=(difference_type n)
        {
            idx_ -= n;
            return *this;
        }

        friend iterator operator+(iterator it, difference_type n)
        {
            it += n;
            return it;
        }

        friend iterator operator+(difference_type n, iterator it)
        {
            it += n;
            return it;
        }

        friend iterator operator-(iterator it, difference_type n)
        {
            it -= n;
            return it;
        }

        friend difference_type operator-(iterator a, iterator b)
        {
            return static_cast<difference_type>(a.idx_) -
                   static_cast<difference_type>(b.idx_);
        }

        friend bool operator==(iterator a, iterator b)
        {
            return a.idx_ == b.idx_;
        }

        friend auto operator<=>(iterator a, iterator b)
        {
            return a.idx_ <=> b.idx_;
        }

      private:
        friend class flat_map;
        flat_map* map_ = nullptr;
        size_type idx_ = 0;

        iterator(flat_map* m, size_type i)
            : map_(m),
              idx_(i)
        {
        }

        template <size_t... Is>
        reference deref_(std::index_sequence<Is...>) const
        {
            return reference{
                map_->keys_[idx_], std::get<Is>(map_->values_)[idx_]...
            };
        }
    };

    // --- const_iterator -----------------------------------------------------

    struct const_iterator
    {
        using iterator_category = std::random_access_iterator_tag;
        using value_type = flat_map::value_type;
        using difference_type = flat_map::difference_type;
        using reference = flat_map::const_reference;

        struct pointer
        {
            reference ref;

            reference* operator->() { return &ref; }
        };

        const_iterator() = default;

        const_iterator(iterator it)
            : map_(it.map_),
              idx_(it.idx_)
        {
        }

        reference operator*() const
        {
            return deref_(std::index_sequence_for<Values...>{});
        }

        pointer operator->() const
        {
            return {deref_(std::index_sequence_for<Values...>{})};
        }

        reference operator[](difference_type n) const { return *(*this + n); }

        const_iterator& operator++()
        {
            ++idx_;
            return *this;
        }

        const_iterator operator++(int)
        {
            auto t = *this;
            ++idx_;
            return t;
        }

        const_iterator& operator--()
        {
            --idx_;
            return *this;
        }

        const_iterator operator--(int)
        {
            auto t = *this;
            --idx_;
            return t;
        }

        const_iterator& operator+=(difference_type n)
        {
            idx_ += n;
            return *this;
        }

        const_iterator& operator-=(difference_type n)
        {
            idx_ -= n;
            return *this;
        }

        friend const_iterator operator+(const_iterator it, difference_type n)
        {
            it += n;
            return it;
        }

        friend const_iterator operator+(difference_type n, const_iterator it)
        {
            it += n;
            return it;
        }

        friend const_iterator operator-(const_iterator it, difference_type n)
        {
            it -= n;
            return it;
        }

        friend difference_type operator-(const_iterator a, const_iterator b)
        {
            return static_cast<difference_type>(a.idx_) -
                   static_cast<difference_type>(b.idx_);
        }

        friend bool operator==(const_iterator a, const_iterator b)
        {
            return a.idx_ == b.idx_;
        }

        friend auto operator<=>(const_iterator a, const_iterator b)
        {
            return a.idx_ <=> b.idx_;
        }

      private:
        friend class flat_map;
        const flat_map* map_ = nullptr;
        size_type idx_ = 0;

        const_iterator(const flat_map* m, size_type i)
            : map_(m),
              idx_(i)
        {
        }

        template <size_t... Is>
        reference deref_(std::index_sequence<Is...>) const
        {
            return reference{
                map_->keys_[idx_], std::get<Is>(map_->values_)[idx_]...
            };
        }
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // --- value_compare ------------------------------------------------------

    class value_compare
    {
        key_compare comp_;

        explicit value_compare(key_compare c)
            : comp_(std::move(c))
        {
        }
        friend class flat_map;

      public:
        template <typename L, typename R>
        bool operator()(const L& lhs, const R& rhs) const
        {
            return comp_(lhs.first, rhs.first);
        }
    };

    using containers = eggs::containers<Key, Values...>;

    // --- Constructors -------------------------------------------------------

    flat_map() = default;

    explicit flat_map(const key_compare& comp)
        : compare_(comp)
    {
    }

    template <typename InputIt>
    flat_map(
        InputIt first, InputIt last, const key_compare& comp = key_compare{}
    )
        : compare_(comp)
    {
        insert(first, last);
    }

    template <typename InputIt>
    flat_map(
        sorted_unique_t,
        InputIt first,
        InputIt last,
        const key_compare& comp = key_compare{}
    )
        : compare_(comp)
    {
        insert(sorted_unique, first, last);
    }

    flat_map(
        std::initializer_list<value_type> init,
        const key_compare& comp = key_compare{}
    )
        : flat_map(init.begin(), init.end(), comp)
    {
    }

    flat_map(
        sorted_unique_t,
        std::initializer_list<value_type> init,
        const key_compare& comp = key_compare{}
    )
        : flat_map(sorted_unique, init.begin(), init.end(), comp)
    {
    }

    flat_map(const flat_map&) = default;
    flat_map(flat_map&&) = default;
    flat_map& operator=(const flat_map&) = default;
    flat_map& operator=(flat_map&&) = default;

    flat_map& operator=(std::initializer_list<value_type> init)
    {
        clear();
        insert(init);
        return *this;
    }

    ~flat_map() = default;

    // --- Iterators ----------------------------------------------------------

    iterator begin() noexcept { return {this, 0}; }

    const_iterator begin() const noexcept { return {this, 0}; }

    iterator end() noexcept { return {this, keys_.size()}; }

    const_iterator end() const noexcept { return {this, keys_.size()}; }

    reverse_iterator rbegin() noexcept { return reverse_iterator{end()}; }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator{end()};
    }

    reverse_iterator rend() noexcept { return reverse_iterator{begin()}; }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator{begin()};
    }

    const_iterator cbegin() const noexcept { return begin(); }

    const_iterator cend() const noexcept { return end(); }

    const_reverse_iterator crbegin() const noexcept { return rbegin(); }

    const_reverse_iterator crend() const noexcept { return rend(); }

    // --- Capacity -----------------------------------------------------------

    [[nodiscard]] bool empty() const noexcept { return keys_.empty(); }

    size_type size() const noexcept { return keys_.size(); }

    size_type max_size() const noexcept
    {
        size_type m = keys_.max_size();
        detail::for_each_index(
            std::index_sequence_for<Values...>{},
            [&](auto I) { m = std::min(m, std::get<I>(values_).max_size()); }
        );
        return m;
    }

    void reserve(size_type n)
    {
        keys_.reserve(n);
        detail::for_each_index(
            std::index_sequence_for<Values...>{},
            [&](auto I) { std::get<I>(values_).reserve(n); }
        );
    }

    void shrink_to_fit()
    {
        keys_.shrink_to_fit();
        detail::for_each_index(
            std::index_sequence_for<Values...>{},
            [&](auto I) { std::get<I>(values_).shrink_to_fit(); }
        );
    }

    // --- Element access -----------------------------------------------------

    std::tuple<Values&...> operator[](const Key& key)
        requires(std::default_initializable<Values> && ...)
    {
        auto it = lower_bound(key);
        if (it == end() || compare_(key, it->first))
            it = insert_at_(it.idx_, key, Values{}...);
        return refs_at_(it.idx_, std::index_sequence_for<Values...>{});
    }

    std::tuple<Values&...> operator[](Key&& key)
        requires(std::default_initializable<Values> && ...)
    {
        auto it = lower_bound(key);
        if (it == end() || compare_(key, it->first))
            it = insert_at_(it.idx_, std::move(key), Values{}...);
        return refs_at_(it.idx_, std::index_sequence_for<Values...>{});
    }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    std::tuple<Values&...> operator[](K&& key)
        requires(std::default_initializable<Values> && ...)
    {
        auto it = lower_bound(key);
        if (it == end() || compare_(key, it->first))
            it = insert_at_(it.idx_, std::forward<K>(key), Values{}...);
        return refs_at_(it.idx_, std::index_sequence_for<Values...>{});
    }

    std::tuple<Values&...> at(const Key& key)
    {
        auto it = find(key);
        if (it == end()) throw std::out_of_range("flat_map::at");
        return refs_at_(it.idx_, std::index_sequence_for<Values...>{});
    }

    std::tuple<const Values&...> at(const Key& key) const
    {
        auto it = find(key);
        if (it == end()) throw std::out_of_range("flat_map::at");
        return const_refs_at_(it.idx_, std::index_sequence_for<Values...>{});
    }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    std::tuple<Values&...> at(const K& key)
    {
        auto it = find(key);
        if (it == end()) throw std::out_of_range("flat_map::at");
        return refs_at_(it.idx_, std::index_sequence_for<Values...>{});
    }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    std::tuple<const Values&...> at(const K& key) const
    {
        auto it = find(key);
        if (it == end()) throw std::out_of_range("flat_map::at");
        return const_refs_at_(it.idx_, std::index_sequence_for<Values...>{});
    }

    // --- Column accessors ---------------------------------------------------

    template <size_t I>
    auto& values() noexcept
    {
        return std::get<I>(values_);
    }

    template <size_t I>
    const auto& values() const noexcept
    {
        return std::get<I>(values_);
    }

    const std::vector<Key>& keys() const noexcept { return keys_; }

    // --- Modifiers: insert --------------------------------------------------

    std::pair<iterator, bool> insert(const value_type& v)
    {
        return insert_value_(v, std::index_sequence_for<Values...>{});
    }

    std::pair<iterator, bool> insert(value_type&& v)
    {
        return insert_value_(
            std::move(v), std::index_sequence_for<Values...>{}
        );
    }

    iterator insert(const_iterator, const value_type& v)
    {
        return insert(v).first;
    }

    iterator insert(const_iterator, value_type&& v)
    {
        return insert(std::move(v)).first;
    }

    template <typename InputIt>
    void insert(InputIt first, InputIt last)
    {
        for (; first != last; ++first) insert(*first);
    }

    template <typename InputIt>
    void insert(sorted_unique_t, InputIt first, InputIt last)
    {
        // input is pre-sorted unique — build a temporary flat_map directly
        // by pushing elements in order, then merge with existing contents
        flat_map tmp(compare_);
        for (auto it = first; it != last; ++it) {
            const auto& v = *it;
            tmp.keys_.push_back(v.first);
            detail::for_each_index(
                std::index_sequence_for<Values...>{}, [&](auto I) {
                    std::get<I>(tmp.values_).push_back(v.template value<I>());
                }
            );
        }
        merge_from_(std::move(tmp));
    }

    void insert(std::initializer_list<value_type> init)
    {
        insert(init.begin(), init.end());
    }

    void insert(sorted_unique_t, std::initializer_list<value_type> init)
    {
        insert(sorted_unique, init.begin(), init.end());
    }

    // --- Modifiers: emplace -------------------------------------------------
    // emplace constructs a value_type from args, then inserts by key

    template <typename... Args>
        requires std::is_constructible_v<value_type, Args...>
    std::pair<iterator, bool> emplace(Args&&... args)
    {
        value_type v(std::forward<Args>(args)...);
        auto it = lower_bound(v.first);
        if (it != end() && !compare_(v.first, it->first)) return {it, false};
        return {
            insert_at_from_value_(
                it.idx_, std::move(v), std::index_sequence_for<Values...>{}
            ),
            true
        };
    }

    template <typename... Args>
    iterator emplace_hint(const_iterator, Args&&... args)
    {
        return emplace(std::forward<Args>(args)...).first;
    }

    template <typename... KeyArgs, typename... ValArgTuples>
    std::pair<iterator, bool> emplace(
        std::piecewise_construct_t,
        std::tuple<KeyArgs...> key_args,
        ValArgTuples&&... val_arg_tuples
    )
    {
        static_assert(sizeof...(ValArgTuples) == sizeof...(Values));
        Key key = std::make_from_tuple<Key>(std::move(key_args));
        auto it = lower_bound(key);
        if (it != end() && !compare_(key, it->first)) return {it, false};
        return {
            insert_at_piecewise_(
                it.idx_,
                std::move(key),
                std::index_sequence_for<Values...>{},
                std::forward<ValArgTuples>(val_arg_tuples)...
            ),
            true
        };
    }

    // --- Modifiers: try_emplace ---------------------------------------------

    template <typename... Args>
    std::pair<iterator, bool> try_emplace(const Key& key, Args&&... args)
        requires(std::is_constructible_v<Values, Args> && ...)
    {
        auto it = lower_bound(key);
        if (it != end() && !compare_(key, it->first)) return {it, false};
        return {insert_at_(it.idx_, key, std::forward<Args>(args)...), true};
    }

    template <typename... Args>
    std::pair<iterator, bool> try_emplace(Key&& key, Args&&... args)
        requires(std::is_constructible_v<Values, Args> && ...)
    {
        auto it = lower_bound(key);
        if (it != end() && !compare_(key, it->first)) return {it, false};
        return {
            insert_at_(it.idx_, std::move(key), std::forward<Args>(args)...),
            true
        };
    }

    template <typename K, typename... Args>
        requires(
            flat_map_detail::is_transparent<key_compare, K> &&
            std::is_constructible_v<Key, K> &&
            (std::is_constructible_v<Values, Args> && ...) &&
            !std::is_convertible_v<K &&, iterator> &&
            !std::is_convertible_v<K &&, const_iterator>
        )
    std::pair<iterator, bool> try_emplace(K&& key, Args&&... args)
    {
        auto it = lower_bound(key);
        if (it != end() && !compare_(key, it->first)) return {it, false};
        return {
            insert_at_(
                it.idx_, std::forward<K>(key), std::forward<Args>(args)...
            ),
            true
        };
    }

    template <typename... Args>
    iterator try_emplace(const_iterator, const Key& key, Args&&... args)
        requires(std::is_constructible_v<Values, Args> && ...)
    {
        return try_emplace(key, std::forward<Args>(args)...).first;
    }

    template <typename... Args>
    iterator try_emplace(const_iterator, Key&& key, Args&&... args)
        requires(std::is_constructible_v<Values, Args> && ...)
    {
        return try_emplace(std::move(key), std::forward<Args>(args)...).first;
    }

    template <typename K, typename... Args>
        requires(
            flat_map_detail::is_transparent<key_compare, K> &&
            std::is_constructible_v<Key, K> &&
            (std::is_constructible_v<Values, Args> && ...)
        )
    iterator try_emplace(const_iterator, K&& key, Args&&... args)
    {
        return try_emplace(std::forward<K>(key), std::forward<Args>(args)...)
            .first;
    }

    // --- Modifiers: insert_or_assign ----------------------------------------
    // M is a single mapped pack (one per value type would be ideal, but we
    // follow std which has one M; here M is a tuple or a single type for
    // the single-value case, and we use Args... for the multi-value extension)

    template <typename... Args>
    std::pair<iterator, bool> insert_or_assign(const Key& key, Args&&... args)
        requires(
            (std::is_constructible_v<Values, Args> && ...) &&
            (std::is_assignable_v<Values&, Args> && ...)
        )
    {
        auto it = lower_bound(key);
        if (it != end() && !compare_(key, it->first)) {
            assign_at_(
                it.idx_,
                std::index_sequence_for<Values...>{},
                std::forward<Args>(args)...
            );
            return {it, false};
        }
        return {insert_at_(it.idx_, key, std::forward<Args>(args)...), true};
    }

    template <typename... Args>
    std::pair<iterator, bool> insert_or_assign(Key&& key, Args&&... args)
        requires(
            (std::is_constructible_v<Values, Args> && ...) &&
            (std::is_assignable_v<Values&, Args> && ...)
        )
    {
        auto it = lower_bound(key);
        if (it != end() && !compare_(key, it->first)) {
            assign_at_(
                it.idx_,
                std::index_sequence_for<Values...>{},
                std::forward<Args>(args)...
            );
            return {it, false};
        }
        return {
            insert_at_(it.idx_, std::move(key), std::forward<Args>(args)...),
            true
        };
    }

    template <typename K, typename... Args>
        requires(
            flat_map_detail::is_transparent<key_compare, K> &&
            std::is_constructible_v<Key, K> &&
            (std::is_constructible_v<Values, Args> && ...) &&
            (std::is_assignable_v<Values&, Args> && ...)
        )
    std::pair<iterator, bool> insert_or_assign(K&& key, Args&&... args)
    {
        auto it = lower_bound(key);
        if (it != end() && !compare_(key, it->first)) {
            assign_at_(
                it.idx_,
                std::index_sequence_for<Values...>{},
                std::forward<Args>(args)...
            );
            return {it, false};
        }
        return {
            insert_at_(
                it.idx_, std::forward<K>(key), std::forward<Args>(args)...
            ),
            true
        };
    }

    template <typename... Args>
    iterator insert_or_assign(const_iterator, const Key& key, Args&&... args)
        requires(
            (std::is_constructible_v<Values, Args> && ...) &&
            (std::is_assignable_v<Values&, Args> && ...)
        )
    {
        return insert_or_assign(key, std::forward<Args>(args)...).first;
    }

    template <typename... Args>
    iterator insert_or_assign(const_iterator, Key&& key, Args&&... args)
        requires(
            (std::is_constructible_v<Values, Args> && ...) &&
            (std::is_assignable_v<Values&, Args> && ...)
        )
    {
        return insert_or_assign(std::move(key), std::forward<Args>(args)...)
            .first;
    }

    template <typename K, typename... Args>
        requires(
            flat_map_detail::is_transparent<key_compare, K> &&
            std::is_constructible_v<Key, K> &&
            (std::is_constructible_v<Values, Args> && ...) &&
            (std::is_assignable_v<Values&, Args> && ...)
        )
    iterator insert_or_assign(const_iterator, K&& key, Args&&... args)
    {
        return insert_or_assign(
                   std::forward<K>(key), std::forward<Args>(args)...
        )
            .first;
    }

    // --- Modifiers: erase ---------------------------------------------------

    iterator erase(iterator pos)
    {
        erase_at_(pos.idx_);
        return {this, pos.idx_};
    }

    iterator erase(const_iterator pos)
    {
        erase_at_(pos.idx_);
        return {this, pos.idx_};
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        erase_range_(first.idx_, last.idx_);
        return {this, first.idx_};
    }

    size_type erase(const Key& key)
    {
        auto it = find(key);
        if (it == end()) return 0;
        erase_at_(it.idx_);
        return 1;
    }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    size_type erase(K&& key)
    {
        auto it = find(key);
        if (it == end()) return 0;
        erase_at_(it.idx_);
        return 1;
    }

    void clear() noexcept
    {
        keys_.clear();
        detail::for_each_index(
            std::index_sequence_for<Values...>{},
            [&](auto I) { std::get<I>(values_).clear(); }
        );
    }

    void swap(flat_map& other) noexcept
    {
        using std::swap;
        swap(keys_, other.keys_);
        swap(values_, other.values_);
        swap(compare_, other.compare_);
    }

    // --- Modifiers: extract / replace ---------------------------------------

    containers extract() &&
    {
        containers c;
        c.keys = std::move(keys_);
        c.values_ = std::move(values_);
        return c;
    }

    void replace(containers&& c)
    {
        keys_ = std::move(c.keys);
        values_ = std::move(c.values_);
    }

    // --- Lookup -------------------------------------------------------------

    iterator find(const Key& key)
    {
        auto it = lower_bound(key);
        return (it != end() && !compare_(key, it->first)) ? it : end();
    }

    const_iterator find(const Key& key) const
    {
        auto it = lower_bound(key);
        return (it != end() && !compare_(key, it->first)) ? it : end();
    }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    iterator find(const K& key)
    {
        auto it = lower_bound(key);
        return (it != end() && !compare_(key, it->first)) ? it : end();
    }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    const_iterator find(const K& key) const
    {
        auto it = lower_bound(key);
        return (it != end() && !compare_(key, it->first)) ? it : end();
    }

    bool contains(const Key& key) const { return find(key) != end(); }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    bool contains(const K& key) const
    {
        return find(key) != end();
    }

    size_type count(const Key& key) const { return contains(key) ? 1 : 0; }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    size_type count(const K& key) const
    {
        return contains(key) ? 1 : 0;
    }

    iterator lower_bound(const Key& key)
    {
        auto it = std::lower_bound(keys_.begin(), keys_.end(), key, compare_);
        return {this, static_cast<size_type>(it - keys_.begin())};
    }

    const_iterator lower_bound(const Key& key) const
    {
        auto it = std::lower_bound(keys_.begin(), keys_.end(), key, compare_);
        return {this, static_cast<size_type>(it - keys_.begin())};
    }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    iterator lower_bound(const K& key)
    {
        auto it = std::lower_bound(keys_.begin(), keys_.end(), key, compare_);
        return {this, static_cast<size_type>(it - keys_.begin())};
    }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    const_iterator lower_bound(const K& key) const
    {
        auto it = std::lower_bound(keys_.begin(), keys_.end(), key, compare_);
        return {this, static_cast<size_type>(it - keys_.begin())};
    }

    iterator upper_bound(const Key& key)
    {
        auto it = std::upper_bound(keys_.begin(), keys_.end(), key, compare_);
        return {this, static_cast<size_type>(it - keys_.begin())};
    }

    const_iterator upper_bound(const Key& key) const
    {
        auto it = std::upper_bound(keys_.begin(), keys_.end(), key, compare_);
        return {this, static_cast<size_type>(it - keys_.begin())};
    }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    iterator upper_bound(const K& key)
    {
        auto it = std::upper_bound(keys_.begin(), keys_.end(), key, compare_);
        return {this, static_cast<size_type>(it - keys_.begin())};
    }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    const_iterator upper_bound(const K& key) const
    {
        auto it = std::upper_bound(keys_.begin(), keys_.end(), key, compare_);
        return {this, static_cast<size_type>(it - keys_.begin())};
    }

    std::pair<iterator, iterator> equal_range(const Key& key)
    {
        return {lower_bound(key), upper_bound(key)};
    }

    std::pair<const_iterator, const_iterator> equal_range(const Key& key) const
    {
        return {lower_bound(key), upper_bound(key)};
    }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    std::pair<iterator, iterator> equal_range(const K& key)
    {
        return {lower_bound(key), upper_bound(key)};
    }

    template <typename K>
        requires flat_map_detail::is_transparent<key_compare, K>
    std::pair<const_iterator, const_iterator> equal_range(const K& key) const
    {
        return {lower_bound(key), upper_bound(key)};
    }

    // --- Observers ----------------------------------------------------------

    key_compare key_comp() const { return compare_; }

    value_compare value_comp() const { return value_compare{compare_}; }

    // --- Non-member friends -------------------------------------------------

    friend bool operator==(const flat_map& a, const flat_map& b)
    {
        return a.keys_ == b.keys_ && a.values_ == b.values_;
    }

    friend auto operator<=>(const flat_map& a, const flat_map& b)
    {
        using order_t = std::common_comparison_category_t<
            decltype(std::declval<Key>() <=> std::declval<Key>()),
            decltype(std::declval<Values>() <=> std::declval<Values>())...,
            std::strong_ordering>; // size() <=> size() is strong
        for (size_type i = 0, n = std::min(a.size(), b.size()); i < n; ++i) {
            if (auto c = order_t(a.keys_[i] <=> b.keys_[i]); c != 0) return c;
            if (auto c = cmp_values_<order_t>(
                    a, b, i, std::index_sequence_for<Values...>{}
                );
                c != 0)
                return c;
        }
        return order_t(a.size() <=> b.size());
    }

    friend void swap(flat_map& lhs, flat_map& rhs) noexcept { lhs.swap(rhs); }

    template <typename Pred>
    friend size_type erase_if(flat_map& c, Pred pred)
    {
        size_type old = c.size();
        for (size_type i = 0; i < c.keys_.size();) {
            if (pred(c.deref_(i, std::index_sequence_for<Values...>{})))
                c.erase_at_(i);
            else
                ++i;
        }
        return old - c.size();
    }

  private:
    std::vector<Key> keys_;
    std::tuple<std::vector<Values>...> values_;
    key_compare compare_;

    // --- Private helpers ----------------------------------------------------

    template <size_t... Is>
    reference deref_(size_type i, std::index_sequence<Is...>)
    {
        return reference{keys_[i], std::get<Is>(values_)[i]...};
    }

    template <size_t... Is>
    std::tuple<Values&...> refs_at_(size_type i, std::index_sequence<Is...>)
    {
        return {std::get<Is>(values_)[i]...};
    }

    template <size_t... Is>
    std::tuple<const Values&...>
    const_refs_at_(size_type i, std::index_sequence<Is...>) const
    {
        return {std::get<Is>(values_)[i]...};
    }

    // push_back to end then rotate into idx — basic exception safety
    template <typename K, typename... Args>
    iterator insert_at_(size_type idx, K&& key, Args&&... args)
    {
        size_type n = keys_.size();
        keys_.push_back(std::forward<K>(key));
        size_type pushed = 0;
        try {
            detail::for_each_index(
                std::index_sequence_for<Values...>{}, [&](auto I) {
                    if constexpr (I < sizeof...(Args))
                        std::get<I>(values_).push_back(
                            std::get<I>(std::forward_as_tuple(
                                std::forward<Args>(args)...
                            ))
                        );
                    else {
                        using V = std::tuple_element_t<
                            decltype(I)::value,
                            std::tuple<Values...>>;
                        std::get<I>(values_).push_back(V{});
                    }
                    ++pushed;
                }
            );
        } catch (...) {
            keys_.pop_back();
            detail::for_each_index(
                std::index_sequence_for<Values...>{}, [&](auto I) {
                    if (I < pushed) std::get<I>(values_).pop_back();
                }
            );
            throw;
        }
        std::rotate(keys_.begin() + idx, keys_.begin() + n, keys_.end());
        detail::for_each_index(
            std::index_sequence_for<Values...>{}, [&](auto I) {
                auto& col = std::get<I>(values_);
                std::rotate(col.begin() + idx, col.begin() + n, col.end());
            }
        );
        return {this, idx};
    }

    template <size_t... Is>
    iterator insert_at_from_value_(
        size_type idx, value_type&& v, std::index_sequence<Is...>
    )
    {
        return insert_at_(
            idx, std::move(v.first), std::move(v.template value<Is>())...
        );
    }

    template <typename K, size_t... Is, typename... ValArgTuples>
    iterator insert_at_piecewise_(
        size_type idx,
        K&& key,
        std::index_sequence<Is...>,
        ValArgTuples&&... val_arg_tuples
    )
    {
        size_type n = keys_.size();
        keys_.push_back(std::forward<K>(key));
        size_type pushed = 0;
        try {
            ((std::get<Is>(values_).push_back(
                  std::make_from_tuple<Values>(
                      std::forward<ValArgTuples>(val_arg_tuples)
                  )
              ),
              ++pushed),
             ...);
        } catch (...) {
            keys_.pop_back();
            detail::for_each_index(
                std::index_sequence_for<Values...>{}, [&](auto I) {
                    if (I < pushed) std::get<I>(values_).pop_back();
                }
            );
            throw;
        }
        std::rotate(keys_.begin() + idx, keys_.begin() + n, keys_.end());
        detail::for_each_index(
            std::index_sequence_for<Values...>{}, [&](auto I) {
                auto& col = std::get<I>(values_);
                std::rotate(col.begin() + idx, col.begin() + n, col.end());
            }
        );
        return {this, idx};
    }

    template <size_t... Is, typename... Args>
    void assign_at_(size_type idx, std::index_sequence<Is...>, Args&&... args)
    {
        ((std::get<Is>(values_)[idx] =
              std::get<Is>(std::forward_as_tuple(std::forward<Args>(args)...))),
         ...);
    }

    void erase_at_(size_type idx)
    {
        keys_.erase(keys_.begin() + idx);
        detail::for_each_index(
            std::index_sequence_for<Values...>{}, [&](auto I) {
                auto& col = std::get<I>(values_);
                col.erase(col.begin() + idx);
            }
        );
    }

    void erase_range_(size_type first, size_type last)
    {
        keys_.erase(keys_.begin() + first, keys_.begin() + last);
        detail::for_each_index(
            std::index_sequence_for<Values...>{}, [&](auto I) {
                auto& col = std::get<I>(values_);
                col.erase(col.begin() + first, col.begin() + last);
            }
        );
    }

    template <size_t... Is>
    std::pair<iterator, bool>
    insert_value_(const value_type& v, std::index_sequence<Is...>)
    {
        auto it = lower_bound(v.first);
        if (it != end() && !compare_(v.first, it->first)) return {it, false};
        return {insert_at_(it.idx_, v.first, v.template value<Is>()...), true};
    }

    template <size_t... Is>
    std::pair<iterator, bool>
    insert_value_(value_type&& v, std::index_sequence<Is...>)
    {
        auto it = lower_bound(v.first);
        if (it != end() && !compare_(v.first, it->first)) return {it, false};
        return {
            insert_at_(it.idx_, v.first, std::move(v.template value<Is>())...),
            true
        };
    }

    // sorted merge — used by insert(sorted_unique, ...)
    void merge_from_(flat_map&& other)
    {
        flat_map result;
        result.reserve(size() + other.size());
        size_type i = 0, j = 0;
        while (i < size() && j < other.size()) {
            if (compare_(keys_[i], other.keys_[j]))
                push_back_row_(*this, result, i++);
            else if (compare_(other.keys_[j], keys_[i]))
                push_back_row_(other, result, j++);
            else {
                push_back_row_(*this, result, i++);
                ++j;
            } // this wins on duplicate
        }
        while (i < size()) push_back_row_(*this, result, i++);
        while (j < other.size()) push_back_row_(other, result, j++);
        *this = std::move(result);
    }

    static void push_back_row_(flat_map& src, flat_map& dst, size_type i)
    {
        dst.keys_.push_back(src.keys_[i]);
        detail::for_each_index(
            std::index_sequence_for<Values...>{}, [&](auto I) {
                std::get<I>(dst.values_).push_back(std::get<I>(src.values_)[i]);
            }
        );
    }

    template <typename Order, size_t... Is>
    static Order cmp_values_(
        const flat_map& a,
        const flat_map& b,
        [[maybe_unused]] size_type i,
        std::index_sequence<Is...>
    )
    {
        Order r = Order::equivalent;
        ((r == Order::equivalent ? void(
                                       r = Order(
                                           std::get<Is>(a.values_)[i] <=>
                                           std::get<Is>(b.values_)[i]
                                       )
                                   )
                                 : void(0)),
         ...);
        return r;
    }
};

// ============================================================================
// Deduction guides
// ============================================================================

template <typename Key, typename... Values>
flat_map(std::initializer_list<eggs::value_type<Key, Values...>>)
    -> flat_map<Key(Values...)>;

template <typename Key, typename... Values, typename Compare>
flat_map(std::initializer_list<eggs::value_type<Key, Values...>>, Compare)
    -> flat_map<Key(Values...), Compare>;

template <typename Key, typename... Values>
flat_map(
    sorted_unique_t, std::initializer_list<eggs::value_type<Key, Values...>>
) -> flat_map<Key(Values...)>;

} // namespace eggs
