#include "catch.hpp"
#include "containers/structured_data_view.hpp"
#include "communication/hpx_md_communicator.hpp"
#include "ops.hpp"
#include "stencil/apply_stencil.hpp"
#include "containers/field.hpp"
#include "containers/partitioned_vector.hpp"


//REGISTER_PARTITION_SERVER(2, double);
//REGISTER_PARTITION_SERVER(2, int);


TEST_CASE("Test StructuredDataView"){

    
    using namespace JADA;

    using comm_data_t = std::vector<int>;
        
    using comm_t = HpxMdCommunicator<comm_data_t, 2, ConnectivityType::Box>;
    std::string comm_name = "structuredDataView2by2";

    size_t num_local_partitions = 2;
    size_t num_localities = hpx::get_num_localities(hpx::launch::sync);
    size_t n_partitions = num_local_partitions * num_localities;
    size_t rank = hpx::get_locality_id();

    dimension<2> global_dims = {10, 10};
    std::array<bool, 2> periodicity  = {true, true};

        
    Decomposition<2> dec(
        global_dims,
        n_partitions,
        periodicity
    );

        

    std::vector<comm_t> comms;
    std::vector<comm_data_t> i_data;
  
    for (size_t i = 0; i < num_local_partitions; ++i){

        idx_t id = idx_t(rank * num_local_partitions + i);
        comms.push_back(
            comm_t(comm_name, id, dec)
        );

        auto local_dims = dec.local_grid_dimensions(id);
        i_data.push_back(comm_data_t(local_dims.elementwise_product(), 1));
        call_sets(i_data[i], local_dims, OpBox{}, comms[i], size_t(0));

    }

    for (size_t i = 0; i < num_local_partitions; ++i){

        auto comm = comms[i];
        auto data = i_data[i];
        idx_t id = idx_t(rank * num_local_partitions + i);


        std::vector<direction<2>> dirs;
        std::vector<hpx::shared_future<comm_data_t>> datas;

        for (auto dir : comm.get_directions()){

            
            if (comm.has_neighbour(dir)) {
                dirs.push_back(dir);
                datas.push_back(comm.get_shared(dir, size_t(0)));

            }

        }

        auto l_dims = dec.local_grid_dimensions(id);
        dimension<2> padding = {1,1};
        auto view = StructuredDataView<2, int, ConnectivityType::Box>(l_dims, padding, i_data[i], datas, dirs);


        REQUIRE_NOTHROW(view.at({-1, 0}));
        REQUIRE_NOTHROW(view.at({-1, -1}));
        REQUIRE_NOTHROW(view.at({-1, 4}));
        


    }



}

namespace JADA{

/*

template<size_t N, class T>
hpx::partitioned_vector<T> make_vector(dimension<N> dims) {

    auto num_segments = n_partitions(dims);

    std::vector<hpx::id_type> locs = hpx::find_all_localities();

    auto layout = hpx::container_layout( num_segments, locs );

    return hpx::partitioned_vector<T>(dims.elementwise_product(), layout);


}



//This returns iterators to the beginning and end of segment number seg_n
template<class T>
auto get_segment(hpx::partitioned_vector<T> v, size_t seg_n) {

    Utils::runtime_assert(seg_n < n_partitions(v), "Segment out of bounds.");

    using iterator = hpx::partitioned_vector<T>::iterator;
    using traits   = hpx::traits::segmented_iterator_traits<iterator>;

    auto seg_it = traits::segment(v.begin()) + std::ptrdiff_t(seg_n);


    auto loc_begin = traits::begin(seg_it);
    auto loc_end   = traits::end(seg_it);

    return std::make_pair(loc_begin, loc_end);

}



template<size_t N, class T>
auto get_segment(hpx::partitioned_vector<T> v, dimension<N> dims, position<N - 1> pos){

    dimension<N - 1> seg_dims;
    for (size_t i = 0; i < N - 1; ++i){
        seg_dims[i] = dims[i];
    }


    auto idx = flatten<N - 1, StorageOrder::RowMajor>(seg_dims, pos);

    return get_segment(v, size_t(idx));



}
*/
}


TEST_CASE("Test partioned vector"){

    using namespace JADA;

    SECTION("segment_count()"){

        CHECK(segment_count(dimension<1>{151}) == size_t(1));
        CHECK(segment_count(dimension<2>{10, 11}) == size_t(10));
        CHECK(segment_count(dimension<3>{10, 11, 12}) == size_t(10 * 11));



    }


    SECTION("make_partioned_vector()"){


        auto v1 = make_partitioned_vector<2, int>(dimension<2>{11, 10});

        CHECK(v1.size() == size_t(11 * 10));
        CHECK(segment_count(v1) == 11);

    }

    SECTION("get_segment()") {

        auto v1  =make_partitioned_vector<2, int>(dimension<2>{9, 4});

        for (size_t i = 0; i < segment_count(v1); ++i){
            auto [begin, end] = get_segment(v1, i);

            REQUIRE(std::distance(begin, end) == 4);

        }



    }

    SECTION("segment_sizes()") {


        auto v1  =make_partitioned_vector<2, int>(dimension<2>{2, 4});

        CHECK(segment_sizes(v1) == std::vector<size_t>{4,4});


    }

    SECTION("uniform_segments()"){


        auto v1 = make_partitioned_vector<2, int>(dimension<2>{2, 4});
        auto v2 = make_partitioned_vector<2, int>(dimension<2>{13, 5});
        auto v3 = make_partitioned_vector<2, int>(dimension<2>{51, 55});
        auto v4 = make_partitioned_vector<2, int>(dimension<2>{5, 1});

        CHECK(uniform_segments(v1));
        CHECK(uniform_segments(v2));
        CHECK(uniform_segments(v3));
        CHECK(uniform_segments(v4));



    }


    SECTION("local_segment_count()"){

        auto v1 = make_partitioned_vector<2, int>(dimension<2>{13, 5});

        CHECK(local_segment_count(v1) == size_t(13));



    }



}


TEST_CASE("Test Field"){

    using namespace JADA;


    SECTION("Constructors"){

        Field<2, int> f1({1,1});
        Field<2, double> f2({1,1});

        f1 = 3;
        f2 = 4.0;
        
        CHECK(*f1.begin() == 3);
        CHECK(*f2.begin() == 4.0);


    }


    SECTION("ASD"){

        /*
        dimension<2> dims = {10, 11};
        auto v = make_vector<2, int>(dims);


        


        std::iota(v.begin(), v.end(), 0);


        auto [s_begin, s_end] = get_segment(v, 1);

        for (auto it = s_begin; it != s_end; ++it){
            std::cout << *it << std::endl;
        }
        */


    }



}