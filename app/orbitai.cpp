#include <mochimochi/binary_classifier.hpp>
#include <mochimochi/utility.hpp>
#include <boost/program_options.hpp>
#include <iostream>

int main(const int ac, const char* const * const av) {


  using namespace boost::program_options;

  options_description description("options");
  description.add_options()
    ("help", "")
    ("dim", value<int>()->default_value(0), "データの次元数")
    ("train", value<std::string>()->default_value(""), "学習データのファイルパス")
    ("test", value<std::string>()->default_value(""), "評価データのファイルパス")
    ("c", value<double>()->default_value(0.5), "ハイパパラメータ(c)")
    ("eta", value<double>()->default_value(0.5), "ハイパパラメータ(eta)");

  variables_map vm;
  store(parse_command_line(ac, av, description), vm);
  notify(vm);

  if(vm.count("help")) { std::cout << description << std::endl; }


  std::cout << "success..." << std::endl;

  
  return 0;
}
