#include <digital_filters/filters.hpp>
#include <fstream>
#include <map>
#include <random>

typedef digital_filters::Filter<double,double> FilterDD;

int main(int argc, char** argv) {
  // get the name of the file
  std::string filename = argc < 2 ? "filter.txt" : argv[1];
  // open the file
  std::ofstream file;
  file.open(filename);
  // check that the file was actually opened
  if(!file.is_open()) {
    throw std::runtime_error("Faild to open '" + filename + "'");
  }

  // time constants
  const double dt = 0.05;
  const double Fs = 1/dt;
  const double Tmax = 10.0;

  // random number generation
  std::random_device rand_dev;
  std::mt19937 gen(rand_dev());
  std::uniform_real_distribution<double> noise(-0.02, 0.02);


  // Construct some filters
  std::map<std::string,FilterDD> filters;
  filters.insert(std::make_pair("butterworth", digital_filters::butterworth<double,double>(4, Fs/4, Fs)));
  filters.insert(std::make_pair("average", digital_filters::average<double,double>(5)));
  filters.insert(std::make_pair("exponential", digital_filters::exponential<double,double>(dt, 3*dt)));

  // write the header of the file
  file << "time raw";
  for(const auto& filt : filters)
    file << " " << filt.first;
  file << std::endl;

  // time to filter!
  for(double t=0.0; t<=Tmax; t+=dt) {
    // generate raw signal
    double x = std::sin(t) + 0.1*std::cos(3*t) + 0.1 * std::sin(20*t) + noise(gen);
    file << t << " " << x;

    // filter and write down data for each filter
    for(auto& filt : filters)
      file << " " << filt.second.filter(x);

    // don't forget this! ;)
    file << std::endl;
  }

  // close the file at the end
  file.close();

  return 0;
}
