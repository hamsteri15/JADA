#include <hpx/serialization.hpp>
#include <hpx/hpx.hpp>

#include "containers/md_indexable.hpp"
#include "loops/dimension.hpp"
#include "loops/position.hpp"
#include "loops/direction.hpp"
#include "grid/neighbours.hpp"

#define HPX_PPP_CAT(a, b, c) HPX_PP_CAT(HPX_PP_CAT(a, b), c)

namespace JADA {




///
///@brief partition_data
///
///@tparam N 
///@tparam T 
///
template<size_t N, class T>
struct partition_data : public MdIndexable<N, partition_data<N, T>> {
private:
    using buffer_type = hpx::serialization::serialize_buffer<T>;

public:

    partition_data() = default;
        

    // Create a new (uninitialized) partition of the given size.
    explicit partition_data(dimension<N> dims)
        : m_data( dims.elementwise_product())
        , m_dims(dims)
        {}


    //CRTP injections
    constexpr const T* get_ptr() const { return m_data.data();}
    constexpr       T* get_ptr()       { return m_data.data(); }
    constexpr dimension<N> const get_dimension() const { return m_dims; }

    

private:
    // Serialization support: even if all of the code below runs on one
    // locality only, we need to provide an (empty) implementation for the
    // serialization as all arguments passed to actions have to support this.
    friend class hpx::serialization::access;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int) {
        ar & m_data & m_dims; 
    }

private:
    buffer_type  m_data;
    dimension<N> m_dims;
    
};



template<size_t N, class T>
struct partition_server
  : hpx::components::component_base<partition_server<N, T>>
{
    static constexpr ConnectivityType CT = ConnectivityType::Box; //TODO: fix

    partition_server() = default;

    explicit partition_server(const partition_data<N, T>& data)
      : data_(data)
    {}

    explicit partition_server(dimension<N> dims)
      : data_(dims)
    {}

    // TODO: This should return the a slice of the data based on the direction and dims. Avoid copies if on same locality
    partition_data<N, T> get_data([[maybe_unused]] direction<N> dir, [[maybe_unused]] dimension<N> dims) const
    {

        //

        /*
        switch (t)
        {
        case left_partition:
            return partition_data(data_, data_.size()-1);

        case middle_partition:
            break;

        case right_partition:
            return partition_data(data_, 0);

        default:
            HPX_ASSERT(false);
            break;
        }
        */
        return data_;
    }

    // Every member function which has to be invoked remotely needs to be
    // wrapped into a component action. The macro below defines a new type
    // 'get_data_action' which represents the (possibly remote) member function
    // partition::get_data().

    //NEED TO USE PPCAT HERE AS WELL
    //HPX_DEFINE_COMPONENT_DIRECT_ACTION(partition_server, get_data, get_data_action_double2);
    HPX_DEFINE_COMPONENT_DIRECT_ACTION(partition_server, get_data, HPX_PPP_CAT(get_data_action, N, T));

private:
    partition_data<N, T> data_;
};




} // namespace JADA


/*

WITHOUT TEMPLATE PARAMETERS 

// The macros below are necessary to generate the code required for exposing
// our partition type remotely.
//
// HPX_REGISTER_COMPONENT() exposes the component creation
// through hpx::new_<>().
typedef hpx::components::component<partition_server> partition_server_type;
HPX_REGISTER_COMPONENT(partition_server_type, partition_server);

// HPX_REGISTER_ACTION() exposes the component member function for remote
// invocation.
typedef partition_server::get_data_action get_data_action;
HPX_REGISTER_ACTION(get_data_action);

*/

//WITH TEMPLATE PARAMETERS

//This doesnt work, check hpx/examples/accumulators/template_accumulator

#define REGISTER_PARTITION_SERVER_DECLARATION(N, type)\
    HPX_REGISTER_ACTION_DECLARATION(\
        JADA::partition_server<N, type>::get_data_action,\
        HPX_PPP_CAT(__partition_server_get_data_action_, N, type));         


#define REGISTER_PARTITION_SERVER(N, type)\
    HPX_REGISTER_ACTION(\
        JADA::partition_server<N,type>::get_data_action,\
        HPX_PPP_CAT(__partition_server_get_data_action_, N, type));\
    typedef ::hpx::components::component<\
        JADA::partition_server<N,type>\
    > HPX_PPP_CAT(__partition_server_, N, type);\
    HPX_REGISTER_COMPONENT(HPX_PPP_CAT(__partition_server_, N, type))        


