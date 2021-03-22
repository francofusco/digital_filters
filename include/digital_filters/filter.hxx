#pragma once

#include <stdexcept>


namespace digital_filters {

template<class DataType, class CoeffType>
Filter<DataType,CoeffType>::Filter(
  const std::vector<CoeffType>& b_num,
  const std::vector<CoeffType>& a_den
)
: b_(b_num)
, a_(a_den)
, in_(b_num.size())
, out_(a_den.size())
{
  // check sizes
  if(b_num.size() == 0)
    throw std::runtime_error("Filter: numerator (b) is empty");
  if(a_den.size() == 0)
    throw std::runtime_error("Filter: denominator (a) is empty");

  // normalize numerator and denominator
  for(auto& bi : b_)
    bi = bi / a_den[0];
  for(auto& ai : a_)
    ai = ai / a_den[0];
}


template<class DataType, class CoeffType>
Filter<DataType,CoeffType> Filter<DataType,CoeffType>::operator*(
  const Filter<DataType,CoeffType>& other
) const
{
  return Filter<DataType,CoeffType>( polyProd(b_, other.b), polyProd(a_, other.a_) );
}


template<class DataType, class CoeffType>
template<class OtherDataType, class OtherCoeffType>
Filter<OtherDataType,OtherCoeffType> Filter<DataType,CoeffType>::as() const
{
  // copy-convert numerator and denominator
  std::vector<OtherCoeffType> a(a_.begin(), a_.end());
  std::vector<OtherCoeffType> b(b_.begin(), b_.end());
  // create new filter
  auto filter = Filter<OtherDataType,OtherCoeffType>(b,a);
  // make sure to initialize the inputs and outputs
  filter.initInput(std::vector<OtherCoeffType>(in_.begin(), in_.end()));
  filter.initOutput(std::vector<OtherCoeffType>(out_.begin(), out_.end()));
  // return the result
  return filter;
}


template<class DataType, class CoeffType>
void Filter<DataType,CoeffType>::initInput(
  const DataType& input
)
{
  for(auto& val : in_)
    val = input;
}


template<class DataType, class CoeffType>
void Filter<DataType,CoeffType>::initInput(
  const std::vector<DataType>& input
)
{
  // check sizes
  if(input.size() != in_.size() && input.size() != in_.size()-1) {
    throw std::runtime_error(
      "Filter::initInput: 'input' has " + std::to_string(input.size()) +
      " elements, but only " + std::to_string(in_.size()) + " or " +
      std::to_string(in_.size()-1) + " elements are allowed"
    );
  }
  // copy the values
  for(unsigned int i=0; i<input.size(); i++)
    in_[i] = input[i];
}


template<class DataType, class CoeffType>
void Filter<DataType,CoeffType>::initOutput(
  const DataType& output
)
{
  for(auto& val : out_)
    val = output;
}


template<class DataType, class CoeffType>
void Filter<DataType,CoeffType>::initOutput(
  const std::vector<DataType>& output
)
{
  // check sizes
  if(output.size() != out_.size() && output.size() != out_.size()-1) {
    throw std::runtime_error(
      "Filter::initInput: 'output' has " + std::to_string(output.size()) +
      " elements, but only " + std::to_string(out_.size()) + " or " +
      std::to_string(out_.size()-1) + " elements are allowed"
    );
  }
  // copy the values
  for(unsigned int i=0; i<output.size(); i++)
    out_[i] = output[i];
}


template<class DataType, class CoeffType>
const DataType& Filter<DataType,CoeffType>::filter(
  const DataType& x
)
{
  // remove unnecessary input/output
  in_.pop_back();
  out_.pop_back();

  // add new input/output
  in_.push_front(x);
  out_.push_front(x * b_[0]);

  // complete the filtering
  auto& y = out_[0];

  for(unsigned int i=1; i<b_.size(); i++)
    y = y + b_[i] * in_[i];
  for(unsigned int i=1; i<a_.size(); i++)
    y = y - a_[i] * out_[i];
  return y;
}


template<class DataType, class CoeffType>
std::vector<DataType> filter(
  const std::vector<DataType>& x,
  const std::vector<DataType>& y0
)
{
  #warning "Filtering sequences: yet to be implemented"
  throw std::runtime_error(std::string(__FILE__) + " line " + std::to_string(__LINE__) + std::string(__func__) + " not implemented");
}

} // namespace digital_filters
