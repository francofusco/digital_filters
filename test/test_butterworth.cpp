#include <digital_filters/filters.hpp>
#include <gtest/gtest.h>

typedef digital_filters::Filter<double,double> FilterDD;

/*
NOTE: the "ground-truth" has been obtained via the following script:

from scipy.signal import butter, lfilter
import numpy as np
order = 7
cutoff = 40.
sampling = 100.
wn = 2*cutoff/sampling
b,a = butter(order, wn)
print("std::vector<double> a = {", ', '.join(map(str,a.tolist())), "};")
print("std::vector<double> b = {", ', '.join(map(str,b.tolist())), "};")
x = np.zeros((20,))
x[5:15] = 1.
y = lfilter(b, a, x)
print("std::vector<double> x = {", ', '.join(map(str,x.tolist())), "};")
print("std::vector<double> y = {", ', '.join(map(str,y.tolist())), "};")

*/


class ButterworthFixture : public ::testing::Test {
protected:
  ButterworthFixture()
  : order(7)
  , cutoff(40.)
  , sampling(100.)
  , filter(digital_filters::butterworth<double,double>(order, cutoff, sampling))
  {
    // no extra code needed
  }

  const int order;
  const double cutoff;
  const double sampling;
  FilterDD filter;
};


TEST_F(ButterworthFixture, Coefficients) {
  // these come from
  std::vector<double> a = { 1.0, 4.182330089320615, 7.871719202212808, 8.530942129298456, 5.7099448078894826, 2.3492472280524366, 0.5482647747786813, 0.05584467100692739 };
  std::vector<double> b = { 0.2363147883012453, 1.6542035181087171, 4.962610554326151, 8.271017590543586, 8.271017590543586, 4.962610554326151, 1.6542035181087171, 0.2363147883012453 };

  ASSERT_EQ(a.size(), filter.denominator().size());
  ASSERT_EQ(b.size(), filter.numerator().size());

  for(unsigned int i=0; i<a.size(); i++) {
    ASSERT_FLOAT_EQ(a.at(i), filter.denominator().at(i)) << "Mismatching a[" << i << "]";
  }

  for(unsigned int i=0; i<b.size(); i++) {
    ASSERT_FLOAT_EQ(b.at(i), filter.numerator().at(i)) << "Mismatching b[" << i << "]";
  }
}


TEST_F(ButterworthFixture, Filtering) {
  std::vector<double> x0(filter.numerator().size()-1);
  std::vector<double> y0(filter.denominator().size()-1);
  std::vector<double> x = { 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  std::vector<double> y = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.2363147883012453, 0.9021718567462329, 1.219744701690328, 0.905140172370198, 0.9623609747014226, 1.0957692260718024, 0.9183129814554234, 1.030563189595846, 1.0209410448749332, 0.9500231315585492, 0.8145064916887352, 0.0670177297560195, -0.21652124034665265, 0.1143303065500686, 0.007708014753141312 };
  std::vector<double> yfilt = filter.filter(x, x0, y0);

  for(unsigned int i=0; i<y.size(); i++) {
    ASSERT_FLOAT_EQ(y.at(i), yfilt.at(i)) << "Mismatching sample i=" << i;
  }
}


int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
