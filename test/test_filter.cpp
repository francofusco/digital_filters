#include <digital_filters/filters.hpp>
#include <gtest/gtest.h>
#include <sstream>

typedef digital_filters::Filter<double,double> FilterDD;
typedef digital_filters::Filter<float,float> FilterFF;



TEST(TestFilters, ConstructorCompiles) {
  FilterDD filterdd({1, 2}, {-1});
  FilterFF filterff = filterdd.as<float,float>();
}


TEST(TestFilters, ConstructFilters) {
  FilterDD butter = digital_filters::butterworth<double,double>(4, 20, 100);
  FilterDD avg1 = digital_filters::average<double,double>(5);
  FilterDD avg2 = digital_filters::average<double,double>(5, 3.5);
  FilterDD expo1 = digital_filters::exponential<double,double>(0.8);
  FilterDD expo2 = digital_filters::exponential<double,double>(0.01, 0.05);
}


int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
