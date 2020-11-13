#pragma once

#include <array>
#include <map>
#include <vector>

#include "grid/block.hpp"
#include "grid/local_global_mapping.hpp"
#include "grid/split.hpp"
#include "loops/unflatten_index.hpp"
#include "loops/flatten_index.hpp"
#include "utils/runtime_assert.hpp"
#include "utils/array_cast.hpp"


namespace JADA {


template<size_t Dim>
struct BlockConnection{
    int from_id;
    int to_id;
    std::array<int, Dim> direction;


    bool is_valid() const {
        return (from_id != BLOCK_ID_NULL) && (to_id != BLOCK_ID_NULL)
         && (from_id != to_id);
    }


};






template<size_t Dim> class ConnectivityGraph{

    using storage_t = std::map<int, std::vector<BlockConnection<Dim>>>;

    storage_t m_storage;


public:

    ConnectivityGraph() = default;


    void insert(int block_id, const std::vector<BlockConnection<Dim>>& connections){

        m_storage.insert({block_id, connections});


    }







};




template <size_t Dim> struct BlockTopology {

    using block_vector       = std::vector<Block<Dim>>;
    using connectivity_graph = std::map<int, std::vector<int>>;

    BlockTopology(block_vector blocks, connectivity_graph con)
        : m_blocks(blocks)
        , m_connectivity(con) {}


    std::vector<int> get_neighbours(int id) const {
        return m_connectivity.at(id); //[id];
    }

    Block<Dim> get_block(int id) const {
        return *std::find_if(m_blocks.begin(),
                             m_blocks.end(),
                             [&](Block<Dim> b) { return b.id == id; });
    }

private:
    block_vector       m_blocks;
    connectivity_graph m_connectivity;
};


template <size_t N>
BlockTopology<N> build_sub_blocks(Block<N> parent,
                              idx_t                n_blocks) {

    auto b_grid = split(parent.density, n_blocks);

    std::array<double, N> stepsize;

    for (idx_t i = 0; i < N; ++i) {
        stepsize[i] = (parent.p1[i] - parent.p0[i]) / double(parent.density[i]);
    }

    std::vector<Block<N>> blocks(n_blocks);

    /*
    for (auto coord : md_indices(std::array<idx_t, N>{}, b_grid)) {

        auto block_density =
            LocalGlobalMapping<N>::local_extent(dims, b_grid, coord);

        auto [begin, end] =
            LocalGlobalMapping<N>::get_bounds(dims, b_grid, coord);

        Point<N> block_p0;
        Point<N> block_p1;

        for (idx_t i = 0; i < N; ++i) {
            block_p0[i] = begin[i] * stepsize[i];
            block_p1[i] = end[i] * stepsize[i];
        }

        Block<N> block(block_density, block_p0, block_p1, id);

        blocks.push_back(block);

        id++;
    }
    */
}




/*

template <size_t N> struct BlockTopologyNearest : public BlockTopology<N> {

    using base_type = BlockTopology<N>;
    using connectivity_graph = typename base_type::connectivity_graph;


    BlockTopologyNearest(std::vector<Block<N>> blocks,
                         std::array<idx_t, N>  topo_dims,
                         std::array<idx_t, N>  periods)
        : BlockTopology<N>(blocks, create_graph(blocks, topo_dims, periods)) {}

    static connectivity_graph
    create_graph(std::vector<Block<N>> blocks,
                 std::array<idx_t, N>  topo_dims,
                 std::array<idx_t, N>  periods) {

        connectivity_graph graph;

        for (auto block : blocks) {
            auto neighbours = get_neighbours(block.id, topo_dims, periods);
            graph.insert(std::pair(block.id, neighbours));
        }

        return graph;
    }

    static int coord_to_id(std::array<int, N> coords, const std::array<idx_t, N> topo_dims) {

        for (size_t i = 0; i < N; ++i) {
            if ((coords[i] < 0) || (coords[i] >= int(topo_dims[i]))) { return BLOCK_ID_NULL; }
        }

        //std::array<idx_t, N> casted_coords = Utils::array_cast<idx_t, int, N>(coords);

        throw "Failing on coord_to_id because not implemented";


        return 4;

        
        //TODO: find a way to perform the array casts
        //return flatten<N, StorageOrder::RowMajor>(
        //    topo_dims,
        //    Utils::array_cast<idx_t>(coords)
        //);
        

    }



    static std::vector<int> get_neighbours(int                id,
                                             std::array<idx_t, N> topo_dims,
                                             std::array<idx_t, N> periods) {

        const int total  = int(std::accumulate(topo_dims.begin(),
                                            topo_dims.end(),
                                            idx_t(1),
                                            std::multiplies<idx_t>{}));


        Utils::runtime_assert(id >= 0, "Negative block id.");
        Utils::runtime_assert(id < total, "Block id out of bounds");

        const auto coord = unflatten<N, StorageOrder::RowMajor>(topo_dims, idx_t(id));


        std::vector<int> neighbours;

        for (idx_t i = 0; i < N; ++i) {
            std::array<int, N> dir{};
            dir[i] = 1;
 
            auto [source, dest] = shift(dir, coord, topo_dims, periods);

            neighbours.push_back(coord_to_id(source, topo_dims));
            neighbours.push_back(coord_to_id(dest, topo_dims));

            
        }
        return neighbours;
    }





    static std::pair<std::array<int, N>, std::array<int, N>>
    shift(std::array<int, N>   direction,
          std::array<idx_t, N> from,
          std::array<idx_t, N> topo_dims,
          std::array<idx_t, N> periods) {

        std::array<int, N> source_coords;
        std::array<int, N> dest_coords;

        for (idx_t i = 0; i < N; ++i) {
            source_coords[i] = int(from[i]) + direction[i];
            dest_coords[i]   = int(from[i]) - direction[i];
        }

        source_coords = fix_periodicity(source_coords, topo_dims, periods);
        dest_coords   = fix_periodicity(dest_coords, topo_dims, periods);

        return std::make_pair(source_coords, dest_coords);
    }

    static std::array<int, N> fix_periodicity(std::array<int, N>   coords,
                                              std::array<idx_t, N> topo_dims,
                                              std::array<idx_t, N> periods) {

        std::array<int, N> new_coords{coords};

        for (size_t i = 0; i < N; ++i) {

            if (periods[i] == 1) {

                if (coords[i] == -1) {
                    new_coords[i] = int(topo_dims[i] - 1);
                } else if (coords[i] == int(topo_dims[i])) {
                    new_coords[i] = 0;
                }
            }
        }

        return new_coords;
    }

    static bool in_bounds(std::array<int, N>   coord,
                          std::array<idx_t, N> topo_dims) {

        for (idx_t i = 0; i < N; ++i) {
            if ((coord[i] < 0) || (coord[i] >= int(topo_dims[i]))) {
                return false;
            }
        }
        return true;
    }
};

template <size_t N>
BlockTopology<N> build_blocks(Point<N>             p0,
                              Point<N>             p1,
                              std::array<idx_t, N> dims,
                              std::array<idx_t, N> periodicity,
                              idx_t                n_blocks) {

    auto b_grid = split(dims, n_blocks);

    std::array<double, N> stepsize;

    for (idx_t i = 0; i < N; ++i) {
        stepsize[i] = (p1[i] - p0[i]) / double(dims[i]);
    }

    std::vector<Block<N>> blocks(n_blocks);

    idx_t id = 0;
    for (auto coord : md_indices(std::array<idx_t, N>{}, b_grid)) {

        auto block_density =
            LocalGlobalMapping<N>::local_extent(dims, b_grid, coord);

        auto [begin, end] =
            LocalGlobalMapping<N>::get_bounds(dims, b_grid, coord);

        Point<N> block_p0;
        Point<N> block_p1;

        for (idx_t i = 0; i < N; ++i) {
            block_p0[i] = begin[i] * stepsize[i];
            block_p1[i] = end[i] * stepsize[i];
        }

        Block<N> block(block_density, block_p0, block_p1, id);

        blocks.push_back(block);

        id++;
    }
}
*/

} // namespace JADA