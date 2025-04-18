//
// Created by kuznetsov on 30/01/24.
//

#pragma once

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>


namespace DefiniteIntegrals
{
    template <class Tuple>
    struct decay_args_tuple;

    template <class... Args>
    struct decay_args_tuple<std::tuple<Args...>>
    {
        using type = std::tuple<std::decay_t<Args>...>;
    };

    template <typename T>
    using decay_args_tuple_t = typename decay_args_tuple<T>::type;


    namespace detail
    {
        template <typename Function>
        struct unwrap_function_impl;

        template <typename _RTy, typename... _ATy>
        struct unwrap_function_impl<_RTy(_ATy...)>
        {
            /// The return type of the function.
            typedef _RTy return_type;

            typedef std::tuple<_ATy...> argument_tuple_type;

            /// The function provided as std::function
            typedef std::function<_RTy(_ATy...)> function_type;

            /// The function provided as function_ptr
            typedef _RTy (*function_ptr)(_ATy...);
        };

        /// STL: std::function
        template <typename _RTy, typename... _ATy>
        struct unwrap_function_impl<std::function<_RTy(_ATy...)>> : unwrap_function_impl<_RTy(_ATy...)>
        {
        };

        /// STL: std::tuple
        template <typename _RTy, typename... _ATy>
        struct unwrap_function_impl<std::tuple<_RTy, _ATy...>> : unwrap_function_impl<_RTy(_ATy...)>
        {
        };

        /// Function pointers
        template <typename _RTy, typename... _ATy>
        struct unwrap_function_impl<_RTy (*const)(_ATy...)> : unwrap_function_impl<_RTy(_ATy...)>
        {
        };

        /// Class Method pointers
        template <typename _CTy, typename _RTy, typename... _ATy>
        struct unwrap_function_impl<_RTy (_CTy::*)(_ATy...) const> : unwrap_function_impl<_RTy(_ATy...)>
        {
        };

        /// Function pointers
        template <typename _RTy, typename... _ATy>
        struct unwrap_function_impl<_RTy (*)(_ATy...)> : unwrap_function_impl<_RTy(_ATy...)>
        {
        };

        /// Class Method pointers
        template <typename _CTy, typename _RTy, typename... _ATy>
        struct unwrap_function_impl<_RTy (_CTy::*)(_ATy...)> : unwrap_function_impl<_RTy(_ATy...)>
        {
        };

        /// Unwrap through pointer of functor.
        template <typename Function>
        static auto select_best_unwrap_unary_arg(int) -> unwrap_function_impl<decltype(&Function::operator())>;

        /// Unwrap through plain type.
        template <typename Function>
        static auto select_best_unwrap_unary_arg(std::size_t) -> unwrap_function_impl<Function>;

        template <typename... _FTy>
        struct select_best_unwrap;

        /// Enable only if 1 template argument is given.
        template <typename _FTy>
        struct select_best_unwrap<_FTy>
        {
            typedef decltype(select_best_unwrap_unary_arg<_FTy>(0)) type;
        };

        // Enable if more then 1 template argument is given.
        // (Handles lazy typing)
        template <typename _RTy, typename... _ATy>
        struct select_best_unwrap<_RTy, _ATy...>
        {
            typedef unwrap_function_impl<_RTy(_ATy...)> type;
        };

        template <typename>
        struct to_true : std::true_type
        {
        };

        /// std::true_type if unwrappable
        template <typename... Function>
        static auto test_unwrappable(int) -> to_true<typename select_best_unwrap<Function...>::type::function_type>;

        /// std::false_type if not unwrappable
        template <typename... Function>
        static auto test_unwrappable(std::size_t) -> std::false_type;
    } // namespace detail

    /// Trait to unwrap function parameters of various types:
    /// Function style definition: Result(Parameters...)
    /// STL `std::function` : std::function<Result(Parameters...)>
    /// STL `std::tuple` : std::tuple<Result, Parameters...>
    /// C++ Function pointers: `Result(*)(Parameters...)`
    /// C++ Class method pointers: `Result(Class::*)(Parameters...)`
    /// Lazy typed signatures: `Result, Parameters...`
    /// Also provides optimized unwrap of functors and functional objects.
    template <typename... Function>
    using unwrap_function = typename detail::select_best_unwrap<Function...>::type;

    /// Trait which defines the return type of the function.
    template <typename... Function>
    using return_type_of_t = typename detail::select_best_unwrap<Function...>::type::return_type;

    /// Trait which defines the argument types of the function packed in std::tuple.
    template <typename... Function>
    using argument_type_of_t = typename detail::select_best_unwrap<Function...>::type::argument_type;

    template <typename... Function>
    using argument_tuple_type_of_t = typename detail::select_best_unwrap<Function...>::type::argument_tuple_type;

    /// Trait which defines the std::function type of the function.
    template <typename... Function>
    using function_type_of_t = typename detail::select_best_unwrap<Function...>::type::function_type;

    /// Trait which defines the function pointer type of the function.
    template <typename... Function>
    using function_ptr_of_t = typename detail::select_best_unwrap<Function...>::type::function_ptr;

    /// Trait which defines if the given function is unwrappable or not.
    template <typename... Function>
    struct is_unwrappable : decltype(detail::test_unwrappable<Function...>(0))
    {
    };
} // namespace DefiniteIntegrals
