#include <digital_filters/filters.hpp>
#include <fstream>
#include <map>
#include <random>


int main(int argc, char** argv) {

  // time constants
  const double dt = 0.05;
  const double Fs = 1/dt;
  const double Tmax = 10.0;

  // random number generation
  std::random_device rand_dev;
  std::mt19937 gen(rand_dev());
  std::uniform_real_distribution<double> noise(-0.05, 0.05);

  // Create a Butterworth filter
  auto filter = digital_filters::butterworth<double,double>(3, Fs/25, Fs);

  // Generate a noisy signal
  std::vector<double> x;
  x.reserve(static_cast<unsigned int>(Tmax/dt));
  for(double t=0.0; t<=Tmax; t+=dt) {
    x.push_back(std::sin(t) + 0.1 * std::sin(20*t) + noise(gen));
  }

  // filter using the single and double filtering
  std::vector<double> x0(filter.numerator().size()-1, x[0]);
  std::vector<double> y0(filter.denominator().size()-1, x[0]);
  auto y1 = filter.filter(x, x0, y0);
  auto y2 = filter.filter2(x);

  // open a file to store the results
  std::string filename = argc < 2 ? "filter2.txt" : argv[1];
  std::ofstream file;
  file.open(filename);
  // check that the file was actually opened
  if(!file.is_open()) {
    throw std::runtime_error("Faild to open '" + filename + "'");
  }

  // write down the header and then the signals
  file << "time raw filter filter2" << std::endl;
  for(unsigned int i=0; i<x.size(); i++) {
    file << i*dt << " " << x[i] << " " << y1[i] << " " << y2[i] << std::endl;
  }

  // close the file at the end
  file.close();

  return 0;
}
