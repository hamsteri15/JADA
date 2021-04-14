#pragma once

#include "containers/md_array.hpp"
#include "containers/halo_offsets.hpp"
#include "grid/neighbours.hpp"
#include "loops/direction.hpp"

namespace JADA {

template <size_t N, class T, ConnectivityType CT = ConnectivityType::Box> 
struct StructuredData {

    using storage_t = MdArray<N, T, StorageOrder::RowMajor>;
    

    StructuredData() = default;


    ///
    ///@brief Construct from dimensions and padding
    ///
    ///@param dim dimensions of the data
    ///@param padding padding to allow element access outside dimensions (for neighbour communication etc.)
    ///
    StructuredData(dimension<N> dim, dimension<N> padding)
        : m_dim(dim)
        , m_padding(padding)
        , m_data(dim)
        , m_halos(create_halos(dim, padding)) {}



    StructuredData(const storage_t& interior, dimension<N> dim, dimension<N> padding) : 
    StructuredData(dim, padding)
    {
        m_data = interior;
    }


    const storage_t& get_interior() const {
        return m_data;
    }




    ///
    ///@brief Puts data to the halo storage indicated by dir
    ///
    ///@param data data to put
    ///@param dir  direction indicating where to put
    ///
    void put_halo(const storage_t& data, direction<N> dir) {

        size_t idx = static_cast<size_t>(m_neighbours.idx(dir));
        Utils::runtime_assert(m_halos.at(idx).size() == data.size(),
                              "Halo size mismatch");
        m_halos.at(idx) = data;
    }


    ///
    ///@brief Get the halo data from direction dir
    ///
    ///@param dir direction to get the data from 
    ///@return const storage_t& the data at direction
    ///
    const storage_t& get_halo(direction<N> dir) const {
        size_t idx = static_cast<size_t>(m_neighbours.idx(dir));
        return m_halos.at(idx);
    }


    ///
    ///@brief Get the halo data from direction dir
    ///
    ///@param dir direction to get the data from 
    ///@return storage_t& the data at direction
    ///
    storage_t& get_halo(direction<N> dir) {
        size_t idx = static_cast<size_t>(m_neighbours.idx(dir));
        return m_halos.at(idx);
    }



    ///
    ///@brief Returns a contiguous storage where the halos are placed in to their correct positions
    ///       based on the directions.
    ///
    ///@return storage_t a contiguous storage with dimensions: dimensions + 2*padding
    ///
    storage_t get_combined() const {

        auto dims = m_dim + m_padding * size_t(2);
        MdArray<N, T> ret(dims);

        for (auto pos : md_indices(position<N>{}, position<N>(dims))){
            auto my_pos = pos - position<N>(m_padding);
            ret[pos]= this->at(my_pos);
        }
        return ret;

    }


    ///
    ///@brief Get a non-modifyable reference to the element at position pos. This call allows for
    ///       accessing data at the halos such that the input pos can exceed the m_dims
    ///       by min/max m_padding. For example dims = {10, 10} and padding{1,1}, would allow
    ///       positions: {-1, -1} or {11, 11} to be accessed
    ///
    ///@param pos the position of the element to get
    ///@return const T& reference to element at pos
    ///
    const T& at(position<N> pos) const {

        if (in_interior(pos)) {
            return m_data[pos];
        }
        return const_halo_element_access(pos);
    }

    ///
    ///@brief Get a modifyable reference to the element at position pos. This call allows for
    ///       accessing data at the halos such that the input pos can exceed the m_dims
    ///       by min/max m_padding. For example dims = {10, 10} and padding{1,1}, would allow
    ///       positions: {-1, -1} or {11, 11} to be accessed
    ///
    ///@param pos the position of the element to get
    ///@return T& reference to element at pos
    ///
    T& at(position<N> pos) {

        if (in_interior(pos)) {
            return m_data[pos];
        }
        return halo_element_access(pos);
    }



    position<N> begin() const {
        return position<N>();
    }

    position<N> end() const {
        return m_dim;
    }


    dimension<N> get_padding() const {
        return m_padding;
    }

    dimension<N> get_dimension() const {
        return m_dim;
    }



private:
    static constexpr Neighbours<N, CT> m_neighbours{};

    dimension<N>               m_dim;
    dimension<N>               m_padding;
    storage_t              m_data;
    std::vector<storage_t> m_halos;

    ///
    ///@brief Checks if a position belongs to interior region (m_data)
    ///
    ///@param pos position to query
    ///@return true if belongs
    ///@return false otherwise
    ///
    bool in_interior(position<N> pos) const {

        return pos.all_positive() && (pos < m_dim);
    }

    T& halo_element_access(position<N> pos) {

        const auto dir = get_direction(pos);
        const auto part_idx = m_neighbours.idx(dir);
        const auto x = pos - halo_begin(dir);

        return m_halos[size_t(part_idx)][x];

    }

    const T& const_halo_element_access(position<N> pos) const{

        const auto dir = get_direction(pos);
        const auto part_idx = m_neighbours.idx(dir);
        const auto x = pos - halo_begin(dir);

        return m_halos[size_t(part_idx)][x];

    }

    ///
    ///@brief Creates the galo buffers based on the dimensions and padding.
    ///
    ///@param dim dimensions of the "internal" data
    ///@param padding external padding to the internal data
    ///@return std::vector<storage_t> vector of halos in correct order
    ///
    static std::vector<storage_t> create_halos(dimension<N> dim,
                                                   dimension<N> padding) {

        std::vector<storage_t> ret; 
        ret.reserve(m_neighbours.count()); 

        for (direction<N> dir : m_neighbours.get()) {

            ret.emplace_back(
                //MdArray<N,T>(halo_dims(dim, padding, dir))
                MdArray<N,T>(compute_halo_dims(dim, dir, padding))
            );            

        }
        return ret;

    }

    ///
    ///@brief Get the beginning position of the halo at direction dir
    ///
    ///@param dir direction of the halo region
    ///@return position<N> beginning position of the halo
    ///
    position<N> halo_begin(direction<N> dir) const {

        position<N> begin{};

        for (size_t i = 0; i < N; ++i){
            if (dir[i] < 0) {
                begin[i] = -idx_t(m_padding[i]);
            }

            if (dir[i] > 0) {
                begin[i] = idx_t(m_dim[i]);
            }
        }
        return begin;
    }

    static dimension<N> compute_halo_dims(dimension<N> dims,
                            position<N>  dir,
                            dimension<N> padding) {

        dimension<N> h_dims{};

        for (size_t i = 0; i < N; ++i) {
            if (dir[i] != 0) {
                h_dims[i] = padding[i];
            } 
            else {
                h_dims[i] = dims[i];
            }
        }

        return h_dims;
    }




    direction<N> get_direction(position<N> pos) const {


        direction<N> dir{};

        for (size_t i = 0; i < N; ++i) {

            if (pos[i] < 0) {
                dir[i] = -1;
            }

            else if (pos[i] >= idx_t(m_dim[i])) {
                dir[i] = 1;
            }
        }
        return dir;
    }



};

} // namespace JADA
