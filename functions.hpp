//
//  functions.hpp
//  cozjesc_projekt_zaliczeniowy
//
//  Created by Aleksandra on 29/12/2021.
//

#ifndef functions_hpp
#define functions_hpp
#include <iostream>
#include <fstream>
#include <vector>

class recipes{
public:
    std::string name;
    std::vector <std::string> ingredients;
    std::string recipe;
};

std::vector<recipes> analyseRecipeFile(std::fstream &file);
std::vector<std::string> separateBy(std::string str, char by);

#endif /* functions_hpp */
