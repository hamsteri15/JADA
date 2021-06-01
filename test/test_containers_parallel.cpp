#include "catch.hpp"
#include "containers/structured_data_view.hpp"
#include "communication/hpx_md_communicator.hpp"
#include "ops.hpp"
#include "stencil/apply_stencil.hpp"
#include "containers/field.hpp"
#include "containers/partitioned_vector.hpp"


//REGISTER_PARTITION_SERVER(2, double);
//REGISTER_PARTITION_SERVER(2, int);

/*
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
*/




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


    SECTION("local_segment_count() / local_first_segment_number()"){

        auto v1 = make_partitioned_vector<2, int>(dimension<2>{2, 4});
        auto v2 = make_partitioned_vector<2, int>(dimension<2>{13, 5});
        auto v3 = make_partitioned_vector<2, int>(dimension<2>{51, 55});
        auto v4 = make_partitioned_vector<2, int>(dimension<2>{5, 1});

        std::vector<hpx::partitioned_vector<int>> vs = {v1, v2, v3, v4};

        auto n_locs = size_t(hpx::get_num_localities().get());

        for (size_t i = 0; i < n_locs; ++i){

        for (auto v : vs) {
            auto local_sbegin = v.segment_begin(uint32_t(i));
            auto local_send = v.segment_end(uint32_t(i));

            size_t count = local_segment_count(v, uint32_t(i));
            size_t s0 = local_first_segment_number(v, uint32_t(i));
            size_t s1 = local_last_segment_number(v, uint32_t(i));

            CHECK(count == size_t(std::distance(local_sbegin, local_send)));
            CHECK(count == s1 - s0);

        } 

        }

        //auto v1 = make_partitioned_vector<2, int>(dimension<2>{13, 5});

//        CHECK(local_segment_count(v1) == size_t(13) / hpx::get_num_localities());



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