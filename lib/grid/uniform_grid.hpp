#pragma once
#include "grid/grid.hpp"
#include "loops/md_index_loops.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA {

template <idx_t N> struct UniformGrid : public Grid<N, UniformGrid<N>> {

    using base_type = Grid<N, UniformGrid<N>>;

    using base_type::elementwise_product;



    UniformGrid() = default;


    ///
    ///@brief Construct from a given point count. Sets unity stepsize in each
    ///direction.
    ///
    ///@param dimensions Number of points in each direction.
    ///
    UniformGrid(GridDims<N> dimensions)
        : m_dimensions(dimensions) {
        for (idx_t i = 0; i < N; ++i) { m_stepsize[i] = double(1); }
    }

    UniformGrid(Point<N> begin, Point<N> end, GridDims<N> dimensions)
        : m_dimensions(dimensions)
        , m_stepsize(compute_stepsize(dimensions, point_difference(begin, end)))
        , m_points(create_points(begin, end, dimensions)) {}



    std::array<double, N> stepsize() const { return m_stepsize; }

    const std::vector<Point<N>>& get_points() const {
        return m_points;
    }


    GridDims<N> get_dimensions() const { return m_dimensions; }

private:
    GridDims<N>           m_dimensions;
    std::array<double, N> m_stepsize;
    std::vector<Point<N>> m_points;

    static std::array<double, N>
    compute_stepsize(GridDims<N> dimensions, std::array<double, N> lengths) {

        std::array<double, N> stepsize;
        for (idx_t i = 0; i < N; ++i) {
            Utils::runtime_assert(lengths[i] >= 0, "Negative grid length");
            stepsize[i] = lengths[i] / double(dimensions[i]);
        }
        return stepsize;
    }

    static std::array<double, N> point_difference(Point<N> begin,
                                                  Point<N> end) {

        Point<N> diff;
        for (idx_t i = 0; i < N; ++i) { diff[i] = end[i] - begin[i]; }
        return diff;
    }

    static Point<N> create_point(Point<N>              begin,
                                 position<N>           pos,
                                 std::array<double, N> stepsize) {

        Point<N> point;
        for (idx_t i = 0; i < N; ++i) {
            point[i] =
                begin[i] + 0.5 * stepsize[i] + double(pos[i]) * stepsize[i];
        }

        return point;
    }

    static std::vector<Point<N>>
    create_points(Point<N> begin, Point<N> end, GridDims<N> dimensions) {

        auto L        = point_difference(begin, end);
        auto stepsize = compute_stepsize(dimensions, L);

        idx_t                 n_points = elementwise_product(dimensions);
        std::vector<Point<N>> points(n_points);

        idx_t counter = 0;
        for (auto pos : md_indices(std::array<idx_t, N>{}, dimensions)) {
            points[counter] = create_point(begin, pos, stepsize);
            counter++;
        }

        return points;
    }
};

} // namespace JADA