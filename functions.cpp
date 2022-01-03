//
//  functions.cpp
//  cozjesc_projekt_zaliczeniowy
//
//  Created by Aleksandra on 29/12/2021.
//

#include "functions.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector<string> separateBy(string str, char by){
    vector<string> separated;
    string temp="";
    for(int i=0; i<str.length(); i++){
        if(i==str.length()-1){
            temp+=str[i];
            separated.push_back(temp);
        }
        else if(str[i]!=by){
            if(str[i]==' ' && by!=' ') continue;
            temp+=str[i];
        }
        else{
            separated.push_back(temp);
            temp="";
        }
    }
    return separated;
}


vector<recipes> analyseRecipeFile(fstream &file){
    vector<recipes> recipe;
    string line="";
    string temp="";
    int flag=0;
    while(file){
        getline(file, line);
        //recipes object;
        recipes* object=new recipes;
//nazwa potrawy
        for(int i=6; line[i]!=39; i++){
            temp+=line[i];
        }
        object->name=temp;
        temp="";
//tablica skladnikow
        for(int i=line.find("[")+1; line[i]!=']'; i++){
            temp+=line[i];
        }
        object->ingredients=separateBy(temp, ',');
        temp="";
//przepis
        for(int i=line.find("recipe='")+8; line[i]!=39; i++){
            temp+=line[i];
        }
        object->recipe=temp;
        temp="";
        recipe.push_back(*object);
    }
    recipe.pop_back();
    return recipe;
}





