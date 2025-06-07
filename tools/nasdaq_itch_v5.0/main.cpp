#include "utils/program_options/log.hpp"
//#include <boost/program_options.hpp>
#include "loggers.hpp"
#include <boost/format.hpp>
#include <list>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>


boost::program_options::options_description common_options()
{
   auto description = boost::program_options::options_description{ "configuration" };
   // clang-format off
   description.add_options()
       ("filename,f", boost::program_options::value<std::string>()->required(), "\tnasdaq file")
       ("verbose,v",  boost::program_options::value<bool>()->default_value(false)->implicit_value(true), "\tverbose")
       ("buffer-size,b", boost::program_options::value<std::size_t>()->default_value(1500), "\tbuffer size")
       ("unpack-first,u", boost::program_options::value<bool>()->default_value(false)->implicit_value(true), "\tfirst unpack, than process")
       ("run,r", boost::program_options::value<std::string>()->default_value("count"), "\trun one of: count, instrument-stats, dump, snapshot, hdf5")
       ;
   // clang-format on
   return description;
}

std::ostream& help(std::ostream& out, boost::program_options::options_description& description)
{
   // clang-format off
    out << description
        << "\n\n more details: https://github.com/serge-klim/nasdaq/README.md \n";
   // clang-format on
   return out;
}

bool parse_config(boost::program_options::options_description const& description,boost::program_options::variables_map& vm)
{
   auto res = false;
   auto const& config_opt = vm["config"];
   auto config_filename = config_opt.empty() ? std::make_pair(false, std::string{"config.ini"}) : std::make_pair(true, config_opt.as<std::string>());
   if (auto ifs = std::ifstream{config_filename.second}) {

      auto parsed = parse_config_file(ifs, description, true);
      store(parsed, vm);

      //auto const& additional = collect_unrecognized(parsed.options, boost::program_options::include_positional);
      //init_log_from_unrecognized_program_options(additional);
      if (!init_log_from_unrecognized_program_options(parsed, vm))
          configure_default_logger(vm);

      //notify(vm); // check config file options sanity
      res = true;
   } else if (config_filename.first)
      throw std::runtime_error{str(boost::format("can't open configuration file \"%1%\"") % config_filename.second)};
   return res;
}



int main(int argc, char* argv[])
{
   auto description = boost::program_options::options_description{ "general" };
   try {
      // clang-format off
        description.add_options()
            ("help,h", "\tprint usage message")
            ("config,c", boost::program_options::value<std::string>(), "\tconfiguration file")
            ;
      // clang-format on

     auto config_file_options = common_options();
     description.add(config_file_options);

     boost::program_options::positional_options_description positional;
     positional.add("filename", -1);
       
     auto parsed = boost::program_options::command_line_parser(
         argc, argv)
         .options(description).positional(positional).run();
     boost::program_options::variables_map vm;
     store(parsed, vm);

     if (vm.count("help") != 0) {
        help(std::cout, description);
        return 0;
     }
     if (!parse_config(config_file_options, vm) && !init_log_from_unrecognized_program_options(parsed, vm))
         /*add_unrecognized_program_options(parsed, vm);*/
         configure_default_logger(vm);

     notify(vm); // check cmd-line option sanity
     void run(boost::program_options::variables_map const& options);
     run(vm);

   } catch (boost::program_options::error& e) {
      std::cerr << "error : " << e.what() << "\n\n";
      help(std::cerr, description);
   } catch (std::exception& e) {
      std::cerr << "error : " << e.what() << std::endl;
   } catch (...) {
      std::cerr << "miserably failed:(" << std::endl;
   }

   return 0;
}
