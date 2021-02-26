#define CATCH_CONFIG_RUNNER

#include "catch.hpp"
#include <hpx/hpx_init.hpp>
#include <hpx/hpx_start.hpp>

int hpx_main(int argc, char* argv[])
{
    // Any HPX application logic goes here...
    Catch::Session session;

    std::cout << "Running on " << hpx::get_num_localities(hpx::launch::sync) << " localities." << std::endl;


    [[maybe_unused]] const int result = session.run(argc, argv);
    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    return hpx::init(argc, argv); 
}
