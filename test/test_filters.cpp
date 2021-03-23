#include <digital_filters/filters.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <cmath>

typedef digital_filters::Filter<double,double> FilterDD;
typedef digital_filters::Filter<float,float> FilterFF;



// Make sure that we can change filter data types
TEST(TestFilters, TypeChange) {
  // Create a simple filter
  FilterDD filterdd({1.0, 0.5}, {1.0, 0.25});
  // Change the filter so that it uses floats
  FilterFF filterff = filterdd.as<float,float>();

  // Create a very simple input vector
  const unsigned int INPUT_SIZE = 20;
  const float INPUT_VALUE_F = 5.0f;
  const double INPUT_VALUE_D = static_cast<double>(INPUT_VALUE_F);
  std::vector<float> xf(INPUT_SIZE, INPUT_VALUE_F);
  std::vector<double> xd(INPUT_SIZE, INPUT_VALUE_D);

  // make sure that we get the same outputs (up to a small tolerance)
  for(unsigned int i=0; i<INPUT_SIZE; i++) {
    float yf = filterff.filter(xf[i]);
    double yd = filterdd.filter(xd[i]);
    ASSERT_NEAR(yf, yd, 1e-6);
  }
}


// Make sure that we can change filter data types
TEST(TestFilters, FilterSequence) {
  // Create a simple filter
  FilterDD filterdd({1.0, 0.5}, {1.0, 0.25});

  // init the filter
  std::vector<double> x0 = {-1.0};
  std::vector<double> y0 = {-2.0};
  filterdd.initInput(x0);
  filterdd.initOutput(y0);

  // Create a whole input sequence, then filter it
  std::vector<double> xseq(20, 5.0);
  auto yseq = filterdd.filter(x0, y0, xseq);

  // make sure that we get the same outputs (up to a small tolerance)
  for(unsigned int i=0; i<xseq.size(); i++) {
    auto yk = filterdd.filter(xseq.at(i));
    ASSERT_FLOAT_EQ(yk, yseq.at(i)) << "at step i=" << i;
  }
}


// Check that filtering with H=G*F is the same as filtering with F and then G
TEST(TestFilters, ConcatenateFilters) {
  // Create two filters
  FilterDD F({1.0, 0.5}, {1.0, 0.25});
  FilterDD G({3.0, -2.0, 1.0, -0.05}, {1.7, 0.5,-0.8});
  // Concatenate the filters, in both directions
  FilterDD H1 = G*F;
  FilterDD H2 = F*G;

  // make sure that we get the same outputs (up to a small tolerance)
  for(double t=0; t<10.0; t+=0.01) {
    double x = std::sin(t) + 0.5*std::cos(10*t);
    auto yfg = G.filter(F.filter(x));
    auto yh1 = H1.filter(x);
    auto yh2 = H2.filter(x);
    ASSERT_FLOAT_EQ(yfg, yh1) << "at time t=" << t;
    ASSERT_FLOAT_EQ(yfg, yh2) << "at time t=" << t;
  }
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
