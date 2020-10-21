#pragma once

#include <vector>
#include <array>
#include "grid/block.hpp"
#include "grid/split.hpp"
#include "grid/local_global_mapping.hpp"
#include "loops/unflatten_index.hpp"
#include "utils/runtime_assert.hpp"

namespace JADA{

template<idx_t Dim>
struct BlockTopology : private LocalGlobalMapping<Dim>{

    BlockTopology(Block<Dim> parent, idx_t n_children) : 
    m_parent(parent),
    m_dimensions(split(parent.dimensions, n_children)),
    m_children(create_children(parent, m_dimensions, n_children))
    {}


    Block<Dim> get_child(idx_t id) const {
        
        Utils::runtime_assert(id < m_children.size(), "Invalid child id");        
        return m_children[id];
    }


    index_generator<Dim> global_md_indices(idx_t child_id) {

        auto child_coords = topo_coords(child_id);
        auto [begin, end] = this->get_bounds(m_parent.dimensions, m_dimensions, child_coords);
        return md_indices(begin, end);

    }

    index_generator<Dim> local_md_indices(idx_t child_id) {
        auto child = get_child(child_id);
        return md_indices(position<Dim>{}, child.dimensions);
    }




    std::vector<Block<Dim>> get_children() const {
        return m_children;
    }


    Block<Dim> get_parent() const {return m_parent;}




private:

    Block<Dim> m_parent;
    std::array<idx_t, Dim> m_dimensions;
    std::vector<Block<Dim>> m_children;


    static std::vector<Block<Dim>> create_children(Block<Dim> parent, std::array<idx_t, Dim> topo_dims, idx_t n_children){

        std::vector<Block<Dim>> blocks(n_children);

        idx_t counter = 0;
        for (auto pos : md_indices(std::array<idx_t, Dim>{}, topo_dims)) {
            auto coords = pos;
            auto new_dims = BlockTopology::local_extent(parent.dimensions, topo_dims, coords);
            blocks[counter] = Block(new_dims, counter);
            counter++;
        }

        
        return blocks;


    }


    std::array<idx_t, Dim> topo_coords(idx_t child_id){

        return unflatten(m_dimensions, child_id);

    }




};




}