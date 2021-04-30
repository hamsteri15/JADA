#pragma once

#include "containers/dimension_handle.hpp"

namespace JADA{


template<size_t N, ConnectivityType CT>
struct StructuredDataView{



private:

    DimensionHandle<N, CT> m_dim_handle;


};





}