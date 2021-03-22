#include <digital_filters/utilities.hpp>
#include <gtest/gtest.h>
#include <sstream>


void checkEquals(
  const std::vector<double>& a,
  const std::vector<double>& b,
  double tolerance
)
{
  ASSERT_EQ(a.size(), b.size())
    << "Mismatched size; the compared vectors are:" << std::endl
    << "[ " << digital_filters::vec2str(a) << " ]" << std::endl
    << "[ " << digital_filters::vec2str(b) << " ]";
  for(unsigned int i=0; i<a.size(); i++) {
    ASSERT_NEAR(a[i], b[i], tolerance)
      << "Elements with index " << i << " are different; the compared vectors are:" << std::endl
      << "[ " << digital_filters::vec2str(a) << " ]" << std::endl
      << "[ " << digital_filters::vec2str(b) << " ]";
  }
}


TEST(TestUtilities, PolyProd) {
  // 1 + x + x**2
  std::vector<double> p1 = {1.0, 1.0, 1.0};
  // 1 + x
  std::vector<double> p2 = {1.0, 1.0};
  // (1+x) * (1+x+x**2) = 1 + 2x + 2x**2 + x**3
  std::vector<double> p3 = {1.0, 2.0, 2.0, 1.0};
  // get the product and check that it is the expected one
  auto pa = digital_filters::polyProd(p1, p2);
  checkEquals(p3, pa, 1e-6);
  // this is just to check that commutativity works ;)
  auto pb = digital_filters::polyProd(p2, p1);
  checkEquals(p3, pb, 1e-6);

  // test with other polynomials
  p1 = { 1.0, -2.0, 0.0, 1.5};
  p2 = {-0.2, 0.0, -2.5, -1.5};
  p3 = {-0.2, 0.4, -2.5, 3.2, 3.0, -3.75, -2.25};
  // pa = p1 * p2
  pa = digital_filters::polyProd(p1, p2);
  checkEquals(p3, pa, 1e-6);
  // pb = p2 * p1
  pb = digital_filters::polyProd(p2, p1);
  checkEquals(p3, pb, 1e-6);
}


int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
