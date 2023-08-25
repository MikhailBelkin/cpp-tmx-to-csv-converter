

#include "pugixml.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>


using namespace std::literals;


int main(int argc, const char** argv) {

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <in_file.tmx> <out_file.csv>" << std::endl;
        return 1;
    }

    
    


    pugi::xml_document doc;
    auto result = doc.load_file(argv[1]);

    if (!result)
    {
        std::cout << "Failed opening TMX: " << argv[1] << std::endl;// +path, Logger::Type::Error);
        std::cout << "Reason: " << std::string(result.description()) << std::endl;
        return false;
    }

    
    std::ofstream csv_file(argv[2]);

    if (csv_file.fail()) {
        std::cout << "Failed opening CSV:" << argv[2] << std::endl;// +path, Logger::Type::Error);


    }

    pugi::xml_node tmx = doc.child("tmx");

    pugi::xml_node header = tmx.first_child();

    pugi::xml_node body = header.next_sibling();

    pugi::xml_node cur_tu = body.first_child();
    
    int counter = 0;

    while (!cur_tu.empty()) {
        std::vector<std::string> save_str_with_tag;
        csv_file << counter++;
        int count_tag = 0;
        for (auto inside_tu = cur_tu.children( "tuv").begin(); inside_tu != cur_tu.children("tuv").end(); inside_tu++) {

            std::ostringstream str_with_tag;
            auto x = inside_tu->first_child();

            csv_file << " , " << x.child_value();
            
            auto ofsset =x.offset_debug();
            
            x.print(str_with_tag);
            save_str_with_tag.push_back(str_with_tag.str());
            auto filling = x.first_child().next_sibling();
            while (!filling.empty()) {
                csv_file <<  filling.value();
                filling.print(str_with_tag);
                //save_str_with_tag[count_tag]
                filling = filling.next_sibling();
            }
            if (counter == 309) return 0;
        }
        for (auto i : save_str_with_tag) {
            std::string temp = i.substr(0, i.size() - 1);
            int remove_index = 0;
            while (temp.find("\n", remove_index) != std::string::npos) {
                remove_index = temp.find("\n", remove_index);
                temp[remove_index] = ' ';
            }
            csv_file << ", " << temp;
        }
        csv_file << std::endl;

        cur_tu = cur_tu.next_sibling();
    }

    /*for (auto it = curren_child.first_child(); !it.empty(); it.next_sibling()) {
        for (auto inside_cur = it.children().begin(); inside_cur != it.children().end(); inside_cur++) {
            std::cout<<*inside_cur->name()<<std::endl;
        }


    }*/


    return 0;

}