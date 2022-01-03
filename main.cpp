//
//  main.cpp
//  cozjesc_projekt_zaliczeniowy
//
//  Created by Aleksandra on 29/12/2021.
//

#include <iostream>
#include <fstream>
#include "functions.hpp"

using namespace std;

int main() {
//obsluga pliku
    string file="recipesBase.txt";
    std::fstream fileRecipes;
    fileRecipes.open(file);
    if(!fileRecipes.good()){
        cout<<"Plik nie istnieje. Czy chcesz utworzyć plik z przepisami? t/n"<<endl;
        char choice;
        cin>>choice;
        if(choice=='t'){
            ofstream fileRecipes(file);
            cout<<"Plik został utworzony. ";
        }
        else{
            cout<<"Program zostanie zakończony, ponieważ nie może działać bez pliku z przepisami."<<endl;
        }
    }
//obsluga pliku
    
vector<recipes> analysedFile=analyseRecipeFile(fileRecipes);
    cout<<"hello";
    
    
    return 0;
}
