//
// Created by kuznetsov on 30/01/24.
//

#pragma once

#include "FunctionExtraction.hpp"

namespace DefiniteIntegrals {

namespace detail {

template <typename Arg>
using DeltaType = decltype(std::declval<Arg>() - std::declval<Arg>());

template <typename FReturnType, typename... Args>
struct IntegralTypes {
    static constexpr std::size_t nArgs = sizeof...(Args);
    using ArgsTuple = decay_args_tuple_t<std::tuple<Args...>>;
    using DeltasTuple = decay_args_tuple_t<std::tuple<DeltaType<Args>...>>;
    using ResultType = FReturnType;
};

template <typename FReturnType, typename... Args>
IntegralTypes<FReturnType, Args...> extractIntegralTypes(const FReturnType& /*fReturnType*/,
                                                         const std::tuple<Args...>& /*args*/) {
    return {};
}

template <typename Callable>
using ExtructedIntegralTypes =
    decltype(extractIntegralTypes(std::declval<return_type_of_t<Callable>>(),
                                  std::declval<argument_tuple_type_of_t<Callable>>()));

template <typename ArgsTuple, typename DeltaTuple, typename Points, std::size_t... indexes>
ArgsTuple createPoint(const ArgsTuple& start, const DeltaTuple& deltas, const Points& points,
                      const std::index_sequence<indexes...>& /*sequence*/) {
    return {(std::get<indexes>(start) + std::get<indexes>(deltas) * std::get<indexes>(points))...};
}

template <typename DeltaTuple, std::size_t... indexes>
decltype(auto) calcArea(const DeltaTuple& deltas, const std::index_sequence<indexes...>& /*sequence*/) {
    return (std::get<indexes>(deltas) * ...);
}

template <typename Quadrature, typename Callable, std::size_t... indexes>
typename ExtructedIntegralTypes<Callable>::ResultType calcQuadratureSum(
    const Callable& f, const typename ExtructedIntegralTypes<Callable>::ArgsTuple& startArgs,
    const typename ExtructedIntegralTypes<Callable>::DeltasTuple& deltas,
    const std::index_sequence<indexes...>& /*sumSequence*/) {
    constexpr auto dimSize = std::tuple_size_v<typename ExtructedIntegralTypes<Callable>::ArgsTuple>;
    static_assert(Quadrature::dim == dimSize);
    constexpr auto pointSequence = std::make_index_sequence<dimSize>();

    const auto area = calcArea(deltas, pointSequence);
    const std::array<return_type_of_t<Callable>, sizeof...(indexes)> arr = {
        (Quadrature::nodes[indexes].weight *
         std::apply(f, createPoint(startArgs, deltas, Quadrature::nodes[indexes].point, pointSequence)))...};
    const auto res = (arr[indexes] + ...) * area;

    return res;
}

template <typename DeltaTuple>
struct StepsAndCells {
    DeltaTuple steps;
    std::array<std::size_t, std::tuple_size_v<DeltaTuple>> cells;
    std::size_t cellsNumber;
};

template <typename ArgsTuple, typename DeltaTuple, std::size_t... ind>
StepsAndCells<DeltaTuple> stepsAndCells(const ArgsTuple& start, const ArgsTuple& end, const DeltaTuple& initialSteps,
                                        const std::index_sequence<ind...>& /*seq*/) {
    static_assert(std::tuple_size_v<DeltaTuple> == sizeof...(ind));
    const std::tuple deltas = {std::get<ind>(end) - std::get<ind>(start)...};
    const std::array<std::size_t, std::tuple_size_v<DeltaTuple>> cells_intermediate = {
        (static_cast<std::size_t>(std::get<ind>(deltas) / std::get<ind>(initialSteps)))...};
    const std::array<std::size_t, std::tuple_size_v<DeltaTuple>> cells = {(cells_intermediate[ind] + 1)...};
    const std::tuple steps = {std::get<ind>(deltas) / static_cast<decltype(std::get<ind>(deltas))>(cells[ind])...};
    const std::size_t cellsNumber = (cells[ind] * ...);
    return {steps, cells, cellsNumber};
}

template <typename ArgsTuple, typename DeltaTuple, std::size_t... ind>
ArgsTuple currentArgs(const ArgsTuple& start, const DeltaTuple& steps,
                      const std::array<std::size_t, sizeof...(ind)>& indexND, const std::index_sequence<ind...> /*seq*/) {
    return {std::get<ind>(start) + std::get<ind>(steps) * static_cast<decltype(std::get<ind>(steps))>(indexND[ind])...};
}

}  // namespace detail

/** Выполняет интегрирование в многомерной прямоугольной области
 *
 * @tparam Quadrature       структура квадратуры. Содержит поля :
 *                              dim - размерность квадратуры,
 *                              size - количесто точек в квадратуре
 *                              points - точки квадратуры
 *                              weights - веса квадратуры
 *
 * @tparam Callable         Тип функция для интегрирования. Количество аргументов должно совпадать с dim в квадратуре
 *
 * @param f                 Объект функции для интегрирования
 * @param startArgs         начальные точки для интегрирования (координаты нижней левой точки)
 * @param deltas            длины ребер многомерного прямоугольника, в которой происходит интегрирование
 *
 * @return                  Значение интеграла
 */
template <typename Quadrature, typename Callable>
decltype(auto) integrate(const Callable& f,
                         const typename detail::ExtructedIntegralTypes<Callable>::ArgsTuple& startArgs,
                         const typename detail::ExtructedIntegralTypes<Callable>::DeltasTuple& deltas) {
    static_assert(detail::ExtructedIntegralTypes<Callable>::nArgs == Quadrature::dim);
    const auto res =
        detail::calcQuadratureSum<Quadrature>(f, startArgs, deltas, std::make_index_sequence<Quadrature::size>());
    return res;
}

template <typename Quadrature, typename Callable>
decltype(auto) integrate(const Callable& f,
                         const typename detail::ExtructedIntegralTypes<Callable>::ArgsTuple& startArgs,
                         const typename detail::ExtructedIntegralTypes<Callable>::ArgsTuple& endArgs,
                         const typename detail::ExtructedIntegralTypes<Callable>::DeltasTuple& initialSteps) {
    constexpr std::size_t dim = Quadrature::dim;
    static_assert(detail::ExtructedIntegralTypes<Callable>::nArgs == dim);
    const auto [steps, cells, cellsNumber] =
        detail::stepsAndCells(startArgs, endArgs, initialSteps, std::make_index_sequence<dim>());

    const auto index1DtoND = [&](std::size_t ind) {
        std::array<std::size_t, dim> result;
        auto cellNumber = cellsNumber;
        for (std::size_t i = 0; i < dim - 1; ++i) {
            cellNumber /= cells[i];
            result[i] = ind / cellNumber;
            ind = ind % cellNumber;
        }
        result[dim - 1] = ind;
        return result;
    };

    constexpr auto seq = std::make_index_sequence<dim>();
    auto s = integrate<Quadrature>(f, detail::currentArgs(startArgs, steps, index1DtoND(0), seq), steps);
    for (std::size_t i = 1; i < cellsNumber; ++i) {
        const auto arg = detail::currentArgs(startArgs, steps, index1DtoND(i), seq);
        const auto res = integrate<Quadrature>(f, arg, steps);
        s += res;
    }
    return s;
}

}  // namespace DefiniteIntegrals

